#include "Rete_Network.hh"
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

// Rete 网络数据结构的部分成员函数及相关函数的实现

// 判断是否存在一条从起点到终点的路径
bool find_path(shared_ptr<Reasoning_Node> &start, shared_ptr<Reasoning_Node> &end, map<string,shared_ptr<Reasoning_Node>> &node_hash_table, set<shared_ptr<Reasoning_Node>> &reachable_node_set){
    // cout<<"起点为: "<<start->get_output_str()<<endl;
    // cout<<"终点为: "<<end->get_output_str()<<endl;
    if(start==end || reachable_node_set.find(start)!=reachable_node_set.end())
        return true;
    start->visited = true;
    for(auto out_edge:start->out_edges){
        string neighbor_str = out_edge->fact_end ? out_edge->fact_end->get_output_str() : out_edge->token_end->get_output_str();
        auto it = node_hash_table.find(neighbor_str);
        assert(it!=node_hash_table.end());
        auto &neighbor = it->second;
        // cout<<"当前邻居为: "<<neighbor->get_output_str()<<endl;
        if(neighbor->visited && reachable_node_set.find(neighbor)!=reachable_node_set.end()){
            reachable_node_set.insert(start);
            return true;
        }
        if(!neighbor->visited && find_path(neighbor,end,node_hash_table,reachable_node_set)){
            reachable_node_set.insert(start);
            return true;
        }
    }
    return false;
}

void Reasoning_Graph::print_solving_process(){ // 输出求解过程
    reasoning_graph->draw_all_progress();
    cout<<"边的数量为:"<<this->edges.size()<<endl;
    cout<<"Fact 点的数量为:"<<this->fact_nodes_hash_table.size()<<endl;
    cout<<"Token 点的数量为:"<<this->token_nodes_hash_table.size()<<endl;

    // 构建一个由点主导的图
    // 先把所有的 fact_nodes 和 token_nodes 统一为 Reasoning_Node
    map<string,shared_ptr<Reasoning_Node>> node_hash_table;
    for(auto it:fact_nodes_hash_table){
        auto fact_node = it.second;
        auto node = make_shared<Reasoning_Node>(fact_node);
        node_hash_table.insert(pair<string,shared_ptr<Reasoning_Node>>(node->get_output_str(),node));
    }
    for(auto it:token_nodes_hash_table){
        auto token_fact = it.second;
        auto node = make_shared<Reasoning_Node>(token_fact);
        node_hash_table.insert(pair<string,shared_ptr<Reasoning_Node>>(node->get_output_str(),node));
    }
    // 再遍历边来补充 Reasoning_Node 的入边和出边信息
    shared_ptr<Reasoning_Node> start, end;
    string start_str, end_str;
    for(const auto &edge:edges){
        start_str = edge->fact_start ? edge->fact_start->get_output_str() : edge->token_start->get_output_str();
        end_str = edge->fact_end ? edge->fact_end->get_output_str() : edge->token_end->get_output_str();
        auto it_start = node_hash_table.find(start_str);
        auto it_end = node_hash_table.find(end_str);
        assert(it_start!=node_hash_table.end() && it_end!=node_hash_table.end());
        start = it_start->second;
        end = it_end->second;
        start->out_edges.push_back(edge);
        end->in_edges.push_back(edge);
    }
    this->nodes = node_hash_table;
    #ifndef NDEBUG
        for(auto p:nodes){
            auto node = p.second;
            cout<<node->get_output_str()<<"  (入度: "<< node->in_edges.size() <<")  (出度: "<< node->out_edges.size() <<")"<<endl;
        }
    #endif

    auto to_solve = (*--edges.end())->fact_end; // 最后一条边的终点就是答案，也就是全图的终点
    assert(to_solve); // 目前的 to_solve 只有一个个体
    auto it = node_hash_table.find(to_solve->get_output_str());
    assert(it!=node_hash_table.end());
    shared_ptr<Reasoning_Node> end_node = it->second; // 全图的终点
    set<shared_ptr<Reasoning_Node>> reachable_node_set; // 存在到终点路径的所有节点
    set<shared_ptr<Reasoning_Node>> unreachable_node_set; // 不存在到终点路径的所有节点
    for(auto p:node_hash_table){
        auto &node = p.second;
        if(!find_path(node,end_node,node_hash_table,reachable_node_set)){
            unreachable_node_set.insert(node);
            cout<<"不存在到终点的路径: "<<node->get_output_str()<<endl;
        }
    }
    // 对于所有的不存在到终点路径的节点，删除其入边
    vector<shared_ptr<Reasoning_Edge>> new_edges; // 保留所有有用的边
    for(auto &edge:edges){
        end_str = edge->fact_end ? edge->fact_end->get_output_str() : edge->token_end->get_output_str();
        auto it_end = node_hash_table.find(end_str);
        assert(it_end!=node_hash_table.end());
        end = it_end->second;
        if(unreachable_node_set.find(end)==unreachable_node_set.end()) // 如果边的 end 不存在到终点的路径，则该边无用
            new_edges.push_back(edge);
    }
    this->edges = new_edges; // 保存到 Reasoning_Graph

    // 下面输出所有有用的边（这些边本身就是按照解题顺序生成的，直接顺序输出即可）
    size_t cnt = 0;
    cout<<"解:"<<endl;
    for(auto e:edges){
        if(e->instantiated_rule->description=="组合已知条件") // 这种边实际上并不是 "蕴含" 关系，只是为了保留 "Token 和 组成其的Fact 之间存在连接" 这一信息
            continue;
        cout<<"\t"<<++cnt<<". ";
        cout<< ((e->fact_start) ? e->fact_start->get_output_str() : e->token_start->get_output_str());
        cout<< " => ";
        cout<< ((e->fact_end) ? e->fact_end->get_output_str() : e->token_end->get_output_str())<<endl;
        cout<<"\t("<<e->instantiated_rule->description<<")"<<endl;
        // cout<<"\t"<<*e->instantiated_rule<<endl;
        cout<<endl;
    }
    draw_solving_process();
}

