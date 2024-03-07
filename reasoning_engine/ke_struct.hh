#ifndef AL_STRUCT_HH
#define AL_STRUCT_HH
// #define NDEBUG

// AL抽象语法树相关的数据结构

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <cassert>
using std::make_shared;
using std::shared_ptr;
using std::vector;
using std::pair;
using std::map;
using std::set;
using std::string;
using std::ostream;
using std::cout;
using std::cerr;
using std::endl;


// 为保持数据一致性，原则上，成员中包含更小类对象的一律用（智能）指针，这会导致初始化伴随着大量的指针操作

class Number{
public:
    // 用 int 初始化
    Number(int i):is_int(true),i_val(i){}
    // 用 float 初始化
    Number(double d):is_float(true),f_val(d){}
    Number(){} // 默认构造
    
    Number get_copy();
    string get_output_str() const; // 获取输出字符串
    bool operator==(const Number &rhs) const;
    bool operator>(const Number &rhs) const;
    bool operator<(const Number &rhs) const;
    bool operator>=(const Number &rhs) const;
    bool operator<=(const Number &rhs) const;
    bool operator!=(const Number &rhs) const;
    Number operator+(const Number &rhs) const;
    Number operator-(const Number &rhs) const;
    Number operator*(const Number &rhs) const;
    Number operator/(const Number &rhs) const;
    Number operator^(const Number &rhs) const;

    void trans_to_opposite(){ // 变为相反数
        if(is_int)
            i_val *= -1;
        else
            f_val *= -1;
    }

    bool is_int = false; // 是否是整数
    bool is_float = false; // 是否是浮点数

    int i_val; // 整数
    double f_val; // 浮点数
};

inline bool Number::operator>(const Number &rhs) const{
    double l_val, r_val;
    l_val = is_int ? i_val : f_val;
    r_val = rhs.is_int ? rhs.i_val : rhs.f_val;
    return l_val > r_val;
}

inline bool Number::operator<(const Number &rhs) const{
    double l_val, r_val;
    l_val = is_int ? i_val : f_val;
    r_val = rhs.is_int ? rhs.i_val : rhs.f_val;
    return l_val < r_val;
}

inline bool Number::operator>=(const Number &rhs) const{
    double l_val, r_val;
    l_val = is_int ? i_val : f_val;
    r_val = rhs.is_int ? rhs.i_val : rhs.f_val;
    return l_val >= r_val;
}

inline bool Number::operator<=(const Number &rhs) const{
    double l_val, r_val;
    l_val = is_int ? i_val : f_val;
    r_val = rhs.is_int ? rhs.i_val : rhs.f_val;
    return l_val <= r_val;
}

inline bool Number::operator!=(const Number &rhs) const{
    double l_val, r_val;
    l_val = is_int ? i_val : f_val;
    r_val = rhs.is_int ? rhs.i_val : rhs.f_val;
    return l_val != r_val;
}

inline Number Number::operator+(const Number &rhs) const{
    double l_val, r_val;
    l_val = is_int ? i_val : f_val;
    r_val = rhs.is_int ? rhs.i_val : rhs.f_val;
    double ret = l_val + r_val;
    double int_part;
    double frac_part = std::modf(ret, &int_part); // 将 ret 拆分为整数和小数部分
    if(frac_part == 0.0) // 如果是整数
        return Number((int)ret);
    else
        return Number(ret);
}

inline Number Number::operator-(const Number &rhs) const{
    double l_val, r_val;
    l_val = is_int ? i_val : f_val;
    r_val = rhs.is_int ? rhs.i_val : rhs.f_val;
    double ret = l_val - r_val;
    double int_part;
    double frac_part = std::modf(ret, &int_part); // 将 ret 拆分为整数和小数部分
    if(frac_part == 0.0) // 如果是整数
        return Number((int)ret);
    else
        return Number(ret);
}

inline Number Number::operator*(const Number &rhs) const{
    double l_val, r_val;
    l_val = is_int ? i_val : f_val;
    r_val = rhs.is_int ? rhs.i_val : rhs.f_val;
    double ret = l_val * r_val;
    double int_part;
    double frac_part = std::modf(ret, &int_part); // 将 ret 拆分为整数和小数部分
    if(frac_part == 0.0) // 如果是整数
        return Number((int)ret);
    else
        return Number(ret);
}

inline Number Number::operator/(const Number &rhs) const{
    double l_val, r_val;
    l_val = is_int ? i_val : f_val;
    r_val = rhs.is_int ? rhs.i_val : rhs.f_val;
    double ret = l_val / r_val;
    double int_part;
    double frac_part = std::modf(ret, &int_part); // 将 ret 拆分为整数和小数部分
    if(frac_part == 0.0) // 如果是整数
        return Number((int)ret);
    else
        return Number(ret);
}

