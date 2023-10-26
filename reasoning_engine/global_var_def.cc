// 全局变量定义
#include "global_var_decl.hh"

bool print_var_info = false;
shared_ptr<Reasoning_Graph> reasoning_graph = make_shared<Reasoning_Graph>();
vector<string> built_in_oprts = {"Recip", "Extract_Coeff", "Sqrt", "Mul", "Div", "Generate_Line_Eq"};