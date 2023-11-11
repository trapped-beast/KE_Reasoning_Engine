#include "Rete_Network.hh"
// 构造 Rete 网络

// 为 Concept_Node和Concept_Memory 创建 Concept_Memory
shared_ptr<Concept_Memory> build_or_share_cm(shared_ptr<Rete_Network> rete_network, shared_ptr<Concept_Node> node, shared_ptr<Concept_Memory> mem){
    // 先判断是否有可共享的 CM
    auto new_var_decl = mem->var_decl;
    new_var_decl.insert(node->var_decl);
    auto it = rete_network->cm_hash_table.find(str_of_var_decl(new_var_decl));
    if(it!=rete_network->cm_hash_table.end())
        return it->second;
    // 如果没有可共享的 CM，为该变量声明构造 CM
    auto ret = make_shared<Concept_Memory>(node, mem);
    // 指向 Concept_Memory 对象自身的数据变动要在构造函数结束后执行
    // 还需要处理 前驱Concept_Node的共同后继、前驱Concept_Memory的共同后继
    node->cm_children.push_back(ret);
    mem->cm_children.push_back(ret);
    return ret;
}

// 根据 变量声明 创建 Concept_Memory
shared_ptr<Concept_Memory> build_or_share_cm(shared_ptr<Rete_Network> rete_network, const map<string, shared_ptr<Concept>> &var_decl){
    // 先判断是否有可共享的 CM
    auto it = rete_network->cm_hash_table.find(str_of_var_decl(var_decl));
    if(it!=rete_network->cm_hash_table.end())
        return it->second;
    // 如果没有可共享的 CM，为该变量声明构造 CM
    shared_ptr<Concept_Node> cpt_node;
    shared_ptr<Concept_Memory> cm;
    if(var_decl.size()==1){ // 只有一对变量声明
        cpt_node = make_shared<Concept_Node>(*var_decl.begin()); // 先构造 Concept_Node
        cm = make_shared<Concept_Memory>(cpt_node); // 再构造 CM
        // 指向 Concept_Memory 对象自身的数据变动要在构造函数结束后执行
        // 还需要处理 前驱Concept_Node的自身后继
        cpt_node->cm_child = cm;
        rete_network->root->concept_nodes.push_back(cpt_node); // 把该 Concept_Node 加到根节点后
        rete_network->cm_hash_table.insert(pair<string, shared_ptr<Concept_Memory>>(str_of_var_decl(cm->constraint), cm)); // 保存 CM 到哈希表
    }
    else{ // 有多对变量声明
        // 为第一对变量声明构造 CM
        auto it = var_decl.begin();
        map<string, shared_ptr<Concept>> temp_var_decl;
        temp_var_decl.insert(*it++);
        cm = build_or_share_cm(rete_network, temp_var_decl);
        // 依次为其余变量声明构造 CM
        while(it!=var_decl.end()){
            temp_var_decl.clear();
            temp_var_decl.insert(*it);
            // 从第二个 Concept_Node 开始，它除了有一个自身的 CM 孩子，还有一个和上面 CM 的共同孩子
            shared_ptr<Concept_Memory> self_child = build_or_share_cm(rete_network, temp_var_decl); // 构造自身CM后继
            cm = build_or_share_cm(rete_network, self_child->cpt_node_parent, cm); // 构造共同后继
            rete_network->cm_hash_table.insert(pair<string, shared_ptr<Concept_Memory>>(str_of_var_decl(cm->constraint), cm)); // 保存 CM 到哈希表
            ++it;
        }
    }
    return cm;
}

//为 Intra_Node 找到对应的 CM ，并接在其后
void find_cm_for_intra_node(shared_ptr<Rete_Network> rete_network, shared_ptr<Intra_Node> intra_node){
    // 根据 Intra_Node 的变量声明做哈希一般可以找到对应的 CM
    auto it = rete_network->cm_hash_table.find(str_of_var_decl(intra_node->var_decl));
    shared_ptr<Concept_Memory> ret;
    if(it!=rete_network->cm_hash_table.end())
        ret = it->second;
    else // 找不到的话就要为该 Intra_Node 构造新的 CM
        ret = build_or_share_cm(rete_network, intra_node->var_decl);
    ret->intra_node_children.push_back(intra_node);
    // 还要处理 Intra_Node 的父 CM
    intra_node->parent = ret;
}