inline Number Number::operator^(const Number &rhs) const{
    double l_val, r_val;
    l_val = is_int ? i_val : f_val;
    r_val = rhs.is_int ? rhs.i_val : rhs.f_val;
    double ret = pow(l_val, r_val);
    double int_part;
    double frac_part = std::modf(ret, &int_part); // 将 ret 拆分为整数和小数部分
    if(frac_part == 0.0) // 如果是整数
        return Number((int)ret);
    else
        return Number(ret);
}


class Math_Expr;class Concept;class Math_Individual;
class Math_Func{ // 数学函数
public:
    // 用函数名和 vector<shared_ptr<Math_Expr>>格式的参数列表 初始化
    Math_Func(const string &name,const vector<shared_ptr<Math_Expr>> &expr_list):func_name(name),args(expr_list){}
    Math_Func(){} // 默认构造
    
    string get_output_str() const; // 获取输出字符串
    bool operator==(const Math_Func &rhs) const; // 重载 ==

    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v); // 传播变量声明
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Individual &parent); // 传播变量声明 (Math_Func 的上层可能是 Math_Individual)
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Expr &parent); // 传播变量声明 (Math_Func 的上层可能是 Math_Expr)
    shared_ptr<Math_Func> instantiate(const map<string, string> &abstract_to_concrete); // 实例化
    Math_Func get_copy();

    string func_name; //函数名
    vector<shared_ptr<Math_Expr>> args; // 参数列表

    map<string,shared_ptr<Concept>> var_decl; // 变量声明
};

class Math_Equation;class Coordinate;class Math_Func;
class Math_Expr{ // 数学表达式
public:
    // 用 Number 初始化
    Math_Expr(const Number &n):is_num(true),number_val(make_shared<Number>(n)){}
    // 用 Symbol 初始化
    Math_Expr(const string &var):is_sy(true),sy_val(var){}
    // 用 Math_Func 初始化
    Math_Expr(const Math_Func &func):is_func(true),func_val(make_shared<Math_Func>(func)){}
    // 用 Math_Expr OP Math_Expr 初始化
    Math_Expr(const Math_Expr &l, char op, const Math_Expr &r):is_mathe(true),op_val(op),left(make_shared<Math_Expr>(l)),right(make_shared<Math_Expr>(r)){}
    // 用单个 Math_Expr 初始化 // 为了和 拷贝构造 区分，这里的参数使用指针
    Math_Expr(shared_ptr<Math_Expr> e):is_enclosed(true),enclosed_expr(e){}

    Math_Expr(){} // 默认构造
    string get_output_str() const; // 获取输出字符串
    bool operator==(const Math_Expr &rhs) const; // 重载 ==

    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v); // 传播变量声明
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Individual &parent); // 传播变量声明 (Math_Expr 的上层可能是 Math_Individual)
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Equation &parent); // 传播变量声明 (Math_Expr 的上层可能是 Math_Equation)
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Coordinate &parent); // 传播变量声明 (Math_Expr 的上层可能是 Coordinate)
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Func &parent); // 传播变量声明 (Math_Expr 的上层可能是 Math_Func)
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Expr &parent); // 传播变量声明 (Math_Expr 的上层可能是 Math_Expr)
    shared_ptr<Math_Expr> instantiate(const map<string, string> &abstract_to_concrete); // 实例化
    shared_ptr<Number> get_num_val(); // 获取对应的 Number 值
    Math_Expr get_copy();

    bool is_num = false; // 是否是数
    bool is_sy = false; // 是否是符号
    bool is_func = false; // 是否是数学函数
    bool is_mathe = false; // 是否是 +-*/^
    bool is_enclosed = false; // 是否是括号包起来的

    shared_ptr<Number> number_val = nullptr; // Number
    string sy_val; // Symbol
    shared_ptr<Math_Func> func_val = nullptr; // 函数
    char op_val = '!'; // +-*/^
    shared_ptr<Math_Expr> left = nullptr;
    shared_ptr<Math_Expr> right = nullptr;
    shared_ptr<Math_Expr> enclosed_expr = nullptr; // 被括号包起来的 Math_Expr

    map<string,shared_ptr<Concept>> var_decl; // 变量声明
};

inline shared_ptr<Number> Math_Expr::get_num_val(){ // 获取对应的 Number 值
    // 该函数一般用于 / 式获取临时的 Number 值
    shared_ptr<Number> ret;
    assert(is_mathe && op_val=='/' && left->is_num && right->is_num);
    return make_shared<Number>(*left->number_val / *right->number_val);
}

class Math_Individual;
class Math_Equation{ // 数学方程
public:
    // 用左右两个 Math_Expr 初始化
    Math_Equation(const Math_Expr &l, const Math_Expr &r):left(make_shared<Math_Expr>(l)),right(make_shared<Math_Expr>(r)){}
    Math_Equation(){} // 默认构造
     