void Reasoning_Graph::print_all_progress(){ // 输出所有求解进展
    size_t cnt = 0;
    cout<<"所有的求解尝试:"<<endl;
    for(auto e:edges){
        if(e->instantiated_rule->description=="组合已知条件") // 这种边实际上并不是 "蕴含" 关系，只是为了保留 "Token 和 组成其的Fact 之间存在连接" 这一信息
            continue;
        cout<<"\t"<<++cnt<<". ";
        cout<< ((e->fact_start) ? e->fact_start->get_output_str() : e->token_start->get_output_str());
        cout<< " => ";
        cout<< ((e->fact_end) ? e->fact_end->get_output_str() : e->token_end->get_output_str())<<endl;
        // cout<<"\t("<<e->instantiated_rule->description<<")"<<endl;
        cout<<"\t"<<*e->instantiated_rule<<endl;
        cout<<endl;
    }
    draw_all_progress();
}

void Reasoning_Graph::draw_all_progress(){ // 可视化所有求解进展
    cout<<"开始绘制所有求解进展..."<<endl;
    GVC_t *gvc = gvContext();
    Agraph_t *g = agopen((char *)"all_progress", Agdirected, nullptr); // 创建新图
    Agnode_t *start = nullptr;
    Agnode_t *end = nullptr;
    Agedge_t *e = nullptr;
    string start_name, end_name, edge_name;
    
    for(auto edge:edges){
        start_name = (edge->fact_start) ? edge->fact_start->get_output_str() : edge->token_start->get_output_str();
        end_name = (edge->fact_end) ? edge->fact_end->get_output_str() : edge->token_end->get_output_str();
        edge_name = edge->instantiated_rule->description;
        // cout<<start_name<<" => "<<end_name<<endl;
        start = agnode(g, (char *)start_name.c_str(), 1);
        end = agnode(g, (char *)end_name.c_str(), 1);
        e = agedge(g, start, end, (char *)"", 1);
    }

    // layout + render
    gvLayout(gvc, g, "dot");
    // 输出结果放在当前目录下
    FILE *dotf = fopen("all_progress.dot", "w");
    FILE *pngf = fopen("all_progress.png", "w");
    gvRender(gvc, g, "dot", dotf);
    gvRender(gvc, g, "png", pngf);
    // 回收资源
    gvFreeLayout(gvc, g);
    agclose(g);
    std::cout<<"所有求解进展绘制完成, 详见 all_progress.png 文件!"<<endl;
}

