#ifndef AL_STRUCT_HH
#define AL_STRUCT_HH

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
using std::map;
using std::set;
using std::string;
using std::ostream;
using std::cout;
using std::endl;


// 为保持数据一致性，原则上，成员中包含更小类对象的一律用指针

class Number{
public:
    // 用 int 初始化
    Number(int i):is_int(true),i_val(i){}
    // 用 float 初始化
    Number(double d):is_float(true),f_val(d){}
    Number(){} // 默认构造
    // 拷贝构造
    Number(const Number &rhs):is_int(rhs.is_int),is_float(rhs.is_float),i_val(rhs.i_val),f_val(rhs.f_val){}
    // 拷贝赋值
    Number& operator=(const Number &rhs){is_int = rhs.is_int; is_float = rhs.is_float; i_val = rhs.i_val; f_val = rhs.f_val; return *this;}
    
    string get_output_str() const; // 获取输出字符串
    bool operator==(const Number &rhs) const; // 重载 ==

    bool is_int = false; // 是否是整数
    bool is_float = false; // 是否是浮点数

    int i_val; // 整数
    double f_val; // 浮点数
};

class Math_Expr;
class Math_Func{ // 数学函数
public:
    // 用函数名和 vector<shared_ptr<Math_Expr>>格式的参数列表 初始化
    Math_Func(const string &name,const vector<shared_ptr<Math_Expr>> &expr_list):func_name(name),args(expr_list){}
    Math_Func(){} // 默认构造
    // 拷贝构造
    Math_Func(const Math_Func &rhs):func_name(rhs.func_name),args(rhs.args){}
    // 拷贝赋值
    Math_Func& operator=(const Math_Func &rhs){func_name = rhs.func_name; args = rhs.args; return *this;}
    
    string get_output_str() const; // 获取输出字符串
    bool operator==(const Math_Func &rhs) const; // 重载 ==

    string func_name; //函数名
    vector<shared_ptr<Math_Expr>> args; // 参数列表
};

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
    
    Math_Expr(){} // 默认构造
    // 拷贝构造
    Math_Expr(const Math_Expr &rhs):is_num(rhs.is_num),is_sy(rhs.is_sy),is_func(rhs.is_func),is_mathe(rhs.is_mathe),number_val(rhs.number_val),sy_val(rhs.sy_val),func_val(rhs.func_val),op_val(rhs.op_val),left(rhs.left),right(rhs.right){}
    // 拷贝赋值
    Math_Expr& operator=(const Math_Expr &rhs){is_num = rhs.is_num; is_sy = rhs.is_sy; is_func = rhs.is_func; is_mathe = rhs.is_mathe; number_val = rhs.number_val; sy_val = rhs.sy_val; func_val = rhs.func_val; op_val = rhs.op_val; left = rhs.left; right = rhs.right; return *this;}

    string get_output_str() const; // 获取输出字符串
    bool operator==(const Math_Expr &rhs) const; // 重载 ==

    bool is_num = false; // 是否是数
    bool is_sy = false; // 是否是符号
    bool is_func = false; // 是否是数学函数
    bool is_mathe = false; // 是否是 +-*/^

    shared_ptr<Number> number_val; // Number
    string sy_val; // Symbol
    shared_ptr<Math_Func> func_val; // 函数
    char op_val = '!'; // +-*/^
    shared_ptr<Math_Expr> left;
    shared_ptr<Math_Expr> right;
};

class Math_Equation{ // 数学方程
public:
    // 用左右两个 Math_Expr 初始化
    Math_Equation(const Math_Expr &l, const Math_Expr &r):left(make_shared<Math_Expr>(l)),right(make_shared<Math_Expr>(r)){}
    Math_Equation(){} // 默认构造
    // 拷贝构造
    Math_Equation(const Math_Equation &rhs):left(rhs.left),right(rhs.right){}
    // 拷贝赋值
    Math_Equation& operator=(const Math_Equation &rhs){left = rhs.left; right = rhs.right; return *this;}
    
    bool operator==(const Math_Equation &rhs) const{return left==rhs.left && right==rhs.right;} // 重载 ==
    string get_output_str() const; // 获取输出字符串
    
