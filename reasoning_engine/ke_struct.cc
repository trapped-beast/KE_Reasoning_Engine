#include "ke_struct.hh"
#include "global_var_decl.hh"

// AL抽象语法树相关的数据结构 的相关操作实现


// 变量声明的字符串输出
string str_of_var_decl(const map<string, shared_ptr<Concept>> &var_decl){
    std::ostringstream oss;
    string sep = "";
    for(auto &i:var_decl){
        oss<<sep<<i.first<<":"<<*i.second;
        sep = "; ";
    }
    return oss.str();
}

// 约束变元的实例对应的字符串输出
string str_of_abs_to_con(const map<string, string> &abstrct_to_concrete){
    std::ostringstream oss;
    string sep = "";
    for(auto &i:abstrct_to_concrete){
        oss<<sep<<i.first<<":"<<i.second;
        sep = "; ";
    }
    return oss.str();
}


// 把变量声明改造为 Individual(具体地说是Variable)
shared_ptr<Individual> var_decl_to_indi(const map<string, shared_ptr<Concept>> &var_decl){
    shared_ptr<Individual> ret;
    auto it = var_decl.begin();
    if(var_decl.size()==1){ // 结果是单个 Variable 提升上来的 Individual
        shared_ptr<Variable> var = make_shared<Variable>(it->first,*it->second);
        ret =  make_shared<Individual>(*var);
    }
    else{ // 结果是多个 Individual 组成的 sugar_for_and ，进而提升上来的 Individual
        assert(var_decl.size()>1);
        vector<shared_ptr<Individual>> and_content; // sugar_for_and 的内容
        while(it!=var_decl.end()){
            map<string, shared_ptr<Concept>> new_var_decl;
            new_var_decl.insert(*it);
            and_content.push_back(var_decl_to_indi(new_var_decl));
        }
        ret = make_shared<Individual>(*make_shared<Term>(*make_shared<Sugar_For_And>(and_content)));
    }
    return ret;
}


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
    assert(e.is_num+e.is_sy+e.is_func+e.is_mathe+e.is_enclosed==1);
    if(e.is_mathe){ // 判断是否是 +-*/^
        os<<*e.left<<e.op_val<<*e.right;
    }
    else if(e.is_sy) // 判断是否是符号
        os<<e.sy_val;
    else if(e.is_func) // 判断是否是函数
        os<<*e.func_val;
    else if(e.is_num)  // 判断是否是数
        os<<*e.number_val;
    else if(e.is_enclosed) // 判断是括号包起来的数学表达式
        os<<"("<< *e.enclosed_expr<<")";
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
    assert(e.is_assert+e.is_pred+e.is_var+e.is_def_indi==1);
    if(e.is_assert)
        os<<*e.assertion;
    else if(e.is_pred)
        os<<*e.pred_val;
    else if(e.is_var)
        os<<*e.variable;
    else // is_def_indi
        os<<*e.def_indi;
    if(print_var_info && !e.var_decl.empty())
        cout<<" ["<<"该Fact中变量:"<<e.var_decl<<"] ";
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
    if(e.rhs){
        os<<"(";
        string sep = "";
        for(const auto &i:e.var_decl){
            os<<sep<<i.first<<":"<<*i.second;
            sep = "; ";
        }
        os<<")    ";
        if(e.lhs) // 改造后的lhs部分可能为空个体
            os<<*e.lhs;
        else
            os<<"\u2205"; // lhs为空输出 ∅
        os<<" => "<<*e.rhs<<"  "<<e.description;
    }
    else
        os<<e.description;
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
    if(e.def_indi_hash_table.size()){ // def_indi_hash_table 额外处理
        cout<<"现有个体定义:"<<endl;
        for(auto p:e.def_indi_hash_table){
            cout<<"\t"<<p.first<<endl;
        }
        cout<<endl;
    }
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
        return rhs.is_func && *func_val == *rhs.func_val;
    else if(is_mathe)
        return rhs.is_mathe && op_val==rhs.op_val && *left==*rhs.left && *right==*rhs.right;
    else if(is_enclosed)
        return rhs.is_enclosed && *enclosed_expr==*rhs.enclosed_expr;
    else // number
        return rhs.is_num && *number_val==*rhs.number_val;
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
    bool alt_val_is_equal = false; // alt_val 部分是否相等
    if(alt_vals.size()==rhs.alt_vals.size()){
        alt_val_is_equal = true;
        for(size_t i=0; i!=alt_vals.size(); ++i){
            if(!(*alt_vals[i]==*rhs.alt_vals[i])){
                alt_val_is_equal = false;
                break;
            }
        }
    }
    if(!alt_val_is_equal)
        return false;

    if(is_var)
        return rhs.is_var && *var_val==*rhs.var_val;
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
    return *left==*rhs.left && *right==*rhs.right && predicate==rhs.predicate;
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
        return rhs.is_and && *and_val == *rhs.and_val;
    else if(is_pred)
        return rhs.is_pred && *pred_val == *rhs.pred_val;
    else if(is_ctor)
        return rhs.is_ctor && *ctor_val == *rhs.ctor_val;
    else if(is_oprt_apply)
        return rhs.is_oprt_apply && *oprt_apply_val == *rhs.oprt_apply_val;
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
        return *left==*rhs.left && *right==*rhs.right;
    else // is_sugar_for_true
        return *lonely_left == *rhs.lonely_left;
}

// Def_Individual 重载 ==
bool Def_Individual::operator==(const Def_Individual &rhs) const{
    return symbol==rhs.symbol && *concept==*rhs.concept && *indi==*rhs.indi;
}

