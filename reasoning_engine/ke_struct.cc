#include "ke_struct.hh"
#include "global_var_decl.hh"
// #define NDEBUG

// AL抽象语法树相关的数据结构 的相关操作实现


// 下面是重载 << 相关:

ostream& operator<<(ostream &os, const Number &e){ // 输出 Number
    assert(e.is_int+e.is_float==1);
    if(e.is_int) 
        os<<e.i_val;
    else
        os<<e.f_val;
    return os;
}

ostream& operator<<(ostream &os, const Math_Expr &e){ // 输出数学表达式
    assert(e.is_num+e.is_sy+e.is_func+e.is_mathe==1);
    if(e.is_mathe){ // 判断是否是 +-*/^
        os<<*e.left<<e.op_val<<*e.right;
    }
    else if(e.is_sy) // 判断是否是符号
        os<<e.sy_val;
    else if(e.is_func) // 判断是否是函数
        os<<*e.func_val;
    else if(e.is_num)  // 判断是否是数
        os<<*e.number_val;
    else{
        // 原则上以下语句并不会执行，如果执行了说明存在逻辑错误
        #ifndef NDEBUG
            std::cerr<<"无法识别该 Math_Expr!"<<endl;
        #endif
    }
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Math_Expr中变量:"<<e.var_decl<<"] ";
    return os;
}

ostream& operator<<(ostream &os, const Math_Func &e){ // 输出函数
    os<<e.func_name<<"("<<e.args<<")";
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Math_Func中变量:"<<e.var_decl<<"] ";
    return os;
}

ostream& operator<<(ostream &os, const vector<shared_ptr<Math_Expr>> &e){ // 输出vector<shared_ptr<Math_Expr>>
    string sep = "";
    for(auto i:e){
        os<<sep<<*i;
        sep = ", ";
    }
    return os;
}

ostream& operator<<(ostream &os, const Math_Equation &e){ // 输出数学方程
    os<<*e.left<<"=="<<*e.right;
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Math_Equation中变量:"<<e.var_decl<<"] ";
    return os;
}

ostream& operator<<(ostream &os, const Coordinate &e){ // 输出坐标
    os<<"("<<*e.abscissa<<","<<*e.ordinate<<")";
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Coordinate中变量:"<<e.var_decl<<"] ";
    return os;
}

ostream& operator<<(ostream &os, const Math_Individual &e){ // 输出数学个体
    assert(e.is_equation+e.is_coordinate+e.is_math_expr==1);
    if(e.is_equation)
        os<<*e.equation_val;
    else if(e.is_coordinate)
        os<<*e.coordinate_val;
    else if(e.is_math_expr)
        os<<*e.expr_val;
    else{
        #ifndef NDEBUG
            std::cerr<<"无法识别该 Math_Individual!"<<endl;
        #endif
    }
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Math_Individual中变量:"<<e.var_decl<<"] ";
    return os;
}

ostream& operator<<(ostream &os, const Concept &e){ // 输出概念
    assert(e.is_atomic+e.is_compound==1);
    if(e.is_atomic)
        os<<e.atomic_concept;
    else{ // compound_concept
        os<<e.outer_oprt<<'('<<*e.inner_concept<<')';
    }
    return os;
}

ostream& operator<<(ostream &os, const Variable &e){ // 输出变量
    os<<e.symbol<<':'<<*e.concept;
    return os;
}

ostream& operator<<(ostream &os, const vector<shared_ptr<Variable>> &e){
    string sep = "";
    for(auto i:e){
        os<<sep<<*i;
        sep = "; ";
    }
    return os;
}

ostream& operator<<(ostream &os, const Individual &e){ // 输出个体
    assert(e.is_var+e.is_bool+e.is_cud+e.is_term+e.is_assertion+e.is_math_indi==1);
    if(e.is_var)
        os<<*e.var_val;
    else if(e.is_bool)
        os<<(e.bool_val?"true":"false");
    else if(e.is_cud)
        os<<*e.cud;
    else if(e.is_term)
        os<<*e.term;
    else if(e.is_assertion)
        os<<*e.assertion;
    else if(e.is_math_indi)
        os<<*e.math_indi;
    else{
        #ifndef NDEBUG
            std::cerr<<"无法识别该 Individual!"<<endl;
        #endif
    }
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Individual中变量:"<<e.var_decl<<"] ";
    return os;
}

