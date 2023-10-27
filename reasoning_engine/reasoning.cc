#include "Rete_Network.hh"

// 为 Token 追溯组成其自身的 Fact
void trace_back(shared_ptr<Token> token){
    // Token 本身需要由 Fact 组成，要在 reasoning_graph 中体现这一点
    for(auto f:token->content){ // 每个 fact 都要指向 token
        auto new_rule = make_shared<Rete_Rule>();
        new_rule->description = "组合已知条件"; // 实际上并不是一条"规则"
        shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
        edge->fact_start = reasoning_graph->share_or_build_fact_node(f);
        edge->token_end = reasoning_graph->share_or_build_token_node(token);
        reasoning_graph->edges.push_back(edge);
    }
}

// 尝试为 Individual 补充可能的 alt_val
void sup_possible_alt(Individual &indi, Rete_Question &question){
    if(indi.is_term && indi.term->args.size()==1){ // 只考虑对一元算子进行参数替换: {a=b} => {P(a)=P(b)}
        string oprt = indi.term->oprt;
        auto &old_body = indi.term->args[0]; // 原来的参数即 a
        if(old_body->is_term && old_body->term->args.size()==1){ // 如果 a 是项, 则递归地处理 a
            sup_possible_alt(*old_body,question);
            // 考虑两层的替换: {a=b} => {P(P'(a))=P(P'(b))}
            // 这里的 old_body 为 P'(a)
            for(auto alt:old_body->alt_vals){ // 如果 P'(a) 存在 alt, 考虑替换
                string old_obj_name = indi.get_output_str(); // 即 P(P'(a))
                string new_obj_name = oprt+"("+alt->get_output_str()+")"; // Term 名称对参数进行替换得到新的 Term 名称 ( 即由 P(P'(a)) 替换得到 P(P'(b)) )
                auto it = question.indi_hash_map.find(new_obj_name);
                shared_ptr<Individual> new_obj; // 即 P(P'(b))
                if(it!=question.indi_hash_map.end()){ // 只在 P(P'(b)) 已经存在时生成相关的等式断言
                    new_obj = it->second;
                    // 根据 用于替换的参数之间的相等关系 可以得到 suffice to 关系，而这可以抽象为一条新的规则 (即 {a=b} => {P(P'(a))=P(P'(b))})
                    assert(alt->term->args.size()==1);
                    auto a = old_body->term->args[0];
                    auto b = alt->term->args[0];
                    string lhs_name = "{"+a->get_output_str()+"="+b->get_output_str()+"}"; // 用于替换的参数之间的相等关系，也就是就是引起该 suffice to 关系的 assertion
                    string new_rule_desc = "等量代换";
                    // 用这里的 suffice to 关系创建新的规则加入 reasoning_graph
                    shared_ptr<Rete_Rule> new_rule = make_shared<Rete_Rule>();
                    new_rule->description = new_rule_desc;
                    shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
                    for(auto fact: question.fact_list){ // 在当前已知的 fact 中找到该起点，补充到 edge 上
                        if(fact->get_output_str().find(lhs_name)!=string::npos){
                            edge->fact_start = reasoning_graph->share_or_build_fact_node(fact);
                            break;
                        }
                    }
                    assert(edge->fact_start);
                    // 创建新的 rhs (即 P(a)=P(b)) 作为终点，补充到 edge 上
                    auto it_l = question.indi_hash_map.find(old_obj_name);
                    auto it_r = question.indi_hash_map.find(new_obj_name);
                    assert(it_l!=question.indi_hash_map.end() && it_r!=question.indi_hash_map.end());
                    auto assertion = Assertion(*it_l->second,*it_r->second);
                    auto new_rhs = make_shared<Individual>(assertion);
                    question.normalize_individual(new_rhs); // 保存之前先统一 Individual
                    // 传播变量声明
                    new_rhs->propagate_var_decl(question.var_decl);
                    auto new_fact = make_shared<Fact>(assertion);
                    new_fact = reasoning_graph->share_or_build_fact_node(new_fact);
                    question.fact_list.push_back(new_fact);
                    edge->fact_end = new_fact;
                    reasoning_graph->edges.push_back(edge);
                }
            }
        }
        // 如果 a 是变量, 则生成新的 fact: {P(a)=P(b)}
        else if(old_body->is_math_indi && old_body->math_indi->is_math_expr && old_body->math_indi->expr_val->is_sy){
            for(auto alt:old_body->alt_vals){ // alt 即 b
                string old_obj_name = indi.get_output_str(); // 即 P(a)
                string new_obj_name = oprt+"("+alt->get_output_str()+")"; // Term 名称对参数进行替换得到新的 Term 名称 (即由 P(a) 替换得到 P(b))
                auto it = question.indi_hash_map.find(new_obj_name);
                shared_ptr<Individual> new_obj; // 即 P(b)
                if(it!=question.indi_hash_map.end()){ // 只在 P(b) 已经存在时生成相关的等式断言
                    new_obj = it->second;
                    // 根据 用于替换的参数之间的相等关系 可以得到 suffice to 关系，而这可以抽象为一条新的规则 (即 {a=b} => {P(a)=P(b)})
                    string lhs_name = "{"+old_body->get_output_str()+"="+alt->get_output_str()+"}"; // 用于替换的参数之间的相等关系，也就是就是引起该 suffice to 关系的 assertion
                    string new_rule_desc = "等量代换";
                    // 用这里的 suffice to 关系创建新的规则加入 reasoning_graph
                    shared_ptr<Rete_Rule> new_rule = make_shared<Rete_Rule>();
                    new_rule->description = new_rule_desc;
                    shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
                    for(auto fact: question.fact_list){ // 在当前已知的 fact 中找到该起点，补充到 edge 上
                        if(fact->get_output_str().find(lhs_name)!=string::npos){
                            edge->fact_start = reasoning_graph->share_or_build_fact_node(fact);
                            break;
                        }
                    }
                    assert(edge->fact_start);
                    // 创建新的 rhs (即 P(a)=P(b)) 作为终点，补充到 edge 上
                    auto it_l = question.indi_hash_map.find(old_obj_name);
                    auto it_r = question.indi_hash_map.find(new_obj_name);
                    assert(it_l!=question.indi_hash_map.end() && it_r!=question.indi_hash_map.end());
                    auto assertion = Assertion(*it_l->second,*it_r->second);
                    auto new_rhs = make_shared<Individual>(assertion);
                    question.normalize_individual(new_rhs); // 保存之前先统一 Individual
                    // 传播变量声明
                    new_rhs->propagate_var_decl(question.var_decl);
                    auto new_fact = make_shared<Fact>(assertion);
                    new_fact = reasoning_graph->share_or_build_fact_node(new_fact);
                    question.fact_list.push_back(new_fact);
                    edge->fact_end = new_fact;
                    reasoning_graph->edges.push_back(edge);
                }
            }
        }
    }
}

