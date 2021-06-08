//typedef Ticket::String string;
#include "map.hpp"
#include "String.hpp"
#include"Exception.hpp"
#include"BPlusTree.hpp"
#include<fstream>
//#include"Linear_List.hpp"
//#include"Mainclass.cpp"
#include "FileIO.hpp"
#include"Time.hpp"
#include<vector>
namespace Backend {
    // class BPT_order;
    class order;
    // BPT_order order_op;

    // std::ofstream & operator<<(order & o,ofstream & os);
//    template<size_t Length>
   // size_t hash(const Ticket::String<Length> & str){};
    struct OrderKey{
        size_t hashstr;
      //  Ticket::String<24> str;
        int SN=0;
        bool operator<(const OrderKey & r)const{
            return hashstr<r.hashstr||(hashstr==r.hashstr&&SN<r.SN);
        }
        bool operator==(const OrderKey & r)const{
            return hashstr==r.hashstr&&SN==r.SN;
        }
        bool operator!=(const OrderKey & r)const{
            return hashstr!=r.hashstr||SN!=r.SN;
        }
    };
    enum class order_parameter {
        Username,
        Train_ID,
        Start_Date,
        End_Date,
        SN,
        Num,//Num为Ticketnum
        Price,
        Start,
        End,
        Start_Position,
        End_Position
    };
    enum class state_list {
        Null,
        Success,
        Pending,
        Refund,
        //Error
    };

    class order {
        // friend  std::ofstream & operator<<(order & o,ofstream & os);
    private:

        state_list _state;
        Ticket::String<24> user_name;
        Ticket::Date End_date;
        Ticket::Date Sta_date;//购买时间
        Ticket::String<24> train_ID;
        Ticket::String<36> Sta;
        int sta=0;
        Ticket::String<36> Det;
        int det=0;
        int ticket_num=0;
        int price=0;
        int serial_number=0;
    public:
        order() {
            _state = state_list::Null;
            //   serial_number = ++order_op._order_num;
        }

        void set_num(order_parameter kind, int num) {
            switch (kind) {
                case (order_parameter::Num):
                    ticket_num = num;
                    break;
                case (order_parameter::SN):
                    serial_number = num;
                    break;
                case (order_parameter::Price):
                    price = num;
                    break;
                case(order_parameter::Start_Position):
                    sta=num;
                    break;
                case(order_parameter::End_Position):
                    det=num;
                    break;
                default:
                    throw Ticket::WrongOperation("order_set_num");
            }
        }
/*        template<int Length>
        void set_str(order_parameter kind, const std::string str) {
            switch (kind) {
                case (order_parameter::Train_ID):
                    train_ID = str;
                    break;
                case (order_parameter::Username):
                    user_name = str;
                    break;
                case (order_parameter::Start):
                    Sta = str;
                    break;
                case (order_parameter::End):
                    Det = str;
                    break;
                default:
                    throw Ticket::WrongOperation("order_set_string");
            };
        }*/
        void set_str(order_parameter kind, const Ticket::String<24> & str) {
            switch (kind) {
                case (order_parameter::Train_ID):
                    train_ID = str;
                    break;
                case (order_parameter::Username):
                    user_name = str;
                    break;
                default:
                    throw Ticket::WrongOperation("order_set_string");
            }
        }

        void set_station(order_parameter kind, const Ticket::String<36> & str){
            switch (kind) {
                case (order_parameter::Start):
                    Sta = str;
                    break;
                case (order_parameter::End):
                    Det = str;
                    break;
                default:
                    throw Ticket::WrongOperation("order_set_string");

            }
        }

        void set_Date(order_parameter kind, const Ticket::Date &date) {//0 为出发，1为到达
            switch (kind) {
                case (order_parameter::Start_Date):
                    Sta_date = date;
                    break;
                case (order_parameter::End_Date):
                    End_date = date;
                    break;
                default:
                    throw Ticket::WrongOperation("order_set_date");
                    //break;
            }
        }

