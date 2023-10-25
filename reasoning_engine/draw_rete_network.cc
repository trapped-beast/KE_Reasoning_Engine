// 可视化 Rete 网络
#include "Rete_Network.hh"
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

// 设置根节点属性
void format_root(void *node)
{
    agsafeset(node, (char *)"shape", (char *)"doublecircle", (char *)"");
    agsafeset(node, (char *)"style", (char *)"filled", (char *)"");
    agsafeset(node, (char *)"fillcolor", (char *)"lightcyan", (char *)"");
}

// 设置 Concept_Node 属性
void format_concept_node(void *node)
{
    agsafeset(node, (char *)"shape", (char *)"rect", (char *)"");
    agsafeset(node, (char *)"style", (char *)"filled", (char *)"");
    agsafeset(node, (char *)"fillcolor", (char *)"cadetblue", (char *)"");
}

// 设置 Concept_Memory 属性
void format_cm(void *node)
{
    agsafeset(node, (char *)"shape", (char *)"cylinder", (char *)"");
    agsafeset(node, (char *)"style", (char *)"filled", (char *)"");
    agsafeset(node, (char *)"fillcolor", (char *)"#bbd0c0", (char *)"");
}

// 设置 AM 属性
void format_am(void *node)
{
    agsafeset(node, (char *)"shape", (char *)"cylinder", (char *)"");
    agsafeset(node, (char *)"style", (char *)"filled", (char *)"");
    agsafeset(node, (char *)"fillcolor", (char *)"aquamarine", (char *)"");
}

// 设置 Intra_Node 属性
void format_intra_node(void *node)
{
    agsafeset(node, (char *)"shape", (char *)"rect", (char *)"");
    agsafeset(node, (char *)"style", (char *)"filled", (char *)"");
    agsafeset(node, (char *)"fillcolor", (char *)"#948ed6", (char *)"");
}

// 设置 Join_node 属性
void format_join_node(void *node)
{
    agsafeset(node, (char *)"shape", (char *)"rect", (char *)"");
    agsafeset(node, (char *)"style", (char *)"filled", (char *)"");
    agsafeset(node, (char *)"fillcolor", (char *)"cornsilk", (char *)"");
}

// 设置 BM 属性
void format_bm(void *node)
{
    agsafeset(node, (char *)"shape", (char *)"cylinder", (char *)"");
    agsafeset(node, (char *)"style", (char *)"filled", (char *)"");
    agsafeset(node, (char *)"fillcolor", (char *)"burlywood", (char *)"");
}

// 设置Dummy_top_node属性
void format_dummy_node(void *node)
{
    agsafeset(node, (char *)"shape", (char *)"cylinder", (char *)"");
    agsafeset(node, (char *)"style", (char *)"filled", (char *)"");
    agsafeset(node, (char *)"fillcolor", (char *)"#4a9759", (char *)"");
}

// 设置 Terminal_Node 属性
void format_terminal (void *node)
{
    agsafeset(node, (char *)"shape", (char *)"cylinder", (char *)"");
    agsafeset(node, (char *)"style", (char *)"filled", (char *)"");
    agsafeset(node, (char *)"fillcolor", (char *)"#ee7b67", (char *)"");
}

