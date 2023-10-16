#include "Rete_Network.hh"

// 判断题目是否已解出
bool has_been_solved(shared_ptr<Rete_Question> question){
    // 只要有未知的待求项，解题就尚未结束
    // 目前只处理有一个待求解项的情况
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

    assert(question->to_solve.size()==1);
    auto unknown = *question->to_solve.begin();

    // FIXME:本不该出现下面几行代码, 但是原先的 to_solve 的个体似乎丢失了 alt_val, 所以在哈希表中找到正确的个体
    auto it = question->indi_hash_map.find(unknown->get_output_str());
    assert(it!=question->indi_hash_map.end());
    unknown = it->second;
    

    // cout<<endl<<"!!! alt_vals.size() = "<<unknown->alt_vals.size()<<endl;
    for(auto &alt:unknown->alt_vals){
        if(alt->val_is_known)
            return true;
        else{
            auto ret = eval(alt,*question);
            if(ret){
                alt = ret;
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
        specify_the_question(question,next_fact); // 对于(在 take_action 中产生的) 新 fact, 要指明其所在的 Rete_Question
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
            question->take_action(instantiated_rule->rhs, rete_network->underlying_kb);
        }
        if(instantiated_rules.empty())
            break; // 第二个出口
    }
    while(!has_been_solved(question)); // 第一个出口

    if(has_been_solved(question)){
        cout << endl << "解题完毕!" << endl;
        question->print_result();
    }
    else
        cout << endl << "解题失败!" << endl;
}