// Fact 重载 ==
bool Fact::operator==(const Fact &rhs) const{
    if(rhs.is_assert)
        return this->is_assert && *assertion == *rhs.assertion;
    else if(rhs.is_pred)
        return this->is_pred && *pred_val == *rhs.pred_val;
    else if(rhs.is_var)
        return this->is_var && *variable == *rhs.variable;
    else // is_def_indi
        return this->is_def_indi && *def_indi == *rhs.def_indi;
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

void Sugar_For_Pred::propagate_var_decl(const map<string, shared_ptr<Concept>> &v){ // 传播变量声明
    left->propagate_var_decl(v, *this);
    right->propagate_var_decl(v, *this);
    
}

void Sugar_For_Pred::propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Term &parent){ // 传播变量声明
    propagate_var_decl(v);
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
    else if(is_enclosed){
        enclosed_expr->propagate_var_decl(v, *this);
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
            fact->var_decl = fact->assertion->var_decl;
        }
        else if(fact->is_pred){
            fact->pred_val->propagate_var_decl(rete_question->var_decl);
            fact->var_decl = fact->pred_val->var_decl;
        }
        else if(fact->is_def_indi){
            ; // Def_Indi 暂时不用处理
        }
        else if(fact->is_var){
            ; // Variable 暂时不用处理
        }
    }
    // 处理 to_solve
    for(auto unknown:rete_question->to_solve)
        unknown->propagate_var_decl(rete_question->var_decl);
}





// 下面是 实例化规则 相关: (整体结构和 传播变量声明 部分类似)

map<string, shared_ptr<Concept>> instantiate_var_decl(const map<string, shared_ptr<Concept>> &var_decl, const map<string, string> &abstract_to_concrete){ // 实例化变量声明
    // #ifndef NDEBUG
    //     cout<<"原来的变量声明为: "<< str_of_var_decl(var_decl)<<endl;
    //     cout<<"约束变元的实例对应为: "<< str_of_abs_to_con(abstract_to_concrete)<<endl;
    // #endif
    map<string, shared_ptr<Concept>> ret;
    bool find;
    for(auto each_var_decl:var_decl){ // 每个变量都要在 abstract_to_concrete 中找到对应
        find = true;
        for(auto abs_to_con:abstract_to_concrete){
            if(each_var_decl.first==abs_to_con.first){
                ret.insert(pair<string, shared_ptr<Concept>>(abs_to_con.second, each_var_decl.second));
                find = true;
                break;
            }
            find = false;
        }
        assert(find);
    }
    // #ifndef NDEBUG
    //     cout<<"实例化后的新变量声明为: "<< str_of_var_decl(ret)<<endl;
    // #endif
    return ret;
}

shared_ptr<Assertion> Assertion::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Assertion
    auto ret = make_shared<Assertion>(*this); // 先构造一个和原来相同的 Assertion
    if(is_std){
        cout<<endl<<"------------------------------------------------------------------------------------"<<endl;
        cout<<"当前 Assertion 的左部: "<<*left<<endl;
        cout<<"原来的 alt_vals 个数 :"<<left->alt_vals.size()<<endl;
        if(left->alt_vals.size()){
            cout<<"\t各个 alt 分别为: "<<endl;
            for(auto alt:left->alt_vals){
                cout<<"\t\t"<<*alt<<endl;
            }
        }
        cout<<endl;

        ret->left = left->instantiate(abstract_to_concrete);
        ret->right = right->instantiate(abstract_to_concrete);

        cout<<"实例化后的 Assertion 的左部: "<<*ret->left<<endl;
        cout<<"实例化后的 alt_vals 个数 :"<<ret->left->alt_vals.size()<<endl;
        if(ret->left->alt_vals.size()){
            cout<<"\t各个 alt 分别为: "<<endl;
            for(auto alt:ret->left->alt_vals){
                cout<<"\t\t"<<*alt<<endl;
            }
        }
        cout<<endl;
        cout<<"------------------------------------------------------------------------------------"<<endl<<endl;

    }
    else{
        assert(is_sugar_for_true);
        ret->lonely_left = lonely_left->instantiate(abstract_to_concrete);
    }
    ret->var_decl = instantiate_var_decl(ret->var_decl, abstract_to_concrete); // 处理 Assertion 自身的变量声明
    
    return ret;
}

shared_ptr<Cud> Cud::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Cud
    auto ret = make_shared<Cud>(*this); // 先构造一个和原来相同的 Cud
    ret->left = left->instantiate(abstract_to_concrete);
    ret->right = right->instantiate(abstract_to_concrete);
    return ret;
}

shared_ptr<Sugar_For_And> Sugar_For_And::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Sugar_For_And
    auto ret = make_shared<Sugar_For_And>(*this); // 先构造一个和原来相同的 Sugar_For_And
    vector<shared_ptr<Individual>> new_content;
    for(auto arg:content){ // 处理每一个参数
        new_content.push_back(arg->instantiate(abstract_to_concrete));
    }
    ret->content = new_content;
    ret->var_decl = instantiate_var_decl(ret->var_decl, abstract_to_concrete); // 处理 Sugar_For_And 自身的变量声明
    return ret;
}

shared_ptr<Sugar_For_Oprt_Apply> Sugar_For_Oprt_Apply::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Sugar_For_Oprt_Apply
    auto ret = make_shared<Sugar_For_Oprt_Apply>(*this); // 先构造一个和原来相同的 Sugar_For_Oprt_Apply
    // 找到对应自由变元的个体实例
    for(auto abs_to_con:abstract_to_concrete){
        if(indi==abs_to_con.first){
            ret->indi = abs_to_con.second;
            break;
        }
    }
    ret->var_decl = instantiate_var_decl(ret->var_decl, abstract_to_concrete); // 处理 Sugar_For_Oprt_Apply 自身的变量声明
    return ret;
}

