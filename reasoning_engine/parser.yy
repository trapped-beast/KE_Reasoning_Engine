%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8"
%header

%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include "ke_struct.hh"
  class driver;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
# include "driver.hh"
  using std::cout;
  using std::endl;
}

%define api.token.prefix {TOK_}
%token
  DOT     "."
  PLUS    "+"
  MINUS   "-"
  MUL     "*"
  DIV     "/"
  POWER   "^"
  LPAREN  "("
  RPAREN  ")"
  LBRACE  "{"
  RBRACE  "}"
  COMMA   ","
  SEMI    ";"
  COLON   ":"
  EQ      "="
  ASSIGN  ":="
  MEQ     "=="
  GTE     ">="
  LTE     "<="
  NE      "!="
  GT      ">"
  LT      "<"
  IMPLIES "=>"
  INH     "<-"
;

%token <int> INTEGER
%token <double> FLOAT
%token <bool> BOOL
%token <std::string> DESCRIPTION SYMBOL OPERATOR CONCEPT DEF_CPT DEF_INDI DEF_OP KB_ASSERT KB_MODIFY KB_RETRACT MATH_FUNC_NAME

%nterm <Knowledge_Base> ke_kb
%nterm <Def_Concept> def_concept
%nterm <vector<shared_ptr<Def_Concept>>> def_concept_list
%nterm <Def_Individual> def_individual
%nterm <vector<shared_ptr<Def_Individual>>> def_individual_list
%nterm <Def_Operator> def_operator
%nterm <vector<shared_ptr<Def_Operator>>> def_operator_list
%nterm <Rule> ke_rule
%nterm <vector<shared_ptr<Rule>>> ke_rule_list
%nterm <Individual> individual
%nterm <Term> term
%nterm <Assertion> assertion
%nterm <Sugar_For_And> sugar_for_and
%nterm <Sugar_For_Pred> sugar_for_pred
%nterm <Sugar_For_Ctor> sugar_for_ctor
%nterm <Sugar_For_Oprt_Apply> sugar_for_oprt_apply
%nterm <Cud> cud
%nterm <Assignment> assignment
%nterm <vector<shared_ptr<Assignment>>> assignment_list
%nterm <Math_Individual> math_individual
%nterm <std::string> predicate_operator
%nterm <vector<shared_ptr<Individual>>> individual_list
%nterm <vector<shared_ptr<Individual>>> individual_and_list
%nterm <Concept> concept
%nterm <Variable> variable
%nterm <vector<shared_ptr<Variable>>> variable_list
%nterm <Math_Equation> math_equation
%nterm <Coordinate> coordinate
%nterm <Math_Expr> math_expr
%nterm <Math_Func> math_func
%nterm <vector<shared_ptr<Math_Expr>>> math_expr_list
%nterm <Number> number
%nterm <Question> ke_question
%nterm <vector<shared_ptr<Question>>> ke_questions
%nterm <Fact> fact
%nterm <vector<shared_ptr<Fact>>> fact_list
%nterm <vector<shared_ptr<Individual>>> to_solve


%printer { yyo << $$; } <*>;

%%
statement : ke_kb {drv.kb = make_shared<Knowledge_Base>($1);}
          | ke_questions {drv.questions = $1;}
          ;

ke_kb : def_concept_list def_individual_list def_operator_list ke_rule_list {Knowledge_Base kb($1, $2, $3, $4); $$=kb;};

def_concept_list : def_concept {vector<shared_ptr<Def_Concept>> v; v.push_back(make_shared<Def_Concept>($1)); $$=v;}
                 | def_concept_list def_concept {$1.push_back(make_shared<Def_Concept>($2));$$=$1;}
                 ;

def_concept : DEF_CPT concept "=" "{" variable_list "}" {$$=Def_Concept($2,$5);}
            | DEF_CPT concept "<-" concept "=" "{" variable_list "}" {$$=Def_Concept($2,$4,$7);}
            ;

def_individual_list : def_individual {vector<shared_ptr<Def_Individual>> v; v.push_back(make_shared<Def_Individual>($1)); $$=v;}
                    | def_individual_list def_individual {$1.push_back(make_shared<Def_Individual>($2));$$=$1;}
                    ;

def_individual : DEF_INDI SYMBOL ":" concept "=" individual {$$=Def_Individual($2,$4,$6);};

def_operator_list : def_operator {vector<shared_ptr<Def_Operator>> v; v.push_back(make_shared<Def_Operator>($1)); $$=v;}
                  | def_operator_list def_operator {$1.push_back(make_shared<Def_Operator>($2));$$=$1;}
                  ;

def_operator : DEF_OP OPERATOR "(" variable_list ")" "=" individual {$$=Def_Operator($2,$4,$7);};
            

ke_rule_list : ke_rule {vector<shared_ptr<Rule>> v; v.push_back(make_shared<Rule>($1)); $$=v;}
             | ke_rule_list ke_rule {$1.push_back(make_shared<Rule>($2));$$=$1;}
             ;

ke_rule : individual "=>" individual DESCRIPTION {Rule rule($1,$3,$4); $$=rule;};


individual : variable  {$$=Individual($1);}
           | BOOL      {$$=Individual($1);}
           | cud       {$$=Individual($1);}
           | term      {$$=Individual($1);}
           | assertion {$$=Individual($1);}
           | math_individual {$$=Individual($1);}
           ;

