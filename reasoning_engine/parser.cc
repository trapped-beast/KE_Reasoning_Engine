// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.hh"


// Unqualified %code blocks.
#line 24 "parser.yy"

# include "driver.hh"
  using std::cout;
  using std::endl;

#line 52 "parser.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 144 "parser.cc"

  /// Build a parser object.
  parser::parser (driver& drv_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yy_lac_established_ (false),
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_assertion: // assertion
        value.YY_MOVE_OR_COPY< Assertion > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_assignment: // assignment
        value.YY_MOVE_OR_COPY< Assignment > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_concept: // concept
        value.YY_MOVE_OR_COPY< Concept > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_coordinate: // coordinate
        value.YY_MOVE_OR_COPY< Coordinate > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_cud: // cud
        value.YY_MOVE_OR_COPY< Cud > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_concept: // def_concept
        value.YY_MOVE_OR_COPY< Def_Concept > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_individual: // def_individual
        value.YY_MOVE_OR_COPY< Def_Individual > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_operator: // def_operator
        value.YY_MOVE_OR_COPY< Def_Operator > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fact: // fact
        value.YY_MOVE_OR_COPY< Fact > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_individual: // individual
        value.YY_MOVE_OR_COPY< Individual > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_kb: // ke_kb
        value.YY_MOVE_OR_COPY< Knowledge_Base > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_equation: // math_equation
        value.YY_MOVE_OR_COPY< Math_Equation > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_expr: // math_expr
        value.YY_MOVE_OR_COPY< Math_Expr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_func: // math_func
        value.YY_MOVE_OR_COPY< Math_Func > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_individual: // math_individual
        value.YY_MOVE_OR_COPY< Math_Individual > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_number: // number
        value.YY_MOVE_OR_COPY< Number > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_question: // ke_question
        value.YY_MOVE_OR_COPY< Question > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_rule: // ke_rule
        value.YY_MOVE_OR_COPY< Rule > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_and: // sugar_for_and
        value.YY_MOVE_OR_COPY< Sugar_For_And > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_ctor: // sugar_for_ctor
        value.YY_MOVE_OR_COPY< Sugar_For_Ctor > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_oprt_apply: // sugar_for_oprt_apply
        value.YY_MOVE_OR_COPY< Sugar_For_Oprt_Apply > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_pred: // sugar_for_pred
        value.YY_MOVE_OR_COPY< Sugar_For_Pred > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_term: // term
        value.YY_MOVE_OR_COPY< Term > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_variable: // variable
        value.YY_MOVE_OR_COPY< Variable > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BOOL: // BOOL
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DESCRIPTION: // DESCRIPTION
      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_OPERATOR: // OPERATOR
      case symbol_kind::S_CONCEPT: // CONCEPT
      case symbol_kind::S_DEF_CPT: // DEF_CPT
      case symbol_kind::S_DEF_INDI: // DEF_INDI
      case symbol_kind::S_DEF_OP: // DEF_OP
      case symbol_kind::S_KB_ASSERT: // KB_ASSERT
      case symbol_kind::S_KB_MODIFY: // KB_MODIFY
      case symbol_kind::S_KB_RETRACT: // KB_RETRACT
      case symbol_kind::S_MATH_FUNC_NAME: // MATH_FUNC_NAME
      case symbol_kind::S_predicate_operator: // predicate_operator
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_assignment_list: // assignment_list
        value.YY_MOVE_OR_COPY< vector<shared_ptr<Assignment>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_concept_list: // def_concept_list
        value.YY_MOVE_OR_COPY< vector<shared_ptr<Def_Concept>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_individual_list: // def_individual_list
        value.YY_MOVE_OR_COPY< vector<shared_ptr<Def_Individual>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_operator_list: // def_operator_list
        value.YY_MOVE_OR_COPY< vector<shared_ptr<Def_Operator>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fact_list: // fact_list
        value.YY_MOVE_OR_COPY< vector<shared_ptr<Fact>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_individual_list: // individual_list
      case symbol_kind::S_individual_and_list: // individual_and_list
      case symbol_kind::S_to_solve: // to_solve
        value.YY_MOVE_OR_COPY< vector<shared_ptr<Individual>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_expr_list: // math_expr_list
        value.YY_MOVE_OR_COPY< vector<shared_ptr<Math_Expr>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_questions: // ke_questions
        value.YY_MOVE_OR_COPY< vector<shared_ptr<Question>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_rule_list: // ke_rule_list
        value.YY_MOVE_OR_COPY< vector<shared_ptr<Rule>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_variable_list: // variable_list
        value.YY_MOVE_OR_COPY< vector<shared_ptr<Variable>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_assertion: // assertion
        value.move< Assertion > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_assignment: // assignment
        value.move< Assignment > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_concept: // concept
        value.move< Concept > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_coordinate: // coordinate
        value.move< Coordinate > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_cud: // cud
        value.move< Cud > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_concept: // def_concept
        value.move< Def_Concept > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_individual: // def_individual
        value.move< Def_Individual > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_operator: // def_operator
        value.move< Def_Operator > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fact: // fact
        value.move< Fact > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_individual: // individual
        value.move< Individual > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_kb: // ke_kb
        value.move< Knowledge_Base > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_equation: // math_equation
        value.move< Math_Equation > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_expr: // math_expr
        value.move< Math_Expr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_func: // math_func
        value.move< Math_Func > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_individual: // math_individual
        value.move< Math_Individual > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_number: // number
        value.move< Number > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_question: // ke_question
        value.move< Question > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_rule: // ke_rule
        value.move< Rule > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_and: // sugar_for_and
        value.move< Sugar_For_And > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_ctor: // sugar_for_ctor
        value.move< Sugar_For_Ctor > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_oprt_apply: // sugar_for_oprt_apply
        value.move< Sugar_For_Oprt_Apply > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_pred: // sugar_for_pred
        value.move< Sugar_For_Pred > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_term: // term
        value.move< Term > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_variable: // variable
        value.move< Variable > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BOOL: // BOOL
        value.move< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DESCRIPTION: // DESCRIPTION
      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_OPERATOR: // OPERATOR
      case symbol_kind::S_CONCEPT: // CONCEPT
      case symbol_kind::S_DEF_CPT: // DEF_CPT
      case symbol_kind::S_DEF_INDI: // DEF_INDI
      case symbol_kind::S_DEF_OP: // DEF_OP
      case symbol_kind::S_KB_ASSERT: // KB_ASSERT
      case symbol_kind::S_KB_MODIFY: // KB_MODIFY
      case symbol_kind::S_KB_RETRACT: // KB_RETRACT
      case symbol_kind::S_MATH_FUNC_NAME: // MATH_FUNC_NAME
      case symbol_kind::S_predicate_operator: // predicate_operator
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_assignment_list: // assignment_list
        value.move< vector<shared_ptr<Assignment>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_concept_list: // def_concept_list
        value.move< vector<shared_ptr<Def_Concept>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_individual_list: // def_individual_list
        value.move< vector<shared_ptr<Def_Individual>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_operator_list: // def_operator_list
        value.move< vector<shared_ptr<Def_Operator>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fact_list: // fact_list
        value.move< vector<shared_ptr<Fact>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_individual_list: // individual_list
      case symbol_kind::S_individual_and_list: // individual_and_list
      case symbol_kind::S_to_solve: // to_solve
        value.move< vector<shared_ptr<Individual>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_expr_list: // math_expr_list
        value.move< vector<shared_ptr<Math_Expr>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_questions: // ke_questions
        value.move< vector<shared_ptr<Question>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_rule_list: // ke_rule_list
        value.move< vector<shared_ptr<Rule>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_variable_list: // variable_list
        value.move< vector<shared_ptr<Variable>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_assertion: // assertion
        value.copy< Assertion > (that.value);
        break;

      case symbol_kind::S_assignment: // assignment
        value.copy< Assignment > (that.value);
        break;

      case symbol_kind::S_concept: // concept
        value.copy< Concept > (that.value);
        break;

      case symbol_kind::S_coordinate: // coordinate
        value.copy< Coordinate > (that.value);
        break;

      case symbol_kind::S_cud: // cud
        value.copy< Cud > (that.value);
        break;

      case symbol_kind::S_def_concept: // def_concept
        value.copy< Def_Concept > (that.value);
        break;

      case symbol_kind::S_def_individual: // def_individual
        value.copy< Def_Individual > (that.value);
        break;

      case symbol_kind::S_def_operator: // def_operator
        value.copy< Def_Operator > (that.value);
        break;

      case symbol_kind::S_fact: // fact
        value.copy< Fact > (that.value);
        break;

      case symbol_kind::S_individual: // individual
        value.copy< Individual > (that.value);
        break;

      case symbol_kind::S_ke_kb: // ke_kb
        value.copy< Knowledge_Base > (that.value);
        break;

      case symbol_kind::S_math_equation: // math_equation
        value.copy< Math_Equation > (that.value);
        break;

      case symbol_kind::S_math_expr: // math_expr
        value.copy< Math_Expr > (that.value);
        break;

      case symbol_kind::S_math_func: // math_func
        value.copy< Math_Func > (that.value);
        break;

      case symbol_kind::S_math_individual: // math_individual
        value.copy< Math_Individual > (that.value);
        break;

      case symbol_kind::S_number: // number
        value.copy< Number > (that.value);
        break;

      case symbol_kind::S_ke_question: // ke_question
        value.copy< Question > (that.value);
        break;

      case symbol_kind::S_ke_rule: // ke_rule
        value.copy< Rule > (that.value);
        break;

      case symbol_kind::S_sugar_for_and: // sugar_for_and
        value.copy< Sugar_For_And > (that.value);
        break;

      case symbol_kind::S_sugar_for_ctor: // sugar_for_ctor
        value.copy< Sugar_For_Ctor > (that.value);
        break;

      case symbol_kind::S_sugar_for_oprt_apply: // sugar_for_oprt_apply
        value.copy< Sugar_For_Oprt_Apply > (that.value);
        break;

      case symbol_kind::S_sugar_for_pred: // sugar_for_pred
        value.copy< Sugar_For_Pred > (that.value);
        break;

      case symbol_kind::S_term: // term
        value.copy< Term > (that.value);
        break;

      case symbol_kind::S_variable: // variable
        value.copy< Variable > (that.value);
        break;

      case symbol_kind::S_BOOL: // BOOL
        value.copy< bool > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_DESCRIPTION: // DESCRIPTION
      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_OPERATOR: // OPERATOR
      case symbol_kind::S_CONCEPT: // CONCEPT
      case symbol_kind::S_DEF_CPT: // DEF_CPT
      case symbol_kind::S_DEF_INDI: // DEF_INDI
      case symbol_kind::S_DEF_OP: // DEF_OP
      case symbol_kind::S_KB_ASSERT: // KB_ASSERT
      case symbol_kind::S_KB_MODIFY: // KB_MODIFY
      case symbol_kind::S_KB_RETRACT: // KB_RETRACT
      case symbol_kind::S_MATH_FUNC_NAME: // MATH_FUNC_NAME
      case symbol_kind::S_predicate_operator: // predicate_operator
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_assignment_list: // assignment_list
        value.copy< vector<shared_ptr<Assignment>> > (that.value);
        break;

      case symbol_kind::S_def_concept_list: // def_concept_list
        value.copy< vector<shared_ptr<Def_Concept>> > (that.value);
        break;

      case symbol_kind::S_def_individual_list: // def_individual_list
        value.copy< vector<shared_ptr<Def_Individual>> > (that.value);
        break;

      case symbol_kind::S_def_operator_list: // def_operator_list
        value.copy< vector<shared_ptr<Def_Operator>> > (that.value);
        break;

      case symbol_kind::S_fact_list: // fact_list
        value.copy< vector<shared_ptr<Fact>> > (that.value);
        break;

      case symbol_kind::S_individual_list: // individual_list
      case symbol_kind::S_individual_and_list: // individual_and_list
      case symbol_kind::S_to_solve: // to_solve
        value.copy< vector<shared_ptr<Individual>> > (that.value);
        break;

      case symbol_kind::S_math_expr_list: // math_expr_list
        value.copy< vector<shared_ptr<Math_Expr>> > (that.value);
        break;

      case symbol_kind::S_ke_questions: // ke_questions
        value.copy< vector<shared_ptr<Question>> > (that.value);
        break;

      case symbol_kind::S_ke_rule_list: // ke_rule_list
        value.copy< vector<shared_ptr<Rule>> > (that.value);
        break;

      case symbol_kind::S_variable_list: // variable_list
        value.copy< vector<shared_ptr<Variable>> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_assertion: // assertion
        value.move< Assertion > (that.value);
        break;

      case symbol_kind::S_assignment: // assignment
        value.move< Assignment > (that.value);
        break;

      case symbol_kind::S_concept: // concept
        value.move< Concept > (that.value);
        break;

      case symbol_kind::S_coordinate: // coordinate
        value.move< Coordinate > (that.value);
        break;

      case symbol_kind::S_cud: // cud
        value.move< Cud > (that.value);
        break;

      case symbol_kind::S_def_concept: // def_concept
        value.move< Def_Concept > (that.value);
        break;

      case symbol_kind::S_def_individual: // def_individual
        value.move< Def_Individual > (that.value);
        break;

      case symbol_kind::S_def_operator: // def_operator
        value.move< Def_Operator > (that.value);
        break;

      case symbol_kind::S_fact: // fact
        value.move< Fact > (that.value);
        break;

      case symbol_kind::S_individual: // individual
        value.move< Individual > (that.value);
        break;

      case symbol_kind::S_ke_kb: // ke_kb
        value.move< Knowledge_Base > (that.value);
        break;

      case symbol_kind::S_math_equation: // math_equation
        value.move< Math_Equation > (that.value);
        break;

      case symbol_kind::S_math_expr: // math_expr
        value.move< Math_Expr > (that.value);
        break;

      case symbol_kind::S_math_func: // math_func
        value.move< Math_Func > (that.value);
        break;

      case symbol_kind::S_math_individual: // math_individual
        value.move< Math_Individual > (that.value);
        break;

      case symbol_kind::S_number: // number
        value.move< Number > (that.value);
        break;

      case symbol_kind::S_ke_question: // ke_question
        value.move< Question > (that.value);
        break;

      case symbol_kind::S_ke_rule: // ke_rule
        value.move< Rule > (that.value);
        break;

      case symbol_kind::S_sugar_for_and: // sugar_for_and
        value.move< Sugar_For_And > (that.value);
        break;

      case symbol_kind::S_sugar_for_ctor: // sugar_for_ctor
        value.move< Sugar_For_Ctor > (that.value);
        break;

      case symbol_kind::S_sugar_for_oprt_apply: // sugar_for_oprt_apply
        value.move< Sugar_For_Oprt_Apply > (that.value);
        break;

      case symbol_kind::S_sugar_for_pred: // sugar_for_pred
        value.move< Sugar_For_Pred > (that.value);
        break;

      case symbol_kind::S_term: // term
        value.move< Term > (that.value);
        break;

      case symbol_kind::S_variable: // variable
        value.move< Variable > (that.value);
        break;

      case symbol_kind::S_BOOL: // BOOL
        value.move< bool > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< double > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (that.value);
        break;

      case symbol_kind::S_DESCRIPTION: // DESCRIPTION
      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_OPERATOR: // OPERATOR
      case symbol_kind::S_CONCEPT: // CONCEPT
      case symbol_kind::S_DEF_CPT: // DEF_CPT
      case symbol_kind::S_DEF_INDI: // DEF_INDI
      case symbol_kind::S_DEF_OP: // DEF_OP
      case symbol_kind::S_KB_ASSERT: // KB_ASSERT
      case symbol_kind::S_KB_MODIFY: // KB_MODIFY
      case symbol_kind::S_KB_RETRACT: // KB_RETRACT
      case symbol_kind::S_MATH_FUNC_NAME: // MATH_FUNC_NAME
      case symbol_kind::S_predicate_operator: // predicate_operator
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_assignment_list: // assignment_list
        value.move< vector<shared_ptr<Assignment>> > (that.value);
        break;

      case symbol_kind::S_def_concept_list: // def_concept_list
        value.move< vector<shared_ptr<Def_Concept>> > (that.value);
        break;

      case symbol_kind::S_def_individual_list: // def_individual_list
        value.move< vector<shared_ptr<Def_Individual>> > (that.value);
        break;

      case symbol_kind::S_def_operator_list: // def_operator_list
        value.move< vector<shared_ptr<Def_Operator>> > (that.value);
        break;

      case symbol_kind::S_fact_list: // fact_list
        value.move< vector<shared_ptr<Fact>> > (that.value);
        break;

      case symbol_kind::S_individual_list: // individual_list
      case symbol_kind::S_individual_and_list: // individual_and_list
      case symbol_kind::S_to_solve: // to_solve
        value.move< vector<shared_ptr<Individual>> > (that.value);
        break;

      case symbol_kind::S_math_expr_list: // math_expr_list
        value.move< vector<shared_ptr<Math_Expr>> > (that.value);
        break;

      case symbol_kind::S_ke_questions: // ke_questions
        value.move< vector<shared_ptr<Question>> > (that.value);
        break;

      case symbol_kind::S_ke_rule_list: // ke_rule_list
        value.move< vector<shared_ptr<Rule>> > (that.value);
        break;

      case symbol_kind::S_variable_list: // variable_list
        value.move< vector<shared_ptr<Variable>> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        switch (yykind)
    {
      case symbol_kind::S_INTEGER: // INTEGER
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < int > (); }
#line 956 "parser.cc"
        break;

      case symbol_kind::S_FLOAT: // FLOAT
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < double > (); }
#line 962 "parser.cc"
        break;

      case symbol_kind::S_BOOL: // BOOL
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < bool > (); }
#line 968 "parser.cc"
        break;

      case symbol_kind::S_DESCRIPTION: // DESCRIPTION
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 974 "parser.cc"
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 980 "parser.cc"
        break;

      case symbol_kind::S_OPERATOR: // OPERATOR
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 986 "parser.cc"
        break;

      case symbol_kind::S_CONCEPT: // CONCEPT
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 992 "parser.cc"
        break;

      case symbol_kind::S_DEF_CPT: // DEF_CPT
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 998 "parser.cc"
        break;

      case symbol_kind::S_DEF_INDI: // DEF_INDI
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 1004 "parser.cc"
        break;

      case symbol_kind::S_DEF_OP: // DEF_OP
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 1010 "parser.cc"
        break;

      case symbol_kind::S_KB_ASSERT: // KB_ASSERT
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 1016 "parser.cc"
        break;

      case symbol_kind::S_KB_MODIFY: // KB_MODIFY
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 1022 "parser.cc"
        break;

      case symbol_kind::S_KB_RETRACT: // KB_RETRACT
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 1028 "parser.cc"
        break;

      case symbol_kind::S_MATH_FUNC_NAME: // MATH_FUNC_NAME
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 1034 "parser.cc"
        break;

      case symbol_kind::S_ke_kb: // ke_kb
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Knowledge_Base > (); }
#line 1040 "parser.cc"
        break;

      case symbol_kind::S_def_concept_list: // def_concept_list
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Def_Concept>> > (); }
#line 1046 "parser.cc"
        break;

      case symbol_kind::S_def_concept: // def_concept
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Def_Concept > (); }
#line 1052 "parser.cc"
        break;

      case symbol_kind::S_def_individual_list: // def_individual_list
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Def_Individual>> > (); }
#line 1058 "parser.cc"
        break;

      case symbol_kind::S_def_individual: // def_individual
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Def_Individual > (); }
#line 1064 "parser.cc"
        break;

      case symbol_kind::S_def_operator_list: // def_operator_list
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Def_Operator>> > (); }
#line 1070 "parser.cc"
        break;

      case symbol_kind::S_def_operator: // def_operator
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Def_Operator > (); }
#line 1076 "parser.cc"
        break;

      case symbol_kind::S_ke_rule_list: // ke_rule_list
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Rule>> > (); }
#line 1082 "parser.cc"
        break;

      case symbol_kind::S_ke_rule: // ke_rule
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Rule > (); }
#line 1088 "parser.cc"
        break;

      case symbol_kind::S_individual: // individual
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Individual > (); }
#line 1094 "parser.cc"
        break;

      case symbol_kind::S_term: // term
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Term > (); }
#line 1100 "parser.cc"
        break;

      case symbol_kind::S_assertion: // assertion
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Assertion > (); }
#line 1106 "parser.cc"
        break;

      case symbol_kind::S_sugar_for_and: // sugar_for_and
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Sugar_For_And > (); }
#line 1112 "parser.cc"
        break;

      case symbol_kind::S_sugar_for_pred: // sugar_for_pred
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Sugar_For_Pred > (); }
#line 1118 "parser.cc"
        break;

      case symbol_kind::S_sugar_for_ctor: // sugar_for_ctor
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Sugar_For_Ctor > (); }
#line 1124 "parser.cc"
        break;

      case symbol_kind::S_sugar_for_oprt_apply: // sugar_for_oprt_apply
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Sugar_For_Oprt_Apply > (); }
#line 1130 "parser.cc"
        break;

      case symbol_kind::S_cud: // cud
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Cud > (); }
#line 1136 "parser.cc"
        break;

      case symbol_kind::S_assignment_list: // assignment_list
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Assignment>> > (); }
#line 1142 "parser.cc"
        break;

      case symbol_kind::S_assignment: // assignment
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Assignment > (); }
#line 1148 "parser.cc"
        break;

      case symbol_kind::S_math_individual: // math_individual
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Math_Individual > (); }
#line 1154 "parser.cc"
        break;

      case symbol_kind::S_predicate_operator: // predicate_operator
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 1160 "parser.cc"
        break;

      case symbol_kind::S_individual_list: // individual_list
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Individual>> > (); }
#line 1166 "parser.cc"
        break;

      case symbol_kind::S_individual_and_list: // individual_and_list
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Individual>> > (); }
#line 1172 "parser.cc"
        break;

      case symbol_kind::S_concept: // concept
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Concept > (); }
#line 1178 "parser.cc"
        break;

      case symbol_kind::S_variable: // variable
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Variable > (); }
#line 1184 "parser.cc"
        break;

      case symbol_kind::S_variable_list: // variable_list
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Variable>> > (); }
#line 1190 "parser.cc"
        break;

      case symbol_kind::S_math_equation: // math_equation
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Math_Equation > (); }
#line 1196 "parser.cc"
        break;

      case symbol_kind::S_coordinate: // coordinate
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Coordinate > (); }
#line 1202 "parser.cc"
        break;

      case symbol_kind::S_math_expr: // math_expr
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Math_Expr > (); }
#line 1208 "parser.cc"
        break;

      case symbol_kind::S_math_func: // math_func
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Math_Func > (); }
#line 1214 "parser.cc"
        break;

      case symbol_kind::S_math_expr_list: // math_expr_list
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Math_Expr>> > (); }
#line 1220 "parser.cc"
        break;

      case symbol_kind::S_number: // number
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Number > (); }
#line 1226 "parser.cc"
        break;

      case symbol_kind::S_ke_questions: // ke_questions
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Question>> > (); }
#line 1232 "parser.cc"
        break;

      case symbol_kind::S_ke_question: // ke_question
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Question > (); }
#line 1238 "parser.cc"
        break;

      case symbol_kind::S_fact_list: // fact_list
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Fact>> > (); }
#line 1244 "parser.cc"
        break;

      case symbol_kind::S_fact: // fact
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < Fact > (); }
#line 1250 "parser.cc"
        break;

      case symbol_kind::S_to_solve: // to_solve