void Reasoning_Graph::draw_solving_process(){ // 可视化所有求解进展
    cout<<"开始绘制求解过程..."<<endl;
    GVC_t *gvc = gvContext();
    Agraph_t *g = agopen((char *)"solving_process", Agdirected, nullptr); // 创建新图
    Agnode_t *start = nullptr;
    Agnode_t *end = nullptr;
    Agedge_t *e = nullptr;
    string start_name, end_name, edge_name;
    
    for(auto edge:edges){
        start_name = (edge->fact_start) ? edge->fact_start->get_output_str() : edge->token_start->get_output_str();
        end_name = (edge->fact_end) ? edge->fact_end->get_output_str() : edge->token_end->get_output_str();
        edge_name = edge->instantiated_rule->description;
        start = agnode(g, (char *)start_name.c_str(), 1);
        end = agnode(g, (char *)end_name.c_str(), 1);
        e = agedge(g, start, end, (char *)"", 1);

        auto it_start = nodes.find(start_name);
        auto it_end = nodes.find(end_name);
        auto start_node = it_start->second;
        auto end_node = it_end->second;

        // cout<<start_node->get_output_str()<<"  (入度: "<< start_node->in_edges.size() <<")  (出度: "<< start_node->out_edges.size() <<")"<<endl;
        // cout<<end_node->get_output_str()<<"  (入度: "<< end_node->in_edges.size() <<")  (出度: "<< end_node->out_edges.size() <<")"<<endl;

        if(start_node->in_edges.empty()){ // 初始已知条件的入度为 0
            agsafeset(start, (char *)"style", (char *)"filled", (char *)"");
            agsafeset(start, (char *)"fillcolor", (char *)"cornsilk", (char *)"");
        }
        if(end_node->out_edges.empty()){ // to_solve 的出度为 0
            agsafeset(end, (char *)"style", (char *)"filled", (char *)"");
            agsafeset(end, (char *)"fillcolor", (char *)"#ee7b67", (char *)"");
        }
    }

    // layout + render
    gvLayout(gvc, g, "dot");
    // 输出结果放在当前目录下
    FILE *dotf = fopen("solving_process.dot", "w");
    FILE *pngf = fopen("solving_process.png", "w");
    gvRender(gvc, g, "dot", dotf);
    gvRender(gvc, g, "png", pngf);
    // 回收资源
    gvFreeLayout(gvc, g);
    agclose(g);
    std::cout<<"求解过程绘制完成, 详见 solving_process.png 文件!"<<endl;
}

ostream& operator<<(ostream &os, const Token &e){
    string sep = "";
    for(auto fact:e.content){
        os<<sep<<*fact;
        sep = "; ";
    }
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
    cout<<endl<<"当前测试 "<<get_figure_info()<<endl;
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
    cout<<"Fact: "<<*fact<<(pass?"通过":"未通过")<<"当前测试"<<endl;
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
    this->facts.push_back(make_shared<Fact>(fact->get_copy()));
    cout<<"Fact: "<<*fact<<"保存在 "<<get_figure_info()<<endl;
    propagate_downward(fact);
}