ostream& operator<<(ostream &os, const vector<shared_ptr<Individual>> &e){
    string sep = "";
    for(auto i:e){
        os<<sep<<*i;
        sep = ", ";
    }
    return os;
}


ostream& operator<<(ostream &os, const Assignment &e){ // 输出 Assignment
    os<<e.symbol<<":="<<*e.val;
    return os;
}

ostream& operator<<(ostream &os, const vector<shared_ptr<Assignment>> &e){
    string sep = "";
    for(auto i:e){
        os<<sep<<*i;
        sep = "; ";
    }
    return os;
}

ostream& operator<<(ostream &os, const Cud &e){ // 输出 cud
    os<<e.action<<" "<<*e.left<<"="<<*e.right;
    return os;
}

ostream& operator<<(ostream &os, const Sugar_For_And &e){ // 输出 sugar_for_and
    os<<"{";
    string sep = "";
    for(auto i:e.content){
        os<<sep<<*i;
        sep = "; ";
    }
    os<<"}";
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Sugar_For_And中变量:"<<e.var_decl<<"] ";
    return os;
}

ostream& operator<<(ostream &os, const Sugar_For_Pred &e){ // 输出 sugar_for_pred
    os<<"{"<<*e.left<<e.predicate<<*e.right<<"}";
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Sugar_For_Pred中变量:"<<e.var_decl<<"] ";
    return os;
}

ostream& operator<<(ostream &os, const Sugar_For_Ctor &e){ // 输出 sugar_for_ctor
    os<<"{"<<e.content<<"}";
    return os;
}

ostream& operator<<(ostream &os, const Sugar_For_Oprt_Apply &e){ // 输出 sugar_for_oprt_apply
    os<<e.indi<<"."<<e.uni_oprt;
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Sugar_For_Oprt_Apply中变量:"<<e.var_decl<<"] ";
    return os;
}

ostream& operator<<(ostream &os, const Term &e){ // 输出 term
    assert(e.is_and+e.is_pred+e.is_ctor+e.is_oprt_apply+e.is_std==1);
    if(e.is_and)
        os<<*e.and_val;
    else if(e.is_pred)
        os<<*e.pred_val;
    else if(e.is_ctor)
        os<<*e.ctor_val;
    else if(e.is_oprt_apply)
        os<<*e.oprt_apply_val;
    else if(e.is_std){
        os<<e.oprt<<"(";
        os<<e.args<<")";
    }
    else{
        #ifndef NDEBUG
            std::cerr<<"无法识别该 Term!"<<endl;
        #endif      
    }
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Term中变量:"<<e.var_decl<<"] ";
    return os;
}

ostream& operator<<(ostream &os, const Assertion &e){ // 输出 Assertion
    assert(e.is_std+e.is_sugar_for_true==1);
    if(e.is_std)
        os<<"{"<<*e.left<<"="<<*e.right<<"}";
    else // is_sugar_for_true
        os<<"{"<<*e.lonely_left<<"}";
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Assertion中变量:"<<e.var_decl<<"] ";
    return os;
}

ostream& operator<<(ostream &os, const Def_Individual &e){ // 输出定义个体
    os<<"def_indi "<<e.symbol<<":"<<*e.concept<<"="<<*e.indi;
    return os;
}

ostream& operator<<(ostream &os, const vector<shared_ptr<Def_Individual>> &e){
    for(auto i:e)
        os<<*i<<endl<<endl;
    return os;
}

ostream& operator<<(ostream &os, const Fact &e){ // 输出事实
    assert(e.is_assert+e.is_var+e.is_def_indi==1);
    if(e.is_assert)
        os<<*e.assertion;
    else if(e.is_var)
        os<<*e.variable;
    else // is_def_indi
        os<<*e.def_indi;
    return os;
}

