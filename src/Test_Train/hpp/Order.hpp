//typedef Ticket::String string;
#include "map.hpp"
#include "String.hpp"
#include"Exception.hpp"
#include"BPlusTree.hpp"
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
    struct OrderKey{
        Ticket::String<20> str;
        int SN;
        bool operator<(const OrderKey & r)const{
            return str<r.str||(str==r.str&&SN<r.SN);
        }
        bool operator==(const OrderKey & r){
            return str==r.str&&SN==r.SN;
        }
        bool operator!=(const OrderKey & r)const{
            return str!=r.str||SN!=r.SN;
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
        Error
    };

    class order {
        // friend  std::ofstream & operator<<(order & o,ofstream & os);
    private:

        state_list _state;
        Ticket::String<20> user_name;
        Ticket::Date End_date;
        Ticket::Date Sta_date;//购买时间
        Ticket::String<20> train_ID;
        Ticket::String<40> Sta;
        int sta;
        Ticket::String<40> Det;
        int det;
        int ticket_num;
        int price;
        int serial_number;//只在查询pair 中Key 中 num为0时 指代当前数目
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
                default:
                    throw Ticket::WrongOperation("order_set_num");
            };
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
        void set_str(order_parameter kind, const Ticket::String<20> str) {
            switch (kind) {
                case (order_parameter::Train_ID):
                    train_ID = str;
                    break;
                case (order_parameter::Username):
                    user_name = str;
                    break;
                default:
                    throw Ticket::WrongOperation("order_set_string");
            };
        }

        void set_station(order_parameter kind, const Ticket::String<40> str){
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
                    break;
            };
        }

        const Ticket::Date &get_Date(order_parameter kind) {
            switch (kind) {
                case (order_parameter::Start_Date):
                    return Sta_date;
                case (order_parameter::End_Date):
                    return End_date;
                default:
                    throw Ticket::WrongOperation("order_get_date");
            };
        }

        int get_num(order_parameter kind) {
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
            };
        }

        Ticket::String<20> get_str(order_parameter kind) {
            switch (kind) {
                case (order_parameter::Train_ID):
                    return train_ID;
                case (order_parameter::Username):
                    return user_name;
                default:
                    throw Ticket::WrongOperation("order_set_string");
            };
        }

        Ticket::String<40> get_station(order_parameter kind) {
            switch (kind) {
                case (order_parameter::Start):
                    return Sta;
                case (order_parameter::End):
                    return Det;
                default:
                    throw Ticket::WrongOperation("order_set_string");
            };
        }

        void change_state(state_list state) {
            _state = state;
        }

        bool operator<(order &r) const {
            return serial_number < r.serial_number;
        }

        void print(std::ostream & os) {
            //Date.
            os << '[';
            switch (_state) {
                case (state_list::Refund):
                    os << "Refund";
                    break;
                case (state_list::Success):
                    os << "Success";
                    break;
                case (state_list::Pending):
                    os << "Pending";
                    break;
            };
            os << ']' << ' ' << Sta << ' ' << Sta_date << Sta_date << '-' << '>' << End_date << Det << ' '
                      << ticket_num << '\n';
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
                return l.str < r.str;
            }
        };
        Ticket::BPlusTree<OrderKey, order> Waiting_Queue;//记
    public:
        explicit waiting_queue(const std::string &name) : Waiting_Queue(name) {};

        void insert(const Ticket::String<20> &name, int SN, order &order) {
            OrderKey data_key;
            data_key.str = name;
            data_key.SN = SN;
            Waiting_Queue.insert(data_key, order);
            //Waiting_Queue.write(Ticket::FileIO::END,data);
            //Waiting_Queue.read()
        }

        void Renew(std::vector<OrderKey> &IDvec) {//返回位置 first 为位置 second 为票数
            int sz = IDvec.size();
            for(int i=0;i<sz;i++) Waiting_Queue.erase(IDvec[i]);
        }
        void refund(OrderKey & Pending) {
            //加个鲁棒性
            //erase会失败吗?
            Waiting_Queue.erase(Pending);
        }

        /*std::vector<int> & refund(int n){
        }*/
        void GetPending(const Ticket::String<20> & TrainID, std::vector<order> & TrainOrdervec) {
            OrderKey tmp;
            tmp.SN = 0;
            tmp.str =TrainID;
            std::vector<int>Posvec = Waiting_Queue.route<Comp>(tmp);
            int Possz=Posvec.size();
            for(int j=0;j<Possz;j++) TrainOrdervec.push_back(Waiting_Queue.getVal(Posvec[j]));
        }
    };

    class BPT_order {
    private:
        //int _order_num;
        struct Comp {
            bool operator()(const OrderKey &a, const OrderKey &b)const {
                return a.str < b.str;
            }
        };

        Ticket::BPlusTree<OrderKey, order> _BPT_order;//SN=0为总数，SN=n时为位置
    public:
        explicit  BPT_order(const std::string &filename) : _BPT_order(filename) {};
        /*int find(const  Ticket::String<20> & name ,int & SN){
            Key tmp;
            tmp.SN=SN;
            tmp.name=name;
            return _BPT_order.find(tmp);
        }*/
        int insert(order &data) {
            OrderKey BPT_KEY;
            BPT_KEY.str = data.get_str(order_parameter::Username);
            BPT_KEY.SN = _BPT_order.getSize() + 1;
            return _BPT_order.insert(BPT_KEY, data);
        }

        std::vector<order> query(const Ticket::String<20> &user_name) {
            OrderKey tmp;
            tmp.str = user_name;
            tmp.SN = 0;
            std::vector<int> pos = _BPT_order.route<Comp>(tmp);
            std::vector<order> ret;
            int sz = pos.size();
            for (int i = 0; i < sz; i++) ret[i - 1] = _BPT_order.getVal(pos[i]);
            return ret;
        }

        bool refund(const Ticket::String<20> &name, int n, order & Success,
                    OrderKey & Pending,Ticket::String<20> & Train_ID,char & type) {
            OrderKey tmp;
            tmp.str=name;
            tmp.SN=0;
            std::vector<int> pos = _BPT_order.route<Comp>(tmp);
            int sztmp = pos.size();
            if (sztmp < n) return false;
            order ordertmp;
            ordertmp = _BPT_order.getVal(pos[sztmp - n]);
            Train_ID = ordertmp.get_str(order_parameter::Train_ID);
            switch (ordertmp.State()) {
                case (state_list::Refund):
                    return false;
                case (state_list::Success) : {
                    Success=ordertmp;
                    ordertmp.change_state(state_list::Refund);
                    _BPT_order.modifyVal(pos[sztmp-n],ordertmp);
                    type='S';
                    return true;
                }
                case (state_list::Pending): {
                    OrderKey PeKey;
                    PeKey.str=Train_ID;
                    PeKey.SN=ordertmp.get_num(order_parameter::SN);
                    Pending=PeKey;
                    ordertmp.change_state(state_list::Refund);
                    _BPT_order.modifyVal(pos[sztmp-1-n],ordertmp);
                    type='N';
                    return true;
                }
                default: {
                	throw Ticket::WrongOperation();
                }
            }
        }

        int size() {
            return _BPT_order.getSize();
        }

        void Renew(std::vector<OrderKey> & Renewvec) {
            int sz = Renewvec.size();
            order tmp;
            int pos;
            for (int i = 0; i < sz; i++) {
                pos=_BPT_order.find(Renewvec[i]);
                tmp = _BPT_order.getVal(pos);
                tmp.change_state(state_list::Success);
                _BPT_order.modifyVal(pos, tmp);
            }
        }

        ~BPT_order() = default;
    };



    class Order_op {
    private:
        waiting_queue Que;
        BPT_order Order;
    public:
        explicit Order_op(const std::string & order,const std::string & queue):Order(order),Que(queue){};

        bool refund(Ticket::String<20> & Train_ID,const Ticket::String<20> & name, int n,order & Success ,std::vector<order> & TrainOrdervec,char & type) {
            OrderKey Pending;
           // Ticket::String<20> Trainvec;
            if(Order.refund(name,n,Success,Pending,Train_ID,type)) {
                if(type=='P') Que.refund(Pending);
                else Que.GetPending(Train_ID,TrainOrdervec);
                return true;
            }
            else return false;
        }

        void renew(std::vector<OrderKey> & Renewvec) {
            //将空余票更新后，回到这里,传入更新的火车及其票数
            //再传出订单中的order
            //(?)
            //myp大危机
            Que.Renew(Renewvec);
            Order.Renew(Renewvec);
        }

        void buy_ticket(const std::string &name, const std::string &train_ID, const Ticket::Date Sta_date,
                        const Ticket::Date End_date, const Ticket::String<40> &Sta, Ticket::String<40> &Det, int &sta, int &end,
                        int &n, int &price, bool state) {//0为queue，1为购票成功
            order data;
           /* data.set_str<20>(order_parameter::Username, name);
            data.set_str<40>(order_parameter::Start, Sta);
            data.set_str<40>(order_parameter::End, Det);
            data.set_String<20>(order_parameter::Train_ID, train_ID);*/
            data.set_str(order_parameter::Username, name);
            data.set_station(order_parameter::Start, Sta);
            data.set_station(order_parameter::End, Det);
            data.set_str(order_parameter::Train_ID, train_ID);
            data.set_num(order_parameter::Num, n);
            data.set_num(order_parameter::SN, Order.size() + 1);
            data.set_num(order_parameter::Price, price);
            data.set_Date(order_parameter::Start_Date, Sta_date);
            data.set_Date(order_parameter::End_Date, End_date);
            int pos = Order.insert(data);
            if (pos == -1) throw Ticket::WrongOperation("buy_ticket");
            if (state) data.change_state(state_list::Success);
            else {
                data.change_state(state_list::Pending);
                Que.insert(name, Order.size() + 1, data);
            }
        }

        void query_order(const Ticket::String<20> &name,std::ostream& os) {
            std::vector<order> all_order = Order.query(name);
            //try{ all_order=Order.query(name);}catch(NotFound){return 0;}
            int sz = all_order.size();
            os<<sz<<'\n';
            for (int i = 0; i < sz; i++) all_order[i].print(os);
            return;
        }

    };
}