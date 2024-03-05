#ifndef RETE_NETWORK_HH
#define RETE_NETWORK_HH

// Rete 网络数据结构
#include "ke_struct.hh"
#include "global_var_decl.hh"

class Hash_Input{ // 用于建立哈希索引的对象
public:
    // 用 constraint(测试条件的字符串表达) 和 变量声明 来初始化
    Hash_Input(const string &s, const map<string, shared_ptr<Concept>> &var_info):val(pair<string, map<string, shared_ptr<Concept>>>(s,var_info)){}
    Hash_Input(){} // 默认构造
    
    
    bool operator<(const Hash_Input &rhs) const; // 重载 <
    bool operator==(const Hash_Input &rhs) const; // 重载 ==

    pair<string, map<string, shared_ptr<Concept>>> val; // 哈希对象值
};


inline bool Hash_Input::operator==(const Hash_Input &rhs) const{
    bool ret = true;
    if(val.first==rhs.val.first && val.second.size()==rhs.val.second.size()){
        for(auto it=val.second.begin(); it!=val.second.end(); ++it){
            auto it_r = rhs.val.second.find(it->first);
            if(it_r == rhs.val.second.end() || !(*it_r->second == *it->second)){
                ret = false;
                break;
            }
        }
    }
    return ret;
}

inline bool Hash_Input::operator<(const Hash_Input &rhs) const {
    if (val.first != rhs.val.first)
        return val.first < rhs.val.first;
    if (val.second.size() != rhs.val.second.size())
        return val.second.size() < rhs.val.second.size();

    // 按照字典顺序比较 map 中的元素
    auto it1 = val.second.begin();
    auto it2 = rhs.val.second.begin();

    while (it1 != val.second.end() && it2 != rhs.val.second.end()) {
        if (it1->first != it2->first) 
            return it1->first < it2->first;
        if (!(*(it1->second) == *(it2->second))) 
            return *(it1->second) < *(it2->second);
        ++it1;
        ++it2;
    }
    return false;
}



class Concept_Node;
class Rete_Node{ // 根节点
public:
    void activation(shared_ptr<Fact> fact); // 根节点激活

    vector<shared_ptr<Concept_Node>> concept_nodes; // 所有的最上层的 Concept_Node
};


class Concept_Memory;
class Concept_Node{ // 概念节点 （按 Concept 筛选 Fact）
public:
    // 用 变量声明（兼约束条件） 初始化
    Concept_Node(const pair<string, shared_ptr<Concept>> &var_info):constraint(var_info),var_decl(var_info){}

    // 要在 Rete 网络图输出的信息
    string get_figure_info(){return "Concept_Node: " + constraint.first + ':' + constraint.second->get_output_str();}
    
    vector<shared_ptr<Fact>> perform_concept_test(shared_ptr<Fact> fact); // 测试 fact 是否包含指定概念，返回通过测试的 fact
    void activation(shared_ptr<Fact> fact); // Concept_Node 激活

    pair<string, shared_ptr<Concept>> constraint; // 约束条件: 存在某概念的个体
    pair<string, shared_ptr<Concept>> var_decl; // 变量声明
    shared_ptr<Concept_Memory> cm_child = nullptr; // 对应的后继 Concept_Memory （涉及首层 Concept_Node）
    vector<shared_ptr<Concept_Memory>> cm_children; // (和其它 CM 的共同的) CM 孩子 （涉及第二层（以下）Concept_Node）
};

class Alpha_Memory;
class Intra_Node{ // 用于测试单个条件的节点
public:
    // 用 其孩子AM 初始化
    Intra_Node(shared_ptr<Alpha_Memory> child_am);

    // 要在 Rete 网络图输出的信息
    string get_figure_info(){return "Intra_Node: " + constraint->get_output_str() + " | " + str_of_var_decl(var_decl);}

    bool perform_intra_test(shared_ptr<Fact> fact); // 涉及单 fact 的测试
    void activation(shared_ptr<Fact> fact); // Intra_Node 激活