    shared_ptr<Math_Expr> left; // 方程左部
    shared_ptr<Math_Expr> right; // 方程右部
};

class Coordinate{ // 二维坐标
public:
    // 用横纵坐标的 Math_Expr 初始化
    Coordinate(const Math_Expr &x, const Math_Expr &y):abscissa(make_shared<Math_Expr>(x)),ordinate(make_shared<Math_Expr>(y)){}
    Coordinate(){} // 默认构造
    // 拷贝构造
    Coordinate(const Coordinate &rhs):abscissa(rhs.abscissa),ordinate(rhs.ordinate){}
    // 拷贝赋值
    Coordinate& operator=(const Coordinate &rhs){abscissa = rhs.abscissa; ordinate = rhs.ordinate; return *this;}

    bool operator==(const Coordinate &rhs) const{return abscissa==rhs.abscissa && ordinate==rhs.ordinate;} // 重载 ==
    string get_output_str() const; // 获取输出字符串
    
    shared_ptr<Math_Expr> abscissa; // 横坐标
    shared_ptr<Math_Expr> ordinate; // 纵坐标
};

class Math_Individual{ // 数学个体
public:
    // 用单个 Math_Expr 初始化
    Math_Individual(const Math_Expr &e):is_math_expr(true),expr_val(make_shared<Math_Expr>(e)){}
    // 用方程初始化
    Math_Individual(const Math_Equation &e):is_equation(true),equation_val(make_shared<Math_Equation>(e)){}
    // 用坐标初始化
    Math_Individual(const Coordinate &e):is_coordinate(true),coordinate_val(make_shared<Coordinate>(e)){}
    
    Math_Individual(){} // 默认构造
    // 拷贝构造
    Math_Individual(const Math_Individual &rhs):is_equation(rhs.is_equation),is_coordinate(rhs.is_coordinate),is_math_expr(rhs.is_math_expr),equation_val(rhs.equation_val),coordinate_val(rhs.coordinate_val),expr_val(rhs.expr_val){}
    // 拷贝赋值
    Math_Individual& operator=(const Math_Individual &rhs){is_equation = rhs.is_equation;is_coordinate = rhs.is_coordinate;is_math_expr = rhs.is_math_expr;equation_val = rhs.equation_val;coordinate_val = rhs.coordinate_val;expr_val = rhs.expr_val;return *this;}

    string get_output_str() const; // 获取输出字符串
    bool operator==(const Math_Individual &rhs) const; // 重载 ==
    
    bool is_equation = false; // 是否是方程
    bool is_coordinate = false; // 是否是坐标
    bool is_math_expr = false; // 是否是数学表达式

    shared_ptr<Math_Equation> equation_val; // 方程
    shared_ptr<Coordinate> coordinate_val; // 坐标
    shared_ptr<Math_Expr> expr_val; // 数学表达式
};

class Concept{
public:
    // 用原子概念名初始化
    Concept(const string &n):is_atomic(true),atomic_concept(n){}
    // 用外层算子和内层Concept初始化
    Concept(const string &n, const Concept &c):is_compound(true),outer_oprt(n),inner_concept(make_shared<Concept>(c)){}
    
    Concept(){} // 默认构造
    // 拷贝构造
    Concept(const Concept &rhs):is_atomic(rhs.is_atomic),is_compound(rhs.is_compound),atomic_concept(rhs.atomic_concept),outer_oprt(rhs.outer_oprt),inner_concept(rhs.inner_concept){}
    // 拷贝赋值
    Concept& operator=(const Concept &rhs){is_atomic=rhs.is_atomic,is_compound=rhs.is_compound,atomic_concept=rhs.atomic_concept,outer_oprt=rhs.outer_oprt,inner_concept=rhs.inner_concept;return *this;}

    string get_output_str() const; // 获取输出字符串
    bool operator==(const Concept &rhs) const; // 重载 ==
    // 重载 < 
    // bool operator<(const Concept &rhs) const; ??todo
    
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
    // 拷贝构造
    Variable(const Variable &rhs):symbol(rhs.symbol),concept(rhs.concept){}
    // 拷贝赋值
    Variable& operator=(const Variable &rhs){symbol=rhs.symbol;concept=rhs.concept;return *this;}
    // 重载 <
    // bool operator<(const Variable &rhs) const  ??todo
    