        const Ticket::Date &get_Date(order_parameter kind) {
            switch (kind) {
                case (order_parameter::Start_Date):
                    return Sta_date;
                case (order_parameter::End_Date):
                    return End_date;
                default:
                    throw Ticket::WrongOperation("order_get_date");
            }
        }

        int get_num(order_parameter kind)const {
            switch (kind) {
                case (order_parameter::Num):
                    return ticket_num;
                case (order_parameter::SN):
                    return serial_number;
                case (order_parameter::Price):
                    return price;
                case (order_parameter::Start_Position):
                    return sta;
                case (order_parameter::End_Position):
                    return det;
                default:
                    throw Ticket::WrongOperation("order_set_num");
            }
        }

        Ticket::String<24> get_str(order_parameter kind) {
            switch (kind) {
                case (order_parameter::Train_ID):
                    return train_ID;
                case (order_parameter::Username):
                    return user_name;
                default:
                    throw Ticket::WrongOperation("order_set_string");
            }
        }

        Ticket::String<36> get_station(order_parameter kind) {
            switch (kind) {
                case (order_parameter::Start):
                    return Sta;
                case (order_parameter::End):
                    return Det;
                default:
                    throw Ticket::WrongOperation("order_set_string");
            }
        }

        void change_state(state_list state) {
            _state = state;
        }

        bool operator<(const order &r) const {
            return serial_number < r.serial_number;
        }

        bool operator>(const order & r)const{
            return serial_number>r.serial_number;
        }

 /*       order & operator=(const order && o){

        }

        order & operator=(const order & o){

        }

        order (const order & o){

        }*/

        void print() {
            //Date.
          //  os << '[';
            printf("%c",'[');
            switch (_state) {
                case (state_list::Refund):
                  //  os << "refunded";
                    printf("%s","refunded");
                    break;
                case (state_list::Success):
                  //  os << "success";
                    printf("%s","success");
                    break;
                case (state_list::Pending):
                    printf("%s","pending");
                    //  os << "pending";
                    break;
                default:
                    throw Ticket::WrongOperation();
            }
            std::string tmp=Sta_date.to_string();
     printf("%c",']');printf(" %s",&train_ID);printf(" %s",&Sta);printf(" %s",tmp.c_str());
     printf(" -> %s",&Det);
     tmp=End_date.to_string();
     printf(" %s",tmp.c_str());
     printf( " %d",price);printf(" %d\n",ticket_num);
     //      os << ']' <<' '<<train_ID<< ' ' << Sta << ' ' << Sta_date <<' '<< '-' << '>' <<' '<< Det << ' '<<End_date<< ' '
      //                <<price<<' '<< ticket_num << '\n';
        }

        state_list &State() {
            return _state;
            //wrong_operation();
        };

        ~order() = default;

    };

    class waiting_queue {
    private:
        struct Comp {
            bool operator()(const OrderKey &l,const OrderKey &r)const {
                return l.hashstr < r.hashstr;
            }
        };
        Ticket::BPlusTree<OrderKey, order> Waiting_Queue;//记
        Ticket::BPlusTree<OrderKey,order> Train_Queue;
    public:
        explicit waiting_queue(const std::string &name,const std::string & Trainfilename) : Waiting_Queue(name) ,Train_Queue(Trainfilename){};

        void insert(const Ticket::String<24> &name,const Ticket::String<24> & Train_ID, int SN, order &order) {
            OrderKey data_key;
            data_key.hashstr =hash(name);
            data_key.SN = SN;
            int pos=Waiting_Queue.insert(data_key, order);
          //  if(pos==-1) std::cerr<<"shere"<<'\n';
            //std::cerr<<"here"<<'\n';
            //std::cerr<<data_key.str<<' '<<data_key.SN<<' '<<Train_ID<<' '<<pos<<'\n';
            data_key.hashstr=hash(Train_ID);
            Train_Queue.insert(data_key,order);
            //Waiting_Queue.write(Ticket::FileIO::END,data);
            //Waiting_Queue.read()
        }