    bool operator==(const Math_Equation &rhs) const{return left==rhs.left && right==rhs.right;} // 重载 ==
    string get_output_str() const; // 获取输出字符串

    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Individual &parent); // 传播变量声明
    shared_ptr<Math_Equation> instantiate(const map<string, string> &abstract_to_concrete); // 实例化
    Math_Equation get_copy();

    shared_ptr<Math_Expr> left; // 方程左部
    shared_ptr<Math_Expr> right; // 方程右部

    map<string,shared_ptr<Concept>> var_decl; // 变量声明
};

class Coordinate{ // 二维坐标
public:
    // 用横纵坐标的 Math_Expr 初始化
    Coordinate(const Math_Expr &x, const Math_Expr &y):abscissa(make_shared<Math_Expr>(x)),ordinate(make_shared<Math_Expr>(y)){}
    Coordinate(){} // 默认构造
    
    bool operator==(const Coordinate &rhs) const{return abscissa==rhs.abscissa && ordinate==rhs.ordinate;} // 重载 ==
    string get_output_str() const; // 获取输出字符串

    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Math_Individual &parent); // 传播变量声明
    shared_ptr<Coordinate> instantiate(const map<string, string> &abstract_to_concrete); // 实例化
    Coordinate get_copy();

    shared_ptr<Math_Expr> abscissa; // 横坐标
    shared_ptr<Math_Expr> ordinate; // 纵坐标

    map<string,shared_ptr<Concept>> var_decl; // 变量声明
};

class Individual;
class Math_Individual{ // 数学个体
public:
    // 用单个 Math_Expr 初始化
    Math_Individual(const Math_Expr &e):is_math_expr(true),expr_val(make_shared<Math_Expr>(e)){}
    // 用方程初始化
    Math_Individual(const Math_Equation &e):is_equation(true),equation_val(make_shared<Math_Equation>(e)){}
    // 用坐标初始化
    Math_Individual(const Coordinate &e):is_coordinate(true),coordinate_val(make_shared<Coordinate>(e)){}
    
    Math_Individual(){} // 默认构造
    string get_output_str() const; // 获取输出字符串
    bool operator==(const Math_Individual &rhs) const; // 重载 ==

    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Individual &parent); // 传播变量声明
    shared_ptr<Math_Individual> instantiate(const map<string, string> &abstract_to_concrete); // 实例化
    Math_Individual get_copy();

    bool is_equation = false; // 是否是方程
    bool is_coordinate = false; // 是否是坐标
    bool is_math_expr = false; // 是否是数学表达式

    shared_ptr<Math_Equation> equation_val; // 方程
    shared_ptr<Coordinate> coordinate_val; // 坐标
    shared_ptr<Math_Expr> expr_val; // 数学表达式

    map<string,shared_ptr<Concept>> var_decl; // 变量声明
};

class Concept{
public:
    // 用原子概念名初始化
    Concept(const string &n):is_atomic(true),atomic_concept(n){}
    // 用外层算子和内层Concept初始化
    Concept(const string &n, const Concept &c):is_compound(true),outer_oprt(n),inner_concept(make_shared<Concept>(c)){}
    
    Concept(){} // 默认构造
    string get_output_str() const; // 获取输出字符串
    // 重载 <
    bool operator<(const Concept &rhs) const{return get_output_str()<rhs.get_output_str();}
    // 重载 ==
    bool operator==(const Concept &rhs) const;
    // bool operator==(const Concept &rhs) const{return get_output_str()==rhs.get_output_str();}
    
    bool is_atomic = false; // 是否是原子概念
    bool is_compound = false; // 是否是复合概念

    string atomic_concept; // 原子概念
    string outer_oprt; // 复合概念的外层算子
    shared_ptr<Concept> inner_concept; // 复合概念的内层概念
};

class Variable{ // 变量
public:
    // 用符号和对应概念初始化
    Variable(const string &sy,const Concept &cpt):symbol(sy),concept(make_shared<Concept>(cpt)){}
    Variable(){} // 默认构造
     
    string get_output_str() const; // 获取输出字符串
    bool operator==(const Variable &rhs) const{return symbol==rhs.symbol && *concept==*rhs.concept;} // 重载 ==
    
    string symbol; // 符号
    shared_ptr<Concept> concept; // 对应概念
};

class Cud; class Term; class Assertion; class Sugar_For_And; class Sugar_For_Pred;class Rete_Question;class Fact;
class Individual{
public:
    // 用变量初始化
    Individual(const Variable &e):is_var(true),var_val(make_shared<Variable>(e)){}
    // 用布尔值初始化
    Individual(bool e):is_bool(true),bool_val(e){}
    // 用cud初始化
    Individual(const Cud &e):is_cud(true),cud(make_shared<Cud>(e)){}
    // 用term初始化
    Individual(const Term &e):is_term(true),term(make_shared<Term>(e)){}
    // 用assertion初始化
    Individual(const Assertion &e):is_assertion(true),assertion(make_shared<Assertion>(e)){}
    // 用math_individual初始化
    Individual(const Math_Individual &e):is_math_indi(true),math_indi(make_shared<Math_Individual>(e)){
        if(e.is_math_expr && e.expr_val->is_num)
            val_is_known = true;
        else if(e.is_coordinate && e.coordinate_val->abscissa->is_num && e.coordinate_val->ordinate->is_num)
            val_is_known = true;
    }