shared_ptr<Sugar_For_Pred> Sugar_For_Pred::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Sugar_For_Pred
    auto ret = make_shared<Sugar_For_Pred>(*this); // 先构造一个和原来相同的 Sugar_For_Pred
    ret->left = left->instantiate(abstract_to_concrete);
    ret->right = right->instantiate(abstract_to_concrete);
    ret->var_decl = instantiate_var_decl(ret->var_decl, abstract_to_concrete); // 处理 Sugar_For_Pred 自身的变量声明
    return ret;
}

shared_ptr<Assignment> Assignment::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Assignment
    auto ret = make_shared<Assignment>(*this); // 先构造一个和原来相同的 Assignment
    ret->val = val->instantiate(abstract_to_concrete);
    return ret;
}

shared_ptr<Sugar_For_Ctor> Sugar_For_Ctor::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Sugar_For_Ctor
    auto ret = make_shared<Sugar_For_Ctor>(*this); // 先构造一个和原来相同的 Sugar_For_Ctor
    vector<shared_ptr<Assignment>> new_content;
    for(auto arg:content){ // 处理每一个参数
        new_content.push_back(arg->instantiate(abstract_to_concrete));
    }
    ret->content = new_content;
    return ret;
}

shared_ptr<Term> Term::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化Term
    auto ret = make_shared<Term>(*this); // 先构造一个和原来相同的 Term
    if(is_and)
        ret->and_val = and_val->instantiate(abstract_to_concrete);
    else if(is_pred)
        ret->pred_val = pred_val->instantiate(abstract_to_concrete);
    else if(is_ctor)
        ret->ctor_val = ctor_val->instantiate(abstract_to_concrete);
    else if(is_oprt_apply){
        ret->oprt_apply_val = oprt_apply_val->instantiate(abstract_to_concrete);
    }
    else{
        assert(is_std);
        vector<shared_ptr<Individual>> new_args;
        for(auto arg:args){ // 处理每一个参数
            new_args.push_back(arg->instantiate(abstract_to_concrete));
        }
        ret->args = new_args;
    }

    ret->var_decl = instantiate_var_decl(ret->var_decl, abstract_to_concrete); // 处理 Term 自身的变量声明
    return ret;
}

shared_ptr<Math_Equation> Math_Equation::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Math_Equation
    auto ret = make_shared<Math_Equation>(*this); // 先构造一个和原来相同的 Math_Equation
    ret->left = left->instantiate(abstract_to_concrete);
    ret->right = right->instantiate(abstract_to_concrete);
    ret->var_decl = instantiate_var_decl(ret->var_decl, abstract_to_concrete); // 处理 Math_Equation 自身的变量声明
    return ret;
}

shared_ptr<Coordinate> Coordinate::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Coordinate
    auto ret = make_shared<Coordinate>(*this); // 先构造一个和原来相同的 Coordinate
    ret->abscissa = abscissa->instantiate(abstract_to_concrete);
    ret->ordinate = ordinate->instantiate(abstract_to_concrete);
    ret->var_decl = instantiate_var_decl(ret->var_decl, abstract_to_concrete); // 处理 Coordinate 自身的变量声明
    return ret;
}

shared_ptr<Math_Expr> Math_Expr::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Math_Expr
    auto ret = make_shared<Math_Expr>(*this); // 先构造一个和原来相同的 Math_Expr
    if(is_num)
        ; // Number 不需要处理
    else if(is_sy){
        for(auto abs_to_con:abstract_to_concrete){ // 找到对应自由变元的个体实例
            if(sy_val==abs_to_con.first){
                ret->sy_val = abs_to_con.second;
                break;
            }
        }
    }
    else if(is_func)
        ret->func_val = func_val->instantiate(abstract_to_concrete);
    else if(is_enclosed)
        ret->enclosed_expr = enclosed_expr->instantiate(abstract_to_concrete);
    else{
        assert(is_mathe);
        ret->left = left->instantiate(abstract_to_concrete);
        ret->right = right->instantiate(abstract_to_concrete);
    }
    ret->var_decl = instantiate_var_decl(ret->var_decl, abstract_to_concrete); // 处理 Math_Expr 自身的变量声明
    return ret;
}

shared_ptr<Math_Func> Math_Func::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Math_Func
    auto ret = make_shared<Math_Func>(*this); // 先构造一个和原来相同的 Sugar_For_And
    vector<shared_ptr<Math_Expr>> new_args;
    for(auto arg:args){ // 处理每一个参数
        new_args.push_back(arg->instantiate(abstract_to_concrete));
    }
    ret->args = new_args;
    ret->var_decl = instantiate_var_decl(ret->var_decl, abstract_to_concrete); // 处理 Math_Func 自身的变量声明
    return ret;
}

shared_ptr<Math_Individual> Math_Individual::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Math_Individual
    auto ret = make_shared<Math_Individual>(*this); // 先构造一个和原来相同的 Math_Individual
    if(is_equation)
        ret->equation_val = equation_val->instantiate(abstract_to_concrete);
    else if(is_coordinate)
        ret->coordinate_val = coordinate_val->instantiate(abstract_to_concrete);
    else{
        assert(is_math_expr);
        ret->expr_val = expr_val->instantiate(abstract_to_concrete);
    }
    ret->var_decl = instantiate_var_decl(ret->var_decl, abstract_to_concrete); // 处理 Math_Individual 自身的变量声明
    return ret;
}

shared_ptr<Individual> Individual::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化 Individual
    auto ret = make_shared<Individual>(*this); // 先构造一个和原来相同的 Individual
    // 和传播变量声明时只需要处理个体为：项、断言、数学个体 的情况不同，这里还需要处理 Cud
    if(is_assertion){
        ret->assertion = assertion->instantiate(abstract_to_concrete);
    }
    else if(is_term){
        ret->term = term->instantiate(abstract_to_concrete);
    }
    else if(is_math_indi){
        ret->math_indi = math_indi->instantiate(abstract_to_concrete);
    }
    else if(is_cud){
        ret->cud = cud->instantiate(abstract_to_concrete);
    }
    // 更新 Individual 的 alt_vals
    vector<shared_ptr<Individual>> new_alt_vals;
    for(auto alt:alt_vals){ 
        new_alt_vals.push_back(alt->instantiate(abstract_to_concrete));
    }
    ret->alt_vals = new_alt_vals;
    ret->var_decl = instantiate_var_decl(ret->var_decl, abstract_to_concrete); // 处理 Individual 自身的变量声明
    return ret;
}

