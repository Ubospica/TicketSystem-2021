#ifndef BACKEND_MAINCLASS_HPP
#define BACKEND_MAINCLASS_HPP
#include<iostream>
#include"command.hpp"
#include"Logging.hpp"
#include"Time.hpp"
//#include"Train.hpp"
namespace Backend {
    class Main {
    private:

        std::string _BPT_order_name = "order";
        std::string _BPT_Train_name = "train";
        std::string _BPT_user_name = "user";
        std::string _BPT_station_name = "station";
        Log_op logOp;

        void login(Backend::Cmd_Que *cmdQue,std::ostream & os);
        void logout(Backend::Cmd_Que *cmdQue,std::ostream & os);
        void modifyprofile(Backend::Cmd_Que *cmdQue,std::ostream & os);
        void adduser(Backend::Cmd_Que *cmdQue,std::ostream & os);
        void queryprofile(Backend::Cmd_Que *cmdQue,std::ostream & os);
        void add_train(Backend::Cmd_Que *cmdQue,std::ostream & os);
        bool addtrain(const Ticket::String & SN,int stanum,int seatnum,Ticket::String * stations, int * price,Ticket::Time sta_time,int * traveltime,int * stoppovertime,Ticket::Date * saleDate,char type);
            Ticket::Date stringtodate(const std::string & str) ;
    //    Ticket::Date stringtodate(const Ticket::String & str);
        Ticket::Time stringtotime(const std::string & str);
    //    Ticket::Time stringtotime(const Ticket::String & str);
        int stringtoint(const std::string & str);
    //    int stringtoint(const Ticket::String & str);
    public:
        explicit Main():logOp(_BPT_user_name){};
        std::string BPT_order_name();
        std::string BPT_Train_name();
        std::string BPT_user_name();
        std::string BPT_station_name();
        void initialize(){};
        void OP(Backend::Cmd_Que *cmdQue,std::ostream & os);
    };
}
namespace Backend {

    std::string Main::BPT_order_name() {
        return _BPT_order_name;
    }

    std::string Main::BPT_Train_name() {
        return _BPT_Train_name;
    }

    std::string Main::BPT_user_name() {
        return _BPT_user_name;
    }

    std::string Main::BPT_station_name() {
        return _BPT_station_name;
    }

    //void Main::initialize() {}
    bool Main:: addtrain(const Ticket::String & SN,int stanum,int seatnum,Ticket::String * stations, int * price,Ticket::Time sta_time,int * traveltime,int * stoppovertime,Ticket::Date * saleDate,char type){};
    // int Main::stringtoint(const std::string str){};

    int Main::stringtoint(const std::string & str){
        int nums=0;
        for(int i=0;i<str.size();i++){
            nums*=10;
            nums+=(str[i]-'0');
        }
        return nums;
    }


    //void add_train(Backend::Cmd_Que *cmdQue);
    Ticket::Date Main::stringtodate(const std::string & str){
    };
    //Ticket::Date Main::stringtodate(const Ticket::String str){};
    Ticket::Time Main::stringtotime(const std::string & str){};
    //Ticket::Time Main::stringtotime(const Ticket::String str){};

    void Main::OP(Backend::Cmd_Que *cmdQue,std::ostream & os) {
        const std::string todo = cmdQue->top();
        cmdQue->pop();
        if (todo == "login") {login(cmdQue,os);}
        else if (todo == "logout") {logout(cmdQue,os);}
        else if (todo == "modify_profile") {modifyprofile(cmdQue,os);}
        else if (todo == "add_user") {adduser(cmdQue,os);}
        else if(todo=="query_profile"){queryprofile(cmdQue,os);}
    }

    //登录部分
    void Main::login(Backend::Cmd_Que *cmdQue,std::ostream & os){
        Ticket::String tmp[2];
        //cmdQue->print();
        for(int i=0;i<2;i++){
            tmp[i]=cmdQue->top();
            cmdQue->pop();
        }
       // std::cout<<"?"<<"\n";
        if(logOp.login(tmp[0],tmp[1])) os<<'0'<<'\n';
        else os<<'-'<<'1'<<'\n';
    }

    void Main::queryprofile(Backend::Cmd_Que *cmdQue,std::ostream & os){
        Ticket::String tmp[2];
        //cmdQue->print();
        for(int i=0;i<2;i++){
            tmp[i]=cmdQue->top();
            cmdQue->pop();
        }
        if(logOp.show_user(tmp[0],tmp[1],os)){}
        else os<<'-'<<'1'<<'\n';
    }

    void Main::logout(Backend::Cmd_Que *cmdQue,std::ostream & os) {
        if(logOp.logout(cmdQue->top())) os<<'0'<<'\n';
        else os<<'-'<<'1'<<'\n';
    }

