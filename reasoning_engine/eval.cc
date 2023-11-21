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

// 把表达式中的某个符号替换为数字
shared_ptr<Math_Expr> subst_sy_with_num(shared_ptr<Math_Expr> ori_expr, string var_sy, Number num){
    shared_ptr<Math_Expr> ret = ori_expr;
    // 只需要处理 is_enclosed、is_mathe、is_sy 的情况
    if(ori_expr->is_enclosed)
        ret = subst_sy_with_num(ori_expr->enclosed_expr, var_sy, num);
    else if(ori_expr->is_mathe){
        ori_expr->left = subst_sy_with_num(ori_expr->left, var_sy, num);
        ori_expr->right = subst_sy_with_num(ori_expr->right, var_sy, num);
    }
    else if(ori_expr->is_sy && ori_expr->sy_val==var_sy){
        ori_expr->is_sy = false;
        ori_expr->is_num = true;
        ori_expr->number_val = make_shared<Number>(num);
    }
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
        if(fact->is_assert && fact->assertion->is_std && fact->assertion->left->get_output_str()==args[0]->get_output_str() && fact->assertion->right->is_math_indi && fact->assertion->right->math_indi->is_equation){
            auto target_eq = args[1]->math_indi->equation_val; // 目标方程
            auto test_eq = fact->assertion->right->math_indi->equation_val; // 测试方程
            auto target_left = *target_eq->left;
            auto target_right = *target_eq->right;
            auto test_left = *test_eq->left;
            auto test_right = *test_eq->right;
            cout<<"目标方程左部为: "<<target_left<<endl;
            cout<<"测试方程左部为: "<<test_left<<endl;
            cout<<"目标方程右部为: "<<target_right<<endl;
            cout<<"测试方程右部为: "<<test_right<<endl;
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
            Math_Expr x_2 = Math_Expr(x, '^', Math_Expr(2));
            Math_Expr y_2 = Math_Expr(y, '^', Math_Expr(2));
            Number zero = Number(0);
            if(target_right==Math_Expr(1) && target_right==test_right){ // 前 4 种形式
                if(target_left.op_val == test_left.op_val){ // 最外层的 +- 号要一致
                    if(target_eq->get_output_str()=="x^2/a^2+y^2/b^2==1"){ // 形式 1、2 都满足
                        if(target_left.op_val=='+'){
                            // 左边是 x^2/Math_Expr，右边是 y^2/Math_Expr，或者对调
                            auto left = *test_left.left;
                            auto right = *test_left.right;
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
                            auto left = *test_left.left;
                            auto right = *test_left.right;
                            // 首先，左右两边都必须是 未知数^2/Math_Expr
                            bool left_conform = left.is_mathe && left.left->is_mathe && left.left->op_val=='^';
                            bool right_conform = right.is_mathe && right.left->is_mathe && right.left->op_val=='^';
                            // 其次，两个参数分别是 x、y 或是 y、x
                            bool var_conform = false;
                            if(target_eq->get_output_str()=="x^2/a^2-y^2/b^2==1") // 形式 3
                                var_conform = *left.left->left==x && *right.left->left==y;
                            else if(target_eq->get_output_str()=="y^2/a^2-x^2/b^2==1") // 形式 4
                                var_conform = *left.left->left==y && *right.left->left==x;
                            ret = make_shared<Individual>(left_conform && right_conform && var_conform);
                        }
                    }
                }
            }
            else if(target_left==y_2 && target_left==test_left){
                if(target_eq->get_output_str()=="y^2==2*p*x"){ // 形式 5
                    if(test_right.is_mathe && test_right.op_val=='*' && *test_right.right==x && test_right.left->is_num)
                        ret = make_shared<Individual>(*test_right.left->number_val > zero);
                }
                else if(target_eq->get_output_str()=="y^2==-2*p*x"){
                    if(test_right.is_mathe && test_right.op_val=='*' && *test_right.right==x && test_right.left->is_num)
                        ret = make_shared<Individual>(*test_right.left->number_val < zero);
                }
            }
            else if(target_left==x_2 && target_left==test_left){
                if(target_eq->get_output_str()=="x^2==2*p*y"){ // 形式 7
                    if(test_right.is_mathe && test_right.op_val=='*' && *test_right.right==y && test_right.left->is_num)
                        ret = make_shared<Individual>(*test_right.left->number_val > zero);
                }
                else if(target_eq->get_output_str()=="x^2==-2*p*y"){ // 形式 8
                    if(test_right.is_mathe && test_right.op_val=='*' && *test_right.right==y && test_right.left->is_num)
                        ret = make_shared<Individual>(*test_right.left->number_val < zero);
                }
            }
        }
    }
    // 对于未定义执行性测试求值的算子, 默认返回 false
    // else if(oprt=="Focus_On_Y_Axis" || oprt=="Focus_On_X_Axis"){ // 判断圆锥曲线的焦点是否在Y轴上 (参数是: 圆锥曲线对象 Symbol)
    //     ; // 暂未定义执行性测试求值的算子, 默认返回 false
    // }
    // else{
    //     cerr<<"未定义算子: "<<oprt<<" !"<<endl;
    //     assert(false);
    // }
    
    return ret;
}