shared_ptr<Rete_Rule> Rete_Rule::instantiate(const map<string, string> &abstract_to_concrete){ // 实例化规则
    cout<<"原有 Rule: "<<*this<<endl;
    cout<<"abstract_to_concrete :" <<str_of_abs_to_con(abstract_to_concrete) <<endl;
    auto ret = make_shared<Rete_Rule>();
    // 分别处理 lhs、rhs
    if(lhs) // lhs 可能为空
        ret->lhs = lhs->instantiate(abstract_to_concrete);
    // cout<<"ret->lhs: "<<*ret->lhs<<endl;
    ret->rhs = rhs->instantiate(abstract_to_concrete);
    // cout<<"ret->rhs: "<<*ret->rhs<<endl;
    // 处理 rule 自身的变量声明
    ret->var_decl = instantiate_var_decl(var_decl, abstract_to_concrete);
    ret->description = description;
    cout<<"实例化后得到新的 Rule: "<<*ret<<endl<<endl;
    return ret;
}



// 下面是 获取拷贝 相关 (尚不完整)

Individual Individual::get_copy(){
    Individual ret = Individual(*this);
    if(is_term)
        ret.term = make_shared<Term>(term->get_copy());
    else if(is_assertion)
        ret.assertion = make_shared<Assertion>(assertion->get_copy());
    else if(is_math_indi)
        ret.math_indi = make_shared<Math_Individual>(math_indi->get_copy());
    return ret;
}

Assertion Assertion::get_copy(){
    Assertion ret = Assertion(*this);
    if(is_std){
        ret.left = make_shared<Individual>(left->get_copy());
        ret.right = make_shared<Individual>(right->get_copy());
    }
    else{
        ret.lonely_left = make_shared<Individual>(lonely_left->get_copy());
    }
    return ret;
}

Term Term::get_copy(){
    Term ret = Term(*this);
    if(is_std){
        for(size_t i=0; i!=args.size(); ++i){
            ret.args[i] = make_shared<Individual>(args[i]->get_copy());
        }
    }
    else
        assert(false);
    return ret;
}

Sugar_For_Pred Sugar_For_Pred::get_copy(){
    Sugar_For_Pred ret = Sugar_For_Pred(*this);
    left = make_shared<Individual>(left->get_copy());
    right = make_shared<Individual>(right->get_copy());
    return ret;
}

Math_Individual Math_Individual::get_copy(){
    Math_Individual ret = Math_Individual(*this);
    if(is_equation)
        ret.equation_val = make_shared<Math_Equation>(equation_val->get_copy());
    else if(is_coordinate)
        ret.coordinate_val = make_shared<Coordinate>(coordinate_val->get_copy());
    else
        ret.expr_val = make_shared<Math_Expr>(expr_val->get_copy());
    return ret;
}

Math_Equation Math_Equation::get_copy(){
    Math_Equation ret = Math_Equation(*this);
    ret.left = make_shared<Math_Expr>(left->get_copy());
    ret.right = make_shared<Math_Expr>(right->get_copy());
    return ret;
}

Coordinate Coordinate::get_copy(){
    Coordinate ret = Coordinate(*this);
    ret.abscissa = make_shared<Math_Expr>(abscissa->get_copy());
    ret.ordinate = make_shared<Math_Expr>(ordinate->get_copy());
    return ret;
}

Math_Expr Math_Expr::get_copy(){
    #ifndef NDEBUG
        cout<<"获取 copy: "<<*this<<endl;
    #endif
    Math_Expr ret = Math_Expr(*this);
    if(is_num)
        ret.number_val = make_shared<Number>(number_val->get_copy());
    else if(is_sy)
        ;
    else if(is_mathe){
        ret.left = make_shared<Math_Expr>(left->get_copy());
        ret.right = make_shared<Math_Expr>(right->get_copy());
    }
    else
        assert(false);
    return ret;
}

Number Number::get_copy(){
    Number ret = Number(*this);
    return ret;
}

Fact Fact::get_copy(){
    cout<<"Fact 获取 copy: "<<*this<<endl;
    Fact ret = Fact(*this);
    if(is_assert)
        ret.assertion = make_shared<Assertion>(assertion->get_copy());
    else if(is_pred)
        ret.pred_val = make_shared<Sugar_For_Pred>(pred_val->get_copy());
    else if(is_def_indi)
        ;
    else
        assert(false);
    return ret;
}




// 下面是 统一个体 相关


void Rete_Question::normalize_individual(shared_ptr<Assertion> &assertion){ // 统一 assertion 中的 individual（要保存）
    if(assertion->is_std){
        normalize_individual(assertion->left);
        normalize_individual(assertion->right);
    }
    else{
        assert(assertion->is_sugar_for_true);
        normalize_individual(assertion->lonely_left);
    }
}

void Rete_Question::normalize_individual(shared_ptr<Assignment> &assignment){ // 统一 assignment 中的 individual（要保存）
    normalize_individual(assignment->val);
}