// 根据 Assertion 创建 AM
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Assertion> assertion){
    // 先判断是否有可共享的 AM
    Hash_Input hash_input(assertion->get_output_str(),assertion->var_decl); // 创建哈希对象
    auto it = rete_network->am_hash_table.find(hash_input);
    if(it!=rete_network->am_hash_table.end())
        return it->second;
    // 如果没有可共享的 AM，为该 assertion 构造 AM
    shared_ptr<Alpha_Memory> ret = make_shared<Alpha_Memory>(*assertion);
    // 为该 AM 创建对应的 Intro_Node
    ret->parent = make_shared<Intra_Node>(ret);
    rete_network->am_hash_table.insert(pair<Hash_Input, shared_ptr<Alpha_Memory>>(hash_input, ret)); // 保存 AM 到哈希表
    // 该 AM 的前驱 Intra_Node 需要接在对应 CM 后
    find_cm_for_intra_node(rete_network, ret->parent);
    return ret;
}

// 根据 Sugar_For_Pred 创建 AM
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Sugar_For_Pred> sugar_for_pred){
    // 先判断是否有可共享的 AM
    Hash_Input hash_input(sugar_for_pred->get_output_str(),sugar_for_pred->var_decl); // 创建哈希对象
    auto it = rete_network->am_hash_table.find(hash_input);
    if(it!=rete_network->am_hash_table.end())
        return it->second;
    // 如果没有可共享的 AM，为该 sugar_for_pred 构造 AM
    shared_ptr<Alpha_Memory> ret = make_shared<Alpha_Memory>(*sugar_for_pred);
    // 为该 AM 创建对应的 Intro_Node
    ret->parent = make_shared<Intra_Node>(ret);
    rete_network->am_hash_table.insert(pair<Hash_Input, shared_ptr<Alpha_Memory>>(hash_input, ret)); // 保存 AM 到哈希表
    // 该 AM 的前驱 Intra_Node 需要接在对应 CM 后
    find_cm_for_intra_node(rete_network, ret->parent);
    return ret;
}

// 根据 Sugar_For_And 创建 AM
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Sugar_For_And> sugar_for_and){
    // 先判断是否有可共享的 AM
    Hash_Input hash_input(sugar_for_and->get_output_str(),sugar_for_and->var_decl); // 创建哈希对象
    auto it = rete_network->am_hash_table.find(hash_input);
    if(it!=rete_network->am_hash_table.end())
        return it->second;
    // 如果没有可共享的 AM，为该 sugar_for_and 构造 AM
    shared_ptr<Alpha_Memory> ret = make_shared<Alpha_Memory>(*sugar_for_and);
    // 为该 AM 创建对应的 Intro_Node
    ret->parent = make_shared<Intra_Node>(ret);
    rete_network->am_hash_table.insert(pair<Hash_Input, shared_ptr<Alpha_Memory>>(hash_input, ret)); // 保存 AM 到哈希表
    // 该 AM 的前驱 Intra_Node 需要接在对应 CM 后
    find_cm_for_intra_node(rete_network, ret->parent);
    return ret;
}

// 根据 标准形式的Term 创建 AM
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Term> term){
    // 先判断是否有可共享的 AM
    Hash_Input hash_input(term->get_output_str(),term->var_decl); // 创建哈希对象
    auto it = rete_network->am_hash_table.find(hash_input);
    if(it!=rete_network->am_hash_table.end())
        return it->second;
    // 如果没有可共享的 AM，为该 标准形式的Term 构造 AM
    shared_ptr<Alpha_Memory> ret = make_shared<Alpha_Memory>(*term);
    // 为该 AM 创建对应的 Intro_Node
    ret->parent = make_shared<Intra_Node>(ret);
    rete_network->am_hash_table.insert(pair<Hash_Input, shared_ptr<Alpha_Memory>>(hash_input, ret)); // 保存 AM 到哈希表
    // 该 AM 的前驱 Intra_Node 需要接在对应 CM 后
    find_cm_for_intra_node(rete_network, ret->parent);
    return ret;
}

// 根据 Individual 创建 AM
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Individual> indi){
    // 先判断是否有可共享的 AM
    Hash_Input hash_input(indi->get_output_str(),indi->var_decl); // 创建哈希对象
    auto it = rete_network->am_hash_table.find(hash_input);
    if(it!=rete_network->am_hash_table.end())
        return it->second;

    // 如果没有可共享的 AM，为该 individual 构造 AM
    assert(indi->is_assertion+indi->is_term==1); // LHS 中的 Individual 只会是 Assertion 或 Term (更具体地说是 sugar_for_pred)
    // 直观上理解就是: 要测试的条件总能写成 assertion 的形式，只不过一些二元谓词可以简写为 sugar_for_pred
    shared_ptr<Alpha_Memory> ret;
    if(indi->is_assertion)
        ret = build_or_share_am(rete_network,indi->assertion);
    else{
        auto t = indi->term;
        // LHS 中的 Individual 如果是 Term，那么只会是 sugar_for_pred
        // 不允许 sugar_for_and 的嵌套
        assert(t->is_pred==1);
        // if(t->is_and)
        //     ret = build_or_share_am(rete_network, t->and_val);
        // else if(t->is_pred)
        //     ret = build_or_share_am(rete_network, t->pred_val);
        // else
        //     ret = build_or_share_am(rete_network, t);
        ret = build_or_share_am(rete_network, t->pred_val);
    }
    // 为该 AM 创建对应的 Intro_Node
    ret->parent = make_shared<Intra_Node>(ret);
    // 该 AM 的前驱 Intra_Node 需要接在对应 CM 后
    find_cm_for_intra_node(rete_network, ret->parent);
    return ret;
}