        void Renew(std::vector<OrderKey> &IDvec,const Ticket::String<24> & Train_ID) {//返回位置 first 为位置 second 为票数
            int sz = IDvec.size();
            OrderKey renew;
            for(int i=0;i<sz;i++) {
                renew=IDvec[i];
                Waiting_Queue.erase(renew);
                renew.hashstr=hash(Train_ID);
                Train_Queue.erase(renew);
            }
        }
        void refund(int SN,const Ticket::String<24>& name,const Ticket::String<24>& Train_ID) {
            //加个鲁棒性
            //erase会失败吗?
            OrderKey Pending;
            Pending.SN=SN;
            Pending.hashstr=hash(name);
            Waiting_Queue.erase(Pending);
            Pending.hashstr=hash(Train_ID);
            Train_Queue.erase(Pending);
        }

        /*std::vector<int> & refund(int n){
        }*/
        void GetPending(const Ticket::String<24> & Train_ID, std::vector<order> & TrainOrdervec) {
            OrderKey tmp;
            tmp.SN = 0;
            tmp.hashstr =hash(Train_ID);
           // std::cerr<<"------88**"<<'\n';
            std::vector<int>Posvec = Train_Queue.route<Comp>(tmp);
          //  std::cerr<<tmp.str<<' ';
            int Possz=Posvec.size();
          //  std::cerr<<Possz<<'\n';
            for(int j=0;j<Possz;j++) {
                TrainOrdervec.push_back(Train_Queue.getVal(Posvec[j]));
            }
        }

        void clean(){
            Train_Queue.clear();
            Waiting_Queue.clear();
        }
    };

    class BPT_order {
    private:
        //int _order_num;
        struct Comp {
            bool operator()(const OrderKey &a, const OrderKey &b)const {
                return a.hashstr < b.hashstr;
            }
        };
        Ticket::BPlusTree<OrderKey, order> _BPT_Name_order;//SN=0为总数，SN=n时为位置
    public:
        explicit  BPT_order(const std::string &filename) : _BPT_Name_order(filename) {};
        /*int find(const  Ticket::String<25> & name ,int & SN){
            Key tmp;
            tmp.SN=SN;
            tmp.name=name;
            return _BPT_order.find(tmp);
        }*/
        int insert(order &data) {
            OrderKey BPT_KEY;
            BPT_KEY.hashstr = hash(data.get_str(order_parameter::Username));
            BPT_KEY.SN = data.get_num(order_parameter::SN);
            int pos=_BPT_Name_order.insert(BPT_KEY, data);
            return pos;
            //return _BPT_order.insert(BPT_KEY, data);
        }

        std::vector<order> query(const Ticket::String<24> &user_name) {
            OrderKey tmp;
            tmp.hashstr = hash(user_name);
            tmp.SN = 0;
            std::vector<int> pos = _BPT_Name_order.route<Comp>(tmp);
            std::vector<order> ret;
            int sz = pos.size();
            for (int i = 0; i < sz; i++) {
                ret.push_back(_BPT_Name_order.getVal(pos[i]));
            }
            //std::sort(ret.begin(),ret.end(),std::greater<order>());
            return ret;
        }

        bool refund(const Ticket::String<24> &name, int n, order & Success,
                    OrderKey & Pending,Ticket::String<24> & Train_ID,char & type,int & SN) {
            OrderKey tmp;
            tmp.hashstr=hash(name);
            tmp.SN=0;
            std::vector<int> pos = _BPT_Name_order.route<Comp>(tmp);
            int sztmp = pos.size();
            if (sztmp < n||n<0) return false;
            order ordertmp;
                ordertmp = _BPT_Name_order.getVal(pos[sztmp - n]);
                SN=ordertmp.get_num(order_parameter::SN);
                Train_ID = ordertmp.get_str(order_parameter::Train_ID);
                switch (ordertmp.State()) {
                    case (state_list::Refund):
                        return false;
                    case (state_list::Success) : {
                        Success=ordertmp;
                        ordertmp.change_state(state_list::Refund);
                        //std::cerr<<"!!!!!!!!!!!!"<<'\n';
                        //std::cerr<<ordertmp.get_str(order_parameter::Train_ID)<<' '<<ordertmp.get_Date(order_parameter::Start_Date)<<' '<<ordertmp.get_Date(order_parameter::End_Date)<<'\n';
                        _BPT_Name_order.modifyVal(pos[sztmp-n],ordertmp);
                        type='S';
                        return true;
                    }
                    case (state_list::Pending): {
                        OrderKey PeKey;
                        PeKey.hashstr=hash(name);
                        PeKey.SN=ordertmp.get_num(order_parameter::SN);
                        Pending=PeKey;
                        ordertmp.change_state(state_list::Refund);
                    //    ordertmp.print(std::cout);
                        _BPT_Name_order.modifyVal(pos[sztmp-n],ordertmp);
                        type='P';
                        return true;
                    }
                    default:{
                        throw Ticket::WrongOperation();
                    }
                }
        }