void Rete_Question::normalize_individual(shared_ptr<Term> &term){ // 统一 term 中的 individual（要保存）
    if(term->is_and){
        for(auto &arg:term->and_val->content){
            normalize_individual(arg);
        }
    }
    else if(term->is_pred){
        normalize_individual(term->pred_val->left);
        normalize_individual(term->pred_val->right);
    }
    else if(term->is_std){
        for(auto &arg:term->args){
            normalize_individual(arg);
        }
    }
    else if(term->is_ctor){
        for(auto &arg:term->ctor_val->content)
            normalize_individual(arg);
    }
    else{
        assert(term->is_oprt_apply);
        ; // 无需处理
    }
}

void Rete_Question::normalize_individual(shared_ptr<Cud> &cud){ // 统一 cud 中的 individual（要保存）
    normalize_individual(cud->left);
    normalize_individual(cud->right);
}

void Rete_Question::normalize_individual(shared_ptr<Individual> &indi){ // 统一 individual（要保存）
    // #ifndef NDEBUG
    //     cout<<endl<<"当前 Question 中的所有 Individual 如下: ("<<indi_hash_map.size()<<"个)"<<endl;
    //     for(auto p:indi_hash_map){
    //         cout<<"\t"<<p.first<<"  对应 alt_val: ";
    //         for(auto alt:p.second->alt_vals)
    //             cout<<*alt<<"  ";
    //         cout<<"("<<p.second->alt_vals.size()<<"个)";
    //         cout<<endl;
    //     }
    //     cout<<endl<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
    //     cout<<"当前要统一的 Individual: "<<*indi<<endl;
    //     cout<<"统一前的 Individual 个数为: "<<indi_hash_map.size()<<endl;
    //     if(indi_hash_map.size()){
    //         cout<<"分别为: ";
    //         for(auto p:indi_hash_map){
    //             cout<<p.first<<"  ";
    //         }
    //         cout<<endl<<endl;
    //     }
    // #endif
    // 先迭代处理 alt_vals 部分
    for(auto alt:indi->alt_vals){
        normalize_individual(alt);
    }
    auto it = indi_hash_map.find(indi->get_output_str());
    if(it!=indi_hash_map.end()){ // 如果已经存在则进行 normalization
        cout<<"找到同名的 Individual: "<<*it->second<<endl;
        // 后来的 Individual 可能 (由于新的 Assertion) 保存更新的 alt_vals 信息
        auto new_indi = make_shared<Individual>(*it->second); // 先复制一份已经存在的 Individual
        // 要在原来的 alt_vals 的基础上进行追加
        if(indi->alt_vals.size()){ // 新出现的 Individual 可能会携带一个原来没有的 alt_val
            // assert(indi->alt_vals.size()==1);
            auto new_alt_val = *(--indi->alt_vals.end()); // 携带的那个 alt_val
            // 如果已经在原来的 alt_vals 中, 则不必添加
            bool existed = false;
            for(auto alt:new_indi->alt_vals){
                if(alt->get_output_str()==new_alt_val->get_output_str()){
                    existed = true;
                    break;
                }
            }
            if(!existed)
                new_indi->alt_vals.push_back(new_alt_val);
        }
        *it->second = *new_indi; // 更新 indi_hash_map 中的 Individual
        indi = it->second; // Individual 自身要和 indi_hash_map 同步
    }
    else{ // 如果不存在则保存自身
        // Term 和 Assertion 要迭代处理，其它情况则不需要
        if(indi->is_term)
            normalize_individual(indi->term);
        else if(indi->is_assertion)
            normalize_individual(indi->assertion);
        else if(indi->is_cud)
            normalize_individual(indi->cud);
        else // TODO: 考虑是否有必要在更细的粒度上进行 normalization
            ;
        auto p = indi_hash_map.insert(pair<string,shared_ptr<Individual>>(indi->get_output_str(),indi));
    }
    // #ifndef NDEBUG
    //     cout<<endl<<"统一后的 Individual 个数为: "<<indi_hash_map.size()<<endl;
    //     if(indi_hash_map.size()){
    //         cout<<"分别为: ";
    //         for(auto p:indi_hash_map){
    //             cout<<p.first<<"  ";
    //         }
    //         cout<<endl;
    //     }
    //     cout<<endl<<"当前 Question 中的所有 Individual 如下: ("<<indi_hash_map.size()<<"个)"<<endl;
    //     for(auto p:indi_hash_map){
    //         cout<<"\t"<<p.first<<"  对应 alt_val: ";
    //         for(auto alt:p.second->alt_vals)
    //             cout<<*alt<<"  ";
    //         cout<<"("<<p.second->alt_vals.size()<<"个)";
    //         cout<<endl;
    //     }
    //     cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl<<endl;
    // #endif
}

void Rete_Question::normalize_individual(shared_ptr<Fact> &fact){ // 统一 fact 中的 individual（要保存）
    if(fact->is_assert) // 只需处理 Assertion
        normalize_individual(fact->assertion);
}

void Rete_Question::normalize_individual(shared_ptr<Rete_Rule> &rule){ // 统一 Rete_Rule 中的 individual（要保存）
    if(rule->lhs)
        normalize_individual(rule->lhs);
    normalize_individual(rule->rhs);
}



// 下面是各个类自身特殊的成员函数:

string Individual::get_self_type(Rete_Question &question){ // 查询自身类型
    string ret;
    if(is_var)
        ret = "Var";
    else if(is_bool)
        ret = "Bool";
    else if(is_cud)
        ret = "Cud";
    else if(is_term)
        ret = "Term";
    else if(is_assertion)
        ret = "Assertion";
    else{
        assert(is_math_indi);
        if(math_indi->is_equation)
            ret = "Equation";
        else if(math_indi->is_coordinate)
            ret = "Coordinate";
        else{
            assert(math_indi->is_math_expr);
            ret = "Math_Expr";
        }
    }
    return ret;
}

