#ifndef RETE_NETWORK_HH
#define RETE_NETWORK_HH

// Rete 网络数据结构
#include "ke_struct.hh"

class Hash_Input{ // 用于建立哈希索引的对象
public:
    // 用 constraint(测试条件的字符串表达) 和 变量声明 来初始化
    Hash_Input(const string &s, const map<string, shared_ptr<Concept>> &var_info):val(pair<string, map<string, shared_ptr<Concept>>>(s,var_info)){}
    Hash_Input(){} // 默认构造
    // 拷贝构造
    Hash_Input(const Hash_Input &rhs):val(rhs.val){}
    // 拷贝赋值
    Hash_Input& operator=(const Hash_Input &rhs){val=rhs.val;return *this;}

    // 重载 <
    bool operator<(const Hash_Input &rhs) const{return val<rhs.val;}
    // 重载 ==
    bool operator==(const Hash_Input &rhs) const{return val==rhs.val;}

    pair<string, map<string, shared_ptr<Concept>>> val; // 哈希对象值
};

class Concept_Node;
class Rete_Node{ // 根节点
public:
    vector<shared_ptr<Concept_Node>> concept_nodes; // 所有的最上层的 Concept_Node
};


class Concept_Memory;
class Concept_Node{ // 概念节点 （按 Concept 筛选 Fact）
public:
    // 用 变量声明（兼约束条件） 初始化
    Concept_Node(const pair<string, shared_ptr<Concept>> &var_info):constraint(var_info),var_decl(var_info){}

    // 要在 Rete 网络图输出的信息
    string get_figure_info(){return "Concept_Node: " + constraint.first + ':' + constraint.second->get_output_str();}
    
    pair<string, shared_ptr<Concept>> constraint; // 约束条件: 存在某概念的个体
    pair<string, shared_ptr<Concept>> var_decl; // 变量声明
    shared_ptr<Concept_Memory> cm_child = nullptr; // 对应的后继 Concept_Memory （涉及首层 Concept_Node）
    vector<shared_ptr<Concept_Memory>> cm_children; // (和其它 CM 的共同的) CM 孩子 （涉及第二层（以下）Concept_Node）
    pair<string, string> abstract_to_concrete; // 约束变元的实例对应
};

class Alpha_Memory;
class Intra_Node{ // 用于测试单个条件的节点
public:
    // 用 其孩子AM 初始化
    Intra_Node(shared_ptr<Alpha_Memory> child_am);

    // 要在 Rete 网络图输出的信息
    string get_figure_info(){return "Intra_Node: " + constraint->get_output_str() + " | " + get_output_str(var_decl);}

    shared_ptr<Individual> constraint = nullptr; // 约束条件
    map<string, shared_ptr<Concept>> var_decl; // 变量声明
    shared_ptr<Alpha_Memory> am = nullptr; // 对应的 AM
    shared_ptr<Concept_Memory> parent = nullptr; // 父 CM
    map<string, string> abstract_to_concrete; // 约束变元的实例对应
};


class Concept_Memory{ // 用于存放通过了概念节点的 Fact
public:
    // 用 Concept_Node 初始化（涉及首层 Concept_Node）
    Concept_Memory(shared_ptr<Concept_Node> node){ // 为首层 Concept_Node 构造 CM
        // 注意初始化参数必须用指针，而不能用引用（否则会因为成员值为不同的make_shared生成而导致数据不一致）
        constraint.insert(node->constraint);
        var_decl.insert(node->var_decl);
        // 还需要处理 前驱Concept_Node的自身后继 和 本身Concept_Memory的自身前驱
        // node.cm_child = make_shared<Concept_Memory>(*this); // 指向类对象自身的数据变动要在构造函数结束后执行
        cpt_node_parent = node;
    }
    // 用 Concept_Node和上层CM 重载（涉及第二层（以下）Concept_Node）
    Concept_Memory(shared_ptr<Concept_Node> node, shared_ptr<Concept_Memory> last_cm){ // 为第二层（及以下）Concept_Node 构造 CM（不包含直接CM后继）
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

    // 要在 Rete 网络图输出的信息
    string get_figure_info(){return "CM: " + get_output_str(constraint);}


    map<string, shared_ptr<Concept>> constraint; // 约束条件: 存在某概念的个体
    map<string, shared_ptr<Concept>> var_decl; // 变量声明
    shared_ptr<Concept_Node> cpt_node_parent = nullptr; // 对应的前驱 Concept_Node （涉及首层 Concept_Node）
    vector<shared_ptr<Concept_Memory>> cm_children; // (和其它 Concept_Node 的共同的) CM 孩子 （涉及第二层（以下）Concept_Node）
    shared_ptr<Concept_Node> node_side_parent = nullptr; // Concept_Node 侧的共同父亲 （涉及第二层（以下）Concept_Node）
    shared_ptr<Concept_Memory> mem_side_parent = nullptr; // Concept_Memory 侧的共同父亲 （涉及第二层（以下）Concept_Node）
    vector<shared_ptr<Intra_Node>> intra_node_children; // 对应的后继 Intra_Node
    map<string, string> abstract_to_concrete; // 约束变元的实例对应
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
    string get_figure_info(){return "AM: " + constraint->get_output_str() + " | "  + get_output_str(var_decl);}

    shared_ptr<Individual> constraint = nullptr; // 约束条件
    vector<shared_ptr<Join_Node>> children; // Join_Node 孩子
    shared_ptr<Intra_Node> parent = nullptr; // 父 Intra_Node
    map<string, shared_ptr<Concept>> var_decl; // 变量声明
    map<string, string> abstract_to_concrete; // 约束变元的实例对应
};

// 在Alpha_Memory定义完整后实现
inline Intra_Node::Intra_Node(shared_ptr<Alpha_Memory> child_am):constraint(child_am->constraint),var_decl(child_am->var_decl),am(child_am){}

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