// 判断题目是否已解出
bool has_been_solved(shared_ptr<Rete_Question> question){
    // for(auto unknown:question->to_solve)
    //     question->normalize_individual(unknown);

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
    // 从 to_solve 的 alt_vals 入手
    // 先尝试为 to_solve 补充可能的 alt_val
    sup_possible_alt(*unknown, *question); // 这一步实际上是 eager evaluation

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

    for(auto &alt:unknown->alt_vals){
        // 如果 alt_vals 中的某个已知或可解，则已解出
        if(alt->val_is_known) // alt 本身已知
            return true;
        else if(alt->alt_vals.size()){ // alt 存在已知的 alt
            for(auto alt_alt:alt->alt_vals){
                if(alt_alt->val_is_known){
                    //  (a = b,b = c => a = c)
                    // 找到 a = b、 b = c 部分
                    string part_1_name = "{"+unknown->get_output_str()+"="+alt->get_output_str()+"}";
                    string part_2_name = "{"+alt->get_output_str()+"="+alt_alt->get_output_str()+"}";
                    
                    shared_ptr<Fact> part_1, part_2; // a = b、 b = c 部分本来就在已知 fact 中
                    for(auto f:question->fact_list){
                        if(f->get_output_str().find(part_1_name)!=string::npos)
                            part_1 = f;
                        else if(f->get_output_str().find(part_2_name)!=string::npos)
                            part_2 = f;
                        if(part_1 && part_2)
                            break;
                    }
                    assert(part_1 && part_2);

                    // 构造新 fact (即 a = c 部分)
                    auto new_fact = make_shared<Fact>(Assertion(*unknown,*alt_alt)); // a = c 部分
                    new_fact = reasoning_graph->share_or_build_fact_node(new_fact);
                    // 传播变量声明
                    new_fact->assertion->propagate_var_decl(question->var_decl);
                    new_fact->var_decl = new_fact->assertion->var_decl;
                    question->fact_list.push_back(new_fact);

                    // 新的 fact 的 lhs 由 part_1 和 part_2 组成
                    vector<shared_ptr<Fact>> token_args = {part_1,part_2};
                    auto new_lhs = make_shared<Token>(token_args);
                    new_lhs = reasoning_graph->share_or_build_token_node(new_lhs);
                    trace_back(new_lhs); // 而 Token 本身需要由 Fact 组成，要在 reasoning_graph 中体现这一点
                    auto new_rule = make_shared<Rete_Rule>();
                    new_rule->description = "显然";
                    // 构造新的 Reasoning_Edge (a = b,b = c => a = c)
                    shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
                    edge->token_start = new_lhs;
                    edge->fact_end = new_fact;
                    reasoning_graph->edges.push_back(edge);
                    return true;
                }
            }
        }
        else{ // alt 可解  (这一步实际上是 lazy evaluation)
            string old_alt_name = alt->get_output_str();
            auto conditions = make_shared<vector<shared_ptr<Fact>>>(); // b=c 的前提条件
            auto ret = action_eval(alt,*question, conditions); // (a = b,b = c => a = c)
            // 只有在该函数中，reasoning_edge 的终点才可能是 Token
            if(ret){
                // // 找到求值 fact (即 b = c 部分)
                // shared_ptr<Fact> part_2; // (b = c)
                // string eval_fact_name = "{"+alt->get_output_str()+"="+ret->get_output_str()+"}";
                // part_2 = *(question->fact_list.end()-1); // 刚刚 action_eval 求值结束，所以已知条件的最后一个 fact 即为 b = c 部分
                // assert(part_2 && part_2->get_output_str()==eval_fact_name);
                
                // 构造新 fact (即 b = c 部分)
                auto part_2 = make_shared<Fact>(Assertion(*alt,*ret));
                question->normalize_individual(ret);
                part_2->assertion->propagate_var_decl(question->var_decl);
                part_2->var_decl = part_2->assertion->var_decl;
                question->fact_list.push_back(part_2);
                assert(part_2);

                // b = c 部分计算结果的得出本身需要一定的前提条件
                {
                    auto new_rule = make_shared<Rete_Rule>();
                    new_rule->description = "等量代换后的计算";
                    shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
                    edge->fact_end = reasoning_graph->share_or_build_fact_node(part_2);
                    if(conditions->size()==1)
                        edge->fact_start = reasoning_graph->share_or_build_fact_node(*conditions->begin());
                    else{
                        auto token_start = make_shared<Token>(*conditions);
                        edge->token_start = reasoning_graph->share_or_build_token_node(token_start);
                        trace_back(token_start); // 而 Token 本身需要由 Fact 组成，要在 reasoning_graph 中体现这一点
                    }
                    reasoning_graph->edges.push_back(edge);
                }

                // 构造新 fact (即 a = c 部分)
                auto new_fact = make_shared<Fact>(Assertion(*unknown,*ret)); // a = c 部分
                new_fact = reasoning_graph->share_or_build_fact_node(new_fact);
                // question->normalize_individual(new_fact); // 这里不必进行统一 Individual, assertion 左右两边都已经是实例化后的结果
                // 传播变量声明
                new_fact->assertion->propagate_var_decl(question->var_decl);
                new_fact->var_decl = new_fact->assertion->var_decl;
                question->fact_list.push_back(new_fact);
                
                // 找到 fact (即 a = b 部分)
                string part_1_name = "{"+unknown->get_output_str()+"="+old_alt_name+"}";
                shared_ptr<Fact> part_1; // a = b 部分本来就在已知 fact 中
                for(auto f:question->fact_list){
                    if(f->get_output_str().find(old_alt_name)!=string::npos){
                        part_1 = f;
                        break;
                    }
                }
                assert(part_1);
                
                // 新的 fact 的 lhs 由 part_1 和 part_2 组成
                vector<shared_ptr<Fact>> token_args = {part_1,part_2};
                auto new_lhs = make_shared<Token>(token_args);
                new_lhs = reasoning_graph->share_or_build_token_node(new_lhs);
                trace_back(new_lhs); // 而 Token 本身需要由 Fact 组成，要在 reasoning_graph 中体现这一点
                auto new_rule = make_shared<Rete_Rule>();
                new_rule->description = "显然";
                // 构造新的 Reasoning_Edge (a = b,b = c => a = c)
                shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
                edge->token_start = new_lhs;
                edge->fact_end = new_fact;
                reasoning_graph->edges.push_back(edge);
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
        // #ifndef NDEBUG
        //     cout<<endl<<"当前 Question 中的所有 Individual 如下: ("<<question->indi_hash_map.size()<<"个)"<<endl;
        //     for(auto p:question->indi_hash_map){
        //         cout<<"\t"<<p.first<<"  对应 alt_val: ";
        //         for(auto alt:p.second->alt_vals)
        //             cout<<*alt<<"  ";
        //         cout<<"("<<p.second->alt_vals.size()<<"个)";
        //         cout<<endl;
        //     }
        // #endif
        
        // 执行规则的 RHS
        for(auto instantiated_rule:instantiated_rules){
            cout<<*instantiated_rule<<endl;
            // 对于同一批激活的规则，在执行规则的 RHS 过程中个体在进行更新，可能会导致还未执行的规则中的某些个体变成 "过时的", 所以要重新进行 normalization
            question->normalize_individual(instantiated_rule);
            #ifndef NDEBUG
                cout<<endl<<"当前 Question 中的所有 Individual 如下: ("<<question->indi_hash_map.size()<<"个)"<<endl;
                for(auto p:question->indi_hash_map){
                    cout<<"\t"<<p.first<<"  对应 alt_val: ";
                    for(auto alt:p.second->alt_vals)
                        cout<<*alt<<"  ";
                    cout<<"("<<p.second->alt_vals.size()<<"个)";
                    cout<<endl;
                }
                if(instantiated_rule->lhs)
                    auto lhs = *instantiated_rule->lhs;
                auto rhs = *instantiated_rule->rhs;
                cout<<rhs<<endl;
            #endif

            size_t origin_fact_num = question->fact_list.size();
            question->take_action(instantiated_rule->rhs, rete_network->underlying_kb);

            size_t new_fact_num = question->fact_list.size();
            size_t add_fact_num = new_fact_num-origin_fact_num; // 由该规则导致的新增 fact 的数量
            if(add_fact_num>0){
                cout<<"此次执行导致新增 fact 的数量为: "<<add_fact_num<<endl;
                // 把当前的实例化规则产生新的 fact 的信息加入 Reasoning_Graph
                shared_ptr<Reasoning_Edge> edge;
                size_t first_edge_pos=0; // 记录实例化规则相关的 edge 在 Reasoning_Graph 中的位置
                for(auto e: reasoning_graph->edges){
                    if(e->instantiated_rule->get_output_str()==instantiated_rule->get_output_str()){
                        edge = e;
                        break;
                    }
                    first_edge_pos++;
                }
                assert(edge);
                if(add_fact_num==1){ // 如果只导致新增了一条 fact，只需找到 rule 对应的边，并把终点设置为该 fact
                    edge->fact_end = reasoning_graph->share_or_build_fact_node(question->fact_list.at(origin_fact_num));
                }
                else{ // 否则，需要创建多条这样的边，它们的终点各不相同（注意同一条 rhs 生成的这多条边必须相邻）
                    edge->fact_end = reasoning_graph->share_or_build_fact_node(question->fact_list.at(origin_fact_num));
                    for(size_t i=origin_fact_num+1; i!=new_fact_num; ++i){
                        auto new_edge = make_shared<Reasoning_Edge>(*edge);
                        new_edge->fact_end = reasoning_graph->share_or_build_fact_node(question->fact_list.at(i));
                        auto next_edge_pos = reasoning_graph->edges.begin()+(++first_edge_pos);
                        reasoning_graph->edges.insert(next_edge_pos,new_edge); // 这些边必须相邻
                    }
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
        reasoning_graph->print_solving_process();
    }
    else{
        cout << endl << "解题失败!" << endl;
        reasoning_graph->print_all_progress();
    }
}
