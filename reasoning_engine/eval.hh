#ifndef EVAL_HH
#define EVAL_HH
#include "ke_struct.hh"

// 存放 eval 可能用到的一些辅助函数的声明

shared_ptr<Individual> extract_coeff(shared_ptr<Math_Expr> entire_expr, shared_ptr<Math_Expr> body); // 在某个表达式中提取某个表达式的系数
shared_ptr<Math_Expr> subst_sy_with_num(shared_ptr<Math_Expr> ori_expr, string var_sy, Number num); // 把表达式中的某个符号替换为数字

#endif