// 根据 Concept_Memory 创建 AM
shared_ptr<Alpha_Memory> build_or_share_am(shared_ptr<Rete_Network> rete_network, shared_ptr<Concept_Memory> cm){
    // 先判断是否有可共享的 AM
    // AM 哈希对象是由变量声明在内的两部分组成的，而 CM 的哈希对象本身就是变量声明，虽然不同，但是后者可以很方便的转为前者
    Hash_Input hash_input(str_of_var_decl(cm->var_decl),cm->var_decl); // 创建哈希对象
    auto it = rete_network->am_hash_table.find(hash_input);
    if(it!=rete_network->am_hash_table.end())
        return it->second;
    // 如果没有可共享的 AM，为该 CM 构造 AM
    shared_ptr<Alpha_Memory> ret = make_shared<Alpha_Memory>(*cm);
    // 为该 AM 创建对应的 Intra_Node
    ret->parent = make_shared<Intra_Node>(ret);
    rete_network->am_hash_table.insert(pair<Hash_Input, shared_ptr<Alpha_Memory>>(hash_input, ret)); // 保存 AM 到哈希表
    cm->intra_node_children.push_back(ret->parent); // Concept_Memory 后接 AM对应的Intra_Node
    // 还要处理 Intra_Node 的父 CM
    ret->parent->parent = cm;
    return ret;
}

// 根据 Join_Node 创建 BM
shared_ptr<Beta_Memory> build_or_share_bm(shared_ptr<Rete_Network> rete_network, shared_ptr<Join_Node> join_node){
    // 先判断是否有可共享的 BM
    Hash_Input hash_input(join_node->constraint->get_output_str(),join_node->var_decl); // 创建哈希对象
    auto it = rete_network->bm_hash_table.find(hash_input);
    if(it!=rete_network->bm_hash_table.end())
        return it->second;
    // 如果没有可共享的 BM，为该 Join_Node 构造 BM
    // shared_ptr<Beta_Memory> ret = make_shared<Beta_Memory>(join_node);
    // 本来是打算使用 Beta_Memory 的构造函数来直接创建目标 Beta_Memory 的，但由于 Beta_Memory定义 先于 Join_Node定义，所以先创建一个原始的 BM，后修改其成员
    shared_ptr<Beta_Memory> ret = make_shared<Beta_Memory>();
    ret->constraint = join_node->constraint;
    ret->var_decl = join_node->var_decl;
    join_node->children.push_back(ret);
    ret->parent = join_node;
    rete_network->bm_hash_table.insert(pair<Hash_Input, shared_ptr<Beta_Memory>>(hash_input, ret)); // 保存 BM 到哈希表
    return ret;
}

// 根据 BM 和 Rete_Rule 创建 Terminal_Node
shared_ptr<Terminal_Node> build_or_share_terminal_node(shared_ptr<Rete_Network> rete_network, shared_ptr<Beta_Memory> bm, shared_ptr<Rete_Rule> rule){
    // 先判断是否有可共享的 Terminal_Node
    auto it = rete_network->t_node_hash_table.find(rule->get_output_str()); // Terminal_Node 以 rule 的字符串表示为哈希对象
    if(it!=rete_network->t_node_hash_table.end())
        return it->second;
    // 如果没有可共享的 Terminal_Node，为该 Beta_Memory 构造 Terminal_Node
    shared_ptr<Terminal_Node> ret = make_shared<Terminal_Node>(bm);
    // Terminal_Node 维护一个指向冲突集的指针
    ret->conflict_set = rete_network->conflict_set;
    // 还要处理 父BM的后继
    bm->terminals.push_back(ret);
    rete_network->t_node_hash_table.insert(pair<string, shared_ptr<Terminal_Node>>(rule->get_output_str(), ret)); // 保存 Terminal_Node 到哈希表
    return ret;
}

// 判断某 Join_Node 是否可以被某 AM 和 BM 共享
bool join_node_can_be_shared(shared_ptr<Join_Node> node, shared_ptr<Beta_Memory> bm, shared_ptr<Alpha_Memory> am)
{
    // AM和BM都必须相同
    return (node->parent_am == am && ( (!node->parent_bm && !bm) || (node->parent_bm == bm)));
}