void Concept_Memory::node_side_activation(shared_ptr<Fact> fact){ // 来自共同 Concept_Node 前驱的激活
    // 遍历另一个父 CM，如果在其 CM 中存在相同的 fact 则往下传递
    assert(mem_side_parent);
    #ifndef NDEBUG
        cout<<mem_side_parent->get_figure_info() <<" 中保存着以下 fact: "<<endl;
    #endif
    for(auto target:mem_side_parent->facts){
        #ifndef NDEBUG
            cout<< "\t"<<*target<<endl;
        #endif
        if(target->get_output_str()==fact->get_output_str()){
            #ifndef NDEBUG
                cout<< "该fact和"<<*fact<<"相同, 向下传播" <<endl;
            #endif
            fact->abstract_to_concrete.insert(target->abstract_to_concrete.begin(),target->abstract_to_concrete.end());
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
    for(auto target:node_side_parent->cm_child->facts){
        #ifndef NDEBUG
            cout<< "\t"<<*target<<endl;
        #endif
        if(*target==*fact){
            #ifndef NDEBUG
                cout<< "该fact和"<<*fact<<"相同, 向下传播" <<endl;
            #endif
            fact->abstract_to_concrete.insert(target->abstract_to_concrete.begin(),target->abstract_to_concrete.end());
            propagate_downward(fact);
        }
    }
}

bool Intra_Node::perform_existence_test(shared_ptr<Individual> constraint,shared_ptr<Fact> fact){ // 存在性测试
    // 直接检查当前的 fact 是不是目标事实
    return fact->get_output_str()==constraint->get_output_str();
}

bool Intra_Node::perform_predicate_test(shared_ptr<Sugar_For_Pred> test_constraint, shared_ptr<Fact> fact){ // 涉及 Sugar_For_Pred 的执行性测试
    // 谓词只会是: ">"、"<"、">="、"<="、"!="
    // 目前支持这些比较的值只会是 Number
    cout<<"进入到 predicate_test: "<<*test_constraint<<endl;
    // 把要测试的左右对象求出来
    auto left = test_constraint->left->find_specific_indi("Math_Expr", *fact->where_is);
    if(!left)
        left = action_eval(test_constraint->left,*fact->where_is);
    auto right = test_constraint->right->find_specific_indi("Math_Expr", *fact->where_is);
    if(!right)
        right = action_eval(test_constraint->right,*fact->where_is);
    bool ret = false;
    if(left && right){
        assert(left->math_indi->expr_val->is_num);
        assert(right->math_indi->expr_val->is_num);
        auto left_val = *left->math_indi->expr_val->number_val;
        auto right_val = *right->math_indi->expr_val->number_val;
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
    }

    return ret;
}

bool Intra_Node::perform_assertion_test(shared_ptr<Assertion> test_constraint, shared_ptr<Fact> fact){ // 涉及 Assertion 的执行性测试
    cout<<"进入到 assertion_test: "<<*test_constraint<<endl;
    bool pass = false;
    if(test_constraint->is_sugar_for_true){ // 如果是 Individual = true 的语法糖
        if(*intra_node_eval(test_constraint->lonely_left, fact) == *make_shared<Individual>(true))
            pass = true;
    }
    else{
        assert(test_constraint->is_std);
        pass = *intra_node_eval(test_constraint->left, fact) == *intra_node_eval(test_constraint->right, fact);
        assert(false);
    }
    return pass;
}

bool Intra_Node::perform_intra_test(shared_ptr<Fact> fact){ // 测试单个 fact 是否满足某个约束条件
    cout<<endl<<"当前测试 "<<get_figure_info()<<endl;
    /*
     * Intra_Node 测试可以分为两种:
     *    1. 存在性判断: 只需判断当前已知 fact 中是否存在这样的一条 fact，而由于在题目未解出的情况下所有的 fact 都会被先后送入 Rete 网络，所以检查当前的 fact 是不是目标事实也是等价的。这里我们使用的是后者
     *    2. 执行性判断: 根据对当前已知信息中的相关个体进行求值来判断条件是否成立
     * 
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
     * 其中 (a)、(b) 分支直接通过测试，而(c)、(d) 分支先要进行存在性测试，如若未通过再进行执行性测试
     */

    // 处理 (c)、(d) 分支都需要先根据当前的 abstract_to_concrete 实例化 constraint
    
    // 先检查 abstract_to_concrete 是否就是实例化所需要的
    for(auto p:var_decl){
        if(fact->abstract_to_concrete.find(p.first)==fact->abstract_to_concrete.end())
            return false;
    }

    auto origin_constraint = make_shared<Individual>(*constraint);
    constraint = constraint->instantiate(fact->abstract_to_concrete);
    cout<<"实例化之后的当前测试 "<<get_figure_info()<<endl;
    
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
            pass = perform_existence_test(constraint,fact);
            if(!pass)
                pass = perform_predicate_test(constraint->term->pred_val, fact);
        }
    }
    else{ // 分支 c
        assert(constraint->is_assertion);
        pass = perform_existence_test(constraint,fact);
        if(!pass)
            pass = perform_assertion_test(constraint->assertion, fact);
    }

    cout<<"Fact: "<<*fact<<(pass?"通过":"未通过")<<"当前测试"<<endl;

    constraint = origin_constraint; // 复原 constraint
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
    facts.push_back(make_shared<Fact>(fact->get_copy())); // 先保存该 fact 到 AM
    cout<<"保存 fact:"<<*fact<<" 到 "<<this->get_figure_info()<<endl;
    cout<<get_figure_info()<<" : "<<children.size()<<endl;
    for(auto join_node:children){
        join_node->alpha_side_activation(fact); // 再传递 fact 到后继 Join_Node
    }
}

