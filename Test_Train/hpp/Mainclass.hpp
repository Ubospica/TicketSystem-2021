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
        Order_op order_op;
        Train_manager train_op;
        Log_op log_op;
        void buy_ticket(Cmd_Que * cmd,std::ostream & os);
        void query_ticket(Cmd_Que * cmd,std::ostream & os);
        void refund_ticket(Cmd_Que * cmd,std::ostream & os);

        Ticket::Date stringtodate(const std::string & str) ;
    //    Ticket::Date stringtodate(const Ticket::String & str);
        Ticket::Time stringtotime(const std::string & str);
    //    Ticket::Time stringtotime(const Ticket::String & str);
        int stringtoint(const std::string & str);
    //    int stringtoint(const Ticket::String & str);
    public:
        explicit Main():log_op(_BPT_user_name),
        train_op(_BPT_Train_name,_BPT_station_name,_BPT_Rl_name,_BPT_seat_name),
        order_op(_BPT_order_name,_BPT_queue_name){};
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
    }
    //Ticket::Date Main::stringtodate(const Ticket::String str){};
    Ticket::Time Main::stringtotime(const std::string & str){}
    //Ticket::Time Main::stringtotime(const Ticket::String str){};

    void Main::OP(Backend::Cmd_Que *cmdQue,std::ostream & os) {
        const std::string todo = cmdQue->top();
        cmdQue->pop();
    }

    //订单部分

    void Main::buy_ticket(Cmd_Que * cmd,std::ostream & os){
        std::string name=cmd->top();
        cmd->pop();
        if(log_op.is_log(name)) {
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
                Start_Date.dd = dd;
                Start_Date.mm = mm;
                break;
            }
            int nums = stringtoint(cmd->top());
            Ticket::String<40> Sta = cmd->top();
            cmd->pop();
            Ticket::String<40> End = cmd->top();
            cmd->pop();
            bool state;
            if (cmd->top() == "false") state = false;
            else state = true;
            int sta, end, seat, price;
            train_op.GetSeat(train_ID, Start_Date, End_Date, Sta, End, sta, end, seat, price, nums);
            if (seat == -1 && !state) os << '-' << '1' << '\n';
            else if (seat == -1 && state) {
                order_op.buy_ticket(name, train_ID, Start_Date, End_Date, Sta, End, sta, end, nums, price, state);
                os << "queue" << '\n';
            } else {
                order_op.buy_ticket(name, train_ID, Start_Date, End_Date, Sta, End, sta, end, nums, price, state);
                train_op.RenewSeat(train_ID, Start_Date, Sta, End, -seat);
                long long ans = seat * price;
                os << ans << '\n';
            }
        }
        else os<<'-'<<'1';
    }
    void Main::query_ticket(Cmd_Que * cmd,std::ostream & os){
        Ticket::String<20> name=cmd->top();
        if(log_op.is_log(name)) {
            order_op.query_order(cmd->top(), os);
            cmd->pop();
        }
        else os<<'-'<<'1'<<'\n';
    }
    void Main::refund_ticket(Cmd_Que * cmd,std::ostream & os){
        Ticket::String<20> name=cmd->top();
        cmd->pop();
        if(log_op.is_log(name)) {
            int nums = stringtoint(cmd->top());
            cmd->pop();
            std::vector<order> TrainOrdervec;
            std::vector<OrderKey> Renewvec;
            Ticket::String<20> Train_ID;
            order Success;
            int sz;
            char type;
            if (order_op.refund(Train_ID, name, nums, Success, TrainOrdervec, type)) {
                if (type == 'P') std::cout << '0' << '\n';
                else {
                    sz = TrainOrdervec.size();
                    int seat, sta, end, price;
                    for (int i = 0; i < sz; i++) {
                        Ticket::Date Start_Time = TrainOrdervec[i].get_Date(order_parameter::Start_Date);
                        Ticket::Date End_Time = TrainOrdervec[i].get_Date(order_parameter::End_Date);
                        Ticket::String<40> Sta = TrainOrdervec[i].get_station(order_parameter::Start_Position);
                        Ticket::String<40> End = TrainOrdervec[i].get_station(order_parameter::End_Position);
                        train_op.GetSeat(Train_ID, Start_Time, End_Time, Sta, End, sta, end, seat, price,
                                         TrainOrdervec[i].get_num(order_parameter::Num));
                        if (seat == -1) break;
                        OrderKey orderKeytmp;
                        orderKeytmp.str = TrainOrdervec[i].get_str(order_parameter::Username);
                        orderKeytmp.SN = TrainOrdervec[i].get_num(order_parameter::SN);
                        Renewvec.push_back(orderKeytmp);
                    }
                    order_op.renew(Renewvec);
                    os << '0' << '\n';
                }
            } else os << '-' << '1' << '\n';
        }
        else os<<'-'<<'1';
        /*
         *
         */
    }
}

#endif