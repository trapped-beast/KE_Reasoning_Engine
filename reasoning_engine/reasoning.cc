#include "Rete_Network.hh"

// 判断题目是否已解出
bool has_been_solved(shared_ptr<Rete_Question> question){
    // 只要有未知的待求项，解题就尚未结束
    #ifndef NDEBUG
        cout<<endl<<"当前 Question:"<<endl<<*question<<endl;
        cout<<endl<<"当前 Question 中的所有 Individual 如下: ("<<question->indi_hash_map.size()<<"个)"<<endl;
        for(auto p:question->indi_hash_map){
            cout<<"\t"<<p.first<<(p.second->val_is_known?" \u2713 ":" \u2717 ")<<"  对应 alt_val: ";
            for(auto alt:p.second->alt_vals)
                cout<<*alt<<(alt->val_is_known?" \u2713 ":" \u2717 ")<<"  ";
            cout<<"("<<p.second->alt_vals.size()<<"个)";
            cout<<endl;
        }
    #endif

    assert(question->to_solve.size()==1); // 目前只处理有一个待求解项的情况
    auto unknown = *question->to_solve.begin();
    for(auto &alt:unknown->alt_vals){
        if(alt->val_is_known)
            return true;
        else{
            auto old_alt_name = alt->get_output_str();
            auto ret = eval(alt,*question);
            if(ret){
                alt = ret;

                string new_rule_name = "相等性传递 => " + unknown->get_output_str() + "=" + old_alt_name + "=" + ret->get_output_str();
                cout<<new_rule_name<<endl;
                auto new_rule = make_shared<Rete_Rule>();
                new_rule->description = new_rule_name;
                shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
                reasoning_graph->edges.push_back(edge);
                return true;
            }
        }
    }
    return false;
}

// 寻找 Question 中的下一条尚未加入过 Rete 网络的 Fact
bool get_next_fact(shared_ptr<Rete_Question> question, shared_ptr<Fact> &ret){
    for(auto fact:question->fact_list){
        if(!fact->has_been_added){
            ret = fact;
            return true;
        }
    }
    return false;
}

// 根据当前已知信息，匹配可用规则，并返回实例化后的规则
vector<shared_ptr<Rete_Rule>> get_instantiated_rules(shared_ptr<Rete_Question> question, shared_ptr<Rete_Network> rete_network){
    // TODO: 全程考虑记录推理过程

    cout<<endl<<endl<<"============================开始进行新一轮的模式匹配============================"<<endl;
    vector<shared_ptr<Rete_Rule>> ret;
    size_t start = rete_network->conflict_set->content.size(); // 本轮匹配到的 rule 的起点

    // 把 (未使用过的) 已知的 fact 逐条送入 Rete 网络
    shared_ptr<Fact> next_fact;
    while(get_next_fact(question, next_fact)){
        specify_the_question(question, next_fact); // 对于(在 take_action 中产生的) 新 fact, 要指明其所在的 Rete_Question
        rete_network->add_fact(next_fact);
    }
    size_t end = rete_network->conflict_set->content.size(); // 本轮匹配到的 rule 的终点
    for (; start != end; ++start){
        ret.push_back(rete_network->conflict_set->content.at(start));
    }
    for(auto &r:ret){
        question->normalize_individual(r);
    }
    return ret;
}


// 执行推理
void reasoning(shared_ptr<Rete_Question> question, shared_ptr<Rete_Network> rete_network){
    cout<<"执行推理"<<endl;

    // 统一 Rete_Question 中的 Individual
    for(auto fact:question->fact_list){
        question->normalize_individual(fact);
    }
    for(auto indi:question->to_solve){
        question->normalize_individual(indi);
    }

    #ifndef NDEBUG
        cout<<endl<<"下面是当前 fact 中 assertion 的左部个体的 alt_vals 情况: "<<endl;
        for(auto fact:question->fact_list){
            if(fact->is_assert){
                auto self = fact->assertion->left;
                cout<<"\t"<<*self<<" 的 alt_vals 个数为: "<<self->alt_vals.size()<<endl;
                if(self->alt_vals.size()){
                    cout<<"\t各个 alt 分别为: "<<endl;
                    for(auto alt:self->alt_vals){
                        cout<<"\t\t"<<*alt<<endl;
                    }
                }
            }
        }
    #endif

    do{
        // 找到可用的实例化后的规则
        vector<shared_ptr<Rete_Rule>> instantiated_rules = get_instantiated_rules(question, rete_network);
        #ifndef NDEBUG
            cout<<endl<<"当前 Question 中的所有 Individual 如下: ("<<question->indi_hash_map.size()<<"个)"<<endl;
            for(auto p:question->indi_hash_map){
                cout<<"\t"<<p.first<<"  对应 alt_val: ";
                for(auto alt:p.second->alt_vals)
                    cout<<*alt<<"  ";
                cout<<"("<<p.second->alt_vals.size()<<"个)";
                cout<<endl;
            }
        #endif
        
        // 执行规则的 RHS
        for(auto instantiated_rule:instantiated_rules){
            size_t origin_fact_num = question->fact_list.size();
            question->take_action(instantiated_rule->rhs, rete_network->underlying_kb);

            size_t new_fact_num = question->fact_list.size();
            size_t add_fact_num = new_fact_num-origin_fact_num; // 由该规则导致的新增 fact 的数量
            if(add_fact_num>0){
                cout<<"此次执行导致新增 fact 的数量为: "<<add_fact_num<<endl;
                // 把当前的实例化规则产生新的 fact 的信息加入 Reasoning_Graph
                shared_ptr<Reasoning_Edge> edge;
                for(auto e: reasoning_graph->edges){
                    if(e->instantiated_rule->get_output_str()==instantiated_rule->get_output_str()){
                        edge = e;
                        break;
                    }
                }
                assert(edge);
                if(add_fact_num==1){ // 如果只导致新增了一条 fact，只需找到 rule 对应的边，并把终点设置为该 fact
                    edge->end = question->fact_list.at(origin_fact_num);
                }
                else{ // 否则，需要创建多条这样的边，它们的终点各不相同
                    edge->end = question->fact_list.at(origin_fact_num);
                    for(size_t i=origin_fact_num+1; i!=new_fact_num; ++i){
                        auto new_edge = make_shared<Reasoning_Edge>(*edge);
                        new_edge->end = question->fact_list.at(i);
                        reasoning_graph->edges.push_back(new_edge);
                    }
                }
                for(size_t i=origin_fact_num; i!=new_fact_num; ++i){ // 记录这些新的 fact
                    auto this_fact = question->fact_list.at(i);
                    reasoning_graph->fact_nodes_hash_table.insert(pair<string,shared_ptr<Fact>>(this_fact->get_output_str(),this_fact));
                }
            }
        }
        if(instantiated_rules.empty())
            break; // 第二个出口
    }
    while(!has_been_solved(question)); // 第一个出口

    if(has_been_solved(question)){
        cout << endl << "解题完毕!" << endl;
        question->print_result();
        cout<<"解:"<<endl;
        for(auto e:reasoning_graph->edges){
            if(!e->fact_start && !e->end)
                cout<<e->instantiated_rule->description<<endl;
            else{
                cout<<*e->fact_start<< " => "<<*e->end;
                // cout<<"\t("<<e->instantiated_rule->description<<")";
                cout<<endl;
            }
        }
    }
    else
        cout << endl << "解题失败!" << endl;
}