    Individual(){} // 默认构造
    bool operator==(const Individual &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v); // 传播变量声明
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Term &parent); // 传播变量声明 (Individual 的上层可能是 Term)
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Sugar_For_And &parent); // 传播变量声明 (Individual 的上层可能是 Sugar_For_And)
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Sugar_For_Pred &parent); // 传播变量声明 (Individual 的上层可能是 Sugar_For_Pred)
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Assertion &parent); // 传播变量声明 (Individual 的上层可能是 Assertion)
    shared_ptr<Individual> find_specific_indi(const string &type_name, Rete_Question &question, shared_ptr<vector<shared_ptr<Fact>>> conditions_sp = nullptr); // 找到个体的某个特定类型的值
    shared_ptr<Individual> instantiate(const map<string, string> &abstract_to_concrete); // 实例化
    Individual get_copy();

    bool is_var = false; // 是否是变量
    bool is_bool = false; // 是否是布尔值
    bool is_cud = false; // 是否是cud
    bool is_term = false; // 是否是项
    bool is_assertion = false; // 是否是断言
    bool is_math_indi = false; // 是否是数学个体

    shared_ptr<Variable> var_val; // 变量
    bool bool_val; // 布尔值
    shared_ptr<Cud> cud; // cud
    shared_ptr<Term> term; // 项
    shared_ptr<Assertion> assertion; // 断言
    shared_ptr<Math_Individual> math_indi; // 数学个体

    map<string,shared_ptr<Concept>> var_decl; // 变量声明

    bool val_is_known = false; // 值是否已知
    bool alt_val_is_known = false; // 备用值是否已知
    vector<shared_ptr<Individual>> alt_vals; // 备用值

private:
    string get_self_type(Rete_Question &question); // 查询自身类型
};

class Assignment{ // 赋值式 symbol := individual
public:
    // 用符号和个体初始化
    Assignment(const string &s,const Individual &i):symbol(s),val(make_shared<Individual>(i)){}
    Assignment(){} // 默认构造
    
    bool operator==(const Assignment &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    shared_ptr<Assignment> instantiate(const map<string, string> &abstract_to_concrete); // 实例化

    string symbol; // 符号
    shared_ptr<Individual> val; // 对应的个体
};

class Cud{ // 增、改、删 fact
public:
    // 用 cud标识符、左右两个个体 初始化
    Cud(const string &cud_idf,const Individual &l,const Individual &r):action(cud_idf),left(make_shared<Individual>(l)),right(make_shared<Individual>(r)){}
    Cud(){} // 默认构造
    
    bool operator==(const Cud &rhs) const{return action==rhs.action && left==rhs.left && right==rhs.right;}; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    shared_ptr<Cud> instantiate(const map<string, string> &abstract_to_concrete); // 实例化

    string action; // "assert" "modify" "retract"
    shared_ptr<Individual> left;
    shared_ptr<Individual> right;
    // 虽然目前使用到的 Cud 的形式只有: action Variable = Sugar_For_Ctor, 但为了未来可能的更多需求, 在语法上使用了: action Individual = Individual
};

class Sugar_For_And{ // 逻辑与的语法糖
public:
    // 用 vector<shared_ptr<Individual>>格式的个体列表 初始化
    Sugar_For_And(const vector<shared_ptr<Individual>> &ls):content(ls){}
    Sugar_For_And(){} // 默认构造
    
    bool operator==(const Sugar_For_And &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Term &parent); // 传播变量声明
    shared_ptr<Sugar_For_And> instantiate(const map<string, string> &abstract_to_concrete); // 实例化

    vector<shared_ptr<Individual>> content; // 与式 (注意这其中的分隔符应该是;)
    map<string,shared_ptr<Concept>> var_decl; // 变量声明
};

class Sugar_For_Pred{ // 二元谓词的语法糖
public:
    // 用 两个个体和一个谓词 初始化
    Sugar_For_Pred(const Individual &i_1,const string &pred,const Individual &i_2):left(make_shared<Individual>(i_1)),right(make_shared<Individual>(i_2)),predicate(pred){}
    Sugar_For_Pred(){} // 默认构造
    
    bool operator==(const Sugar_For_Pred &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串
    Sugar_For_Pred get_copy();

    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v); // 传播变量声明
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Term &parent); // 传播变量声明
    shared_ptr<Sugar_For_Pred> instantiate(const map<string, string> &abstract_to_concrete); // 实例化