term : OPERATOR "(" individual_list ")" {$$=Term($1,$3);}
     | sugar_for_and  {$$=Term($1);}
     | sugar_for_pred {$$=Term($1);}
     | sugar_for_ctor {$$=Term($1);}
     | sugar_for_oprt_apply {$$=Term($1);}
     ;

assertion : "{" individual "}" {$$=Assertion($2);}
          | "{" individual "=" individual "}" {$$=Assertion($2,$4);}
          ;

sugar_for_and : "{" individual_and_list ";" individual "}" {$2.push_back(make_shared<Individual>($4)); $$=Sugar_For_And($2);};

sugar_for_pred : "{" individual predicate_operator individual "}" {$$=Sugar_For_Pred($2,$3,$4);};

sugar_for_ctor : "{" assignment_list "}" {$$=Sugar_For_Ctor($2);};

sugar_for_oprt_apply : SYMBOL "." SYMBOL {$$=Sugar_For_Oprt_Apply($1,$3);};

cud : KB_ASSERT individual "=" individual  {$$=Cud($1,$2,$4);}
    | KB_MODIFY individual "=" individual  {$$=Cud($1,$2,$4);}
    | KB_RETRACT individual "=" individual {$$=Cud($1,$2,$4);}
    ;

assignment_list : assignment {vector<shared_ptr<Assignment>> v; v.push_back(make_shared<Assignment>($1)); $$=v;}
                | assignment_list ";" assignment {$1.push_back(make_shared<Assignment>($3));$$=$1;}
                ;

assignment : SYMBOL ":=" individual {$$=Assignment($1,$3);};

math_individual : math_expr     {$$=Math_Individual($1);}
                | math_equation {$$=Math_Individual($1);}
                | coordinate    {$$=Math_Individual($1);}
                ;

%left ">" "<" ">=" "<=";
%left "!=";
predicate_operator : ">=" {$$=std::string(">=");}
                   | "<=" {$$=std::string("<=");}
                   | "!=" {$$=std::string("!=");}
                   | ">"  {$$=std::string(">");}
                   | "<"  {$$=std::string("<");}
                   ;

individual_list : individual {vector<shared_ptr<Individual>> v; v.push_back(make_shared<Individual>($1)); $$=v;}
                | individual_list "," individual {$1.push_back(make_shared<Individual>($3));$$=$1;}
                ;  

individual_and_list : individual {vector<shared_ptr<Individual>> v; v.push_back(make_shared<Individual>($1)); $$=v;}
                    | individual_and_list ";" individual {$1.push_back(make_shared<Individual>($3));$$=$1;}
                    ;  

concept : CONCEPT {$$=Concept($1);}
        | OPERATOR "(" concept ")" {$$=Concept($1,$3);}
        ;

variable : SYMBOL ":" concept {$$=Variable($1,$3);};

variable_list : variable {vector<shared_ptr<Variable>> v; v.push_back(make_shared<Variable>($1)); $$=v;}
              | variable_list ";" variable {$1.push_back(make_shared<Variable>($3));$$=$1;}
              ; 

math_equation : math_expr "==" math_expr {$$=Math_Equation($1,$3);};
coordinate    : "(" math_expr "," math_expr ")" {$$=Coordinate($2,$4);};


%left "+" "-";
%left "*" "/";
%right "^";

math_expr        : number {$$=Math_Expr($1);}
                 | SYMBOL {$$=Math_Expr($1);}
                 | math_expr "+" math_expr {$$=Math_Expr($1,'+',$3);} 
                 | math_expr "-" math_expr {$$=Math_Expr($1,'-',$3);}
                 | math_expr "*" math_expr {$$=Math_Expr($1,'*',$3);}
                 | math_expr "/" math_expr {$$=Math_Expr($1,'/',$3);}
                 | math_expr "^" math_expr {$$=Math_Expr($1,'^',$3);}
                 | "(" math_expr ")" {$$=Math_Expr(make_shared<Math_Expr>($2));}
                 | math_func {$$=Math_Expr($1);}
                 ;

math_func        : MATH_FUNC_NAME "(" math_expr_list ")" {$$=Math_Func($1,$3);};

math_expr_list   : math_expr {vector<shared_ptr<Math_Expr>> v;v.push_back(make_shared<Math_Expr>($1));$$=v;}
                 | math_expr_list "," math_expr {$1.push_back(make_shared<Math_Expr>($3));$$=$1;}
                 ;

number : INTEGER {$$=Number($1);}
       | FLOAT   {$$=Number($1);}
       ;

ke_questions : ke_question {vector<shared_ptr<Question>> v; v.push_back(make_shared<Question>($1)); $$=v;}
             | ke_questions ke_question {$1.push_back(make_shared<Question>($2));$$=$1;}
             ;

ke_question : DESCRIPTION fact_list to_solve {$$=Question($1,$2,$3);};

fact_list : fact {vector<shared_ptr<Fact>> v; v.push_back(make_shared<Fact>($1)); $$=v;}
          | fact_list ";" fact {$1.push_back(make_shared<Fact>($3));$$=$1;}
          ;

fact : assertion {$$=Fact($1);}
     | variable  {$$=Fact($1);}
     | def_individual {$$=Fact($1);}
     ;

to_solve : OPERATOR "(" individual_list ")" {assert($1==std::string("To_Solve"));$$=$3;};

%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