    // 要在 Rete 网络图输出的信息
    string get_figure_info(){
        string ret;
        if(constraint==nullptr) // 为 dummy_top_node
            ret = "BM: dummy_top_node";
        else
            ret = "BM: " + constraint->get_output_str() + " | "  + get_output_str(var_decl);
        return ret;
    }


    shared_ptr<Individual> constraint = nullptr; // 约束条件
    shared_ptr<Join_Node> parent = nullptr; // 父 Join_Node
    shared_ptr<Terminal_Node> terminal = nullptr; // 可能的终端节点
    vector<shared_ptr<Join_Node>> children;
    map<string, shared_ptr<Concept>> var_decl; // 变量声明
    map<string, string> abstract_to_concrete; // 约束变元的实例对应
};

class Join_Node{
public:
    // 用 AM 和 BM 初始化
    Join_Node(shared_ptr<Alpha_Memory> am, shared_ptr<Beta_Memory> bm){
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
        auto this_node = make_shared<Join_Node>(*this);
        am->children.push_back(this_node);
        bm->children.push_back(this_node);
        parent_am = am;
        parent_bm = bm;
    }

    // 要在 Rete 网络图输出的信息
    string get_figure_info(){return "Join_Node: " + constraint->get_output_str() + " | "  + get_output_str(var_decl);}


    shared_ptr<Individual> constraint = nullptr; // 约束条件
    shared_ptr<Beta_Memory> parent_bm = nullptr; // BM 父节点
    shared_ptr<Alpha_Memory> parent_am = nullptr; // AM 父节点
    vector<shared_ptr<Beta_Memory>> children; // Beta_Memory 孩子
    map<string, shared_ptr<Concept>> var_decl; // 变量声明
    map<string, string> abstract_to_concrete; // 约束变元的实例对应
};

class Terminal_Node{
public:
    // 用 BM 初始化
    Terminal_Node(shared_ptr<Beta_Memory> bm):constraint(bm->constraint),var_decl(bm->var_decl){
        // 还要处理 父BM的后继
        auto this_terminal = make_shared<Terminal_Node>(*this);
        bm->terminal = this_terminal;
    }

    // 要在 Rete 网络图输出的信息
    string get_figure_info(){return "Terminal: " + constraint->get_output_str() + " | "  + get_output_str(var_decl);}


    shared_ptr<Individual> constraint = nullptr; // 约束条件
    map<string, shared_ptr<Concept>> var_decl; // 变量声明
    map<string, string> abstract_to_concrete; // 约束变元的实例对应
    shared_ptr<Rete_Rule> match_rule = nullptr; // 匹配的 Rule
};


class Rete_Network{ // Rete 网络
public:
    //  Rete_Network 初始化伴随着创建根节点和dummy_top_node（dummy_top_node 的一个特点是其constraint值为nullptr）
    Rete_Network():root(make_shared<Rete_Node>()),dummy(make_shared<Beta_Memory>()){}

    shared_ptr<Rete_Node> root = nullptr; // 根节点
    shared_ptr<Beta_Memory> dummy = nullptr; // dummy_top_node
    vector<shared_ptr<Join_Node>> join_nodes;
    map<string, shared_ptr<Concept_Node>> cpt_node_hash_table; // Concept_Node 的哈希表 (Concept_Node 只需对变量声明的字符串输出做哈希)
    map<string, shared_ptr<Concept_Memory>> cm_hash_table; // Concept_Memory 的哈希表 (Concept_Memory 只需对变量声明的字符串输出做哈希)
    map<Hash_Input, shared_ptr<Alpha_Memory>> am_hash_table; // AM 的哈希表
    map<Hash_Input, shared_ptr<Beta_Memory>> bm_hash_table; // BM 的哈希表
    map<Hash_Input, shared_ptr<Terminal_Node>> t_node_hash_table; // Terminal_Node 的哈希表
};


shared_ptr<Concept_Memory> build_or_share_cm(shared_ptr<Rete_Network> rete_network, shared_ptr<Concept_Node> node, shared_ptr<Concept_Memory> mem);
shared_ptr<Concept_Memory> build_or_share_cm(shared_ptr<Rete_Network> rete_network, const map<string, shared_ptr<Concept>> &var_decl);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Assertion> assertion);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Sugar_For_Pred> sugar_for_pred);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Sugar_For_And> sugar_for_and);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Term> term);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Individual> indi);
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Concept_Memory> cm);
shared_ptr<Beta_Memory> build_or_share_bm(shared_ptr<Rete_Network> rete_network, shared_ptr<Join_Node> join_node);
shared_ptr<Terminal_Node> build_or_share_terminal_node(shared_ptr<Rete_Network> rete_network, shared_ptr<Beta_Memory> bm);
bool join_node_can_be_shared(shared_ptr<Join_Node> node, shared_ptr<Beta_Memory> bm, shared_ptr<Alpha_Memory> am);
shared_ptr<Join_Node> build_or_share_join_node(shared_ptr<Rete_Network> rete_network, shared_ptr<Alpha_Memory> am, shared_ptr<Beta_Memory> bm);
void add_rule(shared_ptr<Rete_Network> rete_network, shared_ptr<Rete_Rule> rule);
shared_ptr<Rete_Network> construct_rete(const shared_ptr<Knowledge_Base> kb);
void find_cm_for_intra_node(shared_ptr<Rete_Network> rete_network, shared_ptr<Intra_Node> intra_node);

#endif