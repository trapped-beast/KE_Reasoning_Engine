#include "Rete_Network.hh"
#include "eval.hh"

// 在某个表达式中提取某个表达式的系数
shared_ptr<Individual> extract_coeff(shared_ptr<Math_Expr> entire_expr, shared_ptr<Math_Expr> body){
    shared_ptr<Individual> ret;
    // 只需要处理 is_enclosed 和 is_mathe 的情况
    if(entire_expr->is_enclosed)
        ret = extract_coeff(entire_expr->enclosed_expr, body);
    else if(entire_expr->is_mathe){
        if(entire_expr->left->get_output_str().find(body->get_output_str())!=string::npos && entire_expr->left->get_output_str()!=body->get_output_str()){ // 主体出现在 +-*/^式 左侧 (真子段)
            ret = extract_coeff(entire_expr->left, body);
        }
        else if(entire_expr->right->get_output_str().find(body->get_output_str())!=string::npos && entire_expr->right->get_output_str()!=body->get_output_str()){ // 主体出现在 +-*/^式 右侧 (真子段)
            ret = extract_coeff(entire_expr->right, body);
        }
        else{ // 最终会归为 */ 式
            Math_Expr temp_ret;
            if(entire_expr->op_val=='*'){
                if(*entire_expr->left==*body)
                    temp_ret = *entire_expr->right;
                else{
                    assert(*entire_expr->right==*body);
                    temp_ret = *entire_expr->left;
                }
            }
            else{
                assert(entire_expr->op_val=='/');
                auto one = Math_Expr((Number(1)));
                assert(*entire_expr->left==*body); // body 为分子
                temp_ret = Math_Expr(one,'/',*entire_expr->right);
            }
            ret = make_shared<Individual>(*make_shared<Math_Individual>(temp_ret));
        }
    }
    cout<<"计算 extract_coeff("<<*entire_expr<<string(",")<<*body<<string(") = ")<<*ret<<endl;
    return ret;
}



/* 
 * Intra_Node 测试可以分为两种:
 *    1. 存在性判断: 只需判断当前已知 fact 中是否存在这样的一条 fact，因为在题目未解出的情况下所有的 fact 都会被先后送入 Rete 网络，所以检查当前的 fact 是不是目标事实也是等价的，所以这里我们使用的是后者
 *    2. 执行性判断: 根据对当前已知信息中的相关个体进行求值来判断条件是否成立
 */

// 对 term 进行求值，用于 Intra_Node 测试
shared_ptr<Individual> eval(shared_ptr<Term> term, shared_ptr<Fact> fact){
    #ifndef NDEBUG
        cout<<"当前求值的 Term 为: "<<*term<<endl;
        cout<<"当前求值的 fact 为: "<<*fact<<endl;
    #endif
    string oprt = term->oprt;
    shared_ptr<Individual> ret;
    // 内部定义的算子
    if(oprt=="Is_In_Form"){ // 判断某个方程是否满足指定形式 (参数是: 要判断其形式的方程 Term、某个形式的方程 Equation)
        assert(term->args.size()==2);
        // 先根据当前 abstract_to_concrete 找到实际要判断其形式的方程


        // auto equation = term->args[0]->alt_val;
        if(term->args[1]->get_output_str()=="x^2/a^2+y^2/b^2==1"){
            // TODO:实现
            // TODO:
            auto arg_0 = *term->args[0];
            auto arg_1 = *term->args[1];
            ret = make_shared<Individual>(true);
        }
    }
    else if(oprt=="Focus_On_Y_Axis" || oprt=="Focus_On_X_Axis"){ // 判断圆锥曲线的焦点是否在Y轴上 (参数是: 圆锥曲线对象 Symbol)
        assert(term->args.size()==1);
        assert(term->args[0]->is_math_indi && term->args[0]->math_indi->is_math_expr && term->args[0]->math_indi->expr_val->is_sy);
        string abs_sy = term->args[0]->math_indi->expr_val->sy_val; // 圆锥曲线对象
        string con_sy;
        // 先根据当前 abstract_to_concrete 构造一个实例化后的 Term
        for(auto abs_to_con:fact->abstract_to_concrete){
            if(abs_to_con.first==abs_sy){
                con_sy = abs_to_con.second;
                break;
            }
            assert(false);
        }
        vector<shared_ptr<Individual>> new_args; // 新的参数列表 (只有一个唯一的参数: Symbol)
        new_args.push_back(make_shared<Individual>(Math_Individual(Math_Expr(con_sy))));
        auto con_term = Term(oprt,new_args);
        
        // 直接在当前 fact_list 中查找是否包含该 fact
        bool find = false;
        auto con_term_assertion = Assertion(Individual(con_term)); // 如果出现在当前 fact_list 中, 应该以 Assertion 的形式出现
        for(auto f:fact->where_is->fact_list){
            if(con_term_assertion.get_output_str()==f->get_output_str()){
                find = true;
                break;
            }
        }
        ret = find ? make_shared<Individual>(true) : make_shared<Individual>(false);
    }
    else{
        cerr<<"未定义算子: "<<oprt<<" !"<<endl;
        assert(false);
    }
    
    return ret;
}


