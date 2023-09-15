%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring>
# include <string>
# include <vector>
# include <sstream>
# include "driver.hh"
# include "parser.hh"
%}

%option noyywrap nounput noinput batch debug

%{
  yy::parser::symbol_type make_INTEGER (const std::string &s, const yy::parser::location_type& loc);
  yy::parser::symbol_type make_FLOAT (const std::string &s, const yy::parser::location_type& loc);
  yy::parser::symbol_type make_BOOL (const std::string &s, const yy::parser::location_type& loc);
  using std::string;
  using std::vector;
%}

blank    [ \t\r]
double   (\-)?[0-9]+(\.[0-9]+)?
integer  (\-)?[0-9]+
bool     "true"|"false"
cap_id   [A-Z][a-zA-Z_0-9]*
low_id   [a-z][a-zA-Z_0-9]*

%{
  // 每当模式被匹配就会执行的代码
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  yy::location& loc = drv.location;
  // 每当yylex被调用就会执行的代码
  loc.step ();
%}
{blank}+   loc.step ();
\n+        loc.lines (yyleng); loc.step ();

--.*      { /* 忽略单行注释 */ }

"."        return yy::parser::make_DOT      (loc);
"=>"       return yy::parser::make_IMPLIES (loc);
"+"        return yy::parser::make_PLUS     (loc);
"-"        return yy::parser::make_MINUS    (loc);
"*"        return yy::parser::make_MUL      (loc);
"/"        return yy::parser::make_DIV      (loc);
"^"        return yy::parser::make_POWER    (loc);
"("        return yy::parser::make_LPAREN   (loc);
")"        return yy::parser::make_RPAREN   (loc);
"{"        return yy::parser::make_LBRACE   (loc);
"}"        return yy::parser::make_RBRACE   (loc);
","        return yy::parser::make_COMMA    (loc);
";"        return yy::parser::make_SEMI     (loc);
":"        return yy::parser::make_COLON    (loc);
"="        return yy::parser::make_EQ       (loc);
":="       return yy::parser::make_ASSIGN   (loc);
"=="       return yy::parser::make_MEQ      (loc);
">="       return yy::parser::make_GTE      (loc);
"<="       return yy::parser::make_LTE      (loc);
"<-"       return yy::parser::make_INH      (loc);
"!="       return yy::parser::make_NE       (loc);
">"        return yy::parser::make_GT       (loc);
"<"        return yy::parser::make_LT       (loc);

"def_cpt"  return yy::parser::make_DEF_CPT   (yytext, loc);
"def_indi" return yy::parser::make_DEF_INDI  (yytext, loc);
"def_op"   return yy::parser::make_DEF_OP    (yytext, loc);
"assert"   return yy::parser::make_KB_ASSERT (yytext, loc);

#[^#]+#    return yy::parser::make_DESCRIPTION (yytext, loc);

{cap_id}/\(       return yy::parser::make_OPERATOR (yytext, loc);
{cap_id}          return yy::parser::make_CONCEPT (yytext, loc);
{integer}         return make_INTEGER (yytext, loc);
{double}          return make_FLOAT (yytext, loc);
{low_id}          {
                    vector<string> math_fcn_keys{"abs", "sqrt", "pow", "sin", "cos"};
                    vector<string> bool_keys{"true", "false"};
                    if (std::find(math_fcn_keys.begin(),math_fcn_keys.end(),yytext)!=math_fcn_keys.end()) {
                      return yy::parser::make_MATH_FUNC_NAME (yytext, loc);
                    }
                    else if (std::find(bool_keys.begin(),bool_keys.end(),yytext)!=bool_keys.end()){
                      return make_BOOL (yytext, loc);
                    }
                    else{
                      return yy::parser::make_SYMBOL (yytext, loc);
                    }
                  }
.          {throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));}
<<EOF>>    return yy::parser::make_YYEOF (loc);
%%

yy::parser::symbol_type
make_INTEGER (const std::string &s, const yy::parser::location_type& loc)
{
  int num;
  std::istringstream iss(s);
  iss >> num;
  return yy::parser::make_INTEGER (num, loc);
}

yy::parser::symbol_type
make_FLOAT (const std::string &s, const yy::parser::location_type& loc)
{
  double num;
  std::istringstream iss(s);
  iss >> num;
  return yy::parser::make_FLOAT (num, loc);
}

yy::parser::symbol_type
make_BOOL (const std::string &s, const yy::parser::location_type& loc)
{
  return s=="true" ? yy::parser::make_BOOL (true, loc) : yy::parser::make_BOOL (false, loc);
}

void
driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
      exit (EXIT_FAILURE);
    }
}

void
driver::scan_end ()
{
  fclose (yyin);
}
