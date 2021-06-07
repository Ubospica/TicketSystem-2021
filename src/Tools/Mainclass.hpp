#ifndef BACKEND_MAINCLASS_HPP
#define BACKEND_MAINCLASS_HPP
#include<iostream>
#include"command.hpp"
#include"Time.hpp"
#include"Order.hpp"
#include"Train.hpp"
#include"Logging.hpp"
namespace Backend {
    class Main {
    private:

        std::string _BPT_order_name = "order";
        std::string _BPT_Train_name = "train";
        std::string _BPT_user_name = "user";
        std::string _BPT_station_name = "station";
        std::string _BPT_queue_name="queue";
        std::string _BPT_Rl_name="Rl";
        std::string _BPT_seat_name="Seat";
        std::string _Count_name="Name_Count.dat";
        std::string _BPT_Train_Index_name="trainindex";
        Order_op order_op;
        Train_manager train_op;
        Log_op log_op;
        void buy_ticket(Cmd_Que * cmd,std::ostream & os);
        void query_order(Cmd_Que * cmd,std::ostream & os);
        void refund_ticket(Cmd_Que * cmd,std::ostream & os);
        void release_train(Cmd_Que * cmd,std::ostream & os);
        void delete_train(Cmd_Que * cmd,std::ostream & os);
        void query_transfer(Cmd_Que * cmd,std::ostream & os);
        void query_ticket(Cmd_Que * cmd,std::ostream & os);
        void add_train(Cmd_Que * cmd,std::ostream & os);
        void query_train(Cmd_Que * cmd,std::ostream & os);
        void login(Backend::Cmd_Que *cmdQue,std::ostream & os);
        void logout(Backend::Cmd_Que *cmdQue,std::ostream & os);
        void modifyprofile(Backend::Cmd_Que *cmdQue,std::ostream & os);
        void adduser(Backend::Cmd_Que *cmdQue,std::ostream & os);
        void queryprofile(Backend::Cmd_Que *cmdQue,std::ostream & os);
        void clean(Backend::Cmd_Que *cmdQue,std::ostream & os);
        void end(Backend::Cmd_Que *cmdQue,std::ostream & os);


    //    Ticket::Date stringtodate(const Ticket::String & str);
       // Ticket::Time stringtotime(const std::string & str);
    //    Ticket::Time stringtotime(const Ticket::String & str);
        int stringtoint(const std::string & str);
    //    int stringtoint(const Ticket::String & str);
    public:
        explicit Main():log_op(_BPT_user_name),
        train_op(_BPT_Train_name,_BPT_station_name,_BPT_Rl_name,_BPT_seat_name,_Count_name),
        order_op(_BPT_order_name,_BPT_queue_name,_BPT_Train_Index_name){};
        void OP(Backend::Cmd_Que *cmdQue,std::ostream & os);

        void Run(std::istream & is,std::ostream & os){
            Cmd_Que * cmd=new Cmd_Que;
            std::string todo;
            int i=0;
            try {
                while (true) {
                    //is >> todo;
                    getline(is,todo);
                    process(cmd, todo);
                  //  std::cerr<<i<<' ';
                    OP(cmd, os);
                    i++;
                    cmd->clear();

                }
            }catch(End){}
            delete cmd;
        }
        ~Main()=default;
    };
}
namespace Backend {

    //void Main::initialize() {}
    // int Main::stringtoint(const std::string str){};

    int Main::stringtoint(const std::string &str) {
        int nums = 0;
        for (int i = 0; i < str.size(); i++) {
            nums *= 10;
            nums += (str[i] - '0');
        }
        return nums;
    }


    //void add_train(Backend::Cmd_Que *cmdQue);

    //Ticket::Date Main::stringtodate(const Ticket::String str){};
    //Ticket::Time Main::stringtotime(const std::string &str) {}
    //Ticket::Time Main::stringtotime(const Ticket::String str){};

