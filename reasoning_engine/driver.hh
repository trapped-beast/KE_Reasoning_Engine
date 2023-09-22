#ifndef DRIVER_HH
#define DRIVER_HH

// Driver 类包含语法分析所需的信息，并且存储解析后的AST数据

#include <string>
#include <map>
#include "parser.hh"

// 把我们想要的 yylex 的 prototype 传给Flex
# define YY_DECL \
  yy::parser::symbol_type yylex (driver& drv)
YY_DECL;

// 引导 solver 的整个词法分析和语法分析，包含其所需上下文
class driver
{
public:
  driver ();
  int parse (const std::string& f); // 解析文件，成功返回0


  shared_ptr<Knowledge_Base> kb; // 知识库
  vector<shared_ptr<Question>> questions; // 问题信息

  int result; // 解析是否成功
  std::string file; // 要解析的文件路径
  bool trace_parsing; // 是否生成 parser debug traces
  void scan_begin (); // 处理scanner
  void scan_end ();
  bool trace_scanning; // 是否生成 scanner debug traces
  yy::location location; // 是否生成 parser debug traces
};
#endif