    shared_ptr<Individual> constraint = nullptr; // 约束条件
    map<string, shared_ptr<Concept>> var_decl; // 变量声明
    shared_ptr<Alpha_Memory> am = nullptr; // 对应的 AM
    shared_ptr<Concept_Memory> parent = nullptr; // 父 CM

private:
    bool perform_existence_test(shared_ptr<Individual> constraint,shared_ptr<Fact> fact); // 存在性测试
    bool perform_predicate_test(shared_ptr<Sugar_For_Pred> test_constraint, shared_ptr<Fact> fact); // 涉及 Sugar_For_Pred 的执行性测试
    bool perform_assertion_test(shared_ptr<Assertion> test_constraint, shared_ptr<Fact> fact); // 涉及 Assertion 的执行性测试
};


class Concept_Memory{ // 用于存放通过了概念节点的 Fact
public:
    // 用 Concept_Node 初始化（涉及首层 Concept_Node）
    Concept_Memory(shared_ptr<Concept_Node> node);
    // 用 Concept_Node和上层CM 重载（涉及第二层（以下）Concept_Node）
    Concept_Memory(shared_ptr<Concept_Node> node, shared_ptr<Concept_Memory> last_cm);

    // 要在 Rete 网络图输出的信息
    string get_figure_info(){return "CM: " + str_of_var_decl(constraint);}

    void immediate_activation(shared_ptr<Fact> fact); // 来自直接 Concept_Node 前驱的激活
    void node_side_activation(shared_ptr<Fact> fact); // 来自共同 Concept_Node 前驱的激活
    void mem_side_activation(shared_ptr<Fact> fact); // 来自共同 CM 前驱的激活


    map<string, shared_ptr<Concept>> constraint; // 约束条件: 存在某概念的个体
    map<string, shared_ptr<Concept>> var_decl; // 变量声明
    shared_ptr<Concept_Node> cpt_node_parent = nullptr; // 对应的前驱 Concept_Node （涉及首层 Concept_Node）
    vector<shared_ptr<Concept_Memory>> cm_children; // (和其它 Concept_Node 的共同的) CM 孩子 （涉及第二层（以下）Concept_Node）
    shared_ptr<Concept_Node> node_side_parent = nullptr; // Concept_Node 侧的共同父亲 （涉及第二层（以下）Concept_Node）
    shared_ptr<Concept_Memory> mem_side_parent = nullptr; // Concept_Memory 侧的共同父亲 （涉及第二层（以下）Concept_Node）
    vector<shared_ptr<Intra_Node>> intra_node_children; // 对应的后继 Intra_Node
    vector<shared_ptr<Fact>> facts; // 通过了 Concep_Node 测试的 fact

private:
    void propagate_downward(shared_ptr<Fact> fact); // 向下传播 fact
};

class Join_Node;
class Alpha_Memory{
public:
    // 用 Concept_Memory 初始化
    Alpha_Memory(Concept_Memory &cm):constraint(var_decl_to_indi(cm.var_decl)),var_decl(cm.var_decl){}
    // 用 Assertion 初始化
    Alpha_Memory(const Assertion &assertion):constraint(make_shared<Individual>(assertion)),var_decl(assertion.var_decl){}
    //用 Sugar_For_Pred 初始化
    Alpha_Memory(const Sugar_For_Pred &sugar_for_pred):constraint(make_shared<Individual>(*make_shared<Term>(sugar_for_pred))),var_decl(sugar_for_pred.var_decl){}
    //用 Sugar_For_And 初始化
    Alpha_Memory(const Sugar_For_And &sugar_for_and):constraint(make_shared<Individual>(*make_shared<Term>(sugar_for_and))),var_decl(sugar_for_and.var_decl){}
    //用 标准形式的Term 初始化
    Alpha_Memory(const Term &term):constraint(make_shared<Individual>(term)),var_decl(term.var_decl){}
    
    // 要在 Rete 网络图输出的信息
    string get_figure_info(){return "AM: " + constraint->get_output_str() + " | "  + str_of_var_decl(var_decl);}

    void activation(shared_ptr<Fact> fact); // AM 激活