    void Main::OP(Backend::Cmd_Que *cmdQue, std::ostream &os) {
        const std::string todo = cmdQue->top();
        cmdQue->pop();
        if (todo == "query_ticket") query_ticket(cmdQue, os);
        else if (todo == "add_train") add_train(cmdQue, os);
        else if (todo == "query_train") query_train(cmdQue, os);
        else if (todo == "release_train") release_train(cmdQue, os);
        else if (todo == "delete_train") delete_train(cmdQue, os);
        else if (todo == "query_transfer") query_transfer(cmdQue, os);
        else if (todo == "login") { login(cmdQue, os); }
        else if (todo == "logout") { logout(cmdQue, os); }
        else if (todo == "modify_profile") { modifyprofile(cmdQue, os); }
        else if (todo == "add_user") { adduser(cmdQue, os); }
        else if (todo == "query_profile") { queryprofile(cmdQue, os); }
        else if (todo == "query_order") { query_order(cmdQue, os); }
        else if (todo == "refund_ticket") { refund_ticket(cmdQue, os); }
        else if (todo == "buy_ticket") { buy_ticket(cmdQue, os); }
        else if(todo=="clean") {clean(cmdQue,os);}
        else if(todo=="exit"){end(cmdQue,os);}
        else {
           // std::cout << "??????????????????????????" << '\n';
           // std::cout << todo << '\n';
           // std::cerr << "OP" << '\n';
            //throw Ticket::WrongOperation();
        }
    }

    void Main::login(Backend::Cmd_Que *cmdQue, std::ostream &os) {
        std::string tmp[2];
        //cmdQue->print();
        for (int i = 0; i < 2; i++) {
            tmp[i] = cmdQue->top();
            cmdQue->pop();
        }
        // std::cout<<"?"<<"\n";
        if (log_op.login(Ticket::hash(tmp[0]), tmp[1])) os << '0' << '\n';
        else os << '-' << '1' << '\n';
    }

    void Main::queryprofile(Backend::Cmd_Que *cmdQue, std::ostream &os) {
        size_t tmp[2];
        //cmdQue->print();
        for (int i = 0; i < 2; i++) {
            tmp[i] = Ticket::hash(cmdQue->top());
            cmdQue->pop();
        }
        if (log_op.show_user(tmp[0], tmp[1], os)) {}
        else os << '-' << '1' << '\n';
    }

    void Main::logout(Backend::Cmd_Que *cmdQue, std::ostream &os) {
        if (log_op.logout(Ticket::hash(cmdQue->top()))) os << '0' << '\n';
        else os << '-' << '1' << '\n';
    }

    void Main::modifyprofile(Backend::Cmd_Que *cmdQue, std::ostream &os) {
        size_t c, u;
        std::string strs[3];
        bool kind[4];
        int pri = -1;
        //  cmdQue->print();
        c = Ticket::hash(cmdQue->top());
        cmdQue->pop();
        u = Ticket::hash(cmdQue->top());
        cmdQue->pop();
        int sz = cmdQue->size();
        for (int i = 0; i < 4; i++) kind[i] = false;
        for (int i = 0; i < sz; i++) {
            if (cmdQue->top_type() == 'p') {
                kind[0] = true;
                strs[0] = cmdQue->top();
                cmdQue->pop();
            } else if (cmdQue->top_type() == 'n') {
                kind[1] = true;
                strs[1] = cmdQue->top();
                cmdQue->pop();
            } else if (cmdQue->top_type() == 'm') {
                kind[2] = true;
                strs[2] = cmdQue->top();
                cmdQue->pop();
            } else if (cmdQue->top_type() == 'g') {
                kind[3] = true;
                //  std::cout<<'!'<<'\n';
                pri = stringtoint(cmdQue->top());
                // std::cout<<pri<<"\n";
                cmdQue->pop();
            } else {
                std::cerr << "Main::Modify";
                throw Ticket::WrongOperation();
            }
        }
        // for(int i=0;i<4;i++)
        //   if(kind[i]) std::cout<<i<<"\n";
        if (log_op.modify(c, kind, u, strs, pri, os)) {}
        else os << '-' << '1' << '\n';
    }

