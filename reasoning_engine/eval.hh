#ifndef EVAL_HH
#define EVAL_HH
#include "ke_struct.hh"

// 存放 eval 可能用到的一些辅助函数的声明

shared_ptr<Individual> extract_coeff(shared_ptr<Math_Expr> entire_expr, shared_ptr<Math_Expr> body); // 在某个表达式中提取某个表达式的系数

#endif