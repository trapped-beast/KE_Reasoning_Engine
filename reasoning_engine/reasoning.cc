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


shared_ptr<Fact> find_fact_by_name(const string &name, Rete_Question &question){
    shared_ptr<Fact> ret;
    for(auto f:question.fact_list){
        if(f->get_output_str().find(name)!=string::npos){
            ret = f;
            return ret;
        }
    }
    cout<<"找不到fact: "<<name<<endl;
    assert(false);
}

// 把 fact 加入 Reasoning_Graph 中并为其溯源 (接收的参数为新 fact、其作为前提的fact的名称、所在 Question)
void construct_fact_in_graph(shared_ptr<Fact> new_fact, vector<string> dependence, Rete_Question &question){
    // 在 Reasoning_Graph 中体现该 fact
    new_fact = reasoning_graph->share_or_build_fact_node(new_fact);

    vector<shared_ptr<Fact>> token_args; // 组成新 fact 的依赖的 token
    for(auto dep_name:dependence)
        token_args.push_back(find_fact_by_name(dep_name,question));

    auto new_lhs = make_shared<Token>(token_args);
    new_lhs = reasoning_graph->share_or_build_token_node(new_lhs);
    trace_back(new_lhs); // 而 Token 本身需要由 Fact 组成，要在 reasoning_graph 中体现这一点
    auto new_rule = make_shared<Rete_Rule>();
    new_rule->description = "显然";
    shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
    edge->token_start = new_lhs;
    edge->fact_end = new_fact;
    reasoning_graph->edges.push_back(edge);
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
                    // (a = b,b = c => a = c)
                    string part_1_name = "{"+unknown->get_output_str()+"="+alt->get_output_str()+"}";
                    string part_2_name = "{"+alt->get_output_str()+"="+alt_alt->get_output_str()+"}";
                    // 构造新 fact (即 a = c 部分)
                    auto new_fact = make_shared<Fact>(Assertion(*unknown,*alt_alt)); // a = c 部分
                    // 传播变量声明
                    new_fact->assertion->propagate_var_decl(question->var_decl);
                    new_fact->var_decl = new_fact->assertion->var_decl;
                    question->fact_list.push_back(new_fact);

                    // 把 fact 加入 Reasoning_Graph 中并为其溯源
                    vector<string> dependence = {part_1_name, part_2_name};
                    construct_fact_in_graph(new_fact, dependence, *question);
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
                // 找到 fact (即 a = b 部分)
                string part_1_name = "{"+unknown->get_output_str()+"="+old_alt_name+"}";

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
                string part_2_name = part_2->get_output_str();
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
                // question->normalize_individual(new_fact); // 这里不必进行统一 Individual, assertion 左右两边都已经是实例化后的结果
                // 传播变量声明
                new_fact->assertion->propagate_var_decl(question->var_decl);
                new_fact->var_decl = new_fact->assertion->var_decl;
                question->fact_list.push_back(new_fact);

                // 把 fact 加入 Reasoning_Graph 中并为其溯源
                vector<string> dependence = {part_1_name, part_2_name};
                construct_fact_in_graph(new_fact, dependence, *question);
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
        next_fact->where_is = question; // 对于(在 take_action 中产生的) 新 fact, 要指明其所在的 Rete_Question
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

bool transpose(shared_ptr<Fact> origin_eq){ // 尝试对方程进行简单的移项求解
    bool worked = false;
    cout<<"对以下方程进行移项: "<<*origin_eq<<endl;
    assert(origin_eq->is_assert && origin_eq->assertion->is_std);
    auto left = origin_eq->assertion->left;
    auto right = origin_eq->assertion->right;
    shared_ptr<Individual> temp;
    // 目前处理 +-*/^ = num 的例子，统一把数字放在右边
    if(right->is_math_indi && right->math_indi->is_math_expr && right->math_indi->expr_val->is_num)
        ;
    else{
        temp = left;
        left = right;
        right = temp;
    }
    // 处理涉及 +-*/^ 的方程，形如 Mul(num_1, a) = num_2
    vector<string> mathe_op = {"Add", "Sub", "Mul", "Div", "Pow"};
    bool left_is_mathe_with_2_args = left->is_term && std::find(mathe_op.begin(), mathe_op.end(), left->term->oprt)!=mathe_op.end() && left->term->args.size()==2;
    bool right_is_num = right->is_math_indi && right->math_indi->is_math_expr && right->math_indi->expr_val->is_num;
    if(left_is_mathe_with_2_args && right_is_num){
        auto arg_1 = left->term->args[0]; // 即左部的 num_1
        auto arg_2 = left->term->args[1]; // 即左部的 a
        string oprt = left->term->oprt;
        shared_ptr<Fact> condition; // a > 0 或是 a < 0
        
        Individual new_left, new_right; // 移项后得到的新 fact 的左右部分，形如 a = Mul(num_2, num_1)
        vector<shared_ptr<Individual>> new_args;
        shared_ptr<Individual> right_val;
        new_left = *arg_2;
        if(oprt=="Add"){ // num_1 + a = num_2  => a = num_2 - num_1
            new_args.push_back(right);
            new_args.push_back(arg_1);
            right_val = make_shared<Individual>(Term("Sub",new_args));
        }
        else if(oprt=="Sub"){ // num_1 - a = num_2  => a = num_1 - num_2
            new_args.push_back(arg_1);
            new_args.push_back(right);
            right_val = make_shared<Individual>(Term("Sub",new_args));
        }
        else if(oprt=="Mul"){ // num_1 * a = num_2  => a = num_2 / num_1
            new_args.push_back(right);
            new_args.push_back(arg_1);
            right_val = make_shared<Individual>(Term("Div",new_args));
        }
        else if(oprt=="Div"){ // num_1 / a = num_2  => a = num_1 / num_2
            new_args.push_back(arg_1);
            new_args.push_back(right);
            right_val = make_shared<Individual>(Term("Div",new_args));
        }
        else if(oprt=="Pow"){
            /*
             * Pow 运算比较特殊，需要额外知道 a 的正负
             * a ^ 2 = num , a > 0 => a = sqrt(num)
             * a ^ 2 = num , a < 0 => a = -sqrt(-num)
             */
            cout<<*origin_eq<<endl;
            string a_gt_0_str = "{"+arg_1->get_output_str()+">0}";
            string a_lt_0_str = "{"+arg_1->get_output_str()+"<0}";
            bool a_gt_0 = false;
            bool a_lt_0 = false;
            for(auto f:origin_eq->where_is->fact_list){
                if(f->get_output_str().find(a_gt_0_str)!=string::npos){
                    a_gt_0 = true;
                    condition = f;
                    cout<<"condition = "<<*condition<<endl;
                    break;
                }
                else if(f->get_output_str().find(a_lt_0_str)!=string::npos){
                    a_lt_0 = true;
                    condition = f;
                    cout<<"condition = "<<*condition<<endl;
                    break;
                }
            }
            shared_ptr<Individual> temp;
            vector<shared_ptr<Individual>> new_args;
            if(a_gt_0){ // a ^ 2 = num , a > 0 => a = sqrt(num)
                new_args.push_back(right);
                temp = make_shared<Individual>(Term("Sqrt",new_args));
            }
            else{ // a ^ 2 = num , a < 0 => a = -sqrt(-num)
                vector<shared_ptr<Individual>> inner_args, mid_args;
                inner_args.push_back(right);
                auto inner_indi = make_shared<Individual>(Term("Neg",inner_args)); // -num 部分
                mid_args.push_back(inner_indi);
                auto mid = make_shared<Individual>(Term("Sqrt",mid_args)); // sqrt(-num) 部分
                new_args.push_back(mid);
                temp = make_shared<Individual>(Term("Neg",new_args));
            }
            new_left = *arg_1; // Pow 生成的 fact 和 +-*/ 生成的有所不同
            right_val = temp;
        }
        auto new_fact = make_shared<Fact>(Assertion(new_left,*action_eval(right_val,*origin_eq->where_is))); // 移项后得到的新 fact
        bool find = false; // 不重复存入已知 fact
        for(auto f:origin_eq->where_is->fact_list){
            if(f->get_output_str().find(new_fact->get_output_str())!=string::npos){
                find = true;
                break;
            }
        }
        if(!find){
            new_fact->where_is = origin_eq->where_is;
            origin_eq->where_is->normalize_individual(new_fact);
            origin_eq->where_is->fact_list.push_back(new_fact);
            // 在 Reasoning_Graph 中体现这一过程
            auto new_rule = make_shared<Rete_Rule>();
            new_rule->description = "移项";
            shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
            edge->fact_end = reasoning_graph->share_or_build_fact_node(new_fact);
            bool being_squared_is_num = arg_1->is_math_indi && arg_1->math_indi->is_math_expr && arg_1->math_indi->expr_val->is_num;
            if(oprt=="Pow" && !being_squared_is_num){ // 关于变量的 Pow 的逆运算需要的条件是 Token
                vector<shared_ptr<Fact>> token_content = {origin_eq, condition};
                auto token_start = make_shared<Token>(token_content);
                edge->token_start = reasoning_graph->share_or_build_token_node(token_start);
                trace_back(token_start); // 为 Token 本身溯源
            }
            else // +-*/ 的逆运算需要的条件是 Fact
                edge->fact_start = reasoning_graph->share_or_build_fact_node(origin_eq);
            reasoning_graph->edges.push_back(edge);
            worked = true;
            // 对得到的结果进行可能的迭代处理
            if(new_fact->is_assert && new_fact->assertion->is_std && new_fact->assertion->left->is_term && std::find(mathe_op.begin(), mathe_op.end(), new_fact->assertion->left->term->oprt)!=mathe_op.end())
                transpose(new_fact);
        }
    }
    return worked;
}

// 判断 fact 是否是潜在可解的方程
bool is_potentially_solvable_eq(shared_ptr<Fact> fact){
    cout<<"判断是否是潜在可解的方程: "<<*fact<<endl;
    bool ret = false;
    vector<string> mathe_op = {"Add", "Sub", "Mul", "Div", "Pow"};
    if(fact->is_assert && fact->assertion->is_std && fact->assertion->left->is_term && fact->assertion->right->is_term){
        auto left = *fact->assertion->left->term;
        auto right = *fact->assertion->right->term;
        if(std::find(mathe_op.begin(),mathe_op.end(),left.oprt)!=mathe_op.end() && std::find(mathe_op.begin(),mathe_op.end(),right.oprt)!=mathe_op.end())
            ret = true;
    }
    return ret;
}

// 根据 fact 及其依赖创建 Reasoning_Edge
void find_dependence(shared_ptr<Fact> fact, shared_ptr<vector<shared_ptr<Fact>>> conditions){
    cout<<"conditions->size()="<<conditions->size()<<endl;
    for(auto c:*conditions){
        cout<<"\t"<<*c<<endl;
    }
    if(conditions->size()>1){
        if(conditions->size()==1){
            auto new_rule = make_shared<Rete_Rule>();
            new_rule->description = "显然";
            auto edge = make_shared<Reasoning_Edge>(new_rule);
            edge->fact_end = reasoning_graph->share_or_build_fact_node(fact);
            auto self_dep = conditions->at(0);
            edge->fact_start = self_dep;
            reasoning_graph->edges.push_back(edge);
        }
        else{
            vector<string> dependence;
            for(auto c:*conditions)
                dependence.push_back(c->get_output_str());
            construct_fact_in_graph(fact, dependence, *fact->where_is);
        }
    }
    else
        ;// TODO:考虑多个 condition 的情况
    conditions->clear();
}

void solve_eq(shared_ptr<Rete_Question> question, vector<shared_ptr<Rete_Rule>> &rules_not_worked){ // 尝试解方程
    // 先考虑有多个 alt 的个体
    // TODO: 过滤重复的求解
    for(auto p:question->indi_hash_map){
        auto indi = *p.second;
        if(indi.alt_vals.size()>1){
            assert(indi.alt_vals.size()==2); // 目前只处理带2个 alt 的个体
            auto alt_1 = indi.alt_vals[0];
            auto alt_2 = indi.alt_vals[1];
            auto new_fact = make_shared<Fact>(Assertion(*alt_1,*alt_2));
            new_fact->where_is = question;
            // 传播变量声明
            new_fact->assertion->propagate_var_decl(question->var_decl);
            new_fact->var_decl = new_fact->assertion->var_decl;
            question->fact_list.push_back(new_fact);
            // 把 fact 加入 Reasoning_Graph 中并为其溯源
            string cond_1 = "{"+indi.get_output_str()+"="+alt_1->get_output_str()+"}";
            string cond_2 = "{"+indi.get_output_str()+"="+alt_2->get_output_str()+"}";
            vector<string> dependence = {cond_1, cond_2};
            construct_fact_in_graph(new_fact, dependence, *question);
            // 尝试简单的移项求解
            if(transpose(new_fact)){
                // 考虑 rules_not_worked 中的规则是否在此发挥了作用
                for(auto r:rules_not_worked){
                    if(r->get_output_str().find(cond_1)!=string::npos || r->get_output_str().find(cond_2)!=string::npos){
                        r->worked = true;
                        cout<<"cond_1: "<<cond_1<<endl;
                        cout<<"cond_2: "<<cond_2<<endl;
                        cout<<*r<<" 在移项时发挥了作用"<<endl;
                    }
                }
            }
        }
    }
    // 再考虑 fact 中的方程
    auto origin_fact_num = question->fact_list.size();
    for(size_t i=0; i!=origin_fact_num; ++i){
        auto origin_eq = question->fact_list[i];
        if(is_potentially_solvable_eq(origin_eq)){
            auto new_eq = make_shared<Fact>(origin_eq->get_copy());
            cout<<"考虑解方程: "<<*new_eq<<endl;
            // 先对可求解的部分进行计算，而后尝试移项求解
            auto &left = new_eq->assertion->left;
            auto &right = new_eq->assertion->right;
            assert(left->is_term && left->term->is_std && right->is_term && right->term->is_std);
            // 对于方程，尝试直接对左右两边进行计算，如果失败，则对其参数进行尽可能的计算
            shared_ptr<Individual> temp;
            // 求值本身的 dependence 要记录
            auto conditions = make_shared<vector<shared_ptr<Fact>>>();
            auto total_conditions = make_shared<vector<shared_ptr<Fact>>>();
            temp = action_eval(left,*question,conditions);
            total_conditions->insert(total_conditions->end(), conditions->begin(), conditions->end());
            vector<string> inner_conditions; // 记录该过程中的 dependence
            if(temp){
                auto new_fact = make_shared<Fact>(Assertion(*left,*temp));
                left = temp;
                cout<<"加入: "<<*new_fact<<endl;
                question->fact_list.push_back(new_fact);
                inner_conditions.push_back(new_fact->get_output_str());
                new_fact->where_is = question;
                find_dependence(new_fact,conditions); // 建立求值本身的 dependence 到它的边
            }
            else{
                size_t i = 0;
                for(auto &arg:left->term->args){
                    if(arg->is_term){
                        temp = action_eval(arg,*question,conditions);
                        total_conditions->insert(total_conditions->end(), conditions->begin(), conditions->end());
                        if(temp){
                            auto new_fact = make_shared<Fact>(Assertion(*arg,*temp));
                            arg = temp;
                            cout<<"加入: "<<*new_fact<<endl;
                            question->fact_list.push_back(new_fact);
                            inner_conditions.push_back(new_fact->get_output_str());
                            new_fact->where_is = question;
                            find_dependence(new_fact,conditions); // 建立求值本身的 dependence 到它的边
                        }
                    }
                    ++i;
                }
            }
            temp = action_eval(right,*question,conditions);
            total_conditions->insert(total_conditions->end(), conditions->begin(), conditions->end());
            if(temp){
                auto new_fact = make_shared<Fact>(Assertion(*right,*temp));
                right = temp;
                cout<<"加入: "<<*new_fact<<endl;
                question->fact_list.push_back(new_fact);
                inner_conditions.push_back(new_fact->get_output_str());
                new_fact->where_is = question;
                find_dependence(new_fact,conditions); // 建立求值本身的 dependence 到它的边
            }
            else{
                size_t i = 0;
                for(auto &arg:right->term->args){
                    if(arg->is_term){
                        temp = action_eval(arg,*question,conditions);
                        total_conditions->insert(total_conditions->end(), conditions->begin(), conditions->end());
                        if(temp){
                            auto new_fact = make_shared<Fact>(Assertion(*arg,*temp));
                            arg = temp;
                            cout<<"加入: "<<*new_fact<<endl;
                            question->fact_list.push_back(new_fact);
                            inner_conditions.push_back(new_fact->get_output_str());
                            new_fact->where_is = question;
                            find_dependence(new_fact,conditions); // 建立求值本身的 dependence 到它的边
                        }
                    }
                    ++i;
                }
            }
            new_eq->where_is = question;
            question->fact_list.push_back(new_eq);

            // "处理方程" 这一过程需要在 Reasoning_Graph 中体现
            cout<<"原方程: "<<*origin_eq<<endl;
            cout<<"新方程: "<<*new_eq<<endl;
            conditions->push_back(origin_eq);
            new_eq->where_is = question;
            find_dependence(new_eq,conditions);
            total_conditions->push_back(origin_eq);
            cout<<total_conditions->size()<<endl;

            // 把 fact 加入 Reasoning_Graph 中并为其溯源
            cout<<"total_conditions.size() = "<<total_conditions->size()<<endl;
            vector<string> dependence;
            for(auto c:*total_conditions){
                dependence.push_back(c->get_output_str());
                cout<<"\t"<<*c<<endl;
            }
            construct_fact_in_graph(new_eq, dependence, *question);
            // 尝试简单的移项求解
            transpose(new_eq);
        }
    }
    cout<<endl;
}

// 执行规则
            
void take_action(shared_ptr<Rete_Rule> instantiated_rule, shared_ptr<Rete_Question> question, shared_ptr<Rete_Network> rete_network, vector<shared_ptr<Rete_Rule>> &rules_not_worked){
    // 对于同一批激活的规则，在执行规则的 RHS 过程中个体在进行更新，可能会导致还未执行的规则中的某些个体变成 "过时的", 所以要重新进行 normalization
    question->normalize_individual(instantiated_rule);
    cout<<*instantiated_rule<<endl;
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
    bool worked = question->take_action(instantiated_rule, rete_network->underlying_kb);
    if(!worked)
        rules_not_worked.push_back(instantiated_rule);
    size_t new_fact_num = question->fact_list.size();
    size_t add_fact_num = new_fact_num-origin_fact_num; // 由该规则导致的新增 fact 的数量
    if(add_fact_num>0){
        // 对于新增的 fact，先检查是否已经存在 (只需检查最后一条是否已经存在)
        bool fact_existed = false;
        auto last_fact = *(question->fact_list.end()-1);
        for(size_t i=0; i!=question->fact_list.size()-1; ++i){
            if(question->fact_list[i]->get_output_str()==last_fact->get_output_str()){
                fact_existed = true;
                break;
            }
        }
        if(fact_existed){ // 如果已经存在，说明本次 take_action 是重复的
            question->fact_list.erase(question->fact_list.end()-add_fact_num);
            instantiated_rule->worked = true;
            return;
        }
        // 否则，需要在 Reasoning_Graph 中体现本次规则执行
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

// 执行推理
void reasoning(shared_ptr<Rete_Question> question, shared_ptr<Rete_Network> rete_network){
    cout<<"执行推理"<<endl;

    cout<<question->def_indi_hash_table.size()<<endl;

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
            if(fact->is_assert && fact->assertion->is_std){
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

    vector<shared_ptr<Rete_Rule>> rules_not_worked; // 记录未发挥作用的规则
    do{
        // 先尝试执行之前未发挥作用的规则
        for(size_t i =0; i!= rules_not_worked.size(); ++i){
            auto &r = rules_not_worked[i];
            if(!r->worked){
                cout<<"再次尝试执行: "<<*r<<endl;
                take_action(r, question, rete_network, rules_not_worked);
            }
        }
        // 找到可用的实例化后的规则
        vector<shared_ptr<Rete_Rule>> instantiated_rules = get_instantiated_rules(question, rete_network);        
        // 执行规则的 RHS
        for(auto instantiated_rule:instantiated_rules)
            take_action(instantiated_rule, question, rete_network, rules_not_worked);

        cout<<"rules_not_worked.size() = "<<rules_not_worked.size()<<endl;
        for(auto r:rules_not_worked){
            cout<<"\t"<<*r<<endl;
        }

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

        // 考虑解方程
        solve_eq(question, rules_not_worked);

        cout<<"rules_not_worked.size() = "<<rules_not_worked.size()<<endl;
        for(auto r:rules_not_worked){
            cout<<"\t"<<*r<<endl;
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