    string get_output_str() const; // 获取输出字符串
    bool operator==(const Variable &rhs) const{return symbol==rhs.symbol && *concept==*rhs.concept;} // 重载 ==
    
    string symbol; // 符号
    shared_ptr<Concept> concept; // 对应概念
};

class Cud; class Term; class Assertion;

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
    Individual(const Math_Individual &e):is_math_indi(true),math_indi(make_shared<Math_Individual>(e)){}

    Individual(){} // 默认构造
    // 拷贝构造
    Individual(const Individual &rhs):is_var(rhs.is_var),is_bool(rhs.is_bool),is_cud(rhs.is_cud),is_term(rhs.is_term),is_assertion(rhs.is_assertion),is_math_indi(rhs.is_math_indi),
        var_val(rhs.var_val),bool_val(rhs.bool_val),cud(rhs.cud),term(rhs.term),assertion(rhs.assertion),math_indi(rhs.math_indi){}
    // 拷贝赋值
    Individual& operator=(const Individual &rhs){is_var=rhs.is_var;is_bool=rhs.is_bool;is_cud=rhs.is_cud;is_term=rhs.is_term;is_assertion=rhs.is_assertion;is_math_indi=rhs.is_math_indi;
        var_val=rhs.var_val;bool_val=rhs.bool_val;cud=rhs.cud;term=rhs.term;assertion=rhs.assertion;math_indi=rhs.math_indi;return *this;}

    bool operator==(const Individual &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

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
};

class Assignment{ // 赋值式 symbol := individual
public:
    // 用符号和个体初始化
    Assignment(const string &s,const Individual &i):symbol(s),val(make_shared<Individual>(i)){}
    Assignment(){} // 默认构造
    // 拷贝构造
    Assignment(const Assignment &rhs):symbol(rhs.symbol),val(rhs.val){}
    // 拷贝赋值
    Assignment& operator=(const Assignment &rhs){symbol=rhs.symbol;val=rhs.val;return *this;}

    bool operator==(const Assignment &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    string symbol; // 符号
    shared_ptr<Individual> val; // 对应的个体
};

class Cud{ // 增、改、删 fact
public:
    // 用 cud标识符、左右两个个体 初始化
    Cud(const string &cud_idf,const Individual &l,const Individual &r):action(cud_idf),left(make_shared<Individual>(l)),right(make_shared<Individual>(r)){}
    Cud(){} // 默认构造
    // 拷贝构造
    Cud(const Cud &rhs):action(rhs.action),left(rhs.left),right(rhs.right){}
    // 拷贝赋值
    Cud& operator=(const Cud &rhs){action=rhs.action;left=rhs.left;right=rhs.right;return *this;}

    bool operator==(const Cud &rhs) const{return action==rhs.action && left==rhs.left && right==rhs.right;}; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    string action; // "assert" "modify" "retract"
    shared_ptr<Individual> left;
    shared_ptr<Individual> right;
};

class Sugar_For_And{ // 逻辑与的语法糖
public:
    // 用 vector<shared_ptr<Individual>>格式的个体列表 初始化
    Sugar_For_And(const vector<shared_ptr<Individual>> &ls):content(ls){}
    Sugar_For_And(){} // 默认构造
    // 拷贝构造
    Sugar_For_And(const Sugar_For_And &rhs):content(rhs.content){}
    // 拷贝赋值
    Sugar_For_And& operator=(const Sugar_For_And &rhs){content=rhs.content;return *this;}

    bool operator==(const Sugar_For_And &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    vector<shared_ptr<Individual>> content; // 与式 (注意这其中的分隔符应该是;)
};

class Sugar_For_Pred{ // 二元谓词的语法糖
public:
    // 用 两个个体和一个谓词 初始化
    Sugar_For_Pred(const Individual &i_1,const string &pred,const Individual &i_2):left(make_shared<Individual>(i_1)),right(make_shared<Individual>(i_2)),predicate(pred){}
    