// shared_ptr<Individual> Individual::find_specific_indi(const string &type_name, Rete_Question &question, shared_ptr<Fact> condition){ // 找到个体的某个特定类型的值
//     // 每个 Individual 可能有若干个替代值，如果该 Individual 本身不属于指定类型，那么要从其替代值中找到属于该类型的值
//     if(get_self_type()==type_name) // 自身是否就是指定类型
//         return make_shared<Individual>(*this);
//     for(auto alt_val:alt_vals){ // alt_vals 中是否有指定类型
//         if(alt_val->get_self_type()==type_name){
//             if(condition){ // 把 individual = alt 的 fact 依据传回调用方 (如果需要的话)
//                 auto condition_name = "{"+ get_output_str() + "=" + alt_val->get_output_str() +"}";
//                 shared_ptr<Fact> condition_fact;
//                 for(auto f:question.fact_list){
//                     if(f->get_output_str()==condition_name){
//                         condition_fact = f;
//                         break;
//                     }
//                 }
//                 assert(condition_fact);
//                 *condition = *condition_fact;
//             }
//             return alt_val;
//         }
//     }
//     // 除此之外，考虑个体的相等性传递
//     if(is_term && term->is_std && term->args.size()==1){ // 目前只考虑一元算子替换参数的情况 ({a=b} => {P(a)=p(b)})
//         string oprt = term->oprt;
//         auto old_body = term->args[0]; // 原来的参数即 a
//         for(auto alt:old_body->alt_vals){ // alt 即 b
//             string old_obj_name = this->get_output_str(); // 即 P(a)
//             string new_obj_name = oprt+"("+alt->get_output_str()+")"; // Term 名称对参数进行替换得到新的 Term 名称 (即由 P(a) 替换得到 P(b))
//             auto it = question.indi_hash_map.find(new_obj_name);
//             if(it!=question.indi_hash_map.end()){
//                 auto new_obj = it->second; // 即 P(b)
//                 // 根据 用于替换的参数之间的相等关系 可以得到 suffice to 关系，而这可以抽象为一条新的规则 (即 {a=b} => {P(a)=p(b)})
//                 string lhs_name = "{"+old_body->get_output_str()+"="+alt->get_output_str()+"}"; // 用于替换的参数之间的相等关系，也就是就是引起该 suffice to 关系的 assertion
//                 string new_rule_desc = "等量代换";
//                 // 用这里的 suffice to 关系创建新的规则加入 reasoning_graph
//                 shared_ptr<Rete_Rule> new_rule = make_shared<Rete_Rule>();
//                 new_rule->description = new_rule_desc;
//                 shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
//                 for(auto fact: question.fact_list){ // 在当前已知的 fact 中找到该起点，补充到 edge 上
//                     if(fact->get_output_str().find(lhs_name)!=string::npos){
//                         edge->fact_start = fact;
//                         break;
//                     }
//                 }
//                 assert(edge->fact_start);
//                 // 创建新的 rhs (即 P(a)=p(b)) 作为终点，补充到 edge 上
//                 auto it_l = question.indi_hash_map.find(old_obj_name);
//                 auto it_r = question.indi_hash_map.find(new_obj_name);
//                 assert(it_l!=question.indi_hash_map.end() && it_r!=question.indi_hash_map.end());
//                 auto assertion = Assertion(*it_l->second,*it_r->second);
//                 auto new_rhs = make_shared<Individual>(assertion);
//                 question.normalize_individual(new_rhs); // 保存之前先统一 Individual
//                 // 传播变量声明
//                 new_rhs->propagate_var_decl(var_decl);
//                 auto new_fact = make_shared<Fact>(assertion);
//                 question.fact_list.push_back(new_fact);
//                 edge->end = new_fact;
//                 reasoning_graph->edges.push_back(edge);
//                 if(condition) // 把 {P(a)=p(b)} 这一依据传回给调用方 (如果需要的话)
//                     *condition = *new_fact;

//                 return new_obj->find_specific_indi(type_name, question);
//             }
//         }
//     }

//     cerr<<*this<<" 找不到类型为 "<<type_name<<" 的值!"<<endl;
//     return nullptr;
// }