        int size() {
            return _BPT_Name_order.getSize();
        }

        void Renew(std::vector<OrderKey> & Renewvec) {
            int sz = Renewvec.size();
            order tmp;
            int pos;
            for (int i = 0; i < sz; i++) {
                pos=_BPT_Name_order.find(Renewvec[i]);
                tmp = _BPT_Name_order.getVal(pos);
                tmp.change_state(state_list::Success);
                _BPT_Name_order.modifyVal(pos, tmp);
            }
        }

        void clean(){
            _BPT_Name_order.clear();
        }

        ~BPT_order() = default;

    };



    class Order_op {
    private:
        waiting_queue Que;
        BPT_order Order;
    public:
        explicit Order_op(const std::string & order,const std::string & queue,const std::string & TrainIndex):Order(order),Que(queue,TrainIndex){};

        bool refund(Ticket::String<24> & Train_ID,const Ticket::String<24> & name, int n,order & Success ,std::vector<order> & TrainOrdervec,char & type) {
            OrderKey Pending;
           // Ticket::String<25> Trainvec;
           int SN;
            if(Order.refund(name,n,Success,Pending,Train_ID,type,SN)) {
                if(type=='P') Que.refund(SN,name,Train_ID);
                else Que.GetPending(Train_ID,TrainOrdervec);
                return true;
            }
            else return false;
        }

        void renew(std::vector<OrderKey> & Renewvec,const Ticket::String<24> & Train_ID) {
            //将空余票更新后，回到这里,传入更新的火车及其票数
            //再传出订单中的order
            //(?)
            //myp大危机
            Que.Renew(Renewvec,Train_ID);
            Order.Renew(Renewvec);
        }

        void buy_ticket(const std::string &name, const std::string &train_ID, const Ticket::Date & Sta_date,
                        const Ticket::Date & End_date, const Ticket::String<36> &Sta, Ticket::String<36> &Det, int &sta, int &end,
                        int n, int &price, bool state) {//1为queue，0为购票成功
            order data;
            data.set_str(order_parameter::Username, name);
            data.set_station(order_parameter::Start, Sta);
            data.set_station(order_parameter::End, Det);
            data.set_str(order_parameter::Train_ID, train_ID);
            data.set_num(order_parameter::Num, n);
            data.set_num(order_parameter::SN, Order.size() + 1);
            data.set_num(order_parameter::Price, price);
            data.set_num(order_parameter::Start_Position,sta);
            data.set_num(order_parameter::End_Position,end);
            data.set_Date(order_parameter::Start_Date, Sta_date);
            data.set_Date(order_parameter::End_Date, End_date);
            if (state) {
                data.change_state(state_list::Pending);
                Que.insert(name, train_ID,Order.size()+1,data);
            }
            else  data.change_state(state_list::Success);
            int pos = Order.insert(data);
            if (pos == -1) throw Ticket::WrongOperation("buy_ticket");
        }

        void query_order(const Ticket::String<24> &name) {
            std::vector<order> all_order = Order.query(name);
            //try{ all_order=Order.query(name);}catch(NotFound){return 0;}
            int sz = all_order.size();
            printf("%d\n",sz);
            //os<<sz<<'\n';
            for (int i = sz-1; i >=0 ; i--) {
                all_order[i].print();
            }
        }

        void clean(){
            Que.clean();
            Order.clean();
        }
    };
}