#ifndef GLOBSL_VAR_DECL_HH
#define GLOBSL_VAR_DECL_HH
// 全局变量声明
#include "Rete_Network.hh"
#include<string>
#include<vector>
using std::string;
using std::vector;

class Reasoning_Graph;
extern bool print_var_info; // 是否在调试信息中打印变量声明的信息
extern vector<string> built_in_oprts; // 内置的算子
extern shared_ptr<Reasoning_Graph> reasoning_graph; // 推理图

#endif