    shared_ptr<Individual> constraint = nullptr; // 约束条件
    vector<shared_ptr<Join_Node>> children; // Join_Node 孩子
    shared_ptr<Intra_Node> parent = nullptr; // 父 Intra_Node
    map<string, shared_ptr<Concept>> var_decl; // 变量声明
    vector<shared_ptr<Fact>> facts; // 通过了 Intra_Node 测试的 fact
};

// 在Alpha_Memory定义完整后实现
inline Intra_Node::Intra_Node(shared_ptr<Alpha_Memory> child_am):constraint(child_am->constraint),var_decl(child_am->var_decl),am(child_am){}

class Token{ // Fact 的组合
public:
    // 用 vector<shared_ptr<Fact>> 初始化
    Token(vector<shared_ptr<Fact>> facts):content(facts){}
    Token(){}

    void extend(shared_ptr<Fact> fact); // 扩充 Token

    string get_output_str() const; // 获取输出字符串

    vector<shared_ptr<Fact>> content;
    map<string, string> abstract_to_concrete; // 约束变元的实例对应
};

ostream& operator<<(ostream &os, const Token &e);

inline string Token::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}

class Terminal_Node;
class Beta_Memory{
public:
    // // 用 Join_Node 初始化
    // Beta_Memory(shared_ptr<Join_Node> join_node){
    //     constraint = join_node->constraint;
    //     var_decl = join_node->var_decl;
    //     // 还需要处理 Join_Node的后继 和 自身的前驱
    //     auto this_bm = make_shared<Beta_Memory>(*this);
    //     join_node->children.push_back(this_bm);
    //     parent = join_node;
    // }
    // 本来的想法是使用 Join_Node对象 来初始化（如上），但由于到此 Join_Node 只是做了前向声明，不能操作该对象
    // 所以先创建一个原始的 Beta_Memory 对象，其成员被类内初始化，创建完毕再修改其成员值
    Beta_Memory() = default;

    string get_figure_info(); // 要在 Rete 网络图输出的信息
    void activation(shared_ptr<Token> token, shared_ptr<Fact> fact); // BM 激活

    shared_ptr<Individual> constraint = nullptr; // 约束条件
    shared_ptr<Join_Node> parent = nullptr; // 父 Join_Node
    vector<shared_ptr<Terminal_Node>> terminals; // 可能的终端节点孩子
    vector<shared_ptr<Join_Node>> children; // Join_Node 孩子
    map<string, shared_ptr<Concept>> var_decl; // 变量声明
    vector<shared_ptr<Token>> tokens; // 通过了 Join_Node 测试的 Token
};

class Join_Node{
public:
    // 用 AM 和 BM 初始化
    Join_Node(shared_ptr<Alpha_Memory> am, shared_ptr<Beta_Memory> bm);

    // 要在 Rete 网络图输出的信息
    string get_figure_info(){return "Join_Node: " + constraint->get_output_str() + " | "  + str_of_var_decl(var_decl);}

    void alpha_side_activation(shared_ptr<Fact> fact); // AM 传递 Fact 导致的激活
    void beta_side_activation(shared_ptr<Token> token); // BM 传递 Token 导致的激活

    shared_ptr<Individual> constraint = nullptr; // 约束条件
    shared_ptr<Beta_Memory> parent_bm = nullptr; // BM 父节点
    shared_ptr<Alpha_Memory> parent_am = nullptr; // AM 父节点
    vector<shared_ptr<Beta_Memory>> children; // Beta_Memory 孩子
    map<string, shared_ptr<Concept>> var_decl; // 变量声明

private:
    bool perform_join_test(shared_ptr<Token> token, shared_ptr<Fact> fact); // Join_Node 测试
};

class Conflict_Set{ // 冲突集
public: 
    vector<shared_ptr<Rete_Rule>> content; // 保存实例化后的 Rule
    set<string> rule_names; // 保存实例化后的 Rule 的字符串输出 (由于实例化后的 Rule 在执行动作时可能会改变，而又需要能够判断冲突集中是否存在相同规则)
};

