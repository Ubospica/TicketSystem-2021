#include <iostream>
#include"Mainclass.hpp"
#include<string>
using namespace std;
int cur=3;
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
void Add_Train(){
    Backend::Main test;
    Backend::Cmd_Que * cmdQue=new Backend::Cmd_Que;
    for(int i=0;i<100;i++) {
        //std::cout<<i<<"\n";
        string todo = new_train(i);
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
        //cur++;
    }
    for(int i=0;i<100;i++) {
        //std::cout<<i<<"\n";
        string todo = new_train(i);
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
    }
    delete cmdQue;
}
void Release_Train(){
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
        string todo="release_train -c ";
        todo+=to_string(i);
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
    }
    for(int i=100;i<200;i++){
        string todo="release_train -c ";
        todo+=to_string(i);
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
    }
    delete cmdQue;
};
void Delete_Train(){
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
    std::cout<<"-------------"<<'\n';
   /* for(int i=0;i<100;i++) {
        // std::cout<<i<<"\n";
        string todo = new_train(i);
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
        // cur++;
    }*/
    for(int i=0;i<100;i++){
        string todo="delete_train -c ";
        todo+=to_string(i);
        Backend::process(cmdQue,todo);
        test.OP(cmdQue,std::cout);
        cmdQue->clear();
    }
    for(int i=0;i<100;i++) {
        // std::cout<<i<<"\n";
        string todo = new_train(i);
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
        // cur++;
    }
    delete cmdQue;
}
void Query_Train(){
    Backend::Main test;
    Backend::Cmd_Que * cmdQue=new Backend::Cmd_Que;
    for(int i=0;i<1000;i++) {
        // std::cout<<i<<"\n";
        string todo = new_train(i);
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
        cur++;
        // cur++;
    }
    std::cout<<"-----------------"<<'\n';
/*    for(int i=0;i<100;i++) {
        // std::cout<<i<<"\n";
        string todo = "query_train -c ";
        todo+=std::to_string(i);
        todo+=" -d ";
        todo+="06-30";
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
    }*/
    std::cout<<"-----------------"<<'\n';
//    std::cout<<"-----------------"<<'\n';
    for(int i=0;i<1000;i++){
        string todo="release_train -c ";
        todo+=to_string(i);
        Backend::process(cmdQue,todo);
        test.OP(cmdQue,std::cout);
        cmdQue->clear();
    }
/*    std::cout<<"-----------------"<<'\n';
    for(int i=0;i<10;i++){
        string todo="delete_train -c ";
        todo+=to_string(i);
        Backend::process(cmdQue,todo);
        test.OP(cmdQue,std::cout);
        cmdQue->clear();
    }*/
/*    std::cout<<"-----------------"<<'\n';
    for(int i=4;i<=100;i++){
        string todo="delete_train -c ";
        todo+=to_string(i);
        Backend::process(cmdQue,todo);
        test.OP(cmdQue,std::cout);
        cmdQue->clear();
    }*/
    std::cout<<"-----------------"<<'\n';
    for(int i=0;i<1000;i++) {
        // std::cout<<i<<"\n";
        string todo = "query_train -c ";
        todo+=std::to_string(i);
        todo+=" -d ";
        todo+="06-30";
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
    }
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
        cur++;
        // cur++;
    }
    for(int i=0;i<100;i++){
        std::cout<<'i'<<'\n';
        string todo="release_train -c ";
        todo+=to_string(i);
        Backend::process(cmdQue,todo);
        test.OP(cmdQue,std::cout);
        cmdQue->clear();
    }
    for(int i=0;i<=0;i++) {
        // std::cout<<i<<"\n";
        string todo = "query_ticket -s 1 -t 4";
        todo+=" -d ";
        todo+="06-30";
        Backend::process(cmdQue, todo);
        test.OP(cmdQue, std::cout);
        cmdQue->clear();
    }
    delete cmdQue;
}
int main(){
    //Add_Train();
    //Release_Train();
    //Delete_Train();
    //Query_Train();
    Query_Ticket();
    return 0;
}