// 每个 Operator 要规定 自身的参数个数 以及 每个参数的类型


// 对 Individual 进行求值
shared_ptr<Individual> eval(shared_ptr<Individual> indi, const Rete_Question &question){
    #ifndef NDEBUG
        cout<<"当前求值的 Individual 为: "<<*indi<<endl;
    #endif
    shared_ptr<Individual> eval_ret; // 最终的求值结果
    assert(indi->is_term);
    // 要进行求值操作的是标准形式的 Term 或 Sugar_For_Oprt_Apply
    if(indi->term->is_oprt_apply){
        auto oprt_apply_val = indi->term->oprt_apply_val;
        string caller = oprt_apply_val->indi; // 调用个体
        string uni_oprt = oprt_apply_val->uni_oprt; // 调用的一元算子
        for(auto def_indi:question.def_indi_hash_table){ // 在定义个体中查找该个体值
            if(caller==def_indi.second->symbol){
                assert(def_indi.second->indi->is_term && def_indi.second->indi->term->is_ctor);
                for(auto assignment:def_indi.second->indi->term->ctor_val->content){
                    if(uni_oprt==assignment->symbol){
                        eval_ret = assignment->val;
                        break;
                    }
                }
                break;
            }
            assert(false);
        }
    }
    else{
        assert(indi->term->is_std);
        auto t = indi->term;
        string oprt = t->oprt;
        auto &args = t->args;

        // 内部定义的 算子
        if(oprt=="Recip"){ // 对某个数求倒数 (参数是: 求倒数的对象 Math_Expr )
            assert(args.size()==1);
            auto target = args[0]->find_specific_indi("Math_Expr", question);
            if(!target) // 如果没有找到指定类型的个体值，需要迭代求值
                target = eval(args[0], question);
            assert(target->is_math_indi && target->math_indi->is_math_expr);
            auto expr_val = target->math_indi->expr_val;
            auto one = Math_Expr((Number(1)));
            Math_Expr ret; // 求倒数后的结果
            if(expr_val->is_mathe){ // +-*/^式 要特殊处理
                if(expr_val->op_val=='+' || expr_val->op_val=='-'){ // 结果为 1 / (+-式)
                    auto denominator = make_shared<Math_Expr>(expr_val);
                    ret = Math_Expr(one,'/',*denominator);
                }
                else if(expr_val->op_val=='*' || expr_val->op_val=='^'){ // 结果为 1 / *^式
                    ret = Math_Expr(one,'/',*expr_val);
                }
                else{ // 结果为 分子、分母对调
                    assert(expr_val->op_val=='/');
                    if(expr_val->left->get_output_str()=="1") // 考虑对调后分子为 0 的特殊情况
                        ret = *expr_val->right;
                    else
                        ret = Math_Expr(*expr_val->right,'/',*expr_val->left);
                }
            }
            else{ // 其它情况的结果统一为: 1 / Math_Expr
                ret = Math_Expr(one,'/',*expr_val);
            }
            
            eval_ret = make_shared<Individual>(Math_Individual(ret));
        }
        else if(oprt=="Extract_Coeff"){ // 提取某个数学表达式的系数 (参数是: 整个表达式 Math_Expr 或 Equation、要提取系数的主体 Math_Expr)
            assert(args.size()==2);
            auto target_1 = args[0]->find_specific_indi("Math_Expr", question);
            if(!target_1){
                target_1 = args[0]->find_specific_indi("Equation", question);
                if(!target_1)
                    target_1 = eval(args[0], question);
            }
            auto target_2 = args[1]->find_specific_indi("Math_Expr", question);
            if(!target_2)
                target_2 = eval(args[1], question);
            assert(target_1 && target_2);
            assert(target_1->is_math_indi);
            assert(target_2->is_math_indi && target_2->math_indi->is_math_expr);
            auto body = target_2->math_indi->expr_val; // 要提取系数的主体
            if(target_1->math_indi->is_math_expr){
                auto entire_expr = target_1->math_indi->expr_val;
                eval_ret = extract_coeff(entire_expr, body);
            }
            else{
                assert(target_1->math_indi->is_equation);
                auto entire_equation = target_1->math_indi->equation_val;
                if(entire_equation->left->get_output_str().find(body->get_output_str())!=string::npos){ // 主体出现在方程左侧
                    auto entire_expr = entire_equation->left;
                    eval_ret = extract_coeff(entire_expr, body);
                }
                else{ // 主体出现在方程右侧
                    assert(entire_equation->right->get_output_str().find(body->get_output_str())!=string::npos);
                    auto entire_expr = entire_equation->right;
                    eval_ret = extract_coeff(entire_expr, body);
                }
            }
        }
        else if(oprt=="Sqrt"){ // 对数学表达式进行开方 (参数是: 数学表达式 Math_Expr)
            assert(args.size()==1);
            auto target = args[0]->find_specific_indi("Math_Expr", question);
            if(!target)
                target = eval(args[0], question);
            assert(target->is_math_indi && target->math_indi->is_math_expr);
            auto target_val = target->math_indi->expr_val;
            if(target_val->is_num){
                auto num = target_val->number_val;
                double num_val = num->is_int ? num->i_val : num->f_val;
                double sqrt_val = std::sqrt(num_val);
                if(sqrt_val*sqrt_val==num_val){ // 如果开方得到整数
                    eval_ret = make_shared<Individual>(Math_Individual(Math_Expr(Number(int(sqrt_val)))));
                }
                else{ // 否则等于 sqrt(target_val)
                    vector<shared_ptr<Math_Expr>> new_args;
                    new_args.push_back(target_val);
                    eval_ret = make_shared<Individual>(Math_Individual(Math_Expr(Math_Func("sqrt",new_args))));
                }
            }
            else{ // 否则等于 sqrt(target_val)
                vector<shared_ptr<Math_Expr>> new_args;
                new_args.push_back(target_val);
                eval_ret = make_shared<Individual>(Math_Individual(Math_Expr(Math_Func("sqrt",new_args))));
            }
        }
        else if(oprt=="Mul"){ // 对若干个数学表达式进行相乘 (参数是: 若干个数学表达式 Math_Expr)
            // 暂时处理两个参数
            assert(args.size()==2);
            auto num_1 = args[0]->find_specific_indi("Math_Expr", question);
            if(!num_1)
                num_1 = eval(args[0],question);
            auto num_2 = args[1]->find_specific_indi("Math_Expr", question);
            if(!num_2)
                num_2 = eval(args[1],question);
            if(num_1 && num_1->is_math_indi && num_1->math_indi->is_math_expr && num_2 && num_2->is_math_indi && num_2->math_indi->is_math_expr){
                auto val_1 = num_1->math_indi->expr_val;
                auto val_2 = num_2->math_indi->expr_val;
                Math_Expr ret;
                if(val_1->is_num && val_2->is_num)
                    ret = Math_Expr(Number(*val_1->number_val * *val_2->number_val));
                else
                    ret = Math_Expr(val_1,'*',val_2);
                eval_ret = make_shared<Individual>(Math_Individual(ret));
            }
        }
    }
    if(eval_ret)
        cout<<"求值 "<<*indi<<" 得到的结果为: "<<*eval_ret<<endl;
    return eval_ret;
}