// 每个 Operator 要规定 自身的参数个数 以及 每个参数的类型


// 对 Individual 进行求值，用于 action_operator
shared_ptr<Individual> action_eval(shared_ptr<Individual> indi, Rete_Question &question, shared_ptr<vector<shared_ptr<Fact>>> conditions_sp){
    #ifndef NDEBUG
        cout<<"当前求值的 Individual 为: "<<*indi<<endl;
    #endif

    shared_ptr<Individual> eval_ret; // 最终的求值结果    assert(indi->is_term);
    // 要进行求值操作的是标准形式的 Term 或 Sugar_For_Oprt_Apply 或 Assertion
    if(indi->is_assertion){ // 使用到 KB 中定义的算子时会出现
        assert(indi->assertion->is_std);
        auto left = indi->assertion->left;
        auto right = indi->assertion->right;
        cout<<left->get_output_str()<<endl;
        cout<<right->get_output_str()<<endl;
        auto new_left = action_eval(left, question);
        auto new_right = action_eval(right, question);
        cout<<"right: "<<*new_right<<endl;
        if(new_left)
            eval_ret = make_shared<Individual>(Assertion(*new_left, *new_right));
        else{
            auto new_assertion = Assertion(*left, *new_right);
            eval_ret = make_shared<Individual>(Assertion(new_assertion));
            // auto new_fact = make_shared<Fact>(Assertion(new_assertion));
            // question.fact_list.push_back(new_fact);
            // question.normalize_individual(new_fact);
        }
    }
    else if(indi->is_term && indi->term->is_oprt_apply){
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
        }
    }
    else if(indi->is_term){
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
                target = action_eval(args[0], question, conditions_sp);
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
                    target_1 = action_eval(args[0], question, conditions_sp);
            }
            auto target_2 = args[1]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!target_2)
                target_2 = action_eval(args[1], question, conditions_sp);
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
        else if(oprt=="Neg"){ // 对数学表达式进行取相反数 (参数是: 数学表达式 Math_Expr)
            assert(args.size()==1);
            auto target = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!target)
                target = action_eval(args[0], question, conditions_sp);
            assert(target->is_math_indi && target->math_indi->is_math_expr);
            auto target_val = target->math_indi->expr_val;
            if(target_val->is_num){
                Number temp = Number(*target_val->number_val);
                temp.trans_to_opposite();
                eval_ret = make_shared<Individual>(Math_Individual(temp));
            }
            else if(target_val->is_mathe && target_val->left->is_num){ // 只处理 left 为 Number 的情况
                // +-*/^ 式的相反数只需对其左部去相反数
                Number temp_left = Number(*target_val->left->number_val);
                temp_left.trans_to_opposite();
                Math_Expr temp;
                if(target_val->right->is_num)
                    temp = Math_Expr(Math_Expr(temp_left),target_val->op_val,Math_Expr(*target_val->right->number_val));
                else
                    temp = Math_Expr(Math_Expr(temp_left),target_val->op_val,target_val->right);
                eval_ret = make_shared<Individual>(Math_Individual(temp));
            }
        }
        else if(oprt=="Sqrt"){ // 对数学表达式进行开方 (参数是: 数学表达式 Math_Expr)
            assert(args.size()==1);
            auto target = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!target)
                target = action_eval(args[0], question, conditions_sp);
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
                    vector<shared_ptr<Individual>> new_args;
                    new_args.push_back(make_shared<Individual>(Math_Individual(Math_Expr(*target_val))));
                    eval_ret = make_shared<Individual>(Term("Sqrt",new_args));
                    eval_ret->val_is_known = true;
                }
            }
            else{ // 否则等于 sqrt(target_val)
                vector<shared_ptr<Individual>> new_args;
                new_args.push_back(make_shared<Individual>(Math_Individual(Math_Expr(*target_val))));
                eval_ret = make_shared<Individual>(Term("Sqrt",new_args));
                eval_ret->val_is_known = true;
                // vector<shared_ptr<Math_Expr>> new_args;
                // new_args.push_back(target_val);
                // eval_ret = make_shared<Individual>(Math_Individual(Math_Expr(Math_Func("sqrt",new_args))));
            }
        }
        else if(oprt=="Add"){ // 对若干个数学表达式进行相加 (参数是: 两个数学表达式 Math_Expr)
            // 暂时处理两个参数
            assert(args.size()==2);
            auto num_1 = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_1)
                num_1 = action_eval(args[0],question, conditions_sp);
            auto num_2 = args[1]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_2)
                num_2 = action_eval(args[1],question, conditions_sp);
            if(num_1 && num_1->is_math_indi && num_1->math_indi->is_math_expr && num_2 && num_2->is_math_indi && num_2->math_indi->is_math_expr){
                auto val_1 = num_1->math_indi->expr_val;
                auto val_2 = num_2->math_indi->expr_val;
                Math_Expr ret;
                if(val_1->is_num && val_2->is_num)
                    ret = Math_Expr(Number(*val_1->number_val + *val_2->number_val));
                else
                    ret = Math_Expr(val_1,'+',val_2);
                eval_ret = make_shared<Individual>(Math_Individual(ret));
            }
        }
        else if(oprt=="Sub"){ // 对若干个数学表达式进行相减 (参数是: 两个数学表达式 Math_Expr)
            // 暂时处理两个参数
            assert(args.size()==2);
            auto num_1 = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_1)
                num_1 = action_eval(args[0],question, conditions_sp);
            auto num_2 = args[1]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_2)
                num_2 = action_eval(args[1],question, conditions_sp);
            if(num_1 && num_1->is_math_indi && num_1->math_indi->is_math_expr && num_2 && num_2->is_math_indi && num_2->math_indi->is_math_expr){
                auto val_1 = num_1->math_indi->expr_val;
                auto val_2 = num_2->math_indi->expr_val;
                Math_Expr ret;
                if(val_1->is_num && val_2->is_num)
                    ret = Math_Expr(Number(*val_1->number_val - *val_2->number_val));
                else
                    ret = Math_Expr(val_1,'-',val_2);
                eval_ret = make_shared<Individual>(Math_Individual(ret));
            }
        }
        else if(oprt=="Mul"){ // 对若干个数学表达式进行相乘 (参数是: 若干个数学表达式 Math_Expr)
            // 暂时处理两个参数
            assert(args.size()==2);
            auto num_1 = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_1)
                num_1 = action_eval(args[0],question, conditions_sp);
            auto num_2 = args[1]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_2)
                num_2 = action_eval(args[1],question, conditions_sp);
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
        else if(oprt=="Div"){ // 对若干个数学表达式进行相除 (参数是: 若干个数学表达式 Math_Expr)
            // 暂时处理两个参数
            assert(args.size()==2);
            auto num_1 = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_1)
                num_1 = action_eval(args[0],question, conditions_sp);
            auto num_2 = args[1]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_2)
                num_2 = action_eval(args[1],question, conditions_sp);
            if(num_1 && num_1->is_math_indi && num_1->math_indi->is_math_expr && num_2 && num_2->is_math_indi && num_2->math_indi->is_math_expr){
                auto val_1 = num_1->math_indi->expr_val;
                auto val_2 = num_2->math_indi->expr_val;
                Math_Expr ret;
                Number div_val = *val_1->number_val / *val_2->number_val;
                if(val_1->is_num && val_2->is_num){
                    if(div_val.is_int)
                        ret = Math_Expr(div_val);
                    else
                        ret = Math_Expr(Math_Expr(Number(*val_1->number_val)),'/',Math_Expr(Number(*val_2->number_val)));
                }
                else if(val_1->is_num && !val_2->is_num)
                    ret = Math_Expr(val_1,'/',Math_Expr(Number(*val_2->number_val)));
                else if(!val_1->is_num && val_2->is_num)
                    ret = Math_Expr(Math_Expr(Number(*val_1->number_val)),'/',val_2);
                else
                    ret = Math_Expr(val_1,'/',val_2);
                eval_ret = make_shared<Individual>(Math_Individual(ret));
            }
        }
        else if(oprt=="Pow"){ // 对两个数进行幂运算 (参数是: 若干个数学表达式 Number)
            // 暂时处理两个参数
            assert(args.size()==2);
            auto num_1 = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_1)
                num_1 = action_eval(args[0],question, conditions_sp);
            auto num_2 = args[1]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_2)
                num_2 = action_eval(args[1],question, conditions_sp);
            if(num_1 && num_1->is_math_indi && num_1->math_indi->is_math_expr && num_2 && num_2->is_math_indi && num_2->math_indi->is_math_expr){
                auto val_1 = num_1->math_indi->expr_val;
                auto val_2 = num_2->math_indi->expr_val;
                Math_Expr ret;
                if(val_1->is_num && val_2->is_num){
                    ret = Math_Expr(Number(*val_1->number_val ^ *val_2->number_val));
                    eval_ret = make_shared<Individual>(Math_Individual(ret));
                }
            }
        }
        else if(oprt=="Generate_Line_Eq"){ // 根据系数 a、b、c 生成直线方程 ax+by+c=0 (参数: 三个 Math_Expr)
            assert(args.size()==3);
            auto a = args[0]->find_specific_indi("Math_Expr", question);
            if(!a)
                a = action_eval(args[0],question);
            auto b = args[1]->find_specific_indi("Math_Expr", question);
            if(!b)
                b = action_eval(args[1],question);
            auto c = args[2]->find_specific_indi("Math_Expr", question);
            if(!c)
                c = action_eval(args[2],question);
            if(a && a->is_math_indi && a->math_indi->is_math_expr && b && b->is_math_indi && b->math_indi->is_math_expr && c && c->is_math_indi && c->math_indi->is_math_expr){
                Number zero = Number(0);
                Number one = Number(1);
                Math_Expr x = Math_Expr("x");
                Math_Expr y = Math_Expr("y");
                auto expr_a = *a->math_indi->expr_val;
                auto expr_b = *b->math_indi->expr_val;
                auto expr_c = *c->math_indi->expr_val;
                // assert(expr_a.is_num && expr_b.is_num && expr_c.is_num);
                bool a_eq_0 =false, b_eq_0=false, c_eq_0=false;
                if(expr_a.is_num)
                    a_eq_0 = *expr_a.number_val == zero;
                if(expr_b.is_num)
                    b_eq_0 = *expr_b.number_val == zero;
                if(expr_b.is_num)
                    c_eq_0 = *expr_c.number_val == zero;
                assert(!(a_eq_0 && b_eq_0)); // a、b 不能同时为 0

                Math_Expr part_a, part_b, part_c; // a、b、c 都不为 0 时, 方程为: a*x +- |b|*y +- |c| = 0
                if(!(a_eq_0)){ // x 的系数可正可负
                    if(expr_a.is_num)
                        part_a = expr_a==one ? x : Math_Expr(expr_a,'*',x); // part_a 即 a*x
                    else if(expr_a.is_mathe)
                        part_a = Math_Expr(Math_Expr(make_shared<Math_Expr>(expr_a)),'*',x);
                    else
                        assert(false);
                }
                if(*expr_b.number_val > zero || (expr_b.is_mathe && *expr_b.get_num_val()>zero)){
                    if(expr_b.is_num)
                        part_b = expr_b==one ? y : Math_Expr(expr_b,'*',y);
                    else if(expr_b.is_mathe)
                        part_b = Math_Expr(Math_Expr(make_shared<Math_Expr>(expr_b)),'*',y);
                    else
                        assert(false);
                }
                else if(*expr_b.number_val < zero || (expr_b.is_mathe && *expr_b.get_num_val()<zero)){  // y 的系数不能为负
                    if(expr_b.is_num){
                        Number temp = Number(*expr_b.number_val);
                        temp.trans_to_opposite();
                        part_b = temp==one ? y : Math_Expr(temp,'*',y); // part_b 即 |b|*y
                    }
                    else if(expr_b.is_mathe && expr_b.left->is_num){ // 只处理 left 为 Number 的情况
                        // +-*/^ 式的相反数只需对其左部去相反数
                        Number temp_left = Number(*expr_b.left->number_val);
                        temp_left.trans_to_opposite();
                        Math_Expr temp;
                        if(expr_b.is_num)
                            temp = Math_Expr(Math_Expr(temp_left),expr_b.op_val,Math_Expr(*expr_b.right->number_val));
                        else
                            Math_Expr(Math_Expr(temp_left),expr_b.op_val,expr_b.right);
                        part_b = Math_Expr(Math_Expr(make_shared<Math_Expr>(temp)),'*',y);
                    }
                    else
                        assert(false);
                }
                assert(expr_c.is_num); // 简单处理
                if(*expr_c.number_val > zero) // 常数项不能为负
                    part_c = expr_c;
                else if(*expr_c.number_val < zero){
                    Number temp = Number(*expr_c.number_val);
                    temp.trans_to_opposite();
                    part_c = Math_Expr(temp); // part_c 即 |c|
                }
                
                Math_Expr eq_left; // 直线方程的左部
                Math_Expr eq_right = Math_Expr(zero); // 直线方程的右部
                // 先处理 a*x +- |b|*y 部分
                Math_Expr part_a_b;
                if(a_eq_0 && !b_eq_0)
                    part_a_b = part_b;
                else if(!a_eq_0 && b_eq_0)
                    part_a_b = part_a;
                else{
                    assert(!a_eq_0 && !b_eq_0);
                    if(*expr_b.number_val > zero)
                        part_a_b = Math_Expr(part_a, '+', part_b);
                    else
                        part_a_b = Math_Expr(part_a, '-', part_b);
                }
                // 补充上 +- |c| 部分
                if(*expr_c.number_val > zero)
                    eq_left = Math_Expr(part_a_b,'+',part_c);
                else if(*expr_c.number_val < zero)
                    eq_left = Math_Expr(part_a_b,'-',part_c);
                else if(*expr_c.number_val == zero)
                    eq_left = part_a_b;
                
                auto ret = Math_Equation(eq_left,eq_right);
                eval_ret = make_shared<Individual>(Math_Individual(ret));
                eval_ret->val_is_known = true;
            }
        }
        else if(oprt=="Generate_Ellipse_Eq"){ // 根据 椭圆参数 a、b、椭圆中心 生成标准椭圆方程 x^2/a^2+y^2/b^2=1 或 x^2/b^2+y^2/a^2=1 (参数: 椭圆参数 a、b Math_Expr、椭圆中心 Point)
            assert(args.size()==3);
            auto num_1 = args[0]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_1)
                num_1 = action_eval(args[0],question, conditions_sp);
            auto num_2 = args[1]->find_specific_indi("Math_Expr", question, conditions_sp);
            if(!num_2)
                num_2 = action_eval(args[1],question, conditions_sp);
            auto p = args[2]->find_specific_indi("Point", question, conditions_sp);
            if(!p)
                p = action_eval(args[1],question, conditions_sp);
            if(num_1 && num_1->is_math_indi && num_1->math_indi->is_math_expr && num_2 && num_2->is_math_indi && num_2->math_indi->is_math_expr && p && p->is_term && p->term->is_ctor){
                auto val_1 = num_1->math_indi->expr_val; // x^2 的 分母
                auto val_2 = num_2->math_indi->expr_val; // y^2 的 分母
                auto center = p->term->ctor_val;
                shared_ptr<Individual> center_x,center_y;
                for(auto assignment:center->content){
                    if(assignment->symbol=="x")
                        center_x = assignment->val;
                    else if(assignment->symbol=="y")
                        center_y = assignment->val;
                }
                auto center_x_num = *center_x->math_indi->expr_val->number_val;
                auto center_y_num = *center_y->math_indi->expr_val->number_val;
                Number zero = Number(0);
                Number two = Number(2);
                Math_Expr sq_x = Math_Expr(Math_Expr("x"),'^',two);
                Math_Expr sq_y = Math_Expr(Math_Expr("y"),'^',two);
                Math_Expr part_x,part_y;
                if(center_x_num==zero && center_y_num==zero){
                    if(val_1->is_num && val_2->is_num){
                        part_x = Math_Expr(sq_x,'/',Math_Expr(*val_1->number_val * *val_1->number_val));
                        part_y = Math_Expr(sq_y,'/',Math_Expr(*val_2->number_val * *val_2->number_val));
                        auto left = Math_Expr(part_x,'+',part_y);
                        auto ellipse_eq = Math_Equation(left,Math_Expr(Number(1)));
                        eval_ret = make_shared<Individual>(Math_Individual(ellipse_eq));
                        eval_ret->val_is_known = true;
                    }
                }
            }
        }
        else if(oprt=="Ex_Or"){ // 对多个个体进行求值, 结果保存在 Exclusive_Or 容器
            vector<shared_ptr<Individual>> new_args;
            for(auto arg:args){
                if(arg->val_is_known)
                    return eval_ret;
                new_args.push_back(action_eval(arg,question, conditions_sp));
            }
            eval_ret = make_shared<Individual>(Term(oprt,new_args));
            eval_ret->val_is_known = true;
        }
        else if(oprt=="Left"){ //  求方程的左部 (参数: Equation)
            assert(args.size()==1);
            auto ori_eq = args[0]->find_specific_indi("Equation", question);
            if(!ori_eq)
                ori_eq = action_eval(args[0], question);
            if(ori_eq){
                auto ret = Math_Expr(*ori_eq->math_indi->equation_val->left);
                eval_ret = make_shared<Individual>(Math_Individual(ret));
            }
        }
        else if(oprt=="Right"){ //  求方程的右部 (参数: Equation)
            assert(args.size()==1);
            auto ori_eq = args[0]->find_specific_indi("Equation", question);
            if(!ori_eq)
                ori_eq = action_eval(args[0], question);
            if(ori_eq){
                auto ret = Math_Expr(*ori_eq->math_indi->equation_val->right);
                eval_ret = make_shared<Individual>(Math_Individual(ret));
            }
        }
        else if(oprt=="Subst"){ // 对数学表达式中进行符号替换 (参数: 三个 Math_Expr)
            assert(args.size()==3);
            auto ori_expr = args[0]->find_specific_indi("Math_Expr", question); // 最初的数学表达式
            if(!ori_expr)
                ori_expr = action_eval(args[0], question);
            auto var = args[1]->find_specific_indi("Math_Expr", question); // 要替换的变量
            if(!var)
                var = action_eval(args[1], question);
            auto val = args[2]->find_specific_indi("Math_Expr", question); // 用于替换的值
            if(!val)
                val = action_eval(args[2], question);
            cout<<"即: 把 "<<*ori_expr<<" 中的 "<<*var<<" 替换为 "<<*val<<endl;
            assert(ori_expr->is_math_indi && ori_expr->math_indi->is_math_expr);
            assert(var->is_math_indi && var->math_indi->is_math_expr && var->math_indi->expr_val->is_sy);
            assert(val->is_math_indi && val->math_indi->is_math_expr && val->math_indi->expr_val->is_num);
            string var_sy = var->math_indi->expr_val->sy_val; // 要替换的变量的名称
            auto val_num = *val->math_indi->expr_val->number_val; // 用于替换的 Number
            auto ret = subst_sy_with_num(make_shared<Math_Expr>(ori_expr->math_indi->expr_val->get_copy()), var_sy, val_num);
            cout<<"替换完之后的表达式为: "<<*ret<<endl;
            eval_ret = make_shared<Individual>(Math_Individual(*ret));
        }
        else if(oprt=="Solve_Unknown"){ // 求解方程中的唯一的未知数
            assert(args.size()==1);
            auto ori_eq = args[0]->find_specific_indi("Equation", question); // 原方程
            if(!ori_eq)
                ori_eq = action_eval(args[0], question);
            // 这里简单处理，只处理特定形式的方程
            assert(ori_eq->is_assertion);
            auto l = ori_eq->assertion->left;
            auto r = ori_eq->assertion->right;
            assert(l->is_math_indi && l->math_indi->is_math_expr && l->math_indi->expr_val->op_val=='^' && l->math_indi->expr_val->left->is_num);
            auto l_l_num = l->math_indi->expr_val->left->number_val;
            auto l_r_num = l->math_indi->expr_val->right->number_val;
            auto l_num = Number(*l_l_num ^ 2);
            assert(r->is_math_indi && r->math_indi->is_math_expr && r->math_indi->expr_val->op_val=='*' && r->math_indi->expr_val->left->op_val=='*' && r->math_indi->expr_val->right->is_num);
            assert(r->math_indi->expr_val->left->left->is_num && r->math_indi->expr_val->right->is_num);
            auto r_l_num = *r->math_indi->expr_val->left->left->number_val;
            auto r_r_num = *r->math_indi->expr_val->right->number_val;
            auto ret = l_num / r_l_num / r_r_num;
            eval_ret = make_shared<Individual>(Math_Individual(Math_Expr(Number(ret))));
        }
        else if(oprt=="Subst_Unknown"){ // 替换方程中的未知数 (参数是: 原方程 Equation、未知数的值)
            assert(args.size()==2);
            auto ori_eq = args[0]->find_specific_indi("Equation", question); // 原方程
            auto unknown_num = args[1]->find_specific_indi("Math_Expr", question); // 未知数的值
            if(!unknown_num)
                unknown_num = action_eval(args[1], question);
            assert(unknown_num->is_math_indi && unknown_num->math_indi->is_math_expr && unknown_num->math_indi->expr_val->is_num);
            // 这里简单处理，只处理特定形式的代入
            auto ori_eq_l = ori_eq->math_indi->equation_val->left->get_copy();
            auto ori_eq_r = ori_eq->math_indi->equation_val->right->get_copy();
            assert(ori_eq_r.is_mathe && ori_eq_r.left->is_mathe && ori_eq_r.left->left->is_num && ori_eq_r.left->right->is_sy);
            auto new_eq_r = Math_Expr(ori_eq_r); // 替换后的方程右部
            *new_eq_r.left = *ori_eq_r.left->left->number_val * *unknown_num->math_indi->expr_val->number_val;
            auto ret = Math_Equation(ori_eq_l, new_eq_r);
            eval_ret = make_shared<Individual>(Math_Individual(Math_Equation(ret)));
            eval_ret->val_is_known = true;
        }
        else{
            auto it = question.kb->def_oprt_hash_table.find(oprt); // KB 中定义的 Opearator
            if(it != question.kb->def_oprt_hash_table.end()){
                map<string,string> abs_to_con; // 结合定义时的 Opearator 来构建 abstract_to_concrete
                assert(args.size()==it->second->input.size());
                for(size_t i=0; i!=args.size(); ++i){
                    abs_to_con.insert(pair<string,string>(it->second->input[i]->symbol, args[i]->get_output_str()));
                }
                auto def_val = *it->second; // 算子定义的值
                auto ins_def_val = *def_val.instantiate(abs_to_con);
                cout<<"实例化后的 Def_Operator 为: "<<ins_def_val.get_output_str()<<endl;
                question.normalize_individual(ins_def_val.output);
                ins_def_val.output->propagate_var_decl(indi->var_decl);
                eval_ret = action_eval(ins_def_val.output, question);
            }
            else
                cout<<oprt<<" 算子未定义!"<<endl;
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