#line 101 "parser.yy"
                 { yyo << yysym.value.template as < vector<shared_ptr<Individual>> > (); }
#line 1256 "parser.cc"
        break;

      default:
        break;
    }
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // Discard the LAC context in case there still is one left from a
    // previous invocation.
    yy_lac_discard_ ("init");

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (drv));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        if (!yy_lac_establish_ (yyla.kind ()))
          goto yyerrlab;
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        if (!yy_lac_establish_ (yyla.kind ()))
          goto yyerrlab;

        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    yy_lac_discard_ ("shift");
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_assertion: // assertion
        yylhs.value.emplace< Assertion > ();
        break;

      case symbol_kind::S_assignment: // assignment
        yylhs.value.emplace< Assignment > ();
        break;

      case symbol_kind::S_concept: // concept
        yylhs.value.emplace< Concept > ();
        break;

      case symbol_kind::S_coordinate: // coordinate
        yylhs.value.emplace< Coordinate > ();
        break;

      case symbol_kind::S_cud: // cud
        yylhs.value.emplace< Cud > ();
        break;

      case symbol_kind::S_def_concept: // def_concept
        yylhs.value.emplace< Def_Concept > ();
        break;

      case symbol_kind::S_def_individual: // def_individual
        yylhs.value.emplace< Def_Individual > ();
        break;

      case symbol_kind::S_def_operator: // def_operator
        yylhs.value.emplace< Def_Operator > ();
        break;

      case symbol_kind::S_fact: // fact
        yylhs.value.emplace< Fact > ();
        break;

      case symbol_kind::S_individual: // individual
        yylhs.value.emplace< Individual > ();
        break;

      case symbol_kind::S_ke_kb: // ke_kb
        yylhs.value.emplace< Knowledge_Base > ();
        break;

      case symbol_kind::S_math_equation: // math_equation
        yylhs.value.emplace< Math_Equation > ();
        break;

      case symbol_kind::S_math_expr: // math_expr
        yylhs.value.emplace< Math_Expr > ();
        break;

      case symbol_kind::S_math_func: // math_func
        yylhs.value.emplace< Math_Func > ();
        break;

      case symbol_kind::S_math_individual: // math_individual
        yylhs.value.emplace< Math_Individual > ();
        break;

      case symbol_kind::S_number: // number
        yylhs.value.emplace< Number > ();
        break;

      case symbol_kind::S_ke_question: // ke_question
        yylhs.value.emplace< Question > ();
        break;

      case symbol_kind::S_ke_rule: // ke_rule
        yylhs.value.emplace< Rule > ();
        break;

      case symbol_kind::S_sugar_for_and: // sugar_for_and
        yylhs.value.emplace< Sugar_For_And > ();
        break;

      case symbol_kind::S_sugar_for_ctor: // sugar_for_ctor
        yylhs.value.emplace< Sugar_For_Ctor > ();
        break;

      case symbol_kind::S_sugar_for_oprt_apply: // sugar_for_oprt_apply
        yylhs.value.emplace< Sugar_For_Oprt_Apply > ();
        break;

      case symbol_kind::S_sugar_for_pred: // sugar_for_pred
        yylhs.value.emplace< Sugar_For_Pred > ();
        break;

      case symbol_kind::S_term: // term
        yylhs.value.emplace< Term > ();
        break;

      case symbol_kind::S_variable: // variable
        yylhs.value.emplace< Variable > ();
        break;

      case symbol_kind::S_BOOL: // BOOL
        yylhs.value.emplace< bool > ();
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_DESCRIPTION: // DESCRIPTION
      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_OPERATOR: // OPERATOR
      case symbol_kind::S_CONCEPT: // CONCEPT
      case symbol_kind::S_DEF_CPT: // DEF_CPT
      case symbol_kind::S_DEF_INDI: // DEF_INDI
      case symbol_kind::S_DEF_OP: // DEF_OP
      case symbol_kind::S_KB_ASSERT: // KB_ASSERT
      case symbol_kind::S_KB_MODIFY: // KB_MODIFY
      case symbol_kind::S_KB_RETRACT: // KB_RETRACT
      case symbol_kind::S_MATH_FUNC_NAME: // MATH_FUNC_NAME
      case symbol_kind::S_predicate_operator: // predicate_operator
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_assignment_list: // assignment_list
        yylhs.value.emplace< vector<shared_ptr<Assignment>> > ();
        break;

      case symbol_kind::S_def_concept_list: // def_concept_list
        yylhs.value.emplace< vector<shared_ptr<Def_Concept>> > ();
        break;

      case symbol_kind::S_def_individual_list: // def_individual_list
        yylhs.value.emplace< vector<shared_ptr<Def_Individual>> > ();
        break;

      case symbol_kind::S_def_operator_list: // def_operator_list
        yylhs.value.emplace< vector<shared_ptr<Def_Operator>> > ();
        break;

      case symbol_kind::S_fact_list: // fact_list
        yylhs.value.emplace< vector<shared_ptr<Fact>> > ();
        break;

      case symbol_kind::S_individual_list: // individual_list
      case symbol_kind::S_individual_and_list: // individual_and_list
      case symbol_kind::S_to_solve: // to_solve
        yylhs.value.emplace< vector<shared_ptr<Individual>> > ();
        break;

      case symbol_kind::S_math_expr_list: // math_expr_list
        yylhs.value.emplace< vector<shared_ptr<Math_Expr>> > ();
        break;

      case symbol_kind::S_ke_questions: // ke_questions
        yylhs.value.emplace< vector<shared_ptr<Question>> > ();
        break;

      case symbol_kind::S_ke_rule_list: // ke_rule_list
        yylhs.value.emplace< vector<shared_ptr<Rule>> > ();
        break;

      case symbol_kind::S_variable_list: // variable_list
        yylhs.value.emplace< vector<shared_ptr<Variable>> > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // statement: ke_kb