bool Join_Node::perform_join_test(shared_ptr<Token> token, shared_ptr<Fact> fact){ // Join_Node 测试
    // cout<<"fact: "<<*fact<<endl;
    // cout<<"token: "<<*token<<endl;
    /*
     * fact中的abstract_to_concrete 和 token中的abstract_to_concrete不能冲突
     * 不能冲突有两方面的含义:
     * 1.对于fact中的每一个abstract，token中要么不存在同名的abstract，要么对应的concrete相同
     * 2.对于fact中的每一个concrete，token中要么不存在同名的concrete，要么对应的abstract相同
     */
    for(auto t_abs_to_con:token->abstract_to_concrete){
        for(auto f_abs_to_con:fact->abstract_to_concrete){
            bool abs_is_eq = t_abs_to_con.first==f_abs_to_con.first;
            bool con_is_eq = t_abs_to_con.second==f_abs_to_con.second;
            if(abs_is_eq + con_is_eq == 1)
                return false;
        }
    }
    cout<<"token: "<<*token<<" 和 fact: "<<*fact<<" 通过 join test!"<<endl;
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
    cout<<"原来的 token: "<<*this<<endl;
    cout<<"添加的 fact: "<<*fact<<endl;
    for(auto f:this->content){ // 已有 fact 不必重复添加
        if(f->get_output_str()==fact->get_output_str())
            return;
    }
    content.push_back(make_shared<Fact>(fact->get_copy())); // 增加 fact
    for(auto abs_to_con:fact->abstract_to_concrete){ // 更新 abstract_to_concrete
        this->abstract_to_concrete.insert(abs_to_con);
    }
    cout<<"新的 token: "<<*this<<endl;
    cout<<endl;
}

void Beta_Memory::activation(shared_ptr<Token> token, shared_ptr<Fact> fact){ // BM 激活
    token->extend(make_shared<Fact>(fact->get_copy()));
    this->tokens.push_back(token); // 先保存新的 token 到 BM // TODO: token->get_copy()
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
        edge->fact_start = reasoning_graph->share_or_build_fact_node(fact);
    }
    else{
        edge->token_start = reasoning_graph->share_or_build_token_node(token);
        trace_back(token);
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