ostream& operator<<(ostream &os, const vector<shared_ptr<Fact>> &e){
    string sep = "";
    for(auto i:e){
        os<<sep<<*i;
        sep = "; ";
    }
    return os;
}

ostream& operator<<(ostream &os, const Question &e){ // 输出问题
    os<<e.description<<endl;
    os<<e.fact_list<<endl;
    os<<"To_Solve("<<e.to_solve<<")"<<endl;
    return os;
}

ostream& operator<<(ostream &os, const vector<shared_ptr<Question>> &e){
    for(auto i:e)
        os<<*i<<endl;
    return os;
}

ostream& operator<<(ostream &os, const Def_Concept &e){ // 输出定义概念
    assert(e.with_parent+e.without_parent==1);
    if(e.without_parent)
        os<<"def_cpt "<<*e.concept<<"="<<"{"<<e.members<<"}";
    else // with_parent
        os<<"def_cpt "<<*e.concept<<"<-"<<*e.parent_cpt<<"="<<"{"<<e.members<<"}";
    return os;
}

ostream& operator<<(ostream &os, const vector<shared_ptr<Def_Concept>> &e){
    for(auto i:e)
        os<<*i<<endl<<endl;
    return os;
}

ostream& operator<<(ostream &os, const Def_Operator &e){ // 输出定义算子
    os<<"def_op "<<e.oprt<<"("<<e.input<<")"<<"="<<*e.output;
    return os;
}

ostream& operator<<(ostream &os, const vector<shared_ptr<Def_Operator>> &e){
    for(auto i:e)
        os<<*i<<endl<<endl;
    return os;
}

ostream& operator<<(ostream &os, const Rule &e){ // 输出规则
    os<<*e.lhs<<" => "<<*e.rhs<<"  "<<e.description;
    return os;
}

ostream& operator<<(ostream &os, const vector<shared_ptr<Rule>> &e){
    for(auto i:e)
        os<<*i<<endl<<endl;
    return os;
}

ostream& operator<<(ostream &os, const Knowledge_Base &e){ // 输出知识库
    os<<"-- Concept 定义部分"<<endl<<endl;
    os<<e.def_concepts<<endl;
    os<<"-- Individual 定义部分"<<endl<<endl;
    os<<e.def_individuals<<endl;
    os<<"-- Operator 定义部分"<<endl<<endl;
    os<<e.def_operators<<endl;
    os<<"-- 知识部分"<<endl<<endl;
    os<<e.rules<<endl; 
    return os;
}

ostream& operator<<(ostream &os, const Rete_Rule &e){ // 输出适应推理系统改造后的规则
    os<<"(";
    string sep = "";
    for(const auto &i:e.var_decl){
        cout<<sep<<i.first<<":"<<*i.second;
        sep = "; ";
    }
    os<<")\t";
    if(e.lhs) // 改造后的lhs部分可能为空个体
        os<<*e.lhs;
    else
        os<<"\u2205"; // lhs为空输出 ∅
    os<<" => "<<*e.rhs<<"  "<<e.description;
    return os;
}

ostream& operator<<(ostream &os, const map<string, shared_ptr<Concept>> &m){ // 输出变量声明
    string sep = "";
    for(const auto &i:m){
        cout<<sep<<i.first<<":"<<*i.second;
        sep = "; ";
    }
    return os;
}

ostream& operator<<(ostream &os, const Rete_Question &e){ // 输出适应推理系统改造后的问题
    os<<e.description<<endl;
    os<<"(";
    string sep = "";
    for(const auto &i:e.var_decl){
        cout<<sep<<i.first<<":"<<*i.second;
        sep = "; ";
    }
    os<<")"<<endl;
    os<<e.fact_list<<endl;
    os<<"To_Solve("<<e.to_solve<<")"<<endl;
    return os;
}


// 下面是重载 == 相关:

// Number 重载 ==
bool Number::operator==(const Number &rhs) const{
    if(is_int)
        return rhs.is_int && i_val==rhs.i_val;
    else // float
        return rhs.is_float && f_val==rhs.f_val;
}

