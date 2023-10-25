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


// 对 Individual 进行求值，用于 Intra_Node 的执行性测试
shared_ptr<Individual> intra_node_eval(shared_ptr<Individual> indi, shared_ptr<Fact> fact){
    #ifndef NDEBUG
        cout<<"当前求值的 Individual 为: "<<*indi<<endl;
        cout<<"当前求值的 fact 为: "<<*fact<<endl;
    #endif
    // 要进行求值操作的是标准形式的 Term // TODO: 支持更多形式如 sugar_for_oprt_apply?
    assert(indi->term->is_std);
    auto t = indi->term;
    string oprt = t->oprt;
    auto &args = t->args;

    shared_ptr<Individual> ret = make_shared<Individual>(false);
    // 内部定义的算子
    if(oprt=="Is_In_Form"){ // 判断某个方程是否满足指定形式 (参数是: 要判断其形式的方程 Term、某个形式的方程 Equation)
        assert(args.size()==2);
        assert(args[0]->is_term);
        assert(args[1]->is_math_indi && args[1]->math_indi->is_equation);
        // 可能通过的 fact 必须具有以下形式: Equation(x) = 某个形式的方程
        if(fact->is_assert && fact->assertion->left->get_output_str()==args[0]->get_output_str() && fact->assertion->right->is_math_indi && fact->assertion->right->math_indi->is_equation){
            auto target_eq = args[1]->math_indi->equation_val; // 目标方程
            auto test_eq = fact->assertion->right->math_indi->equation_val; // 测试方程
            auto target_left = *target_eq->left;
            auto target_right = *target_eq->right;
            auto test_left = *test_eq->left;
            auto test_right = *test_eq->right;
            cout<<"目标方程左部为: "<<target_left<<endl;
            cout<<"测试方程左部为: "<<test_left<<endl;
            /*
             * 能判断的方程形式有:
             *     1. x^2/a^2 + y^2/b^2 = 1 (Ellipse)
             *     2. y^2/a^2 + x^2/b^2 = 1 (Ellipse)
             *     3. x^2/a^2 - y^2/b^2 = 1 (Hyperbola)
             *     4. y^2/a^2 - x^2/b^2 = 1 (Hyperbola)
             *     5. y^2 =  2*p*x (Parabola)
             *     6. y^2 = -2*p*x (Parabola)
             *     7. x^2 =  2*p*y (Parabola)
             *     8. x^2 = -2*p*y (Parabola)
             */
            Math_Expr x = Math_Expr("x");
            Math_Expr y = Math_Expr("y");
            if(target_right==Math_Expr(1) && target_right==test_right){ // 前 4 种形式
                if(target_left.op_val == test_left.op_val){ // 最外层的 +- 号要一致
                    if(target_eq->get_output_str()=="x^2/a^2+y^2/b^2==1"){ // 形式 1、2 都满足
                        if(target_left.op_val=='+'){
                            // 左边是 x^2/Math_Expr，右边是 y^2/Math_Expr，或者对调
                            auto left = *target_left.left;
                            auto right = *target_left.right;
                            // 首先，左右两边都必须是 未知数^2/Math_Expr
                            bool left_conform = left.is_mathe && left.left->is_mathe && left.left->op_val=='^';
                            bool right_conform = right.is_mathe && right.left->is_mathe && right.left->op_val=='^';
                            // 其次，两个参数一个是 x、另一个是 y
                            bool var_conform = (*left.left->left==x && *right.left->left==y) || (*left.left->left==y && *right.left->left==x);
                            ret = make_shared<Individual>(left_conform && right_conform && var_conform);
                        }
                    }
                    else if(target_eq->get_output_str()=="x^2/a^2-y^2/b^2==1" || target_eq->get_output_str()=="y^2/a^2-x^2/b^2==1"){
                        if(target_left.op_val=='-'){
                            // 左边是 x^2/Math_Expr，右边是 y^2/Math_Expr
                            auto left = *target_left.left;
                            auto right = *target_left.right;
                            // 首先，左右两边都必须是 未知数^2/Math_Expr
                            bool left_conform = left.is_mathe && left.left->is_mathe && left.left->op_val=='^';
                            bool right_conform = right.is_mathe && right.left->is_mathe && right.left->op_val=='^';
                            // 其次，两个参数分别是 x、y 或是 y、x
                            bool var_conform;
                            if(target_eq->get_output_str()=="x^2/a^2-y^2/b^2==1") // 形式 3
                                var_conform = *left.left->left==x && *right.left->left==y;
                            else // 形式 4
                                var_conform = *left.left->left==y && *right.left->left==x;
                            ret = make_shared<Individual>(left_conform && right_conform && var_conform);
                        }
                    }
                }
            }
        }
    }
    else if(oprt=="Focus_On_Y_Axis" || oprt=="Focus_On_X_Axis"){ // 判断圆锥曲线的焦点是否在Y轴上 (参数是: 圆锥曲线对象 Symbol)
        ; // 暂未定义执行性测试求值的算子, 默认返回 false
    }
    else{
        cerr<<"未定义算子: "<<oprt<<" !"<<endl;
        assert(false);
    }
    
    return ret;
}



// 每个 Operator 要规定 自身的参数个数 以及 每个参数的类型


// 对 Individual 进行求值，用于 action_operator
shared_ptr<Individual> action_eval(shared_ptr<Individual> indi, Rete_Question &question, shared_ptr<vector<shared_ptr<Fact>>> conditions_sp){
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
        vector<shared_ptr<Fact>> conditions; // 求值可能需要用到的条件
        shared_ptr<Fact> condition = make_shared<Fact>(); 
        // 内部定义的 算子
        if(oprt=="Recip"){ // 对某个数求倒数 (参数是: 求倒数的对象 Math_Expr )
            assert(args.size()==1);
            auto target = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!target) // 如果没有找到指定类型的个体值，需要迭代求值
                target = action_eval(args[0], question);
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
            auto target_1 = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!target_1){
                target_1 = args[0]->find_specific_indi("Equation", question, conditions_sp);
                if(!target_1)
                    target_1 = action_eval(args[0], question);
            }
            auto target_2 = args[1]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!target_2)
                target_2 = action_eval(args[1], question);
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
            auto target = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!target)
                target = action_eval(args[0], question);
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
            auto num_1 = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_1)
                num_1 = action_eval(args[0],question);
            auto num_2 = args[1]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_2)
                num_2 = action_eval(args[1],question);
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
    if(eval_ret){
        cout<<"求值 "<<*indi<<" 得到的结果为: "<<*eval_ret<<endl;
        // // 构造新的 fact // 不是所有的求值细节都要展示
        // if(indi->term->is_std){
        //     auto new_fact = make_shared<Fact>(Assertion(*indi,*eval_ret));
        //     question.normalize_individual(eval_ret);
        //     new_fact->assertion->propagate_var_decl(question.var_decl);
        //     new_fact->var_decl = new_fact->assertion->var_decl;
        //     question.fact_list.push_back(new_fact);
        // }
    }
    return eval_ret;
}