    void Main::adduser(Backend::Cmd_Que *cmdQue, std::ostream &os) {
        std::string tmp[5];
        //cmdQue->print();
        for (int i = 0; i < 5; i++) {
            tmp[i] = cmdQue->top();
            cmdQue->pop();
        }
        int pri = stringtoint(cmdQue->top());
        if (log_op.add_user(Ticket::hash(tmp[0]), tmp[1], tmp[2], tmp[3], tmp[4], pri)) os << '0' << '\n';
        else os << '-' << '1' << '\n';
    }


    //火车部分
    void Main::query_train(Cmd_Que *cmd, std::ostream &os) {

        std::string tmp = cmd->top();
        cmd->pop();
        Ticket::Date Date;
        while (true) {
            int mm = 0, dd = 0, x = 0;
            for (int i = 0; i < 2; ++i, ++x) {
                mm *= 10;
                mm += tmp[x] - '0';
            }
            ++x;
            for (int i = 0; i < 2; ++i, ++x) {
                dd *= 10;
                dd += tmp[x] - '0';
            }
            Ticket::Date Dtmp(mm,dd,0,0);
            Date=Dtmp;
            break;
        }
        Ticket::String<24> Train_ID = cmd->top();
        cmd->pop();
        if (train_op.query_train(hash(Train_ID), Date, os)) {}
        else os << '-' << '1' << '\n';
    }

    void Main::query_ticket(Cmd_Que *cmd, std::ostream &os) {
        Ticket::String<36> Sta = cmd->top();
        cmd->pop();
        Ticket::String<36> End = cmd->top();
        cmd->pop();
        Ticket::Date Date;
        std::string tmp = cmd->top();
        cmd->pop();
        while (true) {
            int mm = 0, dd = 0, x = 0;
            for (int i = 0; i < 2; ++i, ++x) {
                mm *= 10;
                mm += tmp[x] - '0';
            }
            ++x;
            for (int i = 0; i < 2; ++i, ++x) {
                dd *= 10;
                dd += tmp[x] - '0';
            }

            Ticket::Date Dtmp(mm,dd,0,0);
            Date=Dtmp;
            break;
        }
        char type;
        if (cmd->top()[0] == 't') type = 'T';
        else type = 'P';

        if (train_op.query_ticket(Sta, End, Date, type, os)) {}
        else os <<'0' << '\n';
    }

    void Main::query_transfer(Cmd_Que *cmd, std::ostream &os) {
        Ticket::String<36> Sta = cmd->top();
        cmd->pop();
        Ticket::String<36> End = cmd->top();
        cmd->pop();
        Ticket::Date Date;
        std::string tmp = cmd->top();
        cmd->pop();
        while (true) {
            int mm = 0, dd = 0, x = 0;
            for (int i = 0; i < 2; ++i, ++x) {
                mm *= 10;
                mm += tmp[x] - '0';
            }
            ++x;
            for (int i = 0; i < 2; ++i, ++x) {
                dd *= 10;
                dd += tmp[x] - '0';
            }
            Ticket::Date Dtmp(mm,dd,0,0);
            Date=Dtmp;
            break;
        }
        char type;
        if (cmd->top()[0] == 't') type = 'T';
        else type = 'P';
        if (train_op.query_transfer(Sta, End, Date, type, os)) {}
        else os<<'0'<<'\n';
    }

    void Main::release_train(Cmd_Que *cmd, std::ostream &os) {
        Ticket::String<24> Train_ID = cmd->top();
        cmd->pop();
        if (train_op.release_train(hash(Train_ID))) os << '0' << '\n';
        else os << '-' << '1' << '\n';
    }

    void Main::delete_train(Cmd_Que *cmd, std::ostream &os) {
        Ticket::String<24> Train_ID = cmd->top();
        cmd->pop();
        if (train_op.delete_train(hash(Train_ID))) os << '0' << '\n';
        else os << '-' << '1' << '\n';
    }