#line 104 "parser.yy"
                  {drv.kb = make_shared<Knowledge_Base>(yystack_[0].value.as < Knowledge_Base > ());}
#line 1684 "parser.cc"
    break;

  case 3: // statement: ke_questions
#line 105 "parser.yy"
                         {drv.questions = yystack_[0].value.as < vector<shared_ptr<Question>> > ();}
#line 1690 "parser.cc"
    break;

  case 4: // ke_kb: def_concept_list def_individual_list def_operator_list ke_rule_list
#line 108 "parser.yy"
                                                                            {Knowledge_Base kb(yystack_[3].value.as < vector<shared_ptr<Def_Concept>> > (), yystack_[2].value.as < vector<shared_ptr<Def_Individual>> > (), yystack_[1].value.as < vector<shared_ptr<Def_Operator>> > (), yystack_[0].value.as < vector<shared_ptr<Rule>> > ()); yylhs.value.as < Knowledge_Base > ()=kb;}
#line 1696 "parser.cc"
    break;

  case 5: // def_concept_list: def_concept
#line 110 "parser.yy"
                               {vector<shared_ptr<Def_Concept>> v; v.push_back(make_shared<Def_Concept>(yystack_[0].value.as < Def_Concept > ())); yylhs.value.as < vector<shared_ptr<Def_Concept>> > ()=v;}