// 使用 Graphviz 库绘制 Rete 网络
void draw_rete_network(const Rete_Network &rete_network){
    cout<<"开始绘制 Rete 网络..."<<endl;
    GVC_t *gvc = gvContext();
    Agraph_t *g = agopen((char *)"rete_network", Agdirected, nullptr); // 创建新图
    // 为不同类型节点设置不同的格式
    Agraph_t *subg_alpha = agsubg(g, (char *)"cluster_alpha_network", 1);
    Agraph_t *subg_beta = agsubg(g, (char *)"cluster_beta_network", 1);
    Agraph_t *subg_root = agsubg(g, (char *)"root", 1);
    Agraph_t *subg_concept_nodes = agsubg(subg_alpha, (char *)"concept_node", 1);
    Agraph_t *subg_cm = agsubg(subg_alpha, (char *)"concept_memory", 1);
    Agraph_t *subg_intra_nodes = agsubg(subg_alpha, (char *)"intra_node", 1);
    Agraph_t *subg_am = agsubg(subg_alpha, (char *)"alpha_memory", 1);
    Agraph_t *subg_dummy_node = agsubg(subg_beta, (char *)"dummy_top_node", 1);
    Agraph_t *subg_bm = agsubg(subg_beta, (char *)"beta_memory", 1);
    Agraph_t *subg_join_nodes = agsubg(subg_beta, (char *)"join_node", 1);
    Agraph_t *subg_terminal_nodes = agsubg(g, (char *)"terminal_node", 1);

    Agedge_t *e = nullptr;
    // 分析rete_network，添加点和边
    Agnode_t *root = agnode(subg_root, (char *)"root", 1); // 添加 root
    format_root(root);
    // 添加 Concept_Node
    for(auto n:rete_network.root->concept_nodes){
        string cpt_node_name = n->get_figure_info();
        Agnode_t *cpt_node = agnode(subg_concept_nodes, (char *)cpt_node_name.c_str(), 1);
        format_concept_node(cpt_node);
        e = agedge(g, root, cpt_node, (char *)"", 1); // 添加边 root—> concept_node
        // 添加 Concept_Memory
        // 每层的 Concept_Node 情况都要考虑
        string cm_name = n->cm_child->get_figure_info(); // 首层的情况
        Agnode_t *cm = agnode(subg_cm, (char *)cm_name.c_str(), 1);
        format_cm(cm);
        e = agedge(g, cpt_node, cm, (char *)"", 1); // 添加边 concept_node -> concept_memory
        for(auto child:n->cm_children){ // 第二层（以下）的情况
            cm_name = child->get_figure_info();
            cm = agnode(subg_cm, (char *)cm_name.c_str(), 1);
            format_cm(cm);
            e = agedge(g, cpt_node, cm, (char *)"", 1); // 添加边 concept_node -> concept_memory
        }
    }
    // 目前，如果 CM 有共同的CM后继，它们之间的连线并未画出
    for(auto p:rete_network.cm_hash_table){
        // CM 都已画出，只是 CM->其共同的CM后继 的边未画出
        string head_name = p.second->get_figure_info();
        Agnode_t *head = agnode(subg_cm, (char *)head_name.c_str(), 1); // 该点已经存在，不会重复添加
        for(auto child:p.second->cm_children){
            string tail_name = child->get_figure_info();
            Agnode_t *tail = agnode(subg_cm, (char *)tail_name.c_str(), 1); // 该点已经存在，不会重复添加
            e = agedge(g, head, tail, (char *)"", 1);
        }
    }
    // 添加 Alpha_Memory
    for(auto p:rete_network.am_hash_table){
        shared_ptr<Alpha_Memory> n = p.second;
        string am_name = n->get_figure_info();
        Agnode_t *am = agnode(subg_alpha, (char *)am_name.c_str(), 1);
        format_am(am);
        // 添加 Intra_Node
        string intra_node_name = n->parent->get_figure_info();
        Agnode_t *intra_node = agnode(subg_intra_nodes, (char *)intra_node_name.c_str(), 1);
        format_intra_node(intra_node);
        e = agedge(g, intra_node, am, (char *)"", 1); // 添加边 intra_node -> alpha_memory
        // 添加边 concept_memory -> intra_node
        string cm_name = n->parent->parent->get_figure_info();
        Agnode_t *cm = agnode(subg_cm, (char *)cm_name.c_str(), 1); // 该点已经存在，不会重复添加
        e = agedge(g, cm, intra_node, (char *)"", 1);
    }
    // 添加 Join_Node
    for(auto n:rete_network.join_nodes){
        string join_node_name = n->get_figure_info();
        Agnode_t *join_node = agnode(subg_join_nodes, (char *)join_node_name.c_str(), 1);
        format_join_node(join_node);
        // 添加父 BM
        string bm_name = n->parent_bm->get_figure_info();
        Agnode_t *bm = agnode(subg_bm, (char *)bm_name.c_str(), 1);
        if(!n->parent_bm->constraint)
            format_dummy_node(bm);
        else
            format_bm(bm);
        e = agedge(g, bm, join_node, (char *)"", 1); // 添加边 父 BM -> join_node
        // 添加边 父 AM -> join_node
        string am_name = n->parent_am->get_figure_info();
        Agnode_t *am = agnode(subg_am, (char *)am_name.c_str(), 1); // 该点已经存在，不会重复添加
        e = agedge(g, am, join_node, (char *)"", 1);
        // 添加子 BM
        for(auto b:n->children){
            bm_name = b->get_figure_info();
            bm = agnode(subg_bm, (char *)bm_name.c_str(), 1);
            format_bm(bm);
            e = agedge(g, join_node, bm, (char *)"", 1); // 添加边 join_node -> 子 BM
        }
    }
    // 添加 Terminal_Node
    for(auto n:rete_network.bm_hash_table){
        for(auto t_node:n.second->terminals){
            string terminal_name = t_node->get_figure_info();
            Agnode_t *terminal = agnode(subg_terminal_nodes, (char *)terminal_name.c_str(), 1); // 该点已经存在，不会重复添加
            format_terminal(terminal);
            string bm_name = n.second->get_figure_info();
            Agnode_t *bm = agnode(subg_bm, (char *)bm_name.c_str(), 1); // 该点已经存在，不会重复添加
            e = agedge(g, bm, terminal, (char *)"", 1); // 添加边 BM -> Terminal_Node
        }
    }


    // 设置全图属性
    agsafeset(g, (char *)"rankdir", (char *)"LR", (char *)"");  // 边从左向右
    agsafeset(g, (char *)"ordering", (char *)"in", (char *)""); // 边按定义的顺序排列
    agsafeset(g, (char *)"ranksep", (char *)"5 equally", (char *)"");
    agsafeset(subg_alpha, (char *)"style", (char *)"dashed", (char *)"");
    agsafeset(subg_alpha, (char *)"bgcolor", (char *)"lightblue", (char *)"");
    agsafeset(subg_beta, (char *)"style", (char *)"dashed", (char *)"");
    agsafeset(subg_beta, (char *)"bgcolor", (char *)"lightyellow", (char *)"");
    agsafeset(subg_am, (char *)"rank", (char *)"same", (char *)"");
    agsafeset(subg_dummy_node, (char *)"rank", (char *)"same", (char *)"");
    agsafeset(subg_terminal_nodes, (char *)"rank", (char *)"same", (char *)"");

    // 增加图例
    Agraph_t *subg_legend = agsubg(g, (char *)"cluster_legend", 1);
    format_root(agnode(subg_legend, (char *)"Root", 1));
    format_concept_node(agnode(subg_legend, (char *)"Concept_Node", 1));
    format_cm(agnode(subg_legend, (char *)"Concept_Memory", 1));
    format_intra_node(agnode(subg_legend, (char *)"Intra_Node", 1));
    format_am(agnode(subg_legend, (char *)"Alpha_Memory", 1));
    format_dummy_node(agnode(subg_legend, (char *)"dummy_top_node", 1));
    format_bm(agnode(subg_legend, (char *)"Beta_Memory", 1));
    format_join_node(agnode(subg_legend, (char *)"Join_Node", 1));
    format_terminal(agnode(subg_legend, (char *)"Terminal_Node", 1));
    agsafeset(subg_legend, (char *)"style", (char *)"dashed", (char *)"");
    agsafeset(subg_legend, (char *)"label", (char *)"legend", (char *)"");
    agsafeset(subg_legend, (char *)"labelloc", (char *)"b", (char *)"");
    agsafeset(subg_legend, (char *)"fontname", (char *)"Times-BoldItalic", (char *)"");
    agsafeset(subg_legend, (char *)"fontsize", (char *)"20", (char *)"");

    // layout + render
    gvLayout(gvc, g, "dot");
    // 输出结果放在当前目录下
    FILE *dotf = fopen("rete_network.dot", "w");
    FILE *pngf = fopen("rete_network.png", "w");
    gvRender(gvc, g, "dot", dotf);
    gvRender(gvc, g, "png", pngf);

    // 回收资源
    gvFreeLayout(gvc, g);
    agclose(g);
    std::cout<<"Rete 网络绘制完成, 详见 rete_network.png 文件!!"<<endl;
}