    shared_ptr<Individual> left; // 第一个参数
    shared_ptr<Individual> right; // 第二个参数
    string predicate; // 谓词 (">"、"<"、">="、"<="、"!=" 中的一个)

    map<string,shared_ptr<Concept>> var_decl; // 变量声明
};

class Sugar_For_Ctor { // 构造器的语法糖: 构造器是用赋值式列表临时创建的个体
public:
    // 用 vector<shared_ptr<Assignment>>格式的赋值式列表 初始化
    Sugar_For_Ctor(const vector<shared_ptr<Assignment>> &ls):content(ls){}
    Sugar_For_Ctor(){} // 默认构造
    
    bool operator==(const Sugar_For_Ctor &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    shared_ptr<Sugar_For_Ctor> instantiate(const map<string, string> &abstract_to_concrete); // 实例化

    vector<shared_ptr<Assignment>> content;
};

class Sugar_For_Oprt_Apply{ // 一元算子应用的语法糖: a.b 表示 Get_b(a)
public:
    // 用 表示个体的符号和表示一元算子的符号 初始化
    Sugar_For_Oprt_Apply(const string &i="",const string &op=""):indi(i),uni_oprt(op){}
     
    bool operator==(const Sugar_For_Oprt_Apply &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Term &parent); // 传播变量声明
    shared_ptr<Sugar_For_Oprt_Apply> instantiate(const map<string, string> &abstract_to_concrete); // 实例化

    string indi; // 表示个体的符号
    string uni_oprt; // 表示一元算子的符号
    map<string,shared_ptr<Concept>> var_decl; // 变量声明
};

class Term{ // 项
public:
    // 用 Sugar_For_And 初始化
    Term(const Sugar_For_And &e):is_and(true),and_val(make_shared<Sugar_For_And>(e)){}
    // 用 Sugar_For_Pred 初始化
    Term(const Sugar_For_Pred &e):is_pred(true),pred_val(make_shared<Sugar_For_Pred>(e)){}
    // 用 Sugar_For_Ctor 初始化
    Term(const Sugar_For_Ctor &e):is_ctor(true),ctor_val(make_shared<Sugar_For_Ctor>(e)){}
    // 用 Sugar_For_Oprt_Apply 初始化
    Term(const Sugar_For_Oprt_Apply &e):is_oprt_apply(true),oprt_apply_val(make_shared<Sugar_For_Oprt_Apply>(e)){}
    // 用 算子和个体列表 初始化
    Term(const string &op, vector<shared_ptr<Individual>> ls):is_std(true),oprt(op),args(ls){}

    Term(){}// 默认构造
     
    bool operator==(const Term &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Individual &parent); // 传播变量声明
    shared_ptr<Term> instantiate(const map<string, string> &abstract_to_concrete); // 实例化
    Term get_copy();

    bool is_and = false; // 是否是sugar_for_and
    bool is_pred = false; // 是否是sugar_for_pred
    bool is_ctor= false; // 是否是sugar_for_ctor
    bool is_oprt_apply = false; // 是否是sugar_for_oprt_apply
    bool is_std = false; // 是否是标准形式

    shared_ptr<Sugar_For_And> and_val; // sugar_for_and 形式的项
    shared_ptr<Sugar_For_Pred> pred_val; // sugar_for_pred 形式的项
    shared_ptr<Sugar_For_Ctor> ctor_val; // sugar_for_ctor 形式的项
    shared_ptr<Sugar_For_Oprt_Apply> oprt_apply_val; // sugar_for_oprt_apply 形式的项
    string oprt; // 项的算子
    vector<shared_ptr<Individual>> args; // 算子的参数

    map<string,shared_ptr<Concept>> var_decl; // 变量声明
};

class Assertion{
public:
    // 用 左右两个个体 初始化
    Assertion(const Individual &l,const Individual &r):is_std(true),left(make_shared<Individual>(l)),right(make_shared<Individual>(r)){
        // 保存 Assertion 蕴含的 equality 信息
        left->alt_vals.push_back(right);
        // right->alt_vals.push_back(left);
    }
    // 用 单个个体 初始化
    Assertion(const Individual &i):is_sugar_for_true(true),lonely_left(make_shared<Individual>(i)){}
    Assertion(){} // 默认构造
    
    bool operator==(const Assertion &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v); // 传播变量声明
    void propagate_var_decl(const map<string, shared_ptr<Concept>> &v, Individual &parent); // 传播变量声明
    shared_ptr<Assertion> instantiate(const map<string, string> &abstract_to_concrete); // 实例化
    Assertion get_copy();

    bool is_std = false; // 是否是标准形式
    bool is_sugar_for_true = false; // 是否是 Individual=true 的语法糖

    shared_ptr<Individual> left; // 断言的左部
    shared_ptr<Individual> right; // 断言的右部
    shared_ptr<Individual> lonely_left; // Individual=true 中的左部个体