// Math_Expr 重载 ==
bool Math_Expr::operator==(const Math_Expr &rhs) const{
    if(is_sy)
        return rhs.is_sy && sy_val==rhs.sy_val;
    else if(is_func)
        return rhs.is_func && func_val == rhs.func_val;
    else if(is_mathe)
        return rhs.is_mathe && op_val==rhs.op_val && *left==*right;
    else // number
        return rhs.is_num && number_val==rhs.number_val;
}

// Math_Func 重载 ==
bool Math_Func::operator==(const Math_Func &rhs) const{
    if(func_name!=rhs.func_name || args.size()!=rhs.args.size())
        return false;
    for(size_t i=0;i!=args.size();++i){
        if(!(*args[i]==*rhs.args[i]))
            return false;
    }
    return true;
}

// Math_Individual 重载 ==
bool Math_Individual::operator==(const Math_Individual &rhs) const{
    if(is_equation)
        return rhs.is_equation && equation_val==rhs.equation_val;
    else if(is_coordinate)
        return rhs.is_coordinate && coordinate_val==rhs.coordinate_val;
    else // math_expr
        return rhs.is_math_expr && *expr_val==*rhs.expr_val;
}

// Concept 重载 ==
bool Concept::operator==(const Concept &rhs) const{
    if(is_atomic)
        return rhs.is_atomic && atomic_concept==rhs.atomic_concept;
    else // is_compound
        return rhs.is_compound && outer_oprt==rhs.outer_oprt && *inner_concept==*rhs.inner_concept;
}

// Individual 重载 ==
bool Individual::operator==(const Individual &rhs) const{
    if(is_var)
        return rhs.is_var && var_val==rhs.var_val;
    else if(is_bool)
        return rhs.is_bool && bool_val==rhs.bool_val;
    else if(is_cud)
        return rhs.is_cud && *cud==*rhs.cud;
    else if(is_term)
        return rhs.is_term && *term==*rhs.term;
    else if(is_assertion)
        return rhs.is_assertion && *assertion==*rhs.assertion;
    else // math_individual
        return rhs.is_math_indi && *math_indi==*rhs.math_indi;
}

// Assignment 重载 ==
bool Assignment::operator==(const Assignment &rhs) const{
    return symbol==rhs.symbol && *val==*rhs.val;
}

// Sugar_For_And 重载 ==
bool Sugar_For_And::operator==(const Sugar_For_And &rhs) const{
    for(size_t i=0;i!=content.size();++i){
        if(!(*content[i]==*rhs.content[i]))
            return false;
    }
    return true;
}

// Sugar_For_Pred 重载 ==
bool Sugar_For_Pred::operator==(const Sugar_For_Pred &rhs) const{
    return left==rhs.left && right==rhs.right && predicate==rhs.predicate;
}

// Sugar_For_Ctor 重载 ==
bool Sugar_For_Ctor::operator==(const Sugar_For_Ctor &rhs) const{
    for(size_t i=0;i!=content.size();++i){
        if(!(*content[i]==*rhs.content[i]))
            return false;
    }
    return true;
}

// Sugar_For_Oprt_Apply 重载 ==
bool Sugar_For_Oprt_Apply::operator==(const Sugar_For_Oprt_Apply &rhs) const{
    return indi==rhs.indi && uni_oprt==rhs.uni_oprt;
}

// Term 重载 ==
bool Term::operator==(const Term &rhs) const{
    if(is_and)
        return rhs.is_and && and_val == rhs.and_val;
    else if(is_pred)
        return rhs.is_pred && pred_val == rhs.pred_val;
    else if(is_ctor)
        return rhs.is_ctor && ctor_val == rhs.ctor_val;
    else if(is_oprt_apply)
        return rhs.is_oprt_apply && oprt_apply_val == rhs.oprt_apply_val;
    else{ // is_std
        if(!rhs.is_std)
            return false;
        if(!(oprt==rhs.oprt))
            return false;
        else{
            for(size_t i=0;i!=rhs.args.size();++i){
                if(!(*args[i]==*rhs.args[i]))
                    return false;
            }
            return true;
        }
    }
}

// Assertion 重载 ==
bool Assertion::operator==(const Assertion &rhs) const{
    if(is_std)
        return left==rhs.left && right==rhs.right;
    else // is_sugar_for_true
        return lonely_left==rhs.lonely_left;
}

