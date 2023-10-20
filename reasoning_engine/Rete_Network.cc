#include "Rete_Network.hh"

// Rete 网络数据结构的部分成员函数及相关函数的实现

ostream& operator<<(ostream &os, const Token &e){
    string sep = "";
    for(auto fact:e.content){
        os<<sep<<*fact;
        sep = "; ";
    }
    os<<endl;
    return os;
}

// 用 Concept_Node 初始化 Concept_Memory（涉及首层 Concept_Node）
Concept_Memory::Concept_Memory(shared_ptr<Concept_Node> node){ // 为首层 Concept_Node 构造 CM
    // 注意初始化参数必须用指针，而不能用引用（否则会因为成员值为不同的make_shared生成而导致数据不一致）
    constraint.insert(node->constraint);
    var_decl.insert(node->var_decl);
    // 还需要处理 前驱Concept_Node的自身后继 和 本身Concept_Memory的自身前驱
    // node.cm_child = make_shared<Concept_Memory>(*this); // 指向类对象自身的数据变动要在构造函数结束后执行
    cpt_node_parent = node;
}

// 用 Concept_Node和上层CM 重载（涉及第二层（以下）Concept_Node）
Concept_Memory::Concept_Memory(shared_ptr<Concept_Node> node, shared_ptr<Concept_Memory> last_cm){ // 为第二层（及以下）Concept_Node 构造 CM（不包含直接CM后继）
    auto new_constraint = last_cm->constraint;
    new_constraint.insert(node->constraint);
    constraint = new_constraint;
    auto new_var_decl = last_cm->var_decl;
    new_var_decl.insert(node->var_decl);
    var_decl = new_var_decl;
    // 还需要处理 前驱Concept_Node的共同后继、前驱Concept_Memory的共同后继 和 本身Concept_Memory的两侧的前驱
    // auto this_cm = make_shared<Concept_Memory>(*this); // 指向类对象自身的数据变动要在构造函数结束后执行
    // node->cm_children.push_back(this_cm);
    // last_cm->cm_children.push_back(this_cm);
    node_side_parent = node;
    mem_side_parent = last_cm;
}

void Concept_Memory::propagate_downward(shared_ptr<Fact> fact){ // Concept_Memory 向下传播 fact
    // 传递 fact 到共同 CM 后继进行激活
    for(auto co_child:cm_children){
        co_child->mem_side_activation(fact);
    }
    // 传递 fact 到 Intra_Node 后继进行激活
    for(auto child:intra_node_children){
        child->activation(fact);
    }
}

// 用 AM 和 BM 初始化 Join_Node
Join_Node::Join_Node(shared_ptr<Alpha_Memory> am, shared_ptr<Beta_Memory> bm){
    // 整合 AM 和 BM 的约束条件以构造新的约束条件
    if(bm->constraint==nullptr){ // BM 为 dummy_top_node
        constraint = am->constraint;
    }
    else{ // BM 不为 dummy_top_node
        bool am_is_and = am->constraint->is_term && am->constraint->term->is_and; // AM 是否是 sugar_for_and
        bool bm_is_and = bm->constraint->is_term && bm->constraint->term->is_and; // BM 是否是 sugar_for_and
        // 新的约束一定是由 sugar_for_and 提升上来的
        shared_ptr<Sugar_For_And> a_bigger_one; // 整合到一个更大的 sugar_for_and
        vector<shared_ptr<Individual>> temp_vec; // 用于存放 sugar_for_and 中的 Individual
        if(am_is_and && bm_is_and){
            temp_vec = am->constraint->term->and_val->content;
            auto bm_content = bm->constraint->term->and_val->content;
            temp_vec.insert(temp_vec.end(), bm_content.begin(), bm_content.end());
        }
        else if(am_is_and && !bm_is_and){
            temp_vec = am->constraint->term->and_val->content;
            temp_vec.push_back(bm->constraint);
        }
        else if(!am_is_and && bm_is_and){
            temp_vec = bm->constraint->term->and_val->content;
            temp_vec.push_back(am->constraint);
        }
        else{
            temp_vec.push_back(bm->constraint); // 保持原来 Condition 的顺序
            temp_vec.push_back(am->constraint);
        }
        a_bigger_one = make_shared<Sugar_For_And>(temp_vec);
        constraint = make_shared<Individual>(*make_shared<Term>(*a_bigger_one));
    }
    // 整合 AM 和 BM 的变量声明以构造新的变量声明
    for(const auto &p:am->var_decl)
        var_decl[p.first] = p.second;
    for(const auto &p:bm->var_decl)
        var_decl[p.first] = p.second;
    // 还需要处理 父AM的后继、夫BM的后继、节点自身的AM前驱和BM前驱
}