    void Main::modifyprofile(Backend::Cmd_Que *cmdQue,std::ostream & os) {
        Ticket::String c,u;
        Ticket::String strs[3];
        bool kind[4];
        int pri=-1;
      //  cmdQue->print();
        c=cmdQue->top();
        cmdQue->pop();
        u=cmdQue->top();
        cmdQue->pop();
        int sz=cmdQue->size();
        for(int i=0;i<4;i++) kind[i]=false;
        for(int i=0;i<sz;i++){
            if(cmdQue->top_type()=='p'){
                kind[0]=true;
                strs[0]=cmdQue->top();
                cmdQue->pop();
            }
            else if(cmdQue->top_type()=='n'){
                kind[1]=true;
                strs[1]=cmdQue->top();
                cmdQue->pop();
            }
            else if(cmdQue->top_type()=='m'){
                kind[2]=true;
                strs[2]=cmdQue->top();
                cmdQue->pop();
            }
            else if(cmdQue->top_type()=='g'){
                kind[3]=true;
              //  std::cout<<'!'<<'\n';
                pri=stringtoint(cmdQue->top());
               // std::cout<<pri<<"\n";
                cmdQue->pop();
            }
            else{
                std::cerr<<"Main::Modify";
                throw wrong_operation();
            }
        }
       // for(int i=0;i<4;i++)
         //   if(kind[i]) std::cout<<i<<"\n";
        if(logOp.modify(c,kind,u,strs,pri,os)){}
        else os<<'-'<<'1'<<'\n';
    }

    void Main::adduser(Backend::Cmd_Que *cmdQue,std::ostream & os) {
        Ticket::String tmp[5];
        //cmdQue->print();
        for(int i=0;i<5;i++){
            tmp[i]=cmdQue->top();
            cmdQue->pop();
        }
        int pri=stringtoint(cmdQue->top());
        if(logOp.add_user(tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],pri)) os<<'0'<<'\n';
        else os<<'-'<<'1'<<'\n';
    }

    //火车部分
    void Main::add_train(Backend::Cmd_Que *cmdQue,std::ostream & os){
        Ticket::String train_ID;
        int Station_num;
        train_ID=cmdQue->top();
        cmdQue->pop();
        Station_num=stringtoint(cmdQue->top());
        cmdQue->pop();
        int Seat_num;
        Ticket::String Station_name[Station_num];
        int price[Station_num];
        Ticket::Time Starttime;
        int Traveltime[Station_num];
        int Stoppover[Station_num];
        Ticket::Date SaleDate[2];
        char type;
        std::string strtmp;
        int pointer;
        Stoppover[0]=Stoppover[Station_num-1]=Traveltime[0]=price[0]=0;
        Seat_num=stringtoint(cmdQue->top());
        cmdQue->pop();
        strtmp=cmdQue->top();
        pointer=0;
        for(int i=0;i<Station_num;i++){
            std::string stdstrtmp="";
            while(strtmp[pointer]!='|'&&strtmp[pointer]!='\0') stdstrtmp+=strtmp[pointer++];
            Station_name[i]=stdstrtmp;
        }
        cmdQue->pop();
        strtmp=cmdQue->top();
        pointer=0;
        for(int i=1;i<Station_num;i++){
            int tmp=0;
            while(strtmp[pointer]!='|'&&strtmp[pointer]!='\0'){
                tmp*=10;
                tmp+=strtmp[pointer]-'0';
                pointer++;
            }
            price[i]=tmp;
            pointer++;
        }
        cmdQue->pop();
        Starttime=stringtotime(cmdQue->top());
        cmdQue->pop();
        strtmp=cmdQue->top();
        pointer=0;
        for(int i=1;i<Station_num;i++){
            int tmp=0;
            while(strtmp[pointer]!='|'&&strtmp[pointer]!='\0'){
                tmp*=10;
                tmp+=strtmp[pointer]-'0';
                pointer++;
            }
            Traveltime[i]=tmp;
            pointer++;
        }
        cmdQue->pop();
        strtmp=cmdQue->top();
        pointer=0;
        for(int i=1;i<Station_num-1;i++){
            int tmp=0;
            while(strtmp[pointer]!='|'&&strtmp[pointer]!='\0'){
                tmp*=10;
                tmp+=strtmp[pointer]-'0';
                pointer++;
            }
            Stoppover[i]=tmp;
            pointer++;
        }
        cmdQue->pop();
        strtmp=cmdQue->top();
        pointer=0;
        while(true){
            int dd=0,mm=0;
            for(int i=0;i<2;i++){
                dd*=10;
                dd+=strtmp[pointer]-'0';
                pointer++;
                SaleDate[0].dd = dd;
                dd=0;
            }
            pointer++;
            for(int i=0;i<2;i++){
                mm*=10;
                mm+=strtmp[i]-'0';
                pointer++;
                SaleDate[0].mm =mm;
                mm=0;
            }
            pointer++;
            for(int i=0;i<2;i++){
                dd*=10;
                dd+=strtmp[pointer]-'0';
                pointer++;
                SaleDate[1].dd = dd;
                dd=0;
            }
            pointer++;
            for(int i=0;i<2;i++){
                mm*=10;
                mm+=strtmp[i]-'0';
                pointer++;
                SaleDate[1].mm =mm;
                mm=0;
            }
            break;
        }
        cmdQue->pop();
        type=(cmdQue->top())[0];
        if(addtrain(train_ID,Station_num,Seat_num,Station_name,price,Starttime,Traveltime,Stoppover,SaleDate,type)) os<<'0'<<'\n';
        else os<<'-'<<'1'<<'\n';
    }
    //订单部分
}

#endif