shared_ptr<Individual> Individual::find_specific_indi(const string &type_name, Rete_Question &question, shared_ptr<vector<shared_ptr<Fact>>> conditions_sp){ // 找到个体的某个特定类型的值
    auto this_question = make_shared<Rete_Question>(question);
    // 每个 Individual 可能有若干个替代值，如果该 Individual 本身不属于指定类型，那么要从其替代值中找到属于该类型的值
    if(get_self_type(question)==type_name) // 自身是否就是指定类型
        return make_shared<Individual>(*this);
    for(auto alt_val:alt_vals){ // alt_vals 中是否有指定类型
        if(alt_val->get_self_type(question)==type_name){
            if(conditions_sp){ // 把 individual = alt 的 fact 依据传回调用方 (如果需要的话)
                auto condition_name = "{"+ get_output_str() + "=" + alt_val->get_output_str() +"}";
                shared_ptr<Fact> condition_fact;
                for(auto f:question.fact_list){
                    if(f->get_output_str()==condition_name){
                        condition_fact = f;
                        break;
                    }
                }
                assert(condition_fact);
                conditions_sp->push_back(condition_fact);
            }
            return alt_val;
        }
        // 考虑定义个体
        if(alt_val->is_math_indi && alt_val->math_indi->is_math_expr && alt_val->math_indi->expr_val->is_sy){
            string sy = alt_val->math_indi->expr_val->sy_val;
            for(auto p:question.kb->def_indi_hash_table){
                auto def_indi = p.second;
                if(def_indi->symbol==alt_val->get_output_str()){
                    return def_indi->indi;
                }
                assert(false);
            }
        }
    }
    // 除此之外，考虑个体的相等性传递
    if(is_term && term->is_std && term->args.size()==1){ // 目前只考虑一元算子替换参数的情况 ({a=b} => {P(a)=P(b)})
        string oprt = term->oprt;
        auto old_body = term->args[0]; // 原来的参数即 a
        for(auto alt:old_body->alt_vals){ // alt 即 b
            string old_obj_name = this->get_output_str(); // 即 P(a)
            string new_obj_name = oprt+"("+alt->get_output_str()+")"; // Term 名称对参数进行替换得到新的 Term 名称 (即由 P(a) 替换得到 P(b))
            auto it = question.indi_hash_map.find(new_obj_name);
            if(it!=question.indi_hash_map.end()){
                auto new_obj = it->second; // 即 P(b)
                // 根据 用于替换的参数之间的相等关系 可以得到 suffice to 关系，而这可以抽象为一条新的规则 (即 {a=b} => {P(a)=P(b)})
                string lhs_name = "{"+old_body->get_output_str()+"="+alt->get_output_str()+"}"; // 用于替换的参数之间的相等关系，也就是就是引起该 suffice to 关系的 assertion
                string new_rule_desc = "等量代换";
                // 用这里的 suffice to 关系创建新的规则加入 reasoning_graph
                shared_ptr<Rete_Rule> new_rule = make_shared<Rete_Rule>();
                new_rule->description = new_rule_desc;
                shared_ptr<Reasoning_Edge> edge = make_shared<Reasoning_Edge>(new_rule);
                for(auto fact: question.fact_list){ // 在当前已知的 fact 中找到该起点，补充到 edge 上
                    if(fact->get_output_str().find(lhs_name)!=string::npos){
                        edge->fact_start = reasoning_graph->share_or_build_fact_node(fact);
                        break;
                    }
                }
                assert(edge->fact_start);
                // 创建新的 rhs (即 P(a)=P(b)) 作为终点，补充到 edge 上
                auto it_l = question.indi_hash_map.find(old_obj_name);
                auto it_r = question.indi_hash_map.find(new_obj_name);
                assert(it_l!=question.indi_hash_map.end() && it_r!=question.indi_hash_map.end());
                auto assertion = Assertion(*it_l->second,*it_r->second);
                auto new_rhs = make_shared<Individual>(assertion);
                question.normalize_individual(new_rhs); // 保存之前先统一 Individual
                // 传播变量声明
                new_rhs->propagate_var_decl(var_decl);
                auto new_fact = make_shared<Fact>(assertion);
                new_fact = reasoning_graph->share_or_build_fact_node(new_fact);
                question.fact_list.push_back(new_fact);
                edge->fact_end = new_fact;
                reasoning_graph->edges.push_back(edge);
                if(conditions_sp) // 把 {P(a)=P(b)} 这一依据传回给调用方 (如果需要的话)
                    conditions_sp->push_back(new_fact);

                return new_obj->find_specific_indi(type_name, question, conditions_sp);
            }
        }
    }

    cerr<<*this<<" 找不到类型为 "<<type_name<<" 的值!"<<endl;
    return nullptr;
}

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
            else if(indi->is_term){
                assert(indi->term->is_pred); // 其实只会是sugar_for_pred
                new_lhs = make_shared<Individual>(*indi->term);
            }
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
    // 把条件中变量部分单独提为一个变量声明部分
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
    rete_question = ret; // 填充自身的成员值
}

void Rete_Question::print_result(){ // 输出求解结果
    cout<<endl<<"答案: ";
    string sep = "";
    for(auto indi:to_solve){
        for(auto alt:indi->alt_vals){
            if(alt->val_is_known){
                cout<<*indi<<" = "<<*alt<<endl;
                break;
            }
        }
    }
    cout<<endl;
};

void Knowledge_Base::init_def_part(){ // 初始化定义概念、个体、算子的部分
    // 加上知识库中的用户定义部分
    for(auto e:def_concepts){
        string cpt_name = e->concept->get_output_str();
        def_cpt_hash_table.insert(pair<string, shared_ptr<Def_Concept>>(cpt_name, e)); // 用概念名称做哈希对象
        if(e->parent_cpt){
            auto it = cpt_inh_map.find(cpt_name);
            if(it!=cpt_inh_map.end())
                it->second.insert(cpt_name);
            else
                cpt_inh_map.insert(pair<string, set<Concept>>(cpt_name, {*e->parent_cpt}));
        }
    }
    for(auto e:def_individuals){
        def_indi_hash_table.insert(pair<string, shared_ptr<Def_Individual>>(e->symbol, e)); // 用个体名称做哈希对象
    }
    for(auto e:def_operators){
        def_oprt_hash_table.insert(pair<string, shared_ptr<Def_Operator>>(e->oprt, e)); // 用算子名称做哈希对象
    }
}

void Knowledge_Base::print_def_part(){ // 输出 定义概念、个体、算子 的部分
    cout<<endl<<"定义了以下 Concept:"<<endl;
    for(auto e:def_cpt_hash_table){
        cout<<"\t"<<e.first<<" : "<<*e.second<<endl;
    }
    cout<<endl<<"定义了以下 Individual:"<<endl;
    for(auto e:def_indi_hash_table){
        cout<<"\t"<<e.first<<" : "<<*e.second<<endl;
    }
    cout<<endl<<"定义了以下 Operator:"<<endl;
    for(auto e:def_oprt_hash_table){
        cout<<"\t"<<e.first<<" : "<<*e.second<<endl;
    }
    cout<<endl<<"概念继承关系如下:"<<endl;
    for(auto e:cpt_inh_map){
        cout<<e.first<<" <- ";
        for(auto p:e.second)
            cout<<p<<" ";
        cout<<endl;
    }
    cout<<endl;
}