    Sugar_For_Pred(){} // 默认构造
    // 拷贝构造
    Sugar_For_Pred(const Sugar_For_Pred &rhs):left(rhs.left),right(rhs.right),predicate(rhs.predicate){}
    // 拷贝赋值
    Sugar_For_Pred& operator=(const Sugar_For_Pred &rhs){left=rhs.left;right=rhs.right;predicate=rhs.predicate;return *this;}
    
    bool operator==(const Sugar_For_Pred &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    shared_ptr<Individual> left; // 第一个参数
    shared_ptr<Individual> right; // 第二个参数
    string predicate; // 谓词
};

class Sugar_For_Ctor { // 构造器的语法糖: 构造器是用赋值式列表临时创建的个体
public:
    // 用 vector<shared_ptr<Assignment>>格式的赋值式列表 初始化
    Sugar_For_Ctor(const vector<shared_ptr<Assignment>> &ls):content(ls){}
    Sugar_For_Ctor(){} // 默认构造
    // 拷贝构造
    Sugar_For_Ctor(const Sugar_For_Ctor &rhs):content(rhs.content){} 
    // 拷贝赋值
    Sugar_For_Ctor& operator=(const Sugar_For_Ctor &rhs){content=rhs.content;return *this;}

    bool operator==(const Sugar_For_Ctor &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    vector<shared_ptr<Assignment>> content;
};

class Sugar_For_Oprt_Apply{ // 一元算子应用的语法糖: a.b 表示 Get_b(a)
public:
    // 用 表示个体的符号和表示一元算子的符号 初始化
    Sugar_For_Oprt_Apply(const string &i="",const string &op=""):indi(i),uni_oprt(op){}
    // 拷贝构造
    Sugar_For_Oprt_Apply(const Sugar_For_Oprt_Apply &rhs):indi(rhs.indi),uni_oprt(rhs.uni_oprt){}
    // 拷贝赋值
    Sugar_For_Oprt_Apply& operator=(const Sugar_For_Oprt_Apply &rhs){indi=rhs.indi;uni_oprt=rhs.uni_oprt;return *this;}
    