    map<string,shared_ptr<Concept>> var_decl; // 变量声明
};

class Def_Individual{ // 定义个体
public:
    // 用 符号、概念、个体 自定义
    Def_Individual(const string &s,const Concept &c,const Individual &i):symbol(s),concept(make_shared<Concept>(c)),indi(make_shared<Individual>(i)){}
    Def_Individual(){} // 默认构造
    
    bool operator==(const Def_Individual &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    string symbol; // 个体的代表符号
    shared_ptr<Concept> concept; // 所属概念
    shared_ptr<Individual> indi; // 新定义的个体
};

class Rete_Question;
class Fact{ // 事实
public:
    // 用断言定义
    Fact(const Assertion &e):is_assert(true),assertion(make_shared<Assertion>(e)){}
    // 用 Sugar_For_Pred 定义
    Fact(const Sugar_For_Pred &e):is_pred(true),pred_val(make_shared<Sugar_For_Pred>(e)){}
    // 用变量定义
    Fact(const Variable &e):is_var(true),variable(make_shared<Variable>(e)){}
    // 用定义个体定义
    Fact(const Def_Individual &e):is_def_indi(true),def_indi(make_shared<Def_Individual>(e)){}

    Fact(){} // 默认构造

    bool operator==(const Fact &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串
    Fact get_copy();

    bool is_assert = false; // 是否是断言
    bool is_pred = false; // 是否是 Sugar_For_Pred
    bool is_var = false; // 是否是变量
    bool is_def_indi = false; // 是否是定义个体

    shared_ptr<Assertion> assertion; // 断言
    shared_ptr<Sugar_For_Pred> pred_val; // Sugar_For_Pred
    shared_ptr<Variable> variable; // 变量
    shared_ptr<Def_Individual> def_indi; // 定义个体

    map<string,shared_ptr<Concept>> var_decl; // 变量声明
    map<string, string> abstract_to_concrete; // 约束变元的实例对应
    bool has_been_added = false; // 是否已经加入过 Rete 网络
    shared_ptr<Rete_Question> where_is = nullptr; // 所在的 Rete_Question
};


class Question{ // 问题
public:
    // 用 描述信息、事实列表、待求解项 初始化
    Question(const string &s,const vector<shared_ptr<Fact>> &facts,const vector<shared_ptr<Individual>> &unkown):description(s),fact_list(facts),to_solve(unkown){}
    Question(){} // 默认构造
    
    string get_output_str() const; // 获取输出字符串

    void get_adapted_question(); // 获取适配 Rete 算法版本的问题
    void propagate_var_decl(); // 传播变量声明到改造后的问题

    string description; // 描述信息
    vector<shared_ptr<Fact>> fact_list; // 事实列表
    vector<shared_ptr<Individual>> to_solve; // 待求解项

    shared_ptr<Rete_Question> rete_question; // 用于推理系统的问题
};

class Knowledge_Base;
class Rete_Rule;
class Rete_Question:public Question{ // 为适应推理系统对 Question 进行改造后的版本
public:
    // 用 Question和变量声明 初始化
    Rete_Question(const Question &question,const map<string,shared_ptr<Concept>> &var_info):Question(question),var_decl(var_info){}
    Rete_Question(){} // 默认构造
    
    string get_output_str() const; // 获取输出字符串
    bool take_action(shared_ptr<Individual> rhs, shared_ptr<Knowledge_Base> kb); // 执行动作
    bool take_action(shared_ptr<Rete_Rule> rule, shared_ptr<Knowledge_Base> kb); // 执行规则的 RHS
    void print_result(); // 输出求解结果

    void normalize_individual(shared_ptr<Assertion> &assertion); // 统一 assertion 中的 individual（要保存）
    void normalize_individual(shared_ptr<Term> &term); // 统一 term 中的 individual（要保存）
    void normalize_individual(shared_ptr<Cud> &cud); // 统一 cud 中的 individual（要保存）
    void normalize_individual(shared_ptr<Assignment> &assignment); // 统一 assignment 中的 individual（要保存）
    void normalize_individual(shared_ptr<Individual> &indi); // 统一 individual（要保存）
    void normalize_individual(shared_ptr<Fact> &fact); // 统一 fact 中的 individual（要保存）
    void normalize_individual(shared_ptr<Rete_Rule> &rule); // 统一 Rete_Rule 中的 individual（要保存）

    map<string,shared_ptr<Individual>> indi_hash_map; // 存放所有的 Individual
    map<string,shared_ptr<Def_Individual>> def_indi_hash_table; // 存放所有的 Def_Individual
    map<string,shared_ptr<Concept>> var_decl; // 变量声明

