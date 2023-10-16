// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file parser.hh
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_PARSER_HH_INCLUDED
# define YY_YY_PARSER_HH_INCLUDED
// "%code requires" blocks.
#line 10 "parser.yy"

  # include "ke_struct.hh"
  class driver;

#line 54 "parser.hh"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "location.hh"
#include <typeinfo>
#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

namespace yy {
#line 194 "parser.hh"




  /// A Bison parser.
  class parser
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {
      YY_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // assertion
      char dummy1[sizeof (Assertion)];

      // assignment
      char dummy2[sizeof (Assignment)];

      // concept
      char dummy3[sizeof (Concept)];

      // coordinate
      char dummy4[sizeof (Coordinate)];

      // cud
      char dummy5[sizeof (Cud)];

      // def_concept
      char dummy6[sizeof (Def_Concept)];

      // def_individual
      char dummy7[sizeof (Def_Individual)];

      // def_operator
      char dummy8[sizeof (Def_Operator)];

      // fact
      char dummy9[sizeof (Fact)];

      // individual
      char dummy10[sizeof (Individual)];

      // ke_kb
      char dummy11[sizeof (Knowledge_Base)];

      // math_equation
      char dummy12[sizeof (Math_Equation)];

      // math_expr
      char dummy13[sizeof (Math_Expr)];

      // math_func
      char dummy14[sizeof (Math_Func)];

      // math_individual
      char dummy15[sizeof (Math_Individual)];

      // number
      char dummy16[sizeof (Number)];

      // ke_question
      char dummy17[sizeof (Question)];

      // ke_rule
      char dummy18[sizeof (Rule)];

      // sugar_for_and
      char dummy19[sizeof (Sugar_For_And)];

      // sugar_for_ctor
      char dummy20[sizeof (Sugar_For_Ctor)];

      // sugar_for_oprt_apply
      char dummy21[sizeof (Sugar_For_Oprt_Apply)];

      // sugar_for_pred
      char dummy22[sizeof (Sugar_For_Pred)];

      // term
      char dummy23[sizeof (Term)];

      // variable
      char dummy24[sizeof (Variable)];

      // BOOL
      char dummy25[sizeof (bool)];

      // FLOAT
      char dummy26[sizeof (double)];

      // INTEGER
      char dummy27[sizeof (int)];

      // DESCRIPTION
      // SYMBOL
      // OPERATOR
      // CONCEPT
      // DEF_CPT
      // DEF_INDI
      // DEF_OP
      // KB_ASSERT
      // KB_MODIFY
      // KB_RETRACT
      // MATH_FUNC_NAME
      // predicate_operator
      char dummy28[sizeof (std::string)];

      // assignment_list
      char dummy29[sizeof (vector<shared_ptr<Assignment>>)];

      // def_concept_list
      char dummy30[sizeof (vector<shared_ptr<Def_Concept>>)];

      // def_individual_list
      char dummy31[sizeof (vector<shared_ptr<Def_Individual>>)];

      // def_operator_list
      char dummy32[sizeof (vector<shared_ptr<Def_Operator>>)];

      // fact_list
      char dummy33[sizeof (vector<shared_ptr<Fact>>)];

      // individual_list
      // individual_and_list
      // to_solve
      char dummy34[sizeof (vector<shared_ptr<Individual>>)];

      // math_expr_list
      char dummy35[sizeof (vector<shared_ptr<Math_Expr>>)];

      // ke_questions
      char dummy36[sizeof (vector<shared_ptr<Question>>)];

      // ke_rule_list
      char dummy37[sizeof (vector<shared_ptr<Rule>>)];

      // variable_list
      char dummy38[sizeof (vector<shared_ptr<Variable>>)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;

    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        TOK_YYEMPTY = -2,
    TOK_YYEOF = 0,                 // "end of file"
    TOK_YYerror = 1,               // error
    TOK_YYUNDEF = 2,               // "invalid token"
    TOK_DOT = 3,                   // "."
    TOK_PLUS = 4,                  // "+"
    TOK_MINUS = 5,                 // "-"
    TOK_MUL = 6,                   // "*"
    TOK_DIV = 7,                   // "/"
    TOK_POWER = 8,                 // "^"
    TOK_LPAREN = 9,                // "("
    TOK_RPAREN = 10,               // ")"
    TOK_LBRACE = 11,               // "{"
    TOK_RBRACE = 12,               // "}"
    TOK_COMMA = 13,                // ","
    TOK_SEMI = 14,                 // ";"
    TOK_COLON = 15,                // ":"
    TOK_EQ = 16,                   // "="
    TOK_ASSIGN = 17,               // ":="
    TOK_MEQ = 18,                  // "=="
    TOK_GTE = 19,                  // ">="
    TOK_LTE = 20,                  // "<="
    TOK_NE = 21,                   // "!="
    TOK_GT = 22,                   // ">"
    TOK_LT = 23,                   // "<"
    TOK_IMPLIES = 24,              // "=>"
    TOK_INH = 25,                  // "<-"
    TOK_INTEGER = 26,              // INTEGER
    TOK_FLOAT = 27,                // FLOAT
    TOK_BOOL = 28,                 // BOOL
    TOK_DESCRIPTION = 29,          // DESCRIPTION
    TOK_SYMBOL = 30,               // SYMBOL
    TOK_OPERATOR = 31,             // OPERATOR
    TOK_CONCEPT = 32,              // CONCEPT
    TOK_DEF_CPT = 33,              // DEF_CPT
    TOK_DEF_INDI = 34,             // DEF_INDI
    TOK_DEF_OP = 35,               // DEF_OP
    TOK_KB_ASSERT = 36,            // KB_ASSERT
    TOK_KB_MODIFY = 37,            // KB_MODIFY
    TOK_KB_RETRACT = 38,           // KB_RETRACT
    TOK_MATH_FUNC_NAME = 39        // MATH_FUNC_NAME
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 40, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_DOT = 3,                               // "."
        S_PLUS = 4,                              // "+"
        S_MINUS = 5,                             // "-"
        S_MUL = 6,                               // "*"
        S_DIV = 7,                               // "/"
        S_POWER = 8,                             // "^"
        S_LPAREN = 9,                            // "("
        S_RPAREN = 10,                           // ")"
        S_LBRACE = 11,                           // "{"
        S_RBRACE = 12,                           // "}"
        S_COMMA = 13,                            // ","
        S_SEMI = 14,                             // ";"
        S_COLON = 15,                            // ":"
        S_EQ = 16,                               // "="
        S_ASSIGN = 17,                           // ":="
        S_MEQ = 18,                              // "=="
        S_GTE = 19,                              // ">="
        S_LTE = 20,                              // "<="
        S_NE = 21,                               // "!="
        S_GT = 22,                               // ">"
        S_LT = 23,                               // "<"
        S_IMPLIES = 24,                          // "=>"
        S_INH = 25,                              // "<-"
        S_INTEGER = 26,                          // INTEGER
        S_FLOAT = 27,                            // FLOAT
        S_BOOL = 28,                             // BOOL
        S_DESCRIPTION = 29,                      // DESCRIPTION
        S_SYMBOL = 30,                           // SYMBOL
        S_OPERATOR = 31,                         // OPERATOR
        S_CONCEPT = 32,                          // CONCEPT
        S_DEF_CPT = 33,                          // DEF_CPT
        S_DEF_INDI = 34,                         // DEF_INDI
        S_DEF_OP = 35,                           // DEF_OP
        S_KB_ASSERT = 36,                        // KB_ASSERT
        S_KB_MODIFY = 37,                        // KB_MODIFY
        S_KB_RETRACT = 38,                       // KB_RETRACT
        S_MATH_FUNC_NAME = 39,                   // MATH_FUNC_NAME
        S_YYACCEPT = 40,                         // $accept
        S_statement = 41,                        // statement
        S_ke_kb = 42,                            // ke_kb
        S_def_concept_list = 43,                 // def_concept_list
        S_def_concept = 44,                      // def_concept
        S_def_individual_list = 45,              // def_individual_list
        S_def_individual = 46,                   // def_individual
        S_def_operator_list = 47,                // def_operator_list
        S_def_operator = 48,                     // def_operator
        S_ke_rule_list = 49,                     // ke_rule_list
        S_ke_rule = 50,                          // ke_rule
        S_individual = 51,                       // individual
        S_term = 52,                             // term
        S_assertion = 53,                        // assertion
        S_sugar_for_and = 54,                    // sugar_for_and
        S_sugar_for_pred = 55,                   // sugar_for_pred
        S_sugar_for_ctor = 56,                   // sugar_for_ctor
        S_sugar_for_oprt_apply = 57,             // sugar_for_oprt_apply
        S_cud = 58,                              // cud
        S_assignment_list = 59,                  // assignment_list
        S_assignment = 60,                       // assignment
        S_math_individual = 61,                  // math_individual
        S_predicate_operator = 62,               // predicate_operator
        S_individual_list = 63,                  // individual_list
        S_individual_and_list = 64,              // individual_and_list
        S_concept = 65,                          // concept
        S_variable = 66,                         // variable
        S_variable_list = 67,                    // variable_list
        S_math_equation = 68,                    // math_equation
        S_coordinate = 69,                       // coordinate
        S_math_expr = 70,                        // math_expr
        S_math_func = 71,                        // math_func
        S_math_expr_list = 72,                   // math_expr_list
        S_number = 73,                           // number
        S_ke_questions = 74,                     // ke_questions
        S_ke_question = 75,                      // ke_question
        S_fact_list = 76,                        // fact_list
        S_fact = 77,                             // fact
        S_to_solve = 78                          // to_solve
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
    {
      case symbol_kind::S_assertion: // assertion
        value.move< Assertion > (std::move (that.value));
        break;

      case symbol_kind::S_assignment: // assignment
        value.move< Assignment > (std::move (that.value));
        break;

      case symbol_kind::S_concept: // concept
        value.move< Concept > (std::move (that.value));
        break;

      case symbol_kind::S_coordinate: // coordinate
        value.move< Coordinate > (std::move (that.value));
        break;

      case symbol_kind::S_cud: // cud
        value.move< Cud > (std::move (that.value));
        break;

      case symbol_kind::S_def_concept: // def_concept
        value.move< Def_Concept > (std::move (that.value));
        break;

      case symbol_kind::S_def_individual: // def_individual
        value.move< Def_Individual > (std::move (that.value));
        break;

      case symbol_kind::S_def_operator: // def_operator
        value.move< Def_Operator > (std::move (that.value));
        break;

      case symbol_kind::S_fact: // fact
        value.move< Fact > (std::move (that.value));
        break;

      case symbol_kind::S_individual: // individual
        value.move< Individual > (std::move (that.value));
        break;

      case symbol_kind::S_ke_kb: // ke_kb
        value.move< Knowledge_Base > (std::move (that.value));
        break;

      case symbol_kind::S_math_equation: // math_equation
        value.move< Math_Equation > (std::move (that.value));
        break;

      case symbol_kind::S_math_expr: // math_expr
        value.move< Math_Expr > (std::move (that.value));
        break;

      case symbol_kind::S_math_func: // math_func
        value.move< Math_Func > (std::move (that.value));
        break;

      case symbol_kind::S_math_individual: // math_individual
        value.move< Math_Individual > (std::move (that.value));
        break;

      case symbol_kind::S_number: // number
        value.move< Number > (std::move (that.value));
        break;

      case symbol_kind::S_ke_question: // ke_question
        value.move< Question > (std::move (that.value));
        break;

      case symbol_kind::S_ke_rule: // ke_rule
        value.move< Rule > (std::move (that.value));
        break;

      case symbol_kind::S_sugar_for_and: // sugar_for_and
        value.move< Sugar_For_And > (std::move (that.value));
        break;

      case symbol_kind::S_sugar_for_ctor: // sugar_for_ctor
        value.move< Sugar_For_Ctor > (std::move (that.value));
        break;

      case symbol_kind::S_sugar_for_oprt_apply: // sugar_for_oprt_apply
        value.move< Sugar_For_Oprt_Apply > (std::move (that.value));
        break;

      case symbol_kind::S_sugar_for_pred: // sugar_for_pred
        value.move< Sugar_For_Pred > (std::move (that.value));
        break;

      case symbol_kind::S_term: // term
        value.move< Term > (std::move (that.value));
        break;

      case symbol_kind::S_variable: // variable
        value.move< Variable > (std::move (that.value));
        break;

      case symbol_kind::S_BOOL: // BOOL
        value.move< bool > (std::move (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< double > (std::move (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (std::move (that.value));
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
        value.move< std::string > (std::move (that.value));
        break;

      case symbol_kind::S_assignment_list: // assignment_list
        value.move< vector<shared_ptr<Assignment>> > (std::move (that.value));
        break;

      case symbol_kind::S_def_concept_list: // def_concept_list
        value.move< vector<shared_ptr<Def_Concept>> > (std::move (that.value));
        break;

      case symbol_kind::S_def_individual_list: // def_individual_list
        value.move< vector<shared_ptr<Def_Individual>> > (std::move (that.value));
        break;

      case symbol_kind::S_def_operator_list: // def_operator_list
        value.move< vector<shared_ptr<Def_Operator>> > (std::move (that.value));
        break;

      case symbol_kind::S_fact_list: // fact_list
        value.move< vector<shared_ptr<Fact>> > (std::move (that.value));
        break;

      case symbol_kind::S_individual_list: // individual_list
      case symbol_kind::S_individual_and_list: // individual_and_list
      case symbol_kind::S_to_solve: // to_solve
        value.move< vector<shared_ptr<Individual>> > (std::move (that.value));
        break;

      case symbol_kind::S_math_expr_list: // math_expr_list
        value.move< vector<shared_ptr<Math_Expr>> > (std::move (that.value));
        break;

      case symbol_kind::S_ke_questions: // ke_questions
        value.move< vector<shared_ptr<Question>> > (std::move (that.value));
        break;

      case symbol_kind::S_ke_rule_list: // ke_rule_list
        value.move< vector<shared_ptr<Rule>> > (std::move (that.value));
        break;

      case symbol_kind::S_variable_list: // variable_list
        value.move< vector<shared_ptr<Variable>> > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Assertion&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Assertion& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Assignment&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Assignment& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Concept&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Concept& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Coordinate&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Coordinate& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Cud&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Cud& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Def_Concept&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Def_Concept& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Def_Individual&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Def_Individual& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Def_Operator&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Def_Operator& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Fact&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Fact& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Individual&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Individual& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Knowledge_Base&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Knowledge_Base& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Math_Equation&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Math_Equation& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Math_Expr&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Math_Expr& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Math_Func&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Math_Func& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Math_Individual&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Math_Individual& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Number&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Number& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Question&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Question& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Rule&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Rule& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Sugar_For_And&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Sugar_For_And& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Sugar_For_Ctor&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Sugar_For_Ctor& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Sugar_For_Oprt_Apply&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Sugar_For_Oprt_Apply& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Sugar_For_Pred&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Sugar_For_Pred& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Term&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Term& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Variable&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Variable& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, bool&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const bool& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, double&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const double& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, int&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const int& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, vector<shared_ptr<Assignment>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const vector<shared_ptr<Assignment>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, vector<shared_ptr<Def_Concept>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const vector<shared_ptr<Def_Concept>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, vector<shared_ptr<Def_Individual>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const vector<shared_ptr<Def_Individual>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, vector<shared_ptr<Def_Operator>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const vector<shared_ptr<Def_Operator>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, vector<shared_ptr<Fact>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const vector<shared_ptr<Fact>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, vector<shared_ptr<Individual>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const vector<shared_ptr<Individual>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, vector<shared_ptr<Math_Expr>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const vector<shared_ptr<Math_Expr>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, vector<shared_ptr<Question>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const vector<shared_ptr<Question>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, vector<shared_ptr<Rule>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const vector<shared_ptr<Rule>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, vector<shared_ptr<Variable>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const vector<shared_ptr<Variable>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_assertion: // assertion
        value.template destroy< Assertion > ();
        break;

      case symbol_kind::S_assignment: // assignment
        value.template destroy< Assignment > ();
        break;

      case symbol_kind::S_concept: // concept
        value.template destroy< Concept > ();
        break;

      case symbol_kind::S_coordinate: // coordinate
        value.template destroy< Coordinate > ();
        break;

      case symbol_kind::S_cud: // cud
        value.template destroy< Cud > ();
        break;

      case symbol_kind::S_def_concept: // def_concept
        value.template destroy< Def_Concept > ();
        break;

      case symbol_kind::S_def_individual: // def_individual
        value.template destroy< Def_Individual > ();
        break;

      case symbol_kind::S_def_operator: // def_operator
        value.template destroy< Def_Operator > ();
        break;

      case symbol_kind::S_fact: // fact
        value.template destroy< Fact > ();
        break;

      case symbol_kind::S_individual: // individual
        value.template destroy< Individual > ();
        break;

      case symbol_kind::S_ke_kb: // ke_kb
        value.template destroy< Knowledge_Base > ();
        break;

      case symbol_kind::S_math_equation: // math_equation
        value.template destroy< Math_Equation > ();
        break;

      case symbol_kind::S_math_expr: // math_expr
        value.template destroy< Math_Expr > ();
        break;

      case symbol_kind::S_math_func: // math_func
        value.template destroy< Math_Func > ();
        break;

      case symbol_kind::S_math_individual: // math_individual
        value.template destroy< Math_Individual > ();
        break;

      case symbol_kind::S_number: // number
        value.template destroy< Number > ();
        break;

      case symbol_kind::S_ke_question: // ke_question
        value.template destroy< Question > ();
        break;

      case symbol_kind::S_ke_rule: // ke_rule
        value.template destroy< Rule > ();
        break;

      case symbol_kind::S_sugar_for_and: // sugar_for_and
        value.template destroy< Sugar_For_And > ();
        break;

      case symbol_kind::S_sugar_for_ctor: // sugar_for_ctor
        value.template destroy< Sugar_For_Ctor > ();
        break;

      case symbol_kind::S_sugar_for_oprt_apply: // sugar_for_oprt_apply
        value.template destroy< Sugar_For_Oprt_Apply > ();
        break;

      case symbol_kind::S_sugar_for_pred: // sugar_for_pred
        value.template destroy< Sugar_For_Pred > ();
        break;

      case symbol_kind::S_term: // term
        value.template destroy< Term > ();
        break;

      case symbol_kind::S_variable: // variable
        value.template destroy< Variable > ();
        break;

      case symbol_kind::S_BOOL: // BOOL
        value.template destroy< bool > ();
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.template destroy< double > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.template destroy< int > ();
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
        value.template destroy< std::string > ();
        break;

      case symbol_kind::S_assignment_list: // assignment_list
        value.template destroy< vector<shared_ptr<Assignment>> > ();
        break;

      case symbol_kind::S_def_concept_list: // def_concept_list
        value.template destroy< vector<shared_ptr<Def_Concept>> > ();
        break;

      case symbol_kind::S_def_individual_list: // def_individual_list
        value.template destroy< vector<shared_ptr<Def_Individual>> > ();
        break;

      case symbol_kind::S_def_operator_list: // def_operator_list
        value.template destroy< vector<shared_ptr<Def_Operator>> > ();
        break;

      case symbol_kind::S_fact_list: // fact_list
        value.template destroy< vector<shared_ptr<Fact>> > ();
        break;

      case symbol_kind::S_individual_list: // individual_list
      case symbol_kind::S_individual_and_list: // individual_and_list
      case symbol_kind::S_to_solve: // to_solve
        value.template destroy< vector<shared_ptr<Individual>> > ();
        break;

      case symbol_kind::S_math_expr_list: // math_expr_list
        value.template destroy< vector<shared_ptr<Math_Expr>> > ();
        break;

      case symbol_kind::S_ke_questions: // ke_questions
        value.template destroy< vector<shared_ptr<Question>> > ();
        break;

      case symbol_kind::S_ke_rule_list: // ke_rule_list
        value.template destroy< vector<shared_ptr<Rule>> > ();
        break;

      case symbol_kind::S_variable_list: // variable_list
        value.template destroy< vector<shared_ptr<Variable>> > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// The user-facing name of this symbol.
      const char *name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type (token_kind_type (tok), std::move (l))
#else
      symbol_type (int tok, const location_type& l)
        : super_type (token_kind_type (tok), l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::TOK_YYEOF
                   || (token::TOK_YYerror <= tok && tok <= token::TOK_INH));
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, bool v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const bool& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::TOK_BOOL);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, double v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const double& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::TOK_FLOAT);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, int v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const int& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::TOK_INTEGER);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT ((token::TOK_DESCRIPTION <= tok && tok <= token::TOK_MATH_FUNC_NAME));
#endif
      }
    };

    /// Build a parser object.
    parser (driver& drv_yyarg);
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);

    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYEOF (location_type l)
      {
        return symbol_type (token::TOK_YYEOF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYEOF (const location_type& l)
      {
        return symbol_type (token::TOK_YYEOF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror (location_type l)
      {
        return symbol_type (token::TOK_YYerror, std::move (l));
      }
#else
      static
      symbol_type
      make_YYerror (const location_type& l)
      {
        return symbol_type (token::TOK_YYerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF (location_type l)
      {
        return symbol_type (token::TOK_YYUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYUNDEF (const location_type& l)
      {
        return symbol_type (token::TOK_YYUNDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DOT (location_type l)
      {
        return symbol_type (token::TOK_DOT, std::move (l));
      }
#else
      static
      symbol_type
      make_DOT (const location_type& l)
      {
        return symbol_type (token::TOK_DOT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS (location_type l)
      {
        return symbol_type (token::TOK_PLUS, std::move (l));
      }
#else
      static
      symbol_type
      make_PLUS (const location_type& l)
      {
        return symbol_type (token::TOK_PLUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS (location_type l)
      {
        return symbol_type (token::TOK_MINUS, std::move (l));
      }
#else
      static
      symbol_type
      make_MINUS (const location_type& l)
      {
        return symbol_type (token::TOK_MINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MUL (location_type l)
      {
        return symbol_type (token::TOK_MUL, std::move (l));
      }
#else
      static
      symbol_type
      make_MUL (const location_type& l)
      {
        return symbol_type (token::TOK_MUL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIV (location_type l)
      {
        return symbol_type (token::TOK_DIV, std::move (l));
      }
#else
      static
      symbol_type
      make_DIV (const location_type& l)
      {
        return symbol_type (token::TOK_DIV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_POWER (location_type l)
      {
        return symbol_type (token::TOK_POWER, std::move (l));
      }
#else
      static
      symbol_type
      make_POWER (const location_type& l)
      {
        return symbol_type (token::TOK_POWER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LPAREN (location_type l)
      {
        return symbol_type (token::TOK_LPAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_LPAREN (const location_type& l)
      {
        return symbol_type (token::TOK_LPAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RPAREN (location_type l)
      {
        return symbol_type (token::TOK_RPAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_RPAREN (const location_type& l)
      {
        return symbol_type (token::TOK_RPAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LBRACE (location_type l)
      {
        return symbol_type (token::TOK_LBRACE, std::move (l));
      }
#else
      static
      symbol_type
      make_LBRACE (const location_type& l)
      {
        return symbol_type (token::TOK_LBRACE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RBRACE (location_type l)
      {
        return symbol_type (token::TOK_RBRACE, std::move (l));
      }
#else
      static
      symbol_type
      make_RBRACE (const location_type& l)
      {
        return symbol_type (token::TOK_RBRACE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA (location_type l)
      {
        return symbol_type (token::TOK_COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMA (const location_type& l)
      {
        return symbol_type (token::TOK_COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SEMI (location_type l)
      {
        return symbol_type (token::TOK_SEMI, std::move (l));
      }
#else
      static
      symbol_type
      make_SEMI (const location_type& l)
      {
        return symbol_type (token::TOK_SEMI, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COLON (location_type l)
      {
        return symbol_type (token::TOK_COLON, std::move (l));
      }
#else
      static
      symbol_type
      make_COLON (const location_type& l)
      {
        return symbol_type (token::TOK_COLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQ (location_type l)
      {
        return symbol_type (token::TOK_EQ, std::move (l));
      }
#else
      static
      symbol_type
      make_EQ (const location_type& l)
      {
        return symbol_type (token::TOK_EQ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASSIGN (location_type l)
      {
        return symbol_type (token::TOK_ASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_ASSIGN (const location_type& l)
      {
        return symbol_type (token::TOK_ASSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MEQ (location_type l)
      {
        return symbol_type (token::TOK_MEQ, std::move (l));
      }
#else
      static
      symbol_type
      make_MEQ (const location_type& l)
      {
        return symbol_type (token::TOK_MEQ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GTE (location_type l)
      {
        return symbol_type (token::TOK_GTE, std::move (l));
      }
#else
      static
      symbol_type
      make_GTE (const location_type& l)
      {
        return symbol_type (token::TOK_GTE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LTE (location_type l)
      {
        return symbol_type (token::TOK_LTE, std::move (l));
      }
#else
      static
      symbol_type
      make_LTE (const location_type& l)
      {
        return symbol_type (token::TOK_LTE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NE (location_type l)
      {
        return symbol_type (token::TOK_NE, std::move (l));
      }
#else
      static
      symbol_type
      make_NE (const location_type& l)
      {
        return symbol_type (token::TOK_NE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GT (location_type l)
      {
        return symbol_type (token::TOK_GT, std::move (l));
      }
#else
      static
      symbol_type
      make_GT (const location_type& l)
      {
        return symbol_type (token::TOK_GT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LT (location_type l)
      {
        return symbol_type (token::TOK_LT, std::move (l));
      }
#else
      static
      symbol_type
      make_LT (const location_type& l)
      {
        return symbol_type (token::TOK_LT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IMPLIES (location_type l)
      {
        return symbol_type (token::TOK_IMPLIES, std::move (l));
      }
#else
      static
      symbol_type
      make_IMPLIES (const location_type& l)
      {
        return symbol_type (token::TOK_IMPLIES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INH (location_type l)
      {
        return symbol_type (token::TOK_INH, std::move (l));
      }
#else
      static
      symbol_type
      make_INH (const location_type& l)
      {
        return symbol_type (token::TOK_INH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INTEGER (int v, location_type l)
      {
        return symbol_type (token::TOK_INTEGER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INTEGER (const int& v, const location_type& l)
      {
        return symbol_type (token::TOK_INTEGER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT (double v, location_type l)
      {
        return symbol_type (token::TOK_FLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT (const double& v, const location_type& l)
      {
        return symbol_type (token::TOK_FLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BOOL (bool v, location_type l)
      {
        return symbol_type (token::TOK_BOOL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_BOOL (const bool& v, const location_type& l)
      {
        return symbol_type (token::TOK_BOOL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DESCRIPTION (std::string v, location_type l)
      {
        return symbol_type (token::TOK_DESCRIPTION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DESCRIPTION (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_DESCRIPTION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SYMBOL (std::string v, location_type l)
      {
        return symbol_type (token::TOK_SYMBOL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_SYMBOL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_SYMBOL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OPERATOR (std::string v, location_type l)
      {
        return symbol_type (token::TOK_OPERATOR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_OPERATOR (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_OPERATOR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONCEPT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CONCEPT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CONCEPT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CONCEPT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEF_CPT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_DEF_CPT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DEF_CPT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_DEF_CPT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEF_INDI (std::string v, location_type l)
      {
        return symbol_type (token::TOK_DEF_INDI, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DEF_INDI (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_DEF_INDI, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEF_OP (std::string v, location_type l)
      {
        return symbol_type (token::TOK_DEF_OP, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DEF_OP (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_DEF_OP, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KB_ASSERT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_KB_ASSERT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_KB_ASSERT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_KB_ASSERT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KB_MODIFY (std::string v, location_type l)
      {
        return symbol_type (token::TOK_KB_MODIFY, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_KB_MODIFY (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_KB_MODIFY, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KB_RETRACT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_KB_RETRACT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_KB_RETRACT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_KB_RETRACT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MATH_FUNC_NAME (std::string v, location_type l)
      {
        return symbol_type (token::TOK_MATH_FUNC_NAME, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_MATH_FUNC_NAME (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_MATH_FUNC_NAME, v, l);
      }
#endif


    class context
    {
    public:
      context (const parser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token () const YY_NOEXCEPT { return yyla_.kind (); }
      const location_type& location () const YY_NOEXCEPT { return yyla_.location; }

      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens (symbol_kind_type yyarg[], int yyargn) const;

    private:
      const parser& yyparser_;
      const symbol_type& yyla_;
    };

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif

    /// Check the lookahead yytoken.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_check_ (symbol_kind_type yytoken) const;
    /// Establish the initial context if no initial context currently exists.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_establish_ (symbol_kind_type yytoken);
    /// Discard any previous initial lookahead context because of event.
    /// \param event  the event which caused the lookahead to be discarded.
    ///               Only used for debbuging output.
    void yy_lac_discard_ (const char* event);

    /// Stored state numbers (used for stacks).
    typedef unsigned char state_type;

    /// The arguments of the error message.
    int yy_syntax_error_arguments_ (const context& yyctx,
                                    symbol_kind_type yyarg[], int yyargn) const;

    /// Generate an error message.
    /// \param yyctx     the context in which the error occurred.
    virtual std::string yysyntax_error_ (const context& yyctx) const;
    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;



    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const unsigned char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const unsigned char yytable_[];

    static const unsigned char yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const unsigned char yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;
    /// The stack for LAC.
    /// Logically, the yy_lac_stack's lifetime is confined to the function
    /// yy_lac_check_. We just store it as a member of this class to hold
    /// on to the memory and to avoid frequent reallocations.
    /// Since yy_lac_check_ is const, this member must be mutable.
    mutable std::vector<state_type> yylac_stack_;
    /// Whether an initial LAC context was established.
    bool yy_lac_established_;


    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 197,     ///< Last index in yytable_.
      yynnts_ = 39,  ///< Number of nonterminal symbols.
      yyfinal_ = 20 ///< Termination state number.
    };


    // User arguments.
    driver& drv;

  };

  inline
  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    return static_cast<symbol_kind_type> (t);
  }

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_assertion: // assertion
        value.copy< Assertion > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_assignment: // assignment
        value.copy< Assignment > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_concept: // concept
        value.copy< Concept > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_coordinate: // coordinate
        value.copy< Coordinate > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_cud: // cud
        value.copy< Cud > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_concept: // def_concept
        value.copy< Def_Concept > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_individual: // def_individual
        value.copy< Def_Individual > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_operator: // def_operator
        value.copy< Def_Operator > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fact: // fact
        value.copy< Fact > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_individual: // individual
        value.copy< Individual > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_kb: // ke_kb
        value.copy< Knowledge_Base > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_equation: // math_equation
        value.copy< Math_Equation > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_expr: // math_expr
        value.copy< Math_Expr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_func: // math_func
        value.copy< Math_Func > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_individual: // math_individual
        value.copy< Math_Individual > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_number: // number
        value.copy< Number > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_question: // ke_question
        value.copy< Question > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_rule: // ke_rule
        value.copy< Rule > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_and: // sugar_for_and
        value.copy< Sugar_For_And > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_ctor: // sugar_for_ctor
        value.copy< Sugar_For_Ctor > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_oprt_apply: // sugar_for_oprt_apply
        value.copy< Sugar_For_Oprt_Apply > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sugar_for_pred: // sugar_for_pred
        value.copy< Sugar_For_Pred > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_term: // term
        value.copy< Term > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_variable: // variable
        value.copy< Variable > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BOOL: // BOOL
        value.copy< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.copy< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< int > (YY_MOVE (that.value));
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
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_assignment_list: // assignment_list
        value.copy< vector<shared_ptr<Assignment>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_concept_list: // def_concept_list
        value.copy< vector<shared_ptr<Def_Concept>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_individual_list: // def_individual_list
        value.copy< vector<shared_ptr<Def_Individual>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_def_operator_list: // def_operator_list
        value.copy< vector<shared_ptr<Def_Operator>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fact_list: // fact_list
        value.copy< vector<shared_ptr<Fact>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_individual_list: // individual_list
      case symbol_kind::S_individual_and_list: // individual_and_list
      case symbol_kind::S_to_solve: // to_solve
        value.copy< vector<shared_ptr<Individual>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_math_expr_list: // math_expr_list
        value.copy< vector<shared_ptr<Math_Expr>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_questions: // ke_questions
        value.copy< vector<shared_ptr<Question>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ke_rule_list: // ke_rule_list
        value.copy< vector<shared_ptr<Rule>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_variable_list: // variable_list
        value.copy< vector<shared_ptr<Variable>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_assertion: // assertion
        value.move< Assertion > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_assignment: // assignment
        value.move< Assignment > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_concept: // concept
        value.move< Concept > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_coordinate: // coordinate
        value.move< Coordinate > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_cud: // cud
        value.move< Cud > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_def_concept: // def_concept
        value.move< Def_Concept > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_def_individual: // def_individual
        value.move< Def_Individual > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_def_operator: // def_operator
        value.move< Def_Operator > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_fact: // fact
        value.move< Fact > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_individual: // individual
        value.move< Individual > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ke_kb: // ke_kb
        value.move< Knowledge_Base > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_math_equation: // math_equation
        value.move< Math_Equation > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_math_expr: // math_expr
        value.move< Math_Expr > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_math_func: // math_func
        value.move< Math_Func > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_math_individual: // math_individual
        value.move< Math_Individual > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_number: // number
        value.move< Number > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ke_question: // ke_question
        value.move< Question > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ke_rule: // ke_rule
        value.move< Rule > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_sugar_for_and: // sugar_for_and
        value.move< Sugar_For_And > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_sugar_for_ctor: // sugar_for_ctor
        value.move< Sugar_For_Ctor > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_sugar_for_oprt_apply: // sugar_for_oprt_apply
        value.move< Sugar_For_Oprt_Apply > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_sugar_for_pred: // sugar_for_pred
        value.move< Sugar_For_Pred > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_term: // term
        value.move< Term > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_variable: // variable
        value.move< Variable > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_BOOL: // BOOL
        value.move< bool > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< double > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (YY_MOVE (s.value));
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
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_assignment_list: // assignment_list
        value.move< vector<shared_ptr<Assignment>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_def_concept_list: // def_concept_list
        value.move< vector<shared_ptr<Def_Concept>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_def_individual_list: // def_individual_list
        value.move< vector<shared_ptr<Def_Individual>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_def_operator_list: // def_operator_list
        value.move< vector<shared_ptr<Def_Operator>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_fact_list: // fact_list
        value.move< vector<shared_ptr<Fact>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_individual_list: // individual_list
      case symbol_kind::S_individual_and_list: // individual_and_list
      case symbol_kind::S_to_solve: // to_solve
        value.move< vector<shared_ptr<Individual>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_math_expr_list: // math_expr_list
        value.move< vector<shared_ptr<Math_Expr>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ke_questions: // ke_questions
        value.move< vector<shared_ptr<Question>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ke_rule_list: // ke_rule_list
        value.move< vector<shared_ptr<Rule>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_variable_list: // variable_list
        value.move< vector<shared_ptr<Variable>> > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  inline
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  inline
  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  inline
  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  inline
  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  inline
  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  inline
  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


} // yy
#line 3265 "parser.hh"




#endif // !YY_YY_PARSER_HH_INCLUDED