    bool operator==(const Sugar_For_Oprt_Apply &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    string indi; // 表示个体的符号
    string uni_oprt; // 表示一元算子的符号
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
    // 拷贝构造
    Term(const Term &rhs):is_and(rhs.is_and),is_pred(rhs.is_pred),is_ctor(rhs.is_ctor),is_oprt_apply(rhs.is_oprt_apply),is_std(rhs.is_std),and_val(rhs.and_val),pred_val(rhs.pred_val),ctor_val(rhs.ctor_val),oprt_apply_val(rhs.oprt_apply_val),oprt(rhs.oprt),args(rhs.args){}
    // 拷贝赋值
    Term& operator=(const Term &rhs){is_and = rhs.is_and;is_pred = rhs.is_pred;is_ctor = rhs.is_ctor;is_oprt_apply = rhs.is_oprt_apply;is_std = rhs.is_std;and_val = rhs.and_val;pred_val = rhs.pred_val;ctor_val = rhs.ctor_val;oprt_apply_val = rhs.oprt_apply_val;oprt = rhs.oprt;args = rhs.args;return *this;}
    
    bool operator==(const Term &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    bool is_and = false; // 是否是sugar_for_and
    bool is_pred = false; // 是否是sugar_for_pred
    bool is_ctor= false; // 是否是sugar_for_ctor
    bool is_oprt_apply = false; // 是否是sugar_for_oprt_apply
    bool is_std; // 是否是标准形式

    shared_ptr<Sugar_For_And> and_val; // sugar_for_and 形式的项
    shared_ptr<Sugar_For_Pred> pred_val; // sugar_for_pred 形式的项
    shared_ptr<Sugar_For_Ctor> ctor_val; // sugar_for_ctor 形式的项
    shared_ptr<Sugar_For_Oprt_Apply> oprt_apply_val; // sugar_for_oprt_apply 形式的项
    string oprt; // 项的算子
    vector<shared_ptr<Individual>> args; // 算子的参数
};

class Assertion{
public:
    // 用 左右两个个体 初始化
    Assertion(const Individual &l,const Individual &r):is_std(true),left(make_shared<Individual>(l)),right(make_shared<Individual>(r)){}
    // 用 单个个体 初始化
    Assertion(const Individual &i):is_sugar_for_true(true),lonely_left(make_shared<Individual>(i)){}
    Assertion(){} // 默认构造
    // 拷贝构造
    Assertion(const Assertion &rhs):is_std(rhs.is_std),is_sugar_for_true(rhs.is_sugar_for_true),left(rhs.left),right(rhs.right),lonely_left(rhs.lonely_left){}
    // 拷贝赋值
    Assertion& operator=(const Assertion &rhs){is_std = rhs.is_std;is_sugar_for_true = rhs.is_sugar_for_true;left = rhs.left;right = rhs.right;lonely_left = rhs.lonely_left;return *this;}

    bool operator==(const Assertion &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    bool is_std; // 是否是标准形式
    bool is_sugar_for_true; // 是否是 Individual=true 的语法糖

    shared_ptr<Individual> left; // 断言的左部
    shared_ptr<Individual> right; // 断言的右部
    shared_ptr<Individual> lonely_left; // Individual=true 中的左部个体
};

class Def_Individual{ // 定义个体
public:
    // 用 符号、概念、个体 自定义
    Def_Individual(const string &s,const Concept &c,const Individual &i):symbol(s),concept(make_shared<Concept>(c)),indi(make_shared<Individual>(i)){}
    Def_Individual(){} // 默认构造
    Def_Individual(const Def_Individual &rhs):symbol(rhs.symbol),concept(rhs.concept),indi(rhs.indi){}; // 拷贝构造
    Def_Individual& operator=(const Def_Individual &rhs){symbol=rhs.symbol;concept=rhs.concept;indi=rhs.indi;return *this;}; // 拷贝赋值

    bool operator==(const Def_Individual &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    string symbol; // 个体的代表符号
    shared_ptr<Concept> concept; // 所属概念
    shared_ptr<Individual> indi; // 新定义的个体
};

class Fact{ // 事实
public:
    // 用断言定义
    Fact(const Assertion &e):is_assert(true),assertion(make_shared<Assertion>(e)){}
    // 用变量定义
    Fact(const Variable &e):is_var(true),variable(make_shared<Variable>(e)){}
    // 用定义个体定义
    Fact(const Def_Individual &e):is_def_indi(true),def_indi(make_shared<Def_Individual>(e)){}

    Fact(){} // 默认构造
    // 拷贝构造
    Fact(const Fact &rhs):is_assert(rhs.is_assert),is_var(rhs.is_var),is_def_indi(rhs.is_def_indi),assertion(rhs.assertion),variable(rhs.variable),def_indi(rhs.def_indi){}
    // 拷贝赋值
    Fact& operator=(const Fact &rhs){is_assert = rhs.is_assert;is_var = rhs.is_var;is_def_indi = rhs.is_def_indi;assertion = rhs.assertion;variable = rhs.variable;def_indi = rhs.def_indi;return *this;}

    bool operator==(const Fact &rhs) const; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    bool is_assert = false; // 是否是断言
    bool is_var = false; // 是否是变量
    bool is_def_indi = false; // 是否是定义个体

    shared_ptr<Assertion> assertion; // 断言
    shared_ptr<Variable> variable; // 变量
    shared_ptr<Def_Individual> def_indi; // 定义个体
};

class Question{ // 问题
public:
    // 用 描述信息、事实列表、待求解项 初始化
    Question(const string &s,const vector<shared_ptr<Fact>> &facts,const vector<shared_ptr<Individual>> &unkown):description(s),fact_list(facts),to_solve(unkown){}
    Question(){} // 默认构造
    // 拷贝构造
    Question(const Question &rhs):description(rhs.description),fact_list(rhs.fact_list),to_solve(rhs.to_solve){}
    // 拷贝赋值
    Question& operator=(const Question &rhs){description=rhs.description;fact_list=rhs.fact_list;to_solve=rhs.to_solve;return *this;}

    string get_output_str() const; // 获取输出字符串

    string description; // 描述信息
    vector<shared_ptr<Fact>> fact_list; // 事实列表
    vector<shared_ptr<Individual>> to_solve; // 待求解项
};

class Def_Concept{ // 定义概念
public:
    // 用 概念和变量列表 自定义
    Def_Concept(const Concept &c,const vector<shared_ptr<Variable>> &ls):without_parent(true),concept(make_shared<Concept>(c)),members(ls){}
    // 用 概念、父概念、变量列表 自定义
    Def_Concept(const Concept &c,const Concept &p,const vector<shared_ptr<Variable>> &ls):with_parent(true),concept(make_shared<Concept>(c)),parent_cpt(make_shared<Concept>(p)),members(ls){}
    Def_Concept(){} // 默认构造
    // 拷贝构造
    Def_Concept(const Def_Concept &rhs):with_parent(rhs.with_parent),without_parent(rhs.without_parent),concept(rhs.concept),parent_cpt(rhs.parent_cpt),members(rhs.members){};
    // 拷贝赋值
    Def_Concept& operator=(const Def_Concept &rhs){with_parent=rhs.with_parent;without_parent=rhs.without_parent;concept=rhs.concept;parent_cpt=rhs.parent_cpt;members=rhs.members;return *this;};

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
    Def_Operator(const Def_Operator &rhs):oprt(rhs.oprt),input(rhs.input),output(rhs.output){} // 拷贝构造
    Def_Operator& operator=(const Def_Operator &rhs){oprt=rhs.oprt;input=rhs.input;output=rhs.output;return *this;} // 拷贝赋值

    string get_output_str() const; // 获取输出字符串

    string oprt; // 新定义的算子
    vector<shared_ptr<Variable>> input; // 算子的输入
    shared_ptr<Individual> output; // 算子的输出
};

class Rule{ // 定义规则
public:
    // 用 左右个体和描述信息 初始化
    Rule(const Individual &l,const Individual &r,const string &s):lhs(make_shared<Individual>(l)),rhs(make_shared<Individual>(r)),description(s){};
    Rule(){} // 默认构造
    // 拷贝构造
    Rule(const Rule &rhs):lhs(rhs.lhs),rhs(rhs.rhs),description(rhs.description){}
    // 拷贝赋值
    Rule& operator=(const Rule &r){lhs=r.lhs;rhs=r.rhs;description=r.description;return *this;}

    bool operator==(const Rule &r) const {return lhs==r.lhs && rhs==r.rhs && description==r.description;}; // 重载 ==
    string get_output_str() const; // 获取输出字符串

    shared_ptr<Individual> lhs; // left-hand side
    shared_ptr<Individual> rhs; // right-hand side
    string description; // 描述信息
    // map
};

class Knowledge_Base{ // 定义知识库
public:
    // 用 定义概念、定义个体、定义算子、知识的列表 初始化
    Knowledge_Base(const vector<shared_ptr<Def_Concept>> &c,const vector<shared_ptr<Def_Individual>> &i,const vector<shared_ptr<Def_Operator>> &o,const vector<shared_ptr<Rule>> &r):def_concepts(c),def_individuals(i),def_operators(o),rules(r){}
    Knowledge_Base(){} // 默认构造
    // 拷贝构造
    Knowledge_Base(const Knowledge_Base &rhs):def_concepts(rhs.def_concepts),def_individuals(rhs.def_individuals),def_operators(rhs.def_operators),rules(rhs.rules){}
    // 拷贝赋值
    Knowledge_Base& operator=(const Knowledge_Base &rhs){def_concepts=rhs.def_concepts;def_individuals=rhs.def_individuals;def_operators=rhs.def_operators;rules=rhs.rules;return *this;}

    string get_output_str() const; // 获取输出字符串

    vector<shared_ptr<Def_Concept>> def_concepts; // 定义概念
    vector<shared_ptr<Def_Individual>> def_individuals; // 定义个体
    vector<shared_ptr<Def_Operator>> def_operators; // 定义算子
    vector<shared_ptr<Rule>> rules; // 规则形式的知识
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

#endif