#line 1702 "parser.cc"
    break;

  case 6: // def_concept_list: def_concept_list def_concept
#line 111 "parser.yy"
                                                {yystack_[1].value.as < vector<shared_ptr<Def_Concept>> > ().push_back(make_shared<Def_Concept>(yystack_[0].value.as < Def_Concept > ()));yylhs.value.as < vector<shared_ptr<Def_Concept>> > ()=yystack_[1].value.as < vector<shared_ptr<Def_Concept>> > ();}
#line 1708 "parser.cc"
    break;

  case 7: // def_concept: DEF_CPT concept "=" "{" variable_list "}"
#line 114 "parser.yy"
                                                        {yylhs.value.as < Def_Concept > ()=Def_Concept(yystack_[4].value.as < Concept > (),yystack_[1].value.as < vector<shared_ptr<Variable>> > ());}
#line 1714 "parser.cc"
    break;

  case 8: // def_concept: DEF_CPT concept "<-" concept "=" "{" variable_list "}"
#line 115 "parser.yy"
                                                                     {yylhs.value.as < Def_Concept > ()=Def_Concept(yystack_[6].value.as < Concept > (),yystack_[4].value.as < Concept > (),yystack_[1].value.as < vector<shared_ptr<Variable>> > ());}
#line 1720 "parser.cc"
    break;

  case 9: // def_concept: DEF_CPT concept "<-" concept
#line 116 "parser.yy"
                                           {yylhs.value.as < Def_Concept > ()=Def_Concept(yystack_[2].value.as < Concept > (),yystack_[0].value.as < Concept > ());}
#line 1726 "parser.cc"
    break;

  case 10: // def_individual_list: def_individual
#line 119 "parser.yy"
                                     {vector<shared_ptr<Def_Individual>> v; v.push_back(make_shared<Def_Individual>(yystack_[0].value.as < Def_Individual > ())); yylhs.value.as < vector<shared_ptr<Def_Individual>> > ()=v;}
#line 1732 "parser.cc"
    break;

  case 11: // def_individual_list: def_individual_list def_individual
#line 120 "parser.yy"
                                                         {yystack_[1].value.as < vector<shared_ptr<Def_Individual>> > ().push_back(make_shared<Def_Individual>(yystack_[0].value.as < Def_Individual > ()));yylhs.value.as < vector<shared_ptr<Def_Individual>> > ()=yystack_[1].value.as < vector<shared_ptr<Def_Individual>> > ();}
#line 1738 "parser.cc"
    break;

  case 12: // def_individual: DEF_INDI SYMBOL ":" concept "=" individual
#line 123 "parser.yy"
                                                            {yylhs.value.as < Def_Individual > ()=Def_Individual(yystack_[4].value.as < std::string > (),yystack_[2].value.as < Concept > (),yystack_[0].value.as < Individual > ());}
#line 1744 "parser.cc"
    break;

  case 13: // def_operator_list: def_operator
#line 125 "parser.yy"
                                 {vector<shared_ptr<Def_Operator>> v; v.push_back(make_shared<Def_Operator>(yystack_[0].value.as < Def_Operator > ())); yylhs.value.as < vector<shared_ptr<Def_Operator>> > ()=v;}
#line 1750 "parser.cc"
    break;

  case 14: // def_operator_list: def_operator_list def_operator
#line 126 "parser.yy"
                                                   {yystack_[1].value.as < vector<shared_ptr<Def_Operator>> > ().push_back(make_shared<Def_Operator>(yystack_[0].value.as < Def_Operator > ()));yylhs.value.as < vector<shared_ptr<Def_Operator>> > ()=yystack_[1].value.as < vector<shared_ptr<Def_Operator>> > ();}
#line 1756 "parser.cc"
    break;

  case 15: // def_operator: DEF_OP OPERATOR "(" variable_list ")" "=" individual
#line 129 "parser.yy"
                                                                    {yylhs.value.as < Def_Operator > ()=Def_Operator(yystack_[5].value.as < std::string > (),yystack_[3].value.as < vector<shared_ptr<Variable>> > (),yystack_[0].value.as < Individual > ());}
#line 1762 "parser.cc"
    break;

  case 16: // ke_rule_list: ke_rule
#line 132 "parser.yy"
                       {vector<shared_ptr<Rule>> v; v.push_back(make_shared<Rule>(yystack_[0].value.as < Rule > ())); yylhs.value.as < vector<shared_ptr<Rule>> > ()=v;}
#line 1768 "parser.cc"
    break;

  case 17: // ke_rule_list: ke_rule_list ke_rule
#line 133 "parser.yy"
                                    {yystack_[1].value.as < vector<shared_ptr<Rule>> > ().push_back(make_shared<Rule>(yystack_[0].value.as < Rule > ()));yylhs.value.as < vector<shared_ptr<Rule>> > ()=yystack_[1].value.as < vector<shared_ptr<Rule>> > ();}
#line 1774 "parser.cc"
    break;

  case 18: // ke_rule: individual "=>" individual DESCRIPTION
#line 136 "parser.yy"
                                                 {Rule rule(yystack_[3].value.as < Individual > (),yystack_[1].value.as < Individual > (),yystack_[0].value.as < std::string > ()); yylhs.value.as < Rule > ()=rule;}