// Beta_Memory 要在 Rete 网络图输出的信息
string Beta_Memory::get_figure_info(){
    string ret;
    if(constraint==nullptr) // 为 dummy_top_node
        ret = "BM: dummy_top_node";
    else
        ret = "BM: " + constraint->get_output_str() + " | "  + str_of_var_decl(var_decl);
    return ret;
}


// 以下是 Rete 网络激活函数相关

void Rete_Node::activation(shared_ptr<Fact> fact){ // 根节点激活
    for(auto cpt_node:concept_nodes){
        cpt_node->activation(fact);
        fact->abstract_to_concrete.clear();
    }
}

bool Concept_Node::perform_concept_test(shared_ptr<Fact> fact){ // 测试fact是否包含指定概念
    #ifndef NDEBUG
        cout<<"当前测试 "<<get_figure_info()<<endl;
    #endif
    // Concept_Node 是用一对变量声明初始化的

    // fact 的变量声明中要存在一个指定的概念
    bool pass = false;
    for(auto var_info:fact->var_decl){
        if(*var_info.second==*constraint.second){
            fact->abstract_to_concrete.insert(pair<string,string>(constraint.first,var_info.first)); // 补充 abstract_to_concrete
            pass = true;
            break;
        }
    }
    #ifndef NDEBUG
        cout<<"Fact: "<<*fact<<(pass?"通过":"未通过")<<"当前测试"<<endl;
    #endif
    return pass;
}

void Concept_Node::activation(shared_ptr<Fact> fact){ // Concept_Node 激活
    if(perform_concept_test(fact)){
        // 把通过了 Concept_Node 测试的 fact 向后传递到 CM 后继
        // 对于直接后继，直接激活
        assert(cm_child);
        cm_child->immediate_activation(fact);
        // 对于共同后继，激活方式有所不同
        for(auto com_child:cm_children){
            com_child->node_side_activation(fact);
        }
    }
}

void Concept_Memory::immediate_activation(shared_ptr<Fact> fact){ // 来自直接 Concept_Node 前驱的激活
    // 先保存该 fact 到 CM
    this->facts.push_back(fact);
    #ifndef NDEBUG
        cout<<"Fact: "<<*fact<<"保存在 "<<get_figure_info()<<endl;
    #endif
    propagate_downward(fact);
}

void Concept_Memory::node_side_activation(shared_ptr<Fact> fact){ // 来自共同 Concept_Node 前驱的激活
    // 遍历另一个父 CM，如果在其 CM 中存在相同的 fact 则往下传递
    assert(mem_side_parent);
    #ifndef NDEBUG
        cout<<mem_side_parent->get_figure_info() <<" 中保存着以下 fact: "<<endl;
    #endif
    for(auto targer:mem_side_parent->facts){
        #ifndef NDEBUG
            cout<< "\t"<<*targer<<endl;
        #endif
        if(*targer==*fact){
            #ifndef NDEBUG
                cout<< "该fact和"<<*fact<<"相同, 向下传播" <<endl;
            #endif
            propagate_downward(fact);
        }
    }
}

void Concept_Memory::mem_side_activation(shared_ptr<Fact> fact){ // 来自共同 CM 前驱的激活
    // 遍历另一个父 Concept_Node，如果在其 CM 中存在相同的 fact 则往下传递
    assert(node_side_parent);
    #ifndef NDEBUG
        cout<<node_side_parent->cm_child->get_figure_info()<<" 中保存着以下 fact: "<<endl;
    #endif
    for(auto targer:node_side_parent->cm_child->facts){
        #ifndef NDEBUG
            cout<< "\t"<<*targer<<endl;
        #endif
        if(*targer==*fact){
            #ifndef NDEBUG
                cout<< "该fact和"<<*fact<<"相同, 向下传播" <<endl;
            #endif
            propagate_downward(fact);
        }
    }
}