// 根据 AM 和 BM 创建 Join_Node
shared_ptr<Join_Node> build_or_share_join_node(shared_ptr<Rete_Network> rete_network, shared_ptr<Alpha_Memory> am, shared_ptr<Beta_Memory> bm){
    // 先判断是否有可共享的 Join_Node
    for(auto join_node:rete_network->join_nodes){
        if(join_node_can_be_shared(join_node, bm, am))
            return join_node;
    }
    // 如果没有可共享的 Join_Node，为该 AM 和 BM 构造 Join_Node
    shared_ptr<Join_Node> ret = make_shared<Join_Node>(am, bm);
    // 还需要处理 父AM的后继、夫BM的后继、节点自身的AM前驱和BM前驱
    am->children.push_back(ret);
    bm->children.push_back(ret);
    ret->parent_am = am;
    ret->parent_bm = bm;

    rete_network->join_nodes.push_back(ret);
    return ret;
}


// 往 Rete_Network 中添加 Rule
void add_rule(shared_ptr<Rete_Network> rete_network, shared_ptr<Rete_Rule> rule){
    shared_ptr<Alpha_Memory> am;
    shared_ptr<Beta_Memory> bm;
    shared_ptr<Join_Node> join_node;
    shared_ptr<Terminal_Node> terminal;
    // 每个子条件都涉及特定概念的个体，也就是涉及变量声明
    // 为变量声明创建 Concept_Memory
    shared_ptr<Concept_Memory> cm = build_or_share_cm(rete_network, rule->var_decl);
    // 接下来处理 LHS
    auto &lhs = rule->lhs;
    if(lhs){ // LHS 不为空
        assert(lhs->is_assertion + lhs->is_term == 1);
        if(!(lhs->is_term && lhs->term->is_and)){ // LHS 只有一个子条件
            // 即lhs为 assertion 或 非sugar_for_and的term(其实只会是sugar_for_pred)
            if(lhs->is_assertion)
                am = build_or_share_am(rete_network, lhs->assertion);
            else{
                assert(lhs->term->is_pred);
                am = build_or_share_am(rete_network, lhs->term->pred_val);
            }
            // 唯一的一个条件也是第一个条件
            bm = rete_network->dummy;
            join_node = build_or_share_join_node(rete_network, am, bm);
            // 唯一的一个条件也是最后一个条件，最后的 Join_Node 后要接一个 BM 和 Terminal_Node
            bm = build_or_share_bm(rete_network, join_node);
            terminal = build_or_share_terminal_node(rete_network, bm, rule);
        }
        else{ // LHS 有多个子条件
            // 为第一个子条件构造 AM
            am = build_or_share_am(rete_network, lhs->term->and_val->content[0]);
            // 第一层的 Join_Node 的 BM 为 dummy_top_node
            bm = rete_network->dummy;
            // 为第一个子条件构造 Join_Node
            join_node = build_or_share_join_node(rete_network, am, bm);
            // 依次为其余子条件构造 AM 和 Join_Node
            for(size_t i=1; i!=lhs->term->and_val->content.size(); ++i){
                am = build_or_share_am(rete_network, lhs->term->and_val->content[i]);
                bm = build_or_share_bm(rete_network, join_node);
                join_node = build_or_share_join_node(rete_network, am, bm);
            }
            // 为该 Rule 构造 Terminal_Node，接在最后一个 BM 之后
            bm = build_or_share_bm(rete_network, join_node);
            terminal = build_or_share_terminal_node(rete_network, bm, rule);
        }
    }
    else{ // LHS 为空
        am = build_or_share_am(rete_network, cm); // CM 后接 AM
        bm = rete_network->dummy;
        join_node = build_or_share_join_node(rete_network, am, bm);
        // 最后的 Join_Node 后要接一个 BM 和 Terminal_Node
        bm = build_or_share_bm(rete_network, join_node);
        terminal = build_or_share_terminal_node(rete_network, bm, rule);
    }
    terminal->match_rule = rule;
}

shared_ptr<Rete_Network> construct_rete(const shared_ptr<Knowledge_Base> kb){
    cout<<"开始构建 Rete 网络..."<<endl;
    shared_ptr<Rete_Network> rete_network = make_shared<Rete_Network>();
    for(auto rule:kb->rete_rules){ // 处理每一条规则
        cout<<"添加规则: "<<rule->get_output_str()<<endl;
        add_rule(rete_network, rule);
    }
    rete_network->underlying_kb = kb;
    cout<<"Rete 网络构建完成!"<<endl;
    return rete_network;
}