#line 1780 "parser.cc"
    break;

  case 19: // individual: variable
#line 139 "parser.yy"
                       {yylhs.value.as < Individual > ()=Individual(yystack_[0].value.as < Variable > ());}
#line 1786 "parser.cc"
    break;

  case 20: // individual: BOOL
#line 140 "parser.yy"
                       {yylhs.value.as < Individual > ()=Individual(yystack_[0].value.as < bool > ());}
#line 1792 "parser.cc"
    break;

  case 21: // individual: cud
#line 141 "parser.yy"
                       {yylhs.value.as < Individual > ()=Individual(yystack_[0].value.as < Cud > ());}
#line 1798 "parser.cc"
    break;

  case 22: // individual: term
#line 142 "parser.yy"
                       {yylhs.value.as < Individual > ()=Individual(yystack_[0].value.as < Term > ());}
#line 1804 "parser.cc"
    break;

  case 23: // individual: assertion
#line 143 "parser.yy"
                       {yylhs.value.as < Individual > ()=Individual(yystack_[0].value.as < Assertion > ());}
#line 1810 "parser.cc"
    break;

  case 24: // individual: math_individual
#line 144 "parser.yy"
                             {yylhs.value.as < Individual > ()=Individual(yystack_[0].value.as < Math_Individual > ());}
#line 1816 "parser.cc"
    break;

  case 25: // term: OPERATOR "(" individual_list ")"
#line 147 "parser.yy"
                                        {yylhs.value.as < Term > ()=Term(yystack_[3].value.as < std::string > (),yystack_[1].value.as < vector<shared_ptr<Individual>> > ());}
#line 1822 "parser.cc"
    break;

  case 26: // term: sugar_for_and
#line 148 "parser.yy"
                      {yylhs.value.as < Term > ()=Term(yystack_[0].value.as < Sugar_For_And > ());}
#line 1828 "parser.cc"
    break;

  case 27: // term: sugar_for_pred
#line 149 "parser.yy"
                      {yylhs.value.as < Term > ()=Term(yystack_[0].value.as < Sugar_For_Pred > ());}
#line 1834 "parser.cc"
    break;

  case 28: // term: sugar_for_ctor
#line 150 "parser.yy"
                      {yylhs.value.as < Term > ()=Term(yystack_[0].value.as < Sugar_For_Ctor > ());}
#line 1840 "parser.cc"
    break;

  case 29: // term: sugar_for_oprt_apply
#line 151 "parser.yy"
                            {yylhs.value.as < Term > ()=Term(yystack_[0].value.as < Sugar_For_Oprt_Apply > ());}
#line 1846 "parser.cc"
    break;

  case 30: // assertion: "{" individual "}"
#line 154 "parser.yy"
                               {yylhs.value.as < Assertion > ()=Assertion(yystack_[1].value.as < Individual > ());}
#line 1852 "parser.cc"
    break;

  case 31: // assertion: "{" individual "=" individual "}"
#line 155 "parser.yy"
                                              {yylhs.value.as < Assertion > ()=Assertion(yystack_[3].value.as < Individual > (),yystack_[1].value.as < Individual > ());}
#line 1858 "parser.cc"
    break;

  case 32: // sugar_for_and: "{" individual_and_list ";" individual "}"
#line 158 "parser.yy"
                                                           {yystack_[3].value.as < vector<shared_ptr<Individual>> > ().push_back(make_shared<Individual>(yystack_[1].value.as < Individual > ())); yylhs.value.as < Sugar_For_And > ()=Sugar_For_And(yystack_[3].value.as < vector<shared_ptr<Individual>> > ());}
#line 1864 "parser.cc"
    break;

  case 33: // sugar_for_pred: "{" individual predicate_operator individual "}"
#line 160 "parser.yy"
                                                                  {yylhs.value.as < Sugar_For_Pred > ()=Sugar_For_Pred(yystack_[3].value.as < Individual > (),yystack_[2].value.as < std::string > (),yystack_[1].value.as < Individual > ());}
#line 1870 "parser.cc"
    break;

  case 34: // sugar_for_ctor: "{" assignment_list "}"
#line 162 "parser.yy"
                                         {yylhs.value.as < Sugar_For_Ctor > ()=Sugar_For_Ctor(yystack_[1].value.as < vector<shared_ptr<Assignment>> > ());}
#line 1876 "parser.cc"
    break;

  case 35: // sugar_for_oprt_apply: SYMBOL "." SYMBOL
#line 164 "parser.yy"
                                         {yylhs.value.as < Sugar_For_Oprt_Apply > ()=Sugar_For_Oprt_Apply(yystack_[2].value.as < std::string > (),yystack_[0].value.as < std::string > ());}
#line 1882 "parser.cc"
    break;

  case 36: // cud: KB_ASSERT individual "=" individual
#line 166 "parser.yy"
                                           {yylhs.value.as < Cud > ()=Cud(yystack_[3].value.as < std::string > (),yystack_[2].value.as < Individual > (),yystack_[0].value.as < Individual > ());}
#line 1888 "parser.cc"
    break;

  case 37: // cud: KB_MODIFY individual "=" individual
#line 167 "parser.yy"
                                           {yylhs.value.as < Cud > ()=Cud(yystack_[3].value.as < std::string > (),yystack_[2].value.as < Individual > (),yystack_[0].value.as < Individual > ());}
#line 1894 "parser.cc"
    break;

  case 38: // cud: KB_RETRACT individual "=" individual
#line 168 "parser.yy"
                                           {yylhs.value.as < Cud > ()=Cud(yystack_[3].value.as < std::string > (),yystack_[2].value.as < Individual > (),yystack_[0].value.as < Individual > ());}
#line 1900 "parser.cc"
    break;

  case 39: // assignment_list: assignment
#line 171 "parser.yy"
                             {vector<shared_ptr<Assignment>> v; v.push_back(make_shared<Assignment>(yystack_[0].value.as < Assignment > ())); yylhs.value.as < vector<shared_ptr<Assignment>> > ()=v;}
#line 1906 "parser.cc"
    break;

  case 40: // assignment_list: assignment_list ";" assignment
#line 172 "parser.yy"
                                                 {yystack_[2].value.as < vector<shared_ptr<Assignment>> > ().push_back(make_shared<Assignment>(yystack_[0].value.as < Assignment > ()));yylhs.value.as < vector<shared_ptr<Assignment>> > ()=yystack_[2].value.as < vector<shared_ptr<Assignment>> > ();}
#line 1912 "parser.cc"
    break;

  case 41: // assignment: SYMBOL ":=" individual
#line 175 "parser.yy"
                                    {yylhs.value.as < Assignment > ()=Assignment(yystack_[2].value.as < std::string > (),yystack_[0].value.as < Individual > ());}
#line 1918 "parser.cc"
    break;

  case 42: // math_individual: math_expr
#line 177 "parser.yy"
                                {yylhs.value.as < Math_Individual > ()=Math_Individual(yystack_[0].value.as < Math_Expr > ());}
#line 1924 "parser.cc"
    break;

  case 43: // math_individual: math_equation
#line 178 "parser.yy"
                                {yylhs.value.as < Math_Individual > ()=Math_Individual(yystack_[0].value.as < Math_Equation > ());}
#line 1930 "parser.cc"
    break;

  case 44: // math_individual: coordinate
#line 179 "parser.yy"
                                {yylhs.value.as < Math_Individual > ()=Math_Individual(yystack_[0].value.as < Coordinate > ());}
#line 1936 "parser.cc"
    break;

  case 45: // predicate_operator: ">="
#line 184 "parser.yy"
                          {yylhs.value.as < std::string > ()=std::string(">=");}
#line 1942 "parser.cc"
    break;

  case 46: // predicate_operator: "<="
#line 185 "parser.yy"
                          {yylhs.value.as < std::string > ()=std::string("<=");}
#line 1948 "parser.cc"
    break;

  case 47: // predicate_operator: "!="
#line 186 "parser.yy"
                          {yylhs.value.as < std::string > ()=std::string("!=");}
#line 1954 "parser.cc"
    break;

  case 48: // predicate_operator: ">"
#line 187 "parser.yy"
                          {yylhs.value.as < std::string > ()=std::string(">");}
#line 1960 "parser.cc"
    break;

  case 49: // predicate_operator: "<"
#line 188 "parser.yy"
                          {yylhs.value.as < std::string > ()=std::string("<");}
#line 1966 "parser.cc"
    break;

  case 50: // individual_list: individual
#line 191 "parser.yy"
                             {vector<shared_ptr<Individual>> v; v.push_back(make_shared<Individual>(yystack_[0].value.as < Individual > ())); yylhs.value.as < vector<shared_ptr<Individual>> > ()=v;}
#line 1972 "parser.cc"
    break;

  case 51: // individual_list: individual_list "," individual
#line 192 "parser.yy"
                                                 {yystack_[2].value.as < vector<shared_ptr<Individual>> > ().push_back(make_shared<Individual>(yystack_[0].value.as < Individual > ()));yylhs.value.as < vector<shared_ptr<Individual>> > ()=yystack_[2].value.as < vector<shared_ptr<Individual>> > ();}