// 求出抽象的个体在给定已知 fact 时的实例化结果
shared_ptr<Individual> get_con_indi(shared_ptr<Individual> abs_indi,shared_ptr<Fact> fact){
    cout<<endl<<"get_con_indi 要求抽象的个体在给定已知 fact 时的实例化结果"<<endl;
    cout<<"当前的 抽象个体为: "<<*abs_indi<<endl;
    cout<<"当前的 fact 为: "<<*fact<<endl;
    shared_ptr<Individual> ret;
    assert(abs_indi->is_term); // 只处理 Term
    auto t = abs_indi->term;
    if(t->is_oprt_apply){ // 这里的 Term 只会是 sugar_for_oprt_apply 或 标准形式
        string abs_caller_name = t->oprt_apply_val->indi; // sugar_for_oprt_apply 的调用者
        // 找到调用者对应的实例个体
        string con_caller_name; // 在当前 fact 下的实际调用者
        for(auto abs_to_con:fact->abstract_to_concrete){
            if(abs_to_con.first == abs_caller_name){
                con_caller_name = abs_to_con.second;
                break;
            }
            assert(false);
        }
        shared_ptr<Individual> con_caller; // 实际调用(定义)个体的定义值
        // 在当前的定义个体中寻找实际调用者个体
        assert(fact->where_is && fact->where_is->def_indi_hash_table.size());
        for(auto p:fact->where_is->def_indi_hash_table){
            auto def_indi = p.second;
            if(def_indi->symbol==con_caller_name){
                con_caller = def_indi->indi;
                break;
            }
            assert(false);
        }
        // 查询(定义)个体的某个属性值
        string uni_oprt = t->oprt_apply_val->uni_oprt; // 调用的一元算子
        assert(con_caller && con_caller->is_term && con_caller->term->is_ctor);
        for(auto assignment:con_caller->term->ctor_val->content){
            if(assignment->symbol==uni_oprt)
                ret = assignment->val;
        }
    }
    else{
        assert(t->is_std);
        // TODO:实现
    }
    assert(ret);
    cout<<"实例化结果为: "<<*ret<<endl;
    return ret;
}

bool Intra_Node::perform_predicate_test(shared_ptr<Sugar_For_Pred> test_constraint, shared_ptr<Fact> fact){ // 涉及 Sugar_For_Pred 的测试
    // 谓词只会是: ">"、"<"、">="、"<="、"!="
    // 目前支持这些比较的值只会是 Number
    #ifndef NDEBUG
        cout<<"进入到 predicate_test: "<<*test_constraint<<endl;
    #endif
    // 把要测试的左右对象求出来
    auto con_left = get_con_indi(test_constraint->left,fact);
    auto con_right = get_con_indi(test_constraint->right,fact);
    assert(con_left->is_math_indi && con_left->math_indi->is_math_expr && con_left->math_indi->expr_val->is_num);
    assert(con_right->is_math_indi && con_right->math_indi->is_math_expr && con_right->math_indi->expr_val->is_num);
    auto left_val = *con_left->math_indi->expr_val->number_val;
    auto right_val = *con_right->math_indi->expr_val->number_val;
    bool ret = false;
    auto predicate = test_constraint->predicate;
    if(predicate == ">")
        ret = left_val > right_val;
    else if(predicate == "<")
        ret = left_val < right_val;
    else if(predicate == ">=")
        ret = left_val >= right_val;
    else if(predicate == "<=")
        ret = left_val <= right_val;
    else if(predicate == "!=")
        ret = left_val != right_val;

    return ret;
}

