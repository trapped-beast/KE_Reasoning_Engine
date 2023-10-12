#include "Rete_Network.hh"


// 对 term 进行求值
shared_ptr<Individual> eval(shared_ptr<Term> term, shared_ptr<Fact> fact){
    #ifndef NDEBUG
        cout<<"当前求值的 Term 为: "<<*term<<endl;
        cout<<"当前求值的 fact 为: "<<*fact<<endl;
    #endif
    string oprt = term->oprt;
    if(oprt=="Is_In_Form"){
        assert(term->args.size()==2);
        if(term->args[1]->get_output_str()=="x^2/a^2+y^2/b^2==1"){
            // TODO:实现
            return make_shared<Individual>(true);
        }
    }
    
    return nullptr;
}