#line 1978 "parser.cc"
    break;

  case 52: // individual_and_list: individual
#line 195 "parser.yy"
                                 {vector<shared_ptr<Individual>> v; v.push_back(make_shared<Individual>(yystack_[0].value.as < Individual > ())); yylhs.value.as < vector<shared_ptr<Individual>> > ()=v;}
#line 1984 "parser.cc"
    break;

  case 53: // individual_and_list: individual_and_list ";" individual
#line 196 "parser.yy"
                                                         {yystack_[2].value.as < vector<shared_ptr<Individual>> > ().push_back(make_shared<Individual>(yystack_[0].value.as < Individual > ()));yylhs.value.as < vector<shared_ptr<Individual>> > ()=yystack_[2].value.as < vector<shared_ptr<Individual>> > ();}
#line 1990 "parser.cc"
    break;

  case 54: // concept: CONCEPT
#line 199 "parser.yy"
                  {yylhs.value.as < Concept > ()=Concept(yystack_[0].value.as < std::string > ());}
#line 1996 "parser.cc"
    break;

  case 55: // concept: OPERATOR "(" concept ")"
#line 200 "parser.yy"
                                   {yylhs.value.as < Concept > ()=Concept(yystack_[3].value.as < std::string > (),yystack_[1].value.as < Concept > ());}
#line 2002 "parser.cc"
    break;

  case 56: // variable: SYMBOL ":" concept
#line 203 "parser.yy"
                              {yylhs.value.as < Variable > ()=Variable(yystack_[2].value.as < std::string > (),yystack_[0].value.as < Concept > ());}
#line 2008 "parser.cc"
    break;

  case 57: // variable_list: variable
#line 205 "parser.yy"
                         {vector<shared_ptr<Variable>> v; v.push_back(make_shared<Variable>(yystack_[0].value.as < Variable > ())); yylhs.value.as < vector<shared_ptr<Variable>> > ()=v;}
#line 2014 "parser.cc"
    break;

  case 58: // variable_list: variable_list ";" variable
#line 206 "parser.yy"
                                           {yystack_[2].value.as < vector<shared_ptr<Variable>> > ().push_back(make_shared<Variable>(yystack_[0].value.as < Variable > ()));yylhs.value.as < vector<shared_ptr<Variable>> > ()=yystack_[2].value.as < vector<shared_ptr<Variable>> > ();}
#line 2020 "parser.cc"
    break;

  case 59: // math_equation: math_expr "==" math_expr
#line 209 "parser.yy"
                                         {yylhs.value.as < Math_Equation > ()=Math_Equation(yystack_[2].value.as < Math_Expr > (),yystack_[0].value.as < Math_Expr > ());}
#line 2026 "parser.cc"
    break;

  case 60: // coordinate: "(" math_expr "," math_expr ")"
#line 210 "parser.yy"
                                                {yylhs.value.as < Coordinate > ()=Coordinate(yystack_[3].value.as < Math_Expr > (),yystack_[1].value.as < Math_Expr > ());}
#line 2032 "parser.cc"
    break;

  case 61: // math_expr: number
#line 217 "parser.yy"
                          {yylhs.value.as < Math_Expr > ()=Math_Expr(yystack_[0].value.as < Number > ());}
#line 2038 "parser.cc"
    break;

  case 62: // math_expr: SYMBOL
#line 218 "parser.yy"
                          {yylhs.value.as < Math_Expr > ()=Math_Expr(yystack_[0].value.as < std::string > ());}
#line 2044 "parser.cc"
    break;

  case 63: // math_expr: math_expr "+" math_expr
#line 219 "parser.yy"
                                           {yylhs.value.as < Math_Expr > ()=Math_Expr(yystack_[2].value.as < Math_Expr > (),'+',yystack_[0].value.as < Math_Expr > ());}
#line 2050 "parser.cc"
    break;

  case 64: // math_expr: math_expr "-" math_expr
#line 220 "parser.yy"
                                           {yylhs.value.as < Math_Expr > ()=Math_Expr(yystack_[2].value.as < Math_Expr > (),'-',yystack_[0].value.as < Math_Expr > ());}
#line 2056 "parser.cc"
    break;

  case 65: // math_expr: math_expr "*" math_expr
#line 221 "parser.yy"
                                           {yylhs.value.as < Math_Expr > ()=Math_Expr(yystack_[2].value.as < Math_Expr > (),'*',yystack_[0].value.as < Math_Expr > ());}
#line 2062 "parser.cc"
    break;

  case 66: // math_expr: math_expr "/" math_expr
#line 222 "parser.yy"
                                           {yylhs.value.as < Math_Expr > ()=Math_Expr(yystack_[2].value.as < Math_Expr > (),'/',yystack_[0].value.as < Math_Expr > ());}
#line 2068 "parser.cc"
    break;

  case 67: // math_expr: math_expr "^" math_expr
#line 223 "parser.yy"
                                           {yylhs.value.as < Math_Expr > ()=Math_Expr(yystack_[2].value.as < Math_Expr > (),'^',yystack_[0].value.as < Math_Expr > ());}
#line 2074 "parser.cc"
    break;

  case 68: // math_expr: "(" math_expr ")"
#line 224 "parser.yy"
                                     {yylhs.value.as < Math_Expr > ()=Math_Expr(make_shared<Math_Expr>(yystack_[1].value.as < Math_Expr > ()));}
#line 2080 "parser.cc"
    break;

  case 69: // math_expr: math_func
#line 225 "parser.yy"
                             {yylhs.value.as < Math_Expr > ()=Math_Expr(yystack_[0].value.as < Math_Func > ());}
#line 2086 "parser.cc"
    break;

  case 70: // math_func: MATH_FUNC_NAME "(" math_expr_list ")"
#line 228 "parser.yy"
                                                         {yylhs.value.as < Math_Func > ()=Math_Func(yystack_[3].value.as < std::string > (),yystack_[1].value.as < vector<shared_ptr<Math_Expr>> > ());}
#line 2092 "parser.cc"
    break;

  case 71: // math_expr_list: math_expr
#line 230 "parser.yy"
                             {vector<shared_ptr<Math_Expr>> v;v.push_back(make_shared<Math_Expr>(yystack_[0].value.as < Math_Expr > ()));yylhs.value.as < vector<shared_ptr<Math_Expr>> > ()=v;}
#line 2098 "parser.cc"
    break;

  case 72: // math_expr_list: math_expr_list "," math_expr
#line 231 "parser.yy"
                                                {yystack_[2].value.as < vector<shared_ptr<Math_Expr>> > ().push_back(make_shared<Math_Expr>(yystack_[0].value.as < Math_Expr > ()));yylhs.value.as < vector<shared_ptr<Math_Expr>> > ()=yystack_[2].value.as < vector<shared_ptr<Math_Expr>> > ();}
#line 2104 "parser.cc"
    break;

  case 73: // number: INTEGER
#line 234 "parser.yy"
                 {yylhs.value.as < Number > ()=Number(yystack_[0].value.as < int > ());}
#line 2110 "parser.cc"
    break;

  case 74: // number: FLOAT
#line 235 "parser.yy"
                 {yylhs.value.as < Number > ()=Number(yystack_[0].value.as < double > ());}
#line 2116 "parser.cc"
    break;

  case 75: // ke_questions: ke_question
#line 238 "parser.yy"
                           {vector<shared_ptr<Question>> v; v.push_back(make_shared<Question>(yystack_[0].value.as < Question > ())); yylhs.value.as < vector<shared_ptr<Question>> > ()=v;}
#line 2122 "parser.cc"
    break;

  case 76: // ke_questions: ke_questions ke_question
#line 239 "parser.yy"
                                        {yystack_[1].value.as < vector<shared_ptr<Question>> > ().push_back(make_shared<Question>(yystack_[0].value.as < Question > ()));yylhs.value.as < vector<shared_ptr<Question>> > ()=yystack_[1].value.as < vector<shared_ptr<Question>> > ();}
#line 2128 "parser.cc"
    break;

  case 77: // ke_question: DESCRIPTION fact_list to_solve
#line 242 "parser.yy"
                                             {yylhs.value.as < Question > ()=Question(yystack_[2].value.as < std::string > (),yystack_[1].value.as < vector<shared_ptr<Fact>> > (),yystack_[0].value.as < vector<shared_ptr<Individual>> > ());}
#line 2134 "parser.cc"
    break;

  case 78: // fact_list: fact
#line 244 "parser.yy"
                 {vector<shared_ptr<Fact>> v; v.push_back(make_shared<Fact>(yystack_[0].value.as < Fact > ())); yylhs.value.as < vector<shared_ptr<Fact>> > ()=v;}
#line 2140 "parser.cc"
    break;

  case 79: // fact_list: fact_list ";" fact
#line 245 "parser.yy"
                               {yystack_[2].value.as < vector<shared_ptr<Fact>> > ().push_back(make_shared<Fact>(yystack_[0].value.as < Fact > ()));yylhs.value.as < vector<shared_ptr<Fact>> > ()=yystack_[2].value.as < vector<shared_ptr<Fact>> > ();}