bool Intra_Node::perform_assertion_test(shared_ptr<Assertion> test_constraint, shared_ptr<Fact> fact){ // 涉及 Assertion 的测试
    #ifndef NDEBUG
        cout<<"进入到 assertion_test: "<<*test_constraint<<endl;
    #endif
    // TODO:待实现 is_std 的情况
    bool pass = false;
    if(test_constraint->is_sugar_for_true){ // 如果是 Individual = true 的语法糖
        // lonely_left 是一个标准形式的 term
        auto lonely_term = test_constraint->lonely_left->term;
        assert(test_constraint->lonely_left->is_term && lonely_term->is_std);
        auto oprt = lonely_term->oprt;
        auto args = lonely_term->args;
        if(*eval(lonely_term, fact) == *make_shared<Individual>(true))
            pass = true;
    }
    return pass;
}


bool Intra_Node::perform_intra_test(shared_ptr<Fact> fact){ // 测试单个 fact 是否满足某个约束条件
    #ifndef NDEBUG
        cout<<"当前测试 "<<get_figure_info()<<endl;
    #endif
    /*
     * Intra_Node 是由 AM 衍生出来的，负责执行某个条件的测试，可以根据 AM 的构造过程来分析出所需要进行的测试 (可参考函数 construct_rete）
     * 
     * 1. 当 LHS 为空时，AM 直接根据 CM 创造（由于 CM 的约束是变量声明，所以会经历一个从 map<string,shared<Concept>> 到 shared_ptr<Individual> 的转变）
     * 该情况下，Intra_Node 的 constraint 可能是 (a)单个Variable提升上来的 或 (b)多个(a)组成的sugar_for_and提升上来的 (可参考函数 var_decl_to_indi)
     * 由于 AM 是直接根据 CM 创造的，所以该情况下是不需要执行测试的，可以直接通过
     * 
     * 2. 当 LHS 含单个子条件时，该子条件可能是 assertion 或 sugar_for_pred，AM 根据 assertion 或 sugar_for_pred 创造
     * 该情况下，Intra_Node 的 constraint 可能是 (c)Assertion提升上来的 或 (d)Sugar_For_Pred提升上来的
     * 
     * 3. 当 LHS 含多个子条件时，每个子条件可能是 individual，我们要需要考虑的是其子条件的 AM 和 Intra_Node
     * 该情况下，Intra_Node 的 constraint 是 (e) 一般的 individual (更具体的说，是 assertion 或 sugar_for_pred)
     * 而这可以递归到情况(2)
     * 
     * 综上，Intra_Node 的 constraint 要分析4种情况: (a)、(b)、(c)、(d)
     */
    
    // TODO:重构
    // 处理 (c)、(d) 分支都需要先根据当前的 abstract_to_concrete 实例化 constraint
    // auto origin_constraint = make_shared<Individual>(*constraint);
    // constraint = constraint->instantiate(fact->abstract_to_concrete);
    // #ifndef NDEBUG
    //     cout<<"实例化之后的当前测试 "<<get_figure_info()<<endl;
    // #endif

    bool pass = false;
    // assert(constraint->is_var + constraint->is_term + constraint->is_assertion == 1);
    if(constraint->is_var){ // 分支 a
        pass = true;
    }
    else if(constraint->is_term){
        // assert(constraint->term->is_and + constraint->term->is_pred == 1);
        if(constraint->term->is_and){
            bool is_multi_var = true; // 分支 b
            for(auto arg:constraint->term->and_val->content){
                if(!arg->is_var){
                    is_multi_var = false;
                    break;
                }
            }
            pass = is_multi_var;
        }
        else{ // 分支 d
            assert(constraint->term->is_pred);
            pass = perform_predicate_test(constraint->term->pred_val, fact);
        }
    }
    else{ // 分支 c
        assert(constraint->is_assertion);
        pass = perform_assertion_test(constraint->assertion, fact);
    }

    #ifndef NDEBUG
        cout<<"Fact: "<<*fact<<(pass?"通过":"未通过")<<"当前测试"<<endl;
    #endif

    // constraint = origin_constraint;
    return pass;
}

void Intra_Node::activation(shared_ptr<Fact> fact){ // Intra_Node 激活
    if(perform_intra_test(fact)){
        // 把通过了 Intra_Node 测试的 fact 向后传递到 AM 后继
        assert(am);
        am->activation(fact);
    }
}