void Rete_Question::take_action(shared_ptr<Cud> cud, shared_ptr<Knowledge_Base> kb){
    #ifndef NDEBUG
        cout<<"当前 Question:"<<endl<<*this<<endl;
        cout<<"当前要执行的 Cud: "<<*cud<<endl;
    #endif

    if(cud->action=="assert"){
        // 目前只需处理形如 action Variable = Sugar_For_Ctor 的 Cud
        assert(cud->left->is_var && cud->right->is_term && cud->right->term->is_ctor);
        string new_indi_name = cud->left->var_val->symbol; // 个体的名称
        auto new_indi_cpt = cud->left->var_val->concept; // 个体的概念
        auto new_assignment_val = make_shared<Sugar_For_Ctor>(*cud->right->term->ctor_val); // 赋予个体的值（先构造一个和 Cud 相同的 Sugar_For_Ctor，后续要进行计算更新）
        for(auto &assignment:new_assignment_val->content){
            assert(assignment->val->is_term && assignment->val->term->is_std); // Assignment 的右部一定是标准形式的 Term
            assignment->val = action_eval(assignment->val,*this);
        }
        cout<<*new_assignment_val<<endl;
        // 把新的个体定义加入到 question
        auto new_assignment_indi = Individual(Term(*new_assignment_val));
        auto new_def_indi = make_shared<Def_Individual>(new_indi_name,*new_indi_cpt,new_assignment_indi);
        cout<<"得到新的 个体定义: "<<*new_def_indi<<endl;
        def_indi_hash_table.insert(pair<string,shared_ptr<Def_Individual>>(new_def_indi->get_output_str(),new_def_indi));
        // 创建对应的新 fact 加入到 question
        auto new_fact = make_shared<Fact>(*new_def_indi);
        new_fact->var_decl.insert(pair<string,shared_ptr<Concept>>(new_indi_name,new_indi_cpt));
        fact_list.push_back(new_fact);
        // 把新的变量声明加入到 question
        var_decl.insert(pair<string,shared_ptr<Concept>>(new_indi_name,new_indi_cpt));
    }
    #ifndef NDEBUG
        cout<<"Cud 执行结束后的 Question:"<<endl<<*this<<endl;
    #endif
}

// 对 assertion 进行可能的化简
void try_to_simplify(shared_ptr<Assertion> &assertion, Rete_Question &question){
    if(assertion->is_sugar_for_true)
        try_to_simplify(assertion->lonely_left, question);
    else{
        // 化简完毕要把 left 原来的 alt 换成新的 right
        auto old_alt_name = assertion->right->get_output_str();
        try_to_simplify(assertion->left, question);
        try_to_simplify(assertion->right, question);
        for(auto &alt:assertion->left->alt_vals){
            if(alt->get_output_str()==old_alt_name){
                *alt = *assertion->right; // 修改的是 alt 指向的值，而不是指针本身
                break;
            }
        }
    }
}
void try_to_simplify(shared_ptr<Individual> &indi, Rete_Question &question){
    if(indi->is_term){ // 目前只处理 Term
        auto &term = indi->term;
        // if(term->is_oprt_apply || (term->is_std && std::find(built_in_oprts.begin(),built_in_oprts.end(),term->oprt)!=built_in_oprts.end())){
        if(term->is_oprt_apply || term->is_std){
            auto ret = action_eval(indi, question);
            if(ret)
                *indi = *ret;
        }
    }
}

bool Rete_Question::take_action(shared_ptr<Rete_Rule> rule, shared_ptr<Knowledge_Base> kb){ // 执行动作
    cout<<"当前要执行的 Rule: "<<*rule<<endl; // TODO:写 subst_point_to_curve 和 subst 算子
    bool worked = take_action(rule->rhs, kb);
    if(worked){
        rule->worked = true;
        cout<<*rule<<"发挥了作用!"<<endl;
    }
    else
        cout<<*rule<<"未发挥作用!"<<endl;
    return worked;
}

bool Rete_Question::take_action(shared_ptr<Individual> rhs, shared_ptr<Knowledge_Base> kb){ // 执行动作
    #ifndef NDEBUG
        cout<<"当前 Question:"<<endl<<*this<<endl;
    #endif
    cout<<"当前要执行的 RHS: "<<*rhs<<endl;
    bool worked = false; // 该 RHS 是否发挥了作用
    // RHS 要考虑的情况有: Cud、Term、Assertion
    if(rhs->is_cud){
        take_action(rhs->cud, kb);
        worked = true;
    }
    else if(rhs->is_term){
        // RHS 为 Term 时要考虑的情况有: sugar_for_and、sugar_for_pred
        auto t = rhs->term;
        if(t->is_and){
            for(auto indi:t->and_val->content){
                if(take_action(indi, kb))
                    worked = true;
            }
        }
        else{
            assert(t->is_pred);
            auto new_fact = make_shared<Fact>(*t->pred_val);
            normalize_individual(new_fact); // 保存之前先统一 Individual
            // 传播变量声明
            new_fact->pred_val->propagate_var_decl(var_decl);
            new_fact->var_decl = new_fact->pred_val->var_decl;
            fact_list.push_back(new_fact);
            worked = true;
        }
    }
    else{
        assert(rhs->is_assertion);
        try_to_simplify(rhs->assertion,*this); // 对 assertion 进行可能的化简
        auto new_fact = make_shared<Fact>(*rhs->assertion);
        normalize_individual(new_fact); // 保存之前先统一 Individual
        // 传播变量声明
        new_fact->assertion->propagate_var_decl(var_decl);
        new_fact->var_decl = new_fact->assertion->var_decl;
        fact_list.push_back(new_fact);
        if(new_fact->assertion->is_std && !new_fact->assertion->right->val_is_known){
            worked = is_potentially_solvable_eq(new_fact); // 右部如果不可知，则该 rhs 未发挥作用（唯一的例外是生成的fact是潜在可解的方程）
        }
    }
    #ifndef NDEBUG
        cout<<"RHS 执行结束后的 Question:"<<endl<<*this<<endl;
    #endif
    return worked;
}