    void Main::add_train(Cmd_Que *cmd, std::ostream &os) {
        Ticket::String<24> Train_ID = cmd->top();
        cmd->pop();
        int station_num = stringtoint(cmd->top());
        cmd->pop();
        int seat_num = stringtoint(cmd->top());
        if(seat_num==0) {os<<'-'<<'1'<<'\n';return;}
        cmd->pop();
        Ticket::String<36> Stations[station_num];
        int price[station_num];
        int traveltimes[station_num];
        int stoppover[station_num];
        price[0] = traveltimes[0] = stoppover[0] = stoppover[station_num - 1] = 0;
        std::string tmp = cmd->top();
        cmd->pop();
        int pointer = 0;
        //int sz=tmp.size();
        std::string sub;
        //站名
        for (int i = 0; i < station_num; i++) {
            while (tmp[pointer] != '\0' && tmp[pointer] != '|') {
                sub += tmp[pointer];
                pointer++;
            }
            Stations[i] = sub, pointer++;
            sub = "";
        }
        tmp = cmd->top();
        cmd->pop();
        pointer = 0;
        //int sz=tmp.size();
        int sum = 0;
        for (int i = 1; i < station_num; i++) {
            while (tmp[pointer] != '\0' && tmp[pointer] != '|') {
                sum *= 10;
                sum += tmp[pointer] - '0';
                pointer++;
            }
            price[i] = sum, pointer++;
            sum = 0;
        }
        tmp = cmd->top();
        cmd->pop();
        pointer = 0;
        Ticket::Date start_time;
        while (true) {
            int mi = 0, hr = 0;
            for (int i = 0; i < 2; ++i, ++pointer) {
                hr *= 10;
                hr += tmp[pointer] - '0';
            }
            ++pointer;
            for (int i = 0; i < 2; ++i, ++pointer) {
                mi *= 10;
                mi += tmp[pointer] - '0';
            }
            Ticket::Date Dtmp(0,0,hr,mi);
            start_time=Dtmp;
     /*       start_time.mi = mi;
            start_time.hr = hr;*/
            break;
        }
        tmp = cmd->top();
        cmd->pop();
        pointer = 0;
        //int sz=tmp.size();
        sum = 0;
        for (int i = 1; i < station_num; i++) {
            while (tmp[pointer] != '\0' && tmp[pointer] != '|') {
                sum *= 10;
                sum += tmp[pointer] - '0';
                pointer++;
            }
            traveltimes[i] = sum, pointer++;
            sum = 0;
        }
        tmp = cmd->top();
        cmd->pop();
        pointer = 0;
        //int sz=tmp.size();
        sum = 0;
        for (int i = 1; i < station_num - 1; i++) {
            while (tmp[pointer] != '\0' && tmp[pointer] != '|') {
                sum *= 10;
                sum += tmp[pointer] - '0';
                pointer++;
            }
            stoppover[i] = sum, pointer++;
            sum = 0;
        }
        Ticket::Date Date[2];
        tmp = cmd->top();
        cmd->pop();
        pointer = 0;
        while (true) {
            int mm = 0, dd = 0;
            for (int i = 0; i < 2; ++i, ++pointer) {
                mm *= 10;
                mm += tmp[pointer] - '0';
            }
            ++pointer;
            for (int i = 0; i < 2; ++i, ++pointer) {
                dd *= 10;
                dd += tmp[pointer] - '0';
            }
            Ticket::Date Dtmp(mm,dd,0,0);
            Date[0]=Dtmp;
      /*      Date[0].mm = mm;
            Date[0].dd = dd;*/
            mm = dd = 0;
            ++pointer;
            for (int i = 0; i < 2; ++i, ++pointer) {
                mm *= 10;
                mm += tmp[pointer] - '0';
            }
            ++pointer;
            for (int i = 0; i < 2; ++i, ++pointer) {
                dd *= 10;
                dd += tmp[pointer] - '0';
            }
            Ticket::Date Dtmp2(mm,dd,0,0);
            Date[1]=Dtmp2;
       /*     Date[1].mm = mm;
            Date[1].dd = dd;*/
            break;
        }
        char type = cmd->top()[0];
        if (train_op.add_train(Train_ID, station_num, seat_num, Stations, price, start_time, traveltimes, stoppover,
                               Date, type))
            os << '0' << '\n';
        else os << '-' << '1' << '\n';
    }

    //订单部分