    shared_ptr<Knowledge_Base> kb; // 当前的 KB

private:
    void take_action(shared_ptr<Cud> cud, shared_ptr<Knowledge_Base> kb); // 执行 Cud
};


class Def_Concept{ // 定义概念
public:
    // 用 概念和变量列表 自定义
    Def_Concept(const Concept &c,const vector<shared_ptr<Variable>> &ls):without_parent(true),concept(make_shared<Concept>(c)),members(ls){}
    // 用 概念、父概念、变量列表 自定义
    Def_Concept(const Concept &c,const Concept &p,const vector<shared_ptr<Variable>> &ls):with_parent(true),concept(make_shared<Concept>(c)),parent_cpt(make_shared<Concept>(p)),members(ls){}
    // 用 概念、父概念 自定义
    Def_Concept(const Concept &c,const Concept &p):with_parent(true),concept(make_shared<Concept>(c)),parent_cpt(make_shared<Concept>(p)){}
    
    Def_Concept(){} // 默认构造
    
    string get_output_str() const; // 获取输出字符串

    bool with_parent = false; // 有继承
    bool without_parent = false; // 无继承

    shared_ptr<Concept> concept; // 新定义的概念
    shared_ptr<Concept> parent_cpt; // 父概念
    vector<shared_ptr<Variable>> members; // 该概念的成员
};

class Def_Operator{ // 定义算子
public:
    // 用 算子、输入的变量列表、输出的个体 初始化
    Def_Operator(const string &s,const vector<shared_ptr<Variable>> &in,const Individual &out):oprt(s),input(in),output(make_shared<Individual>(out)){}
    Def_Operator(){} // 默认构造
    
    string get_output_str() const; // 获取输出字符串
    shared_ptr<Def_Operator> instantiate(const map<string, string> &abstract_to_concrete); // 实例化

    string oprt; // 新定义的算子
    vector<shared_ptr<Variable>> input; // 算子的输入
    shared_ptr<Individual> output; // 算子的输出
};

class Rete_Rule;
class Rule{ // 定义规则
public:
    // 用 左右个体和描述信息 初始化
    Rule(const Individual &l,const Individual &r,const string &s):lhs(make_shared<Individual>(l)),rhs(make_shared<Individual>(r)),description(s){};
    Rule(){} // 默认构造
    
    bool operator==(const Rule &r) const {return lhs==r.lhs && rhs==r.rhs && description==r.description;}; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    shared_ptr<Rete_Rule> get_adapted(); // 获取适配Rete算法版本的规则

    shared_ptr<Individual> lhs; // left-hand side
    shared_ptr<Individual> rhs; // right-hand side
    string description; // 描述信息
};

class Rete_Rule:public Rule{ // 为适应推理系统对 Rule 进行改造后的版本
public:
    // 用 Rule和变量声明 初始化
    Rete_Rule(const Rule &rule,const map<string,shared_ptr<Concept>> &var_info):Rule(rule),var_decl(var_info){}
    Rete_Rule(){} // 默认构造

    string get_output_str() const; // 获取输出字符串 (改造后的lhs部分可能为空个体)
    shared_ptr<Rete_Rule> instantiate(const map<string, string> &abstract_to_concrete); // 实例化规则
    
    map<string,shared_ptr<Concept>> var_decl; // 变量声明
    bool worked = false; // 是否发挥了作用
};

class Knowledge_Base{ // 定义知识库
public:
    // 用 定义概念、定义个体、定义算子、知识的列表 初始化
    Knowledge_Base(const vector<shared_ptr<Def_Concept>> &c,const vector<shared_ptr<Def_Individual>> &i,const vector<shared_ptr<Def_Operator>> &o,const vector<shared_ptr<Rule>> &r):def_concepts(c),def_individuals(i),def_operators(o),rules(r){
        init_def_part();
        print_def_part();
    }
    Knowledge_Base(){} // 默认构造
    
    string get_output_str() const; // 获取输出字符串

    void get_adapted_rules(){ // 改造原始规则以得到易于进行推理的规则
        cout<<"开始改造原始规则以得到易于进行推理的规则..."<<endl;
        for(auto i:rules)
            rete_rules.push_back(i->get_adapted());
    };
    void propagate_var_decl(); // 传播变量声明到改造后的规则

    vector<shared_ptr<Def_Concept>> def_concepts; // 定义概念
    vector<shared_ptr<Def_Individual>> def_individuals; // 定义个体
    vector<shared_ptr<Def_Operator>> def_operators; // 定义算子
    vector<shared_ptr<Rule>> rules; // 规则形式的知识

    map<string, shared_ptr<Def_Concept>> def_cpt_hash_table; // 存放所有已定义的 Concept
    map<string, shared_ptr<Def_Individual>> def_indi_hash_table; // 存放所有已定义的 Individual
    map<string, shared_ptr<Def_Operator>> def_oprt_hash_table; // 存放所有已定义的 Operator
    map<string, set<Concept>> cpt_inh_map; // 存放概念之间的继承关系

