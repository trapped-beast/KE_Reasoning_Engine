// 求解程序主函数
#include <iostream>
#include <cstdlib>
#include "driver.hh"
#include "Rete_Network.hh"
using std::string;
using std::endl;
using std::cout;

bool ke_parse(const string &,const string &,shared_ptr<Knowledge_Base> &,vector<shared_ptr<Question>> &);
shared_ptr<Rete_Network> construct_rete(const shared_ptr<Knowledge_Base>);


void test(){

}

int main (int argc, char *argv[])
{
  test();
  const string kb_name = "kb.ke"; // 知识库文件
  const string question_name = "question.ke"; // 题目信息文件
  shared_ptr<Knowledge_Base> kb;
  vector<shared_ptr<Question>> questions;
  if(!ke_parse(kb_name,question_name,kb,questions))
    return EXIT_FAILURE;
  size_t num = 1; // 当前要解的题目是第几题
  cout<<"当前例题库中的题目数是: "<<questions.size()<<endl;
  cout<<"第"<<num<<"题:"<<endl<<*questions[num-1];

  // shared_ptr<Rete_Network> rete_network = construct_rete(kb);

  return EXIT_SUCCESS;
}

// 接收知识库文件和题目信息文件的名称（均放在resource目录下），解析后数据存放在 Knowledge_Base 和 Question 对象中
bool ke_parse(const string &kb_name,const string &question_name,shared_ptr<Knowledge_Base> &kb,vector<shared_ptr<Question>> &questions){
  bool ret;
  driver drv;
  const string kb_res_path = "../resource/knowledge_base/"; // 知识库的资源文件目录
  const string ques_res_path = "../resource/questions/"; // 题目信息的资源文件目录

  const string kb_path = kb_res_path + kb_name; // 知识库文件路径
  const string question_path = ques_res_path + question_name; // 题目信息文件路径
  cout<<"开始解析知识库..."<<endl;
  if(!drv.parse(kb_path)){ // drv.parse()解析成功返回的是0
    kb = drv.kb;
    cout<<"知识库解析完成!"<<endl;
    // cout<<*kb<<endl;
    ret = true;
  }
  else{
    cout<<"知识库解析失败!"<<endl;
    ret = false;
  }
  cout<<"开始解析题目信息..."<<endl;
  if(!drv.parse(question_path)){
    questions = drv.questions;
    cout<<"题目信息解析完成!"<<endl;
    // cout<<questions<<endl;
    ret = true;
  }
  else{
    cout<<"题目信息解析失败!"<<endl;
    ret = false;
  }
  kb->get_adapted_rules(); // 改造原始规则以得到易于进行推理的规则
  kb->propagate_var_decl(); // 传播变量声明到改造后的规则
  // for(auto i:kb->rete_rules){
  //   cout<<*i<<endl;
  // }
  cout<<"开始改造原始问题以得到易于进行推理的问题..."<<endl;
  cout<<"开始传播变量声明到改造后的问题..."<<endl;
  for(auto question:questions){
    question->get_adapted_question(); // 改造原始问题以得到易于进行推理的问题
    question->propagate_var_decl(); // 传播变量声明到改造后的问题
  }
  // for(auto question:questions){
  //   cout<<*question->rete_question<<endl;
  // }

  return ret; // 知识库和题目信息均解析成功才返回true
}
