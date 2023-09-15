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
%token <std::string> DESCRIPTION SYMBOL OPERATOR CONCEPT DEF_CPT DEF_INDI DEF_OP KB_ASSERT MATH_FUNC_NAME

%printer { yyo << $$; } <*>;

%%
statement : ke_kb
          | ke_questions
          ;

ke_kb : def_concept_list def_individual_list def_operator_list ke_rule_list;

def_concept_list : def_concept
                 | def_concept_list def_concept
                 ;

def_concept : DEF_CPT concept "=" "{" variable_list "}"
            | DEF_CPT concept "<-" concept "=" "{" variable_list "}"
            ;

def_individual_list : def_individual
                    | def_individual_list def_individual
                    ;

def_individual : DEF_INDI SYMBOL ":" concept "=" individual

def_operator_list : def_operator
                  | def_operator_list def_operator
                  ;

def_operator : DEF_OP OPERATOR "(" variable_list ")" "=" individual;
            

ke_rule_list : ke_rule
             | ke_rule_list ke_rule
             ;

ke_rule : individual "=>" individual DESCRIPTION {cout<<endl<<"sugar_for_rule!"<<endl<<endl;};


individual : variable
           | BOOL
           | math_individual
           | assertion{cout<<"assertion"<<endl;}
           | cud{cout<<"cud"<<endl;}
           | term
           ;

term : OPERATOR "(" individual_list ")"
     | sugar_for_and {cout<<"sugar_for_and"<<endl;}
     | sugar_for_pred{cout<<"sugar_for_pred"<<endl;}
     | sugar_for_ctor{cout<<"sugar_for_ctor"<<endl;}
     | sugar_for_oprt_apply{cout<<"sugar_for_oprt_apply"<<endl;}
     ;

assertion : "{" individual "=" individual "}"
          | "{" individual_list "}"
          ;

sugar_for_and : "{" individual_and_list ";" individual "}";

sugar_for_pred : "{" individual predicate_operator individual "}";

sugar_for_ctor : "{" constructor_list "}";

sugar_for_oprt_apply : SYMBOL DOT SYMBOL;

cud : KB_ASSERT variable "=" "{" assignment_list "}"{cout<<"KB_ASSERT"<<endl;}
    | KB_ASSERT individual "=" individual
    ;

assignment_list : assignment {cout<<"assignment"<<endl;};
                | assignment_list ";" assignment
                ;

assignment : SYMBOL ":=" individual{cout<<"assignment 中的 SYMBOL: "<<$1<<endl;};

math_individual : math_expr
                | math_equation
                | coordinate
                ;

%left ">" "<" ">=" "<=";
%left "!=";
predicate_operator : ">="
                   | "<="
                   | "!="
                   | ">"
                   | "<"
                   ;

constructor_list : constructor
                 | constructor_list ";" constructor 
                 ;

constructor : SYMBOL ":=" individual;

individual_list : individual
                | individual_list "," individual
                ;  

individual_and_list : individual
                    | individual_and_list ";" individual   
                    ;  

concept : CONCEPT
        | OPERATOR "(" concept ")"
        ;

variable : SYMBOL ":" concept;

variable_list : variable
              | variable_list ";" variable
              ; 

math_equation    : math_expr "==" math_expr;
coordinate       : "(" math_expr "," math_expr ")";


%left "+" "-";
%left "*" "/";
%right "^";

math_expr        : number
                 | SYMBOL
                 | math_expr "+" math_expr
                 | math_expr "-" math_expr
                 | math_expr "*" math_expr
                 | math_expr "/" math_expr
                 | math_expr "^" math_expr
                 | math_func
                 ;

math_func        : MATH_FUNC_NAME "(" math_expr_list ")";

math_expr_list   : math_expr
                 | math_expr_list "," math_expr
                 ;

number : INTEGER
       | FLOAT
       ;

ke_questions : ke_question_list;

ke_question_list : ke_question
                 | ke_question_list ke_question
                 ;

ke_question : DESCRIPTION fact_list to_solve;

fact_list : fact
          | fact_list ";" fact
          ;

fact : assertion
     | variable
     | def_individual
     ;

to_solve : OPERATOR "(" individual_list ")" ;

%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}