// Def_Individual 重载 ==
bool Def_Individual::operator==(const Def_Individual &rhs) const{
    return symbol==rhs.symbol && concept==rhs.concept && indi==rhs.indi;
}

// Fact 重载 ==
bool Fact::operator==(const Fact &rhs) const{
    if(rhs.is_assert)
        return assertion==rhs.assertion;
    else if(rhs.is_var)
        return variable==rhs.variable;
    else // is_def_indi
        return def_indi==rhs.def_indi;
}




// 下面是 传播变量声明 相关:

// Knowledge_Base 部分:

void Knowledge_Base::propagate_var_decl(){ // 传播变量声明到改造后的规则
    cout<<"开始传播变量声明到改造后的规则..."<<endl;
    for(auto &r:rete_rules){
        if(r->lhs)
            r->lhs->propagate_var_decl(r->var_decl);
        r->rhs->propagate_var_decl(r->var_decl);
    }
}

// 每个部分一边向下传递变量声明，一边要让下级把所携带变量的信息传回自身（如果自身未处理的话） !! 注意检查是否成功传递

void Individual::propagate_var_decl(const map<string, shared_ptr<Concept>> &v){ // 传播变量声明
    // Individual 有6种形式：变量、布尔值、CUD、项、断言、数学个体
    // 由于这里不可能是变量（经过改造）、布尔值和CUD不需要变量声明信息，所以只需要处理后3种情况：项、断言、数学个体
    if(is_term){
        term->propagate_var_decl(v, *this);
        assert(var_decl.size()==term->var_decl.size());
    }
    else if(is_assertion){
        assertion->propagate_var_decl(v, *this);
        assert(var_decl.size()==assertion->var_decl.size());
    }
    else if(is_math_indi){
        math_indi->propagate_var_decl(v, *this);
        assert(var_decl.size()==math_indi->var_decl.size());
    }
}