void Alpha_Memory::activation(shared_ptr<Fact> fact){ // AM 激活
    facts.push_back(fact); // 先保存该 fact 到 AM
    cout<<"保存 fact:"<<*fact<<" 到 "<<this->get_figure_info()<<endl;
    for(auto join_node:children){
        join_node->alpha_side_activation(fact); // 再传递 fact 到后继 Join_Node
    }
}

bool Join_Node::perform_join_test(shared_ptr<Token> token, shared_ptr<Fact> fact){ // Join_Node 测试
    // TODO: implement
    return true;
}

void Join_Node::alpha_side_activation(shared_ptr<Fact> fact){ // AM 传递 Fact 导致的激活
    if(!parent_bm->constraint){ // 如果 父BM 是 dummy_top_node，无需测试
        shared_ptr<Token> token = make_shared<Token>();
        for(auto bm:children){
            bm->activation(token, fact); // 把空 token 和 fact 向后传递到 BM
        }
    }
    else{ // 否则要遍历 父BM 中的 token 进行 Join_Node 测试
        for(auto token:parent_bm->tokens){
            if(perform_join_test(token,fact)){
                for(auto bm:this->children){
                    bm->activation(token, fact); // 把通过了 Join_Node 测试的 token 和 fact 向后传递到 BM
                }
            }
        }
    }
}

void Join_Node::beta_side_activation(shared_ptr<Token> token){ // BM 传递 Token 导致的激活
    // 要遍历 父AM 中的 fact 进行 Join_Node 测试
    for(auto fact:parent_am->facts){
        if(perform_join_test(token, fact)){
            for(auto bm:this->children){
                bm->activation(token, fact); // 把通过了 Join_Node 测试的 token 和 fact 向后传递到 BM
            }
        }
    }
}

void Token::extend(shared_ptr<Fact> fact){ // 扩充 Token
    content.push_back(fact); // 增加 fact
    for(auto abs_to_con:fact->abstract_to_concrete){ // 更新 abstract_to_concrete
        this->abstract_to_concrete.insert(abs_to_con);
    }
}

void Beta_Memory::activation(shared_ptr<Token> token, shared_ptr<Fact> fact){ // BM 激活
    token->extend(fact);
    this->tokens.push_back(token); // 先保存新的 token 到 BM
    for(auto join_node:children){
        join_node->beta_side_activation(token); // 再传递新 token 到后继 Join_Node
    }
    for(auto terminal:terminals){
        terminal->activation(token); // 再传递新 token 到后继 Terminal_Node
    }
}

void Terminal_Node::activation(shared_ptr<Token> token){ // Terminal_Node 激活
    cout<<"当前 Terminal: "<<*match_rule<<endl;
    cout<<"当前 Token: "<<*token<<endl;
    shared_ptr<Rete_Rule> new_rule = match_rule->instantiate(token->abstract_to_concrete); // grounding
    // 要将实例化后的规则添加进冲突集，除非当前冲突集中已经存在相同的规则
    string new_rule_name = new_rule->get_output_str();
    if(this->conflict_set->rule_names.find(new_rule_name)!=conflict_set->rule_names.end()){
        cout<<"已经添加过该规则, 不重复添加!"<<endl;
        return;
    }
    conflict_set->content.push_back(new_rule);
    conflict_set->rule_names.insert(new_rule_name);
    cout<<"把该规则添加至冲突集!"<<endl;

    // 把当前的 fact 实例化规则的信息加入 Reasoning_Graph
    shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
    edge->abstract_rule = match_rule;
    if(token->content.size()==1){
        auto fact = *token->content.begin(); // 唯一的 fact
        edge->fact_start = fact;
        reasoning_graph->fact_nodes_hash_table.insert(pair<string,shared_ptr<Fact>>(fact->get_output_str(),fact));
    }
    else{
        edge->token_start = token;
        reasoning_graph->token_nodes_hash_table.insert(pair<string,shared_ptr<Token>>(token->get_output_str(),token));
    }
    reasoning_graph->edges.push_back(edge);
}

// 添加 fact 到 Rete Network
void Rete_Network::add_fact(shared_ptr<Fact> fact){
    cout<<"把以下 Fact 加入 Rete 网络: "<<fact->get_output_str()<<endl;
    root->activation(fact); // 激活根节点
    fact->has_been_added = true;
    cout<<endl<<endl;
}