#line 2146 "parser.cc"
    break;

  case 80: // fact: assertion
#line 248 "parser.yy"
                 {yylhs.value.as < Fact > ()=Fact(yystack_[0].value.as < Assertion > ());}
#line 2152 "parser.cc"
    break;

  case 81: // fact: sugar_for_pred
#line 249 "parser.yy"
                      {yylhs.value.as < Fact > ()=Fact(yystack_[0].value.as < Sugar_For_Pred > ());}
#line 2158 "parser.cc"
    break;

  case 82: // fact: variable
#line 250 "parser.yy"
                 {yylhs.value.as < Fact > ()=Fact(yystack_[0].value.as < Variable > ());}
#line 2164 "parser.cc"
    break;

  case 83: // fact: def_individual
#line 251 "parser.yy"
                      {yylhs.value.as < Fact > ()=Fact(yystack_[0].value.as < Def_Individual > ());}
#line 2170 "parser.cc"
    break;

  case 84: // to_solve: OPERATOR "(" individual_list ")"
#line 254 "parser.yy"
                                            {assert(yystack_[3].value.as < std::string > ()==std::string("To_Solve"));yylhs.value.as < vector<shared_ptr<Individual>> > ()=yystack_[1].value.as < vector<shared_ptr<Individual>> > ();}
#line 2176 "parser.cc"
    break;


#line 2180 "parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      yy_lac_discard_ ("error recovery");
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", ".", "+", "-", "*", "/", "^",
  "(", ")", "{", "}", ",", ";", ":", "=", ":=", "==", ">=", "<=", "!=",
  ">", "<", "=>", "<-", "INTEGER", "FLOAT", "BOOL", "DESCRIPTION",
  "SYMBOL", "OPERATOR", "CONCEPT", "DEF_CPT", "DEF_INDI", "DEF_OP",
  "KB_ASSERT", "KB_MODIFY", "KB_RETRACT", "MATH_FUNC_NAME", "$accept",
  "statement", "ke_kb", "def_concept_list", "def_concept",
  "def_individual_list", "def_individual", "def_operator_list",
  "def_operator", "ke_rule_list", "ke_rule", "individual", "term",
  "assertion", "sugar_for_and", "sugar_for_pred", "sugar_for_ctor",
  "sugar_for_oprt_apply", "cud", "assignment_list", "assignment",
  "math_individual", "predicate_operator", "individual_list",
  "individual_and_list", "concept", "variable", "variable_list",
  "math_equation", "coordinate", "math_expr", "math_func",
  "math_expr_list", "number", "ke_questions", "ke_question", "fact_list",
  "fact", "to_solve", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

#if YYDEBUG
    // Execute LAC once. We don't care if it is successful, we
    // only do it for the sake of debugging output.
    if (!yyparser_.yy_lac_established_)
      yyparser_.yy_lac_check_ (yyla_.kind ());
