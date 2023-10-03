#include "Rete_Network.hh"
// 构造 Rete 网络

// 往 Rete_Network 中添加 Rule
void add_rule(shared_ptr<Rete_Network> rete_network, shared_ptr<Rete_Rule> rule){
    // 每个子条件都涉及特定概念的个体，也就是涉及变量声明
    // 为每个变量声明创建 Concept_Node
    // cpt_node = build_or_share_cpt_node(rete_network, rule->var_decl);
    // 接下来处理 LHS

}

shared_ptr<Rete_Network> construct_rete(const shared_ptr<Knowledge_Base> kb){
    cout<<"开始构建 Rete 网络..."<<endl;
    return nullptr;
    // shared_ptr<Rete_Network> rete_network = make_shared<Rete_Network>();
    // for(auto rule:kb->rete_rules){ // 处理每一条规则
    //     add_rule(rete_network, rule);
    // }
}