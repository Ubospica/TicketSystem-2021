#include <iostream>
#include"Mainclass.hpp"
#include<string>
using namespace std;
string new_train(int n){
    string ret;
    ret+="add_train ";
    ret+="-c ";

    //ID
    ret+=std::to_string(n);
    //ret+="Train_ID "
    ret+=' ';
    ret+="-c ";
    n=99;
    ret+=std::to_string(n);
    ret+=' ';
    ret+="-c ";
    ret+="100 ";
    string tmp;
    //地点
    ret+="-c ";


    for(int i=1;i<=n-1;i++){
        ret+=std::to_string(i);
        ret+='|';
    }
    ret+=std::to_string(n);
    ret+=' ';
    //价格
    ret+="-c ";


    for(int i=1;i<=n-2;i++){
        ret+=std::to_string(i);
        ret+='|';
    }
    ret+=std::to_string(n-1);
    ret+=' ';
    //
    ret+="-c ";

    ret+="23:57";
    ret+=' ';
    ret+="-c ";


    //旅行时间
    for(int i=1;i<=n-2;i++){
        ret+=std::to_string(i);
        ret+='|';
    }
    ret+=std::to_string(n-1);
    ret+=' ';
    ret+="-c ";

    //停靠时间
    for(int i=1;i<=n-3;i++){
        ret+=std::to_string(i);
        ret+='|';
    }
    ret+=std::to_string(n-2);
    ret+=' ';
    ret+="-c ";

    //
    ret+="06-30|07-02";
    ret+=' ';
    ret+="-c ";

    ret+='A';
    return ret;
}
void Query_Ticket(){
    Backend::Main test;
    Backend::Cmd_Que * cmdQue=new Backend::Cmd_Que;
    for(int i=0;i<100;i++) {
        // std::cout<<i<<"\n";
        string todo = new_train(i);
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
        // cur++;
    }
    for(int i=0;i<100;i++){
        //std::cout<<'i'<<'\n';
        string todo="release_train -c ";
        todo+=to_string(i);
        Backend::process(cmdQue,todo);
        test.OP(cmdQue,std::cout);
        cmdQue->clear();
    }
    for(int i=0;i<=0;i++) {
        // std::cout<<i<<"\n";
        string todo = "query_ticket -s 4 -t 6";
        todo+=" -d ";
        todo+="07-03";
        todo+=" -p price";
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
    }
    delete cmdQue;
}
int main(){
	std::ios::sync_with_stdio(0), std::cin.tie(0), std::cout.tie(0);
//	freopen("a.in", "r", stdin);
//	freopen("a.out", "w", stdout);
    Backend::Main OP;
//    其中第一个参数为输入流，第二个参数为输出流
    OP.Run(std::cin,std::cout);
    return 0;
//    Query_Ticket();
}