    vector<shared_ptr<Rete_Rule>> rete_rules; // 用于推理系统的规则

private:
    void init_def_part(); // 初始化 定义概念、个体、算子 的部分
    void print_def_part(); // 输出 定义概念、个体、算子 的部分
};


ostream& operator<<(ostream &os, const Number &e);
ostream& operator<<(ostream &os, const Math_Func &e);
ostream& operator<<(ostream &os, const Math_Expr &e);
ostream& operator<<(ostream &os, const vector<shared_ptr<Math_Expr>> &e);
ostream& operator<<(ostream &os, const Math_Equation &e);
ostream& operator<<(ostream &os, const Coordinate &e);
ostream& operator<<(ostream &os, const Math_Individual &e);
ostream& operator<<(ostream &os, const Concept &e);
ostream& operator<<(ostream &os, const Variable &e);
ostream& operator<<(ostream &os, const vector<shared_ptr<Variable>> &e);
ostream& operator<<(ostream &os, const Individual &e);
ostream& operator<<(ostream &os, const vector<shared_ptr<Individual>> &e);
ostream& operator<<(ostream &os, const Assignment &e);
ostream& operator<<(ostream &os, const vector<shared_ptr<Assignment>> &e);
ostream& operator<<(ostream &os, const Cud &e);
ostream& operator<<(ostream &os, const Sugar_For_And &e);
ostream& operator<<(ostream &os, const Sugar_For_Pred &e);
ostream& operator<<(ostream &os, const Individual &e);
ostream& operator<<(ostream &os, const Sugar_For_Ctor &e);
ostream& operator<<(ostream &os, const Sugar_For_Oprt_Apply &e);
ostream& operator<<(ostream &os, const Term &e);
ostream& operator<<(ostream &os, const Assertion &e);
ostream& operator<<(ostream &os, const Def_Individual &e);
ostream& operator<<(ostream &os, const vector<shared_ptr<Def_Individual>> &e);
ostream& operator<<(ostream &os, const Fact &e);
ostream& operator<<(ostream &os, const vector<shared_ptr<Fact>> &e);
ostream& operator<<(ostream &os, const Question &e);
ostream& operator<<(ostream &os, const vector<shared_ptr<Question>> &e);
ostream& operator<<(ostream &os, const Def_Concept &e);
ostream& operator<<(ostream &os, const vector<shared_ptr<Def_Concept>> &e);
ostream& operator<<(ostream &os, const Def_Operator &e);
ostream& operator<<(ostream &os, const vector<shared_ptr<Def_Operator>> &e);
ostream& operator<<(ostream &os, const Rule &e);
ostream& operator<<(ostream &os, const vector<shared_ptr<Rule>> &e);
ostream& operator<<(ostream &os, const Knowledge_Base &e);
ostream& operator<<(ostream &os, const Rete_Rule &e);
ostream& operator<<(ostream &os, const map<string, shared_ptr<Concept>> &m);
ostream& operator<<(ostream &os, const Rete_Question &e);

string str_of_var_decl(const map<string, shared_ptr<Concept>> &var_decl); // 变量声明的字符串输出
string str_of_abs_to_con(const map<string, string> &abstrct_to_concrete); // 约束变元的实例对应的字符串输出
shared_ptr<Individual> var_decl_to_indi(const map<string, shared_ptr<Concept>> &var_decl); // 把变量声明改造为 Individual(具体地说是Variable)
map<string, shared_ptr<Concept>> instantiate_var_decl(const map<string, shared_ptr<Concept>> &var_decl, const map<string, string> &abstract_to_concrete); // 实例化变量声明
shared_ptr<Individual> action_eval(shared_ptr<Individual> indi, Rete_Question &question, shared_ptr<vector<shared_ptr<Fact>>> conditions = nullptr); // 个体求值
void try_to_simplify(shared_ptr<Assertion> &assertion, Rete_Question &question, shared_ptr<vector<shared_ptr<Fact>>> conditions_sp);
void try_to_simplify(shared_ptr<Individual> &indi, Rete_Question &question, shared_ptr<vector<shared_ptr<Fact>>> conditions_sp);
void construct_fact_in_graph(shared_ptr<Fact> new_fact, vector<string> dependence, Rete_Question &question);
bool is_potentially_solvable_eq(shared_ptr<Fact> fact);

inline string Number::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Math_Func::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Math_Expr::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Math_Equation::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Coordinate::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Math_Individual::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Concept::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Variable::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Individual::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Assignment::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Cud::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Sugar_For_And::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Sugar_For_Pred::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Sugar_For_Ctor::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Sugar_For_Oprt_Apply::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Term::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Assertion::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Def_Individual::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Fact::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Question::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Def_Concept::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Def_Operator::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Rule::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Knowledge_Base::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Rete_Rule::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}
inline string Rete_Question::get_output_str() const{
    std::ostringstream oss;
    oss<<*this;
    return oss.str();
}

#endif