    void Main::buy_ticket(Cmd_Que *cmd, std::ostream &os) {
       // cmd->print();
        std::string name = cmd->top();
        cmd->pop();
        if (log_op.is_log(Ticket::hash(name))) {
            std::string train_ID = cmd->top();
            cmd->pop();
            Ticket::Date Start_Date;
            Ticket::Date End_Date;
            std::string tmp = cmd->top();
            cmd->pop();
            while (true) {
                int mm = 0, dd = 0, x = 0;
                for (int i = 0; i < 2; ++i, ++x) {
                    mm *= 10;
                    mm += tmp[x] - '0';
                }
                ++x;
                for (int i = 0; i < 2; ++i, ++x) {
                    dd *= 10;
                    dd += tmp[x] - '0';
                }
                Ticket::Date Dtmp(mm,dd,0,0);
                Start_Date=Dtmp;
            /*    Start_Date.dd = dd;
                Start_Date.mm = mm;*/
                break;
            }
            int nums = stringtoint(cmd->top());
            cmd->pop();
            Ticket::String<36> Sta = cmd->top();
            cmd->pop();
            Ticket::String<36> End = cmd->top();
            cmd->pop();
            bool state;
            if (cmd->top() == "false") state = false;
            else state = true;
            int sta, end, seat, price;
            //Start_Date带分秒
            Train_manager::Train data;
            if(nums==0) {os<<'-'<<'1'<<'\n';return;}
            if(train_op.GetTrain(data,train_ID)) {
                //这里的Start_Date不带分秒
                train_op.BuyTicekt(data, Start_Date, End_Date, Sta, End, sta, end, seat, price, nums);
                //train_op.GetSeat();
                //这里的Start_Date带分秒
                if (seat == -2 || (seat == -1 && !state)) os << '-' << '1' << '\n';
                else if (seat == -1 && state) {
                    order_op.buy_ticket(name, train_ID, Start_Date, End_Date, Sta, End, sta, end, nums, price, true);
                    os << "queue" << '\n';
                } else {
                    order_op.buy_ticket(name, train_ID, Start_Date, End_Date, Sta, End, sta, end, nums, price, false);
                    //  std::cout<<train_ID<<' '<<Start_Date<<' '<<sta<<' '<<end<<'\n';
                  //  train_op.RenewSeat(data, Start_Date, sta, end, -nums);
                    long long ans = nums * price;
                    os << ans << '\n';
                }
            }
            else os<<'-'<<'1'<<'\n';
        }
        else os << '-' << '1'<<'\n';

    }

    void Main::query_order(Cmd_Que *cmd, std::ostream &os) {
        Ticket::String<24> name = cmd->top();
        if (log_op.is_log(Ticket::hash(name))) {
            order_op.query_order(cmd->top(), os);
            cmd->pop();
        } else os << '-' << '1' << '\n';
    }

    void Main::refund_ticket(Cmd_Que *cmd, std::ostream &os) {
        Ticket::String<24> name = cmd->top();
        cmd->pop();
        if (log_op.is_log(Ticket::hash(name))) {
            int nums = stringtoint(cmd->top());
            cmd->pop();
            std::vector<order> TrainOrdervec;
            std::vector<OrderKey> Renewvec;
            Ticket::String<24> Train_ID;
            order Success;
            char type;
          //  std::cout<<'5'<<'\n';
            if (order_op.refund(Train_ID, name, nums, Success, TrainOrdervec, type)) {
                if (type == 'P') os << '0' << '\n';
                else {
                    Train_manager::Train data;
                    train_op.GetTrain(data,Success.get_str(order_parameter::Train_ID));
                    train_op.RenewN(data,TrainOrdervec,Renewvec,Success);
                    order_op.renew(Renewvec,Train_ID);
                    os << '0' << '\n';
                }
            }
            else os << '-' << '1' << '\n';
        }
        else os << '-' << '1'<<'\n';
        /*
         *
         */
    }

    void Main::clean(Backend::Cmd_Que *cmdQue, std::ostream &os) {
        log_op.clean();
        train_op.clean();
        order_op.clean();
        os<<'0'<<'\n';
    }

    void Main::end(Backend::Cmd_Que *cmdQue, std::ostream &os) {
        os<<"bye"<<'\n';
        throw End();
    }
}

#endif