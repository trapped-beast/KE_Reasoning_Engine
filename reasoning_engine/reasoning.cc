#include "Rete_Network.hh"

// 判断题目是否已解出
bool has_been_solved(shared_ptr<Rete_Question> question){
    // 只要有未知的待求项，解题就尚未结束

    // TODO: 实现

    return true;
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
        rete_network->add_fact(next_fact);
    }
    size_t end = rete_network->conflict_set->content.size(); // 本轮匹配到的 rule 的终点
    for (; start != end; ++start){
        ret.push_back(rete_network->conflict_set->content.at(start));
    }
    return ret;
}


// 执行推理
void reasoning(shared_ptr<Rete_Question> question, shared_ptr<Rete_Network> rete_network){
    cout<<"执行推理"<<endl; 
    do{
        // 找到可用的实例化后的规则
        vector<shared_ptr<Rete_Rule>> instantiated_rules = get_instantiated_rules(question, rete_network);
        // 执行规则的 RHS
        for(auto instantiated_rule:instantiated_rules){
            question->take_action(instantiated_rule->rhs);
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