#endif

    for (int yyx = 0; yyx < YYNTOKENS; ++yyx)
      {
        symbol_kind_type yysym = YY_CAST (symbol_kind_type, yyx);
        if (yysym != symbol_kind::S_YYerror
            && yysym != symbol_kind::S_YYUNDEF
            && yyparser_.yy_lac_check_ (yysym))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
      }
    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }




  bool
  parser::yy_lac_check_ (symbol_kind_type yytoken) const
  {
    // Logically, the yylac_stack's lifetime is confined to this function.
    // Clear it, to get rid of potential left-overs from previous call.
    yylac_stack_.clear ();
    // Reduce until we encounter a shift and thereby accept the token.
#if YYDEBUG
    YYCDEBUG << "LAC: checking lookahead " << symbol_name (yytoken) << ':';
#endif
    std::ptrdiff_t lac_top = 0;
    while (true)
      {
        state_type top_state = (yylac_stack_.empty ()
                                ? yystack_[lac_top].state
                                : yylac_stack_.back ());
        int yyrule = yypact_[+top_state];
        if (yy_pact_value_is_default_ (yyrule)
            || (yyrule += yytoken) < 0 || yylast_ < yyrule
            || yycheck_[yyrule] != yytoken)
          {
            // Use the default action.
            yyrule = yydefact_[+top_state];
            if (yyrule == 0)
              {
                YYCDEBUG << " Err\n";
                return false;
              }
          }
        else
          {
            // Use the action from yytable.
            yyrule = yytable_[yyrule];
            if (yy_table_value_is_error_ (yyrule))
              {
                YYCDEBUG << " Err\n";
                return false;
              }
            if (0 < yyrule)
              {
                YYCDEBUG << " S" << yyrule << '\n';
                return true;
              }
            yyrule = -yyrule;
          }
        // By now we know we have to simulate a reduce.
        YYCDEBUG << " R" << yyrule - 1;
        // Pop the corresponding number of values from the stack.
        {
          std::ptrdiff_t yylen = yyr2_[yyrule];
          // First pop from the LAC stack as many tokens as possible.
          std::ptrdiff_t lac_size = std::ptrdiff_t (yylac_stack_.size ());
          if (yylen < lac_size)
            {
              yylac_stack_.resize (std::size_t (lac_size - yylen));
              yylen = 0;
            }
          else if (lac_size)
            {
              yylac_stack_.clear ();
              yylen -= lac_size;
            }
          // Only afterwards look at the main stack.
          // We simulate popping elements by incrementing lac_top.
          lac_top += yylen;
        }
        // Keep top_state in sync with the updated stack.
        top_state = (yylac_stack_.empty ()
                     ? yystack_[lac_top].state
                     : yylac_stack_.back ());
        // Push the resulting state of the reduction.
        state_type state = yy_lr_goto_state_ (top_state, yyr1_[yyrule]);
        YYCDEBUG << " G" << int (state);
        yylac_stack_.push_back (state);
      }
  }

  // Establish the initial context if no initial context currently exists.
  bool
  parser::yy_lac_establish_ (symbol_kind_type yytoken)
  {
    /* Establish the initial context for the current lookahead if no initial
       context is currently established.

       We define a context as a snapshot of the parser stacks.  We define
       the initial context for a lookahead as the context in which the
       parser initially examines that lookahead in order to select a
       syntactic action.  Thus, if the lookahead eventually proves
       syntactically unacceptable (possibly in a later context reached via a
       series of reductions), the initial context can be used to determine
       the exact set of tokens that would be syntactically acceptable in the
       lookahead's place.  Moreover, it is the context after which any
       further semantic actions would be erroneous because they would be
       determined by a syntactically unacceptable token.

       yy_lac_establish_ should be invoked when a reduction is about to be
       performed in an inconsistent state (which, for the purposes of LAC,
       includes consistent states that don't know they're consistent because
       their default reductions have been disabled).

       For parse.lac=full, the implementation of yy_lac_establish_ is as
       follows.  If no initial context is currently established for the
       current lookahead, then check if that lookahead can eventually be
       shifted if syntactic actions continue from the current context.  */
    if (yy_lac_established_)
      return true;
    else
      {
#if YYDEBUG
        YYCDEBUG << "LAC: initial context established for "
                 << symbol_name (yytoken) << '\n';
#endif
        yy_lac_established_ = true;
        return yy_lac_check_ (yytoken);
      }
  }

  // Discard any previous initial lookahead context.
  void
  parser::yy_lac_discard_ (const char* event)
  {
   /* Discard any previous initial lookahead context because of Event,
      which may be a lookahead change or an invalidation of the currently
      established initial context for the current lookahead.

      The most common example of a lookahead change is a shift.  An example
      of both cases is syntax error recovery.  That is, a syntax error
      occurs when the lookahead is syntactically erroneous for the
      currently established initial context, so error recovery manipulates
      the parser stacks to try to find a new initial context in which the
      current lookahead is syntactically acceptable.  If it fails to find
      such a context, it discards the lookahead.  */
    if (yy_lac_established_)
      {
        YYCDEBUG << "LAC: initial context discarded due to "
                 << event << '\n';
        yy_lac_established_ = false;
      }
  }


  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
         In the first two cases, it might appear that the current syntax
         error should have been detected in the previous state when
         yy_lac_check was invoked.  However, at that time, there might
         have been a different syntax error that discarded a different
         initial context during error recovery, leaving behind the
         current lookahead.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short parser::yypact_ninf_ = -128;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -17,    17,    27,    23,  -128,   103,  -128,   -12,  -128,    35,
      18,    49,  -128,  -128,  -128,  -128,     7,  -128,    68,  -128,
     -11,  -128,  -128,    34,  -128,  -128,   101,    86,  -128,  -128,
    -128,    33,    79,    35,    35,    35,    87,   147,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,   139,
    -128,  -128,    27,    85,    17,    99,  -128,    27,   100,    27,
      84,  -128,     4,  -128,   101,  -128,   167,     5,   147,    38,
    -128,   115,   102,    35,   114,   118,   122,   101,  -128,    35,
    -128,  -128,  -128,  -128,  -128,    35,   101,   101,   101,   101,
     101,   101,  -128,    27,  -128,    35,   129,   112,   132,   140,
    -128,    35,  -128,   127,    77,  -128,   101,    35,  -128,   123,
      35,  -128,  -128,    -3,    35,    35,    35,   184,    47,   142,
     148,    96,    96,   144,   144,   144,   184,   145,    65,  -128,
    -128,   106,   151,   112,  -128,    35,   177,  -128,   159,  -128,
     152,  -128,    35,  -128,  -128,  -128,  -128,   101,  -128,  -128,
      35,  -128,  -128,   112,   112,    39,   136,  -128,  -128,  -128,
     184,  -128,  -128,   107,   162,  -128,  -128,    35,  -128
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,     0,     2,     0,     5,     3,    75,     0,
       0,     0,    83,    80,    81,    82,     0,    78,     0,    54,
       0,     1,     6,     0,    10,    76,     0,     0,    73,    74,
      20,    62,     0,     0,     0,     0,     0,     0,    22,    23,
      26,    27,    28,    29,    21,    24,    19,    43,    44,    42,
      69,    61,     0,     0,     0,     0,    77,     0,     0,     0,
       0,    11,     0,    13,     0,    62,     0,    62,    52,     0,
      39,     0,     0,     0,     0,     0,     0,     0,    30,     0,
      45,    46,    47,    48,    49,     0,     0,     0,     0,     0,
       0,     0,    56,     0,    79,     0,     0,     0,     9,     0,
      14,     4,    16,     0,     0,    68,     0,     0,    34,     0,
       0,    35,    50,     0,     0,     0,     0,    71,     0,     0,
       0,    63,    64,    65,    66,    67,    59,     0,     0,    55,
      57,     0,     0,     0,    17,     0,     0,    41,     0,    40,
      53,    25,     0,    36,    37,    38,    70,     0,    31,    33,
       0,    84,     7,     0,     0,     0,     0,    60,    32,    51,
      72,    12,    58,     0,     0,    18,     8,     0,    15
  };

  const short
  parser::yypgoto_[] =
  {
    -128,  -128,  -128,  -128,   174,  -128,    14,  -128,   124,  -128,
      92,    -9,  -128,     0,  -128,     1,  -128,  -128,  -128,  -128,
      88,  -128,  -128,   104,  -128,   -48,     2,  -127,  -128,  -128,
       3,  -128,  -128,  -128,  -128,   187,  -128,   141,  -128
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     3,     4,     5,     6,    23,    12,    62,    63,   101,
     102,   103,    38,    39,    40,    41,    42,    43,    44,    69,
      70,    45,    85,   113,    71,    20,    46,   131,    47,    48,
      49,    50,   118,    51,     7,     8,    16,    17,    56
  };

  const unsigned char
  parser::yytable_[] =
  {
      37,    13,    14,    15,    92,    58,   155,   141,    72,    96,
     142,    98,     1,    26,    59,    27,     2,     1,    68,    24,
      52,    54,   107,    21,    74,    75,    76,   163,     9,    66,
      28,    29,    30,    52,    31,    32,    72,    61,    55,    60,
      33,    34,    35,    36,    26,   127,    27,    10,    52,   164,
     108,    11,   109,   153,    13,    14,    15,   146,    18,    19,
     147,    28,    29,    30,   112,    31,    32,   104,    11,    60,
     119,    33,    34,    35,    36,   151,   120,    57,   142,    53,
     117,    86,    87,    88,    89,    90,   112,   105,    73,   121,
     122,   123,   124,   125,   126,    26,    77,    27,   137,   130,
      93,   140,    88,    89,    90,   143,   144,   145,    95,   136,
      64,    97,    28,    29,    30,    99,    67,    32,   152,   166,
     153,   153,    33,    34,    35,    36,   156,    28,    29,   110,
     114,    65,   111,   159,   115,   130,     2,    11,   116,   129,
      36,   161,    10,    86,    87,    88,    89,    90,   132,   133,
     160,   135,    90,   138,   148,   162,   130,    91,   168,    78,
     149,   150,   154,    79,   158,   165,    80,    81,    82,    83,
      84,    86,    87,    88,    89,    90,   107,   105,   167,    22,
     106,    86,    87,    88,    89,    90,   100,   157,    86,    87,
      88,    89,    90,   134,    25,    94,     0,   139,     0,   128
  };

  const short
  parser::yycheck_[] =
  {
       9,     1,     1,     1,    52,    16,   133,    10,     3,    57,
      13,    59,    29,     9,    25,    11,    33,    29,    27,     5,
      15,    14,    17,     0,    33,    34,    35,   154,    11,    26,
      26,    27,    28,    15,    30,    31,     3,    23,    31,    35,
      36,    37,    38,    39,     9,    93,    11,    30,    15,    10,
      12,    34,    14,    14,    54,    54,    54,    10,    31,    32,
      13,    26,    27,    28,    73,    30,    31,    64,    34,    35,
      79,    36,    37,    38,    39,    10,    85,     9,    13,    30,
      77,     4,     5,     6,     7,     8,    95,    10,     9,    86,
      87,    88,    89,    90,    91,     9,     9,    11,   107,    97,
      15,   110,     6,     7,     8,   114,   115,   116,     9,   106,
       9,    11,    26,    27,    28,    31,    30,    31,    12,    12,
      14,    14,    36,    37,    38,    39,   135,    26,    27,    14,
      16,    30,    30,   142,    16,   133,    33,    34,    16,    10,
      39,   150,    30,     4,     5,     6,     7,     8,    16,     9,
     147,    24,     8,    30,    12,   153,   154,    18,   167,    12,
      12,    16,    11,    16,    12,    29,    19,    20,    21,    22,
      23,     4,     5,     6,     7,     8,    17,    10,    16,     5,
      13,     4,     5,     6,     7,     8,    62,    10,     4,     5,
       6,     7,     8,   101,     7,    54,    -1,   109,    -1,    95
  };

  const signed char
  parser::yystos_[] =
  {
       0,    29,    33,    41,    42,    43,    44,    74,    75,    11,
      30,    34,    46,    53,    55,    66,    76,    77,    31,    32,
      65,     0,    44,    45,    46,    75,     9,    11,    26,    27,
      28,    30,    31,    36,    37,    38,    39,    51,    52,    53,
      54,    55,    56,    57,    58,    61,    66,    68,    69,    70,
      71,    73,    15,    30,    14,    31,    78,     9,    16,    25,
      35,    46,    47,    48,     9,    30,    70,    30,    51,    59,
      60,    64,     3,     9,    51,    51,    51,     9,    12,    16,
      19,    20,    21,    22,    23,    62,     4,     5,     6,     7,
       8,    18,    65,    15,    77,     9,    65,    11,    65,    31,
      48,    49,    50,    51,    70,    10,    13,    17,    12,    14,
      14,    30,    51,    63,    16,    16,    16,    70,    72,    51,
      51,    70,    70,    70,    70,    70,    70,    65,    63,    10,
      66,    67,    16,     9,    50,    24,    70,    51,    30,    60,
      51,    10,    13,    51,    51,    51,    10,    13,    12,    12,
      16,    10,    12,    14,    11,    67,    51,    10,    12,    51,
      70,    51,    66,    67,    10,    29,    12,    16,    51
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    40,    41,    41,    42,    43,    43,    44,    44,    44,
      45,    45,    46,    47,    47,    48,    49,    49,    50,    51,
      51,    51,    51,    51,    51,    52,    52,    52,    52,    52,
      53,    53,    54,    55,    56,    57,    58,    58,    58,    59,
      59,    60,    61,    61,    61,    62,    62,    62,    62,    62,
      63,    63,    64,    64,    65,    65,    66,    67,    67,    68,
      69,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      71,    72,    72,    73,    73,    74,    74,    75,    76,    76,
      77,    77,    77,    77,    78
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     4,     1,     2,     6,     8,     4,
       1,     2,     6,     1,     2,     7,     1,     2,     4,     1,
       1,     1,     1,     1,     1,     4,     1,     1,     1,     1,
       3,     5,     5,     5,     3,     3,     4,     4,     4,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     3,     1,     4,     3,     1,     3,     3,
       5,     1,     1,     3,     3,     3,     3,     3,     3,     1,
       4,     1,     3,     1,     1,     1,     2,     3,     1,     3,
       1,     1,     1,     1,     4
  };




#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,   104,   104,   105,   108,   110,   111,   114,   115,   116,
     119,   120,   123,   125,   126,   129,   132,   133,   136,   139,
     140,   141,   142,   143,   144,   147,   148,   149,   150,   151,
     154,   155,   158,   160,   162,   164,   166,   167,   168,   171,
     172,   175,   177,   178,   179,   184,   185,   186,   187,   188,
     191,   192,   195,   196,   199,   200,   203,   205,   206,   209,
     210,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     228,   230,   231,   234,   235,   238,   239,   242,   244,   245,
     248,   249,   250,   251,   254
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 2860 "parser.cc"

#line 256 "parser.yy"


void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