void Individual::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Term &parent){ // 传播变量声明 (Individual 的上层可能是 Term)
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Term
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Individual::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Sugar_For_And &parent){ // 传播变量声明 (Individual 的上层可能是 Sugar_For_And)
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Sugar_For_And
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Individual::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Sugar_For_Pred &parent){ // 传播变量声明 (Individual 的上层可能是 Sugar_For_Pred)
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Sugar_For_Pred
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Individual::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Assertion &parent){ // 传播变量声明 (Individual 的上层可能是 Assertion)
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Sugar_For_Pred
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Term::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Individual &parent){ // 传播变量声明
    if(is_and){
        and_val->propagate_var_decl(v, *this);
        assert(var_decl.size()==and_val->var_decl.size());
    }
    else if(is_pred){
        pred_val->propagate_var_decl(v, *this);
        assert(var_decl.size()==pred_val->var_decl.size());
    }
    else if(is_ctor)
        ; // Sugar_For_Ctor 不需要处理
    else if(is_oprt_apply){
        oprt_apply_val->propagate_var_decl(v, *this);
        assert(var_decl.size()==oprt_apply_val->var_decl.size());
    }
    else{
        assert(is_std);
        for(auto &arg:args)
            arg->propagate_var_decl(v, *this);
    }
    // 把自身的变量声明信息向上添加到父 Individual
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Sugar_For_And::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Term &parent){ // 传播变量声明
    for(auto &indi:content)
        indi->propagate_var_decl(v, *this);
    // 把自身的变量声明信息向上添加到父 Term
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Sugar_For_Pred::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Term &parent){ // 传播变量声明
    left->propagate_var_decl(v, *this);
    right->propagate_var_decl(v, *this);
    // 把自身的变量声明信息向上添加到父 Term
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Sugar_For_Oprt_Apply::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Term &parent){ // 传播变量声明
    // 找到对应的变量声明填充自身信息
    for(const auto &var_info:v){
        if(var_info.first==this->indi)
            var_decl.insert(std::pair<string,shared_ptr<Concept>>(var_info.first,var_info.second));
    }
    // 把自身的变量声明信息向上添加到父 Term
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Assertion::propagate_var_decl(const map<string, shared_ptr<Concept>> &v){ // 传播变量声明
    if(is_std){
        left->propagate_var_decl(v, *this);
        right->propagate_var_decl(v, *this);
    }
    else{
        assert(is_sugar_for_true);
        lonely_left->propagate_var_decl(v, *this);
    }
}

void Assertion::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Individual &parent){ // 传播变量声明
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Individual
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Math_Individual::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Individual &parent){ // 传播变量声明
    if(is_equation){
        equation_val->propagate_var_decl(v, *this);
        assert(var_decl.size()==equation_val->var_decl.size());
    }
    else if(is_coordinate){
        coordinate_val->propagate_var_decl(v, *this);
        assert(var_decl.size()==coordinate_val->var_decl.size());
    }
    else{
        assert(is_math_expr);
        expr_val->propagate_var_decl(v, *this);
        assert(var_decl.size()==expr_val->var_decl.size());
    }
    // 把自身的变量声明信息向上添加到父 Individual
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Math_Equation::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Individual &parent){ // 传播变量声明
    left->propagate_var_decl(v, *this);
    right->propagate_var_decl(v, *this);
    // 把自身的变量声明信息向上添加到父 Math_Individual
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Coordinate::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Individual &parent){ // 传播变量声明
    abscissa->propagate_var_decl(v, *this);
    ordinate->propagate_var_decl(v, *this);
    // 把自身的变量声明信息向上添加到父 Math_Individual
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Math_Expr::propagate_var_decl(const map<string, shared_ptr<Concept>> &v){ // 传播变量声明
    if(is_num)
        ; // Number 不需要处理
    else if(is_sy){
        // 找到对应的变量声明填充自身信息
        for(const auto &var_info:v){
            if(var_info.first==this->sy_val)
                var_decl.insert(std::pair<string,shared_ptr<Concept>>(var_info.first,var_info.second));
        }
    }
    else if(is_func){
        func_val->propagate_var_decl(v, *this);
        assert(var_decl.size()==func_val->var_decl.size());
    }
    else{
        assert(is_mathe);
        left->propagate_var_decl(v, *this);
        right->propagate_var_decl(v, *this);
    }
}

void Math_Expr::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Individual &parent){ // 传播变量声明 (Math_Expr 的上层可能是 Math_Individual)
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Math_Individual
    for(auto &var_info:var_decl){
        parent.var_decl.insert(var_info);
    }
}

void Math_Expr::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Equation &parent){ // 传播变量声明 (Math_Expr 的上层可能是 Math_Equation)
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Math_Equation
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Math_Expr::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Coordinate &parent){ // 传播变量声明 (Math_Expr 的上层可能是 Coordinate)
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Coordinate
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Math_Expr::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Func &parent){ // 传播变量声明 (Math_Expr 的上层可能是 Math_Func)
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Math_Func
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Math_Expr::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Expr &parent){ // 传播变量声明 (Math_Expr 的上层可能是 Math_Expr)
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Math_Expr
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Math_Func::propagate_var_decl(const map<string, shared_ptr<Concept>> &v){ // 传播变量声明
    for(auto &arg:args)
        arg->propagate_var_decl(v, *this);
}

void Math_Func::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Individual &parent){ // 传播变量声明 (Math_Func 的上层可能是 Math_Individual)
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Math_Individual
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

void Math_Func::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Expr &parent){ // 传播变量声明 (Math_Func 的上层可能是 Math_Expr)
    propagate_var_decl(v);
    // 把自身的变量声明信息向上添加到父 Math_Expr
    for(auto &var_info:var_decl)
        parent.var_decl.insert(var_info);
}

// Question 部分:

void Question::propagate_var_decl(){ // 传播变量声明到改造后的问题
    // 处理 fact_list
    for(auto fact:rete_question->fact_list){ // 改造后的 fact 只有两种情况：Assertion 和 Def_Indi
        if(fact->is_assert){
            fact->assertion->propagate_var_decl(rete_question->var_decl);
        }
        else if(fact->is_def_indi){
            ; // Def_Indi 暂时不用处理
        }
    }
    // 处理 to_solve
    for(auto unknown:rete_question->to_solve)
        unknown->propagate_var_decl(rete_question->var_decl);
}