class Terminal_Node{
public:
    // 用 BM 初始化
    Terminal_Node(shared_ptr<Beta_Memory> bm):constraint(bm->constraint),var_decl(bm->var_decl){}


    // 要在 Rete 网络图输出的信息
    // string get_figure_info(){return "Terminal: " + constraint->get_output_str() + " | "  + get_output_str(var_decl);}
    string get_figure_info(){return "Terminal: " + match_rule->get_output_str();}

    void activation(shared_ptr<Token> token); // Terminal_Node 激活

    shared_ptr<Individual> constraint = nullptr; // 约束条件
    map<string, shared_ptr<Concept>> var_decl; // 变量声明
    shared_ptr<Rete_Rule> match_rule = nullptr; // 匹配的 Rule
    shared_ptr<Conflict_Set> conflict_set = nullptr; // 冲突集
};

class Rete_Network{ // Rete 网络
public:
    //  Rete_Network 初始化伴随着创建根节点和dummy_top_node（dummy_top_node 的一个特点是其constraint值为nullptr）
    Rete_Network():root(make_shared<Rete_Node>()),dummy(make_shared<Beta_Memory>()),conflict_set(make_shared<Conflict_Set>()){}

    void add_fact(shared_ptr<Fact> fact); // 添加 fact 到 Rete Network

    shared_ptr<Rete_Node> root = nullptr; // 根节点
    shared_ptr<Beta_Memory> dummy = nullptr; // dummy_top_node
    vector<shared_ptr<Join_Node>> join_nodes;
    map<string, shared_ptr<Concept_Node>> cpt_node_hash_table; // Concept_Node 的哈希表 (Concept_Node 只需对变量声明的字符串输出做哈希)
    map<string, shared_ptr<Concept_Memory>> cm_hash_table; // Concept_Memory 的哈希表 (Concept_Memory 只需对变量声明的字符串输出做哈希)
    map<Hash_Input, shared_ptr<Alpha_Memory>> am_hash_table; // AM 的哈希表
    map<Hash_Input, shared_ptr<Beta_Memory>> bm_hash_table; // BM 的哈希表
    map<string, shared_ptr<Terminal_Node>> t_node_hash_table; // Terminal_Node 的哈希表
    shared_ptr<Conflict_Set> conflict_set = nullptr; // 冲突集
    shared_ptr<Knowledge_Base> underlying_kb = nullptr; // underlying 知识库
};

class Reasoning_Edge{ // 推理图的边
public:
    // 用 实例化后的规则 初始化
    Reasoning_Edge(shared_ptr<Rete_Rule> rule):instantiated_rule(rule){}

    shared_ptr<Rete_Rule> abstract_rule; // 原始规则
    shared_ptr<Rete_Rule> instantiated_rule; // 实例化之后的规则
    shared_ptr<Fact> fact_start; // 起点可能是 Fact
    shared_ptr<Token> token_start; // 起点可能是 Token
    shared_ptr<Fact> fact_end; // 终点可能是 Fact
    shared_ptr<Token> token_end; // 终点可能是 Token
};

class Reasoning_Node{ // 推理图的节点
public:
    // 用 shared_ptr<Fact> 初始化
    Reasoning_Node(shared_ptr<Fact> node):fact_node(node){}
    // 用 shared_ptr<Token> 初始化
    Reasoning_Node(shared_ptr<Token> node):token_node(node){}

    string get_output_str(){return fact_node ? fact_node->get_output_str() : token_node->get_output_str();}

    shared_ptr<Fact> fact_node; // 节点可能是 Fact
    shared_ptr<Token> token_node; // 节点可能是 Token
    vector<shared_ptr<Reasoning_Edge>> in_edges; // 入边
    vector<shared_ptr<Reasoning_Edge>> out_edges; // 出边
    bool visited = false; // 是否已经访问过
};

class Reasoning_Graph{ // 推理图
public:
    void print_solving_process(); // 输出求解过程
    void print_all_progress(); // 输出所有求解进展

    // Reasoning_Graph 中的节点不可重复