// 下面是各个类自身特殊的成员函数:

shared_ptr<Rete_Rule> Rule::get_adapted(){ // 获取适配 Rete 算法版本的规则
    // 提取 lhs 中的变量声明
    map<string,shared_ptr<Concept>> var_decl; // 存放变量声明
    shared_ptr<Rete_Rule> ret;
    shared_ptr<Rule> rule;
    // lhs 要么是单个 Variable, 要么是 Sugar_For_And
    if(lhs->is_var){
        var_decl.insert(pair<string,shared_ptr<Concept>>(lhs->var_val->symbol,lhs->var_val->concept));
        // lhs 除去 Variable 后为 nullptr
        shared_ptr<Individual> to_be_empty = make_shared<Individual>(true);
        rule = make_shared<Rule>(*to_be_empty,*rhs,description); // 先使用一个任意的 lhs 来参与初始化
        rule->lhs = nullptr;
    }
    else{
        assert(lhs->is_term && lhs->term->is_and);
        // 提取每个Variable
        vector<shared_ptr<Individual>> not_var; // 暂时存放非变量的个体
        auto &vec = lhs->term->and_val->content;
        for(size_t i=0;i!=vec.size();++i){
            if(vec[i]->is_var)
                var_decl.insert(pair<string,shared_ptr<Concept>>(vec[i]->var_val->symbol,vec[i]->var_val->concept));
            else
                not_var.push_back(vec[i]);
        }
        shared_ptr<Individual> new_lhs;
        if(not_var.size()==0){ // 如果非变量个体数量为0, 处理后的 lhs 为 nullptr
            shared_ptr<Individual> to_be_empty = make_shared<Individual>(true);
            rule = make_shared<Rule>(*to_be_empty,*rhs,description);
            rule->lhs = nullptr;
        }
        else if(not_var.size()==1){ // 如果非变量个体数量为1, 根据类型创建新的 lhs
            auto &indi = not_var[0];
            if(indi->is_assertion)
                new_lhs = make_shared<Individual>(*indi->assertion);
            else if(indi->is_term)
                new_lhs = make_shared<Individual>(*indi->term);
            else // 不可能是其它的情况
                cerr<<"At line "<<__LINE__<<" get_adapted 时出错"<<endl;
            rule = make_shared<Rule>(*new_lhs,*rhs,description);
        }
        else{ // 如果非变量个体数量大于1, 新的 lhs 还是 Sugar_For_And
            new_lhs = make_shared<Individual>(Term(*make_shared<Sugar_For_And>(not_var)));
            rule = make_shared<Rule>(*new_lhs,*rhs,description);
        }
    }
    ret = make_shared<Rete_Rule>(*rule,var_decl);
    return ret;
}

void Question::get_adapted_question(){ // 改造原始问题以得到易于进行推理的问题
    // 提取 fact_list 中的变量声明
    map<string,shared_ptr<Concept>> var_decl; // 存放变量声明
    shared_ptr<Rete_Question> ret;
    vector<shared_ptr<Fact>> new_fact_lists;
    // Fact 是变量或是定义个体时都需要提取变量声明
    for(size_t i=0;i!=fact_list.size();++i){
        if(fact_list[i]->is_var)
            var_decl.insert(pair<string,shared_ptr<Concept>>(fact_list[i]->variable->symbol,fact_list[i]->variable->concept));
        else if(fact_list[i]->is_def_indi){
            var_decl.insert(pair<string,shared_ptr<Concept>>(fact_list[i]->def_indi->symbol,fact_list[i]->def_indi->concept));
            new_fact_lists.push_back(fact_list[i]);
        }
        else{ // is_assert
            new_fact_lists.push_back(fact_list[i]);
        }
    }
    shared_ptr<Question> q = make_shared<Question>(description,new_fact_lists,to_solve);
    ret = make_shared<Rete_Question>(*q,var_decl);
    this->rete_question = ret; // 填充自身的成员值
}