    shared_ptr<Fact> share_or_build_fact_node(shared_ptr<Fact> fact); // 添加或共享 Fact Node
    shared_ptr<Token> share_or_build_token_node(shared_ptr<Token> token); // 添加或共享 Token Node

    map<string,shared_ptr<Fact>> fact_nodes_hash_table; // 节点可能是 Fact
    map<string,shared_ptr<Token>> token_nodes_hash_table; // 节点可能是 Token
    vector<shared_ptr<Reasoning_Edge>> edges; // 边为 Rete_Rule

    map<string,shared_ptr<Reasoning_Node>> nodes; // 统一 fact_node 和 token_node

private:
    void draw_all_progress(); // 可视化所有求解进展
    void draw_solving_process(); // 可视化求解过程
};

inline shared_ptr<Fact> Reasoning_Graph::share_or_build_fact_node(shared_ptr<Fact> fact){ // 添加或共享 Fact Node
    auto it = fact_nodes_hash_table.find(fact->get_output_str());
    if(it!=fact_nodes_hash_table.end())
        return it->second;
    fact_nodes_hash_table.insert(pair<string,shared_ptr<Fact>>(fact->get_output_str(),fact));
    return fact;
}

inline shared_ptr<Token> Reasoning_Graph::share_or_build_token_node(shared_ptr<Token> token){ // 添加或共享 Token Node
    auto it = token_nodes_hash_table.find(token->get_output_str());
    if(it!=token_nodes_hash_table.end())
        return it->second;
    token_nodes_hash_table.insert(pair<string,shared_ptr<Token>>(token->get_output_str(),token));
    return token;
}


shared_ptr<Concept_Memory> build_or_share_cm(shared_ptr<Rete_Network> rete_network, shared_ptr<Concept_Node> node, shared_ptr<Concept_Memory> mem);
shared_ptr<Concept_Memory> build_or_share_cm(shared_ptr<Rete_Network> rete_network, const map<string, shared_ptr<Concept>> &var_decl);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Assertion> assertion);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Sugar_For_Pred> sugar_for_pred);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Sugar_For_And> sugar_for_and);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Term> term);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Individual> indi);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Concept_Memory> cm);
shared_ptr<Beta_Memory> build_or_share_bm(shared_ptr<Rete_Network> rete_network, shared_ptr<Join_Node> join_node);
shared_ptr<Terminal_Node> build_or_share_terminal_node(shared_ptr<Rete_Network> rete_network, shared_ptr<Beta_Memory> bm, shared_ptr<Rete_Rule> rule);
bool join_node_can_be_shared(shared_ptr<Join_Node> node, shared_ptr<Beta_Memory> bm, shared_ptr<Alpha_Memory> am);
shared_ptr<Join_Node> build_or_share_join_node(shared_ptr<Rete_Network> rete_network, shared_ptr<Alpha_Memory> am, shared_ptr<Beta_Memory> bm);
void add_rule(shared_ptr<Rete_Network> rete_network, shared_ptr<Rete_Rule> rule);
shared_ptr<Rete_Network> construct_rete(const shared_ptr<Knowledge_Base> kb);
void find_cm_for_intra_node(shared_ptr<Rete_Network> rete_network, shared_ptr<Intra_Node> intra_node);
shared_ptr<Individual> intra_node_eval(shared_ptr<Individual> indi, shared_ptr<Fact> fact);
void trace_back(shared_ptr<Token> token);
bool find_path(shared_ptr<Reasoning_Node> &start, shared_ptr<Reasoning_Node> &end, map<string,shared_ptr<Reasoning_Node>> &node_hash_table, set<shared_ptr<Reasoning_Node>> &reachable_node_set);
void sup_possible_alt(Individual &indi, Rete_Question &question);
bool binding_conflict(const map<string,string> &abs_to_con_1, const map<string,string> &abs_to_con_2);
void link_alt_to_solve(shared_ptr<Rete_Question> question, vector<shared_ptr<Rete_Rule>> &rules_not_worked);
void solve_self_contained_eq(shared_ptr<Rete_Question> question, vector<shared_ptr<Rete_Rule>> &rules_not_worked);

#endif