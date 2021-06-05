#ifndef BACKEND_TRAIN_HPP
#define BACKEND_TRAIN_HPP

#include<iostream>
#include"BPlusTree.hpp"
#include"map.hpp"
#include<vector>
#include"Exception.hpp"
#include"Time.hpp"
#include"String.hpp"
#include"Utility.hpp"
#include<queue>
namespace Backend {
#define Error(x) throw Ticket::WrongOperation(x)

    class Train_manager {
    private:
        struct Train {
            //private:
            Ticket::String<22> Train_SN;
            Ticket::Date start_day;
            Ticket::Date end_day;
            Ticket::Date start_time;
           // Ticket::String<40> start_station;
            int station_num = 0;
            // char rl='N';
            char type='\0';
            int seat=0;
            //int station;
            struct info {
                Ticket::String<36> station;
                //int prefix_time;
                Ticket::Date Sta_Date;//仅代表日期的起终
                Ticket::Date End_Date;//
                Ticket::Date depart_time;
                int prefix_price = 0;
                int prefix_time = 0;
                //prefix_time是离开时间
                int stopover = 0;
                //int seat;
            };
            info train_info[100];
            //public:
            //Train()=default;
            //~Train()=default;
        };

        struct Station {
            Ticket::String<22> Train_SN;
            int Train_pos = 0;

            //Ticket::Time arri_time;
            Station() = default;

            Station(const Ticket::String<22> &SN, const int &pos) {
                Train_SN = SN;
                Train_pos = pos;
                //arri_time=o.train_info[pos].arri_time;
                //start=o.train_info[pos].arri_Date;
                //end=o.train_info[pos].depart_Date;
            }

        };

        struct Seat_Key {
            Ticket::String<6> time;
            Ticket::String<22> train;
            Ticket::String<36> station;

            Seat_Key() = default;

            Seat_Key(const Ticket::String<22> &Train, const Ticket::String<36> &Station,
                     const Ticket::String<6> &Time) {
                time = Time;
                train = Train;
                station = Station;
            }

            bool operator==(const Seat_Key &r) const {
                return time == r.time && train == r.train && station == r.station;
            }

            bool operator<(const Seat_Key &r) const {
                return time < r.time || (time == r.time && train < r.train) ||
                       (time == r.time && train == r.train && station < r.station);
            }

        };

        /*struct Train_Key{
            Ticket::String Train_SN;
            int pos;
        };*/
        struct Station_Key {
            Ticket::String<36> Station_name;
            int pos = 0;

            bool operator==(const Station_Key &r) const {
                return Station_name == r.Station_name && pos == r.pos;
            }

            bool operator<(const Station_Key &r) const {
                return Station_name < r.Station_name || (Station_name == r.Station_name && pos < r.pos);
            }
        };

        struct Station_Comp {
            bool operator()(const Station_Key &l, const Station_Key &r) {
                return l.Station_name < r.Station_name;
            }
        };

        Ticket::BPlusTree<Seat_Key, int> _BPT_Seat;
        Ticket::BPlusTree<Ticket::String<22>, Train> _BPT_Train;
        Ticket::BPlusTree<Station_Key, Station> _BPT_Station;
        Ticket::BPlusTree<Ticket::String<22>, char> _BPT_Rl;
        Ticket::FileIONoCache Count;
        int cnt = 0;

        inline int
        _get_seat(const Ticket::String<22> &train, const Ticket::String<36> &station, const Ticket::String<6> &date) {
            Seat_Key seatKey(train, station, date);
            int pos = _BPT_Seat.find(seatKey);
            if (pos == -1) return -1;
            else return _BPT_Seat.getVal(pos);
        }

        inline int
        _get_seat_range(const Train &data, const Ticket::String<36> &Sta, const Ticket::Date &Start_Date, int sta,
                        int end) {

            Ticket::pair<int,int> mmdd=Start_Date.getMMDD();
            Ticket::Date Datekey(mmdd.first,mmdd.second,0,0);
            /*   Datekey.mm=Start_Date.mm;
               Datekey.dd=Start_Date.dd;*/
            Seat_Key seatKey;
            seatKey.station = Sta;
            seatKey.train = data.Train_SN;
            seatKey.time = Datekey.getDateStr();
            Ticket::Date Datetmp = Start_Date;
            int seatpos;
            int seat = data.seat;
            int seattmp;
            // std::cout<<"__________________"<<'\n';
            for (int i = sta; i < end; i++) {
                seatKey.station=data.train_info[i].station;
                seatpos = _BPT_Seat.find(seatKey);
                seattmp = _BPT_Seat.getVal(seatpos);
                seat = std::min(seat, seattmp);
                Datetmp += (data.train_info[i + 1].prefix_time - data.train_info[i].prefix_time);
                mmdd=Datetmp.getMMDD();
                Ticket::Date Datekey2(mmdd.first,mmdd.second,0,0);
                /*      Datekey.mm = Datetmp.mm;
                      Datekey.dd = Datetmp.dd;*/
                seatKey.time = Datekey2.getDateStr();
            }
            //  std::cout<<"__________________"<<'\n';
            return seat;
        }


        inline int _get_seat_range(const Ticket::String<22> &Train_ID, const Ticket::String<36> &Sta, const Ticket::Date &Start_Date, int sta,
                                   int end) {
            int pos=_BPT_Train.find(Train_ID);
            Train data=_BPT_Train.getVal(pos);
            Ticket::pair<int,int> mmdd=Start_Date.getMMDD();
            Ticket::Date Datekey(mmdd.first,mmdd.second,0,0);
            /*   Datekey.mm=Start_Date.mm;
               Datekey.dd=Start_Date.dd;*/
            Seat_Key seatKey;
            seatKey.station = Sta;
            seatKey.train = data.Train_SN;
            seatKey.time = Datekey.getDateStr();
            Ticket::Date Datetmp = Start_Date;
            int seatpos;
            int seat = data.seat;
            int seattmp;
            // std::cout<<"__________________"<<'\n';
            for (int i = sta; i < end; i++) {
                seatKey.station=data.train_info[i].station;
                seatpos = _BPT_Seat.find(seatKey);
                seattmp = _BPT_Seat.getVal(seatpos);
                seat = std::min(seat, seattmp);
                Datetmp += (data.train_info[i + 1].prefix_time - data.train_info[i].prefix_time);
                mmdd=Datetmp.getMMDD();
                Ticket::Date Datekey2(mmdd.first,mmdd.second,0,0);
                /*      Datekey.mm = Datetmp.mm;
                      Datekey.dd = Datetmp.dd;*/
                seatKey.time = Datekey2.getDateStr();
            }
            //  std::cout<<"__________________"<<'\n';
            return seat;
        }
        struct Comp {
            int data = 0;
            int pos = 0;
            Ticket::String<22> ID;

            bool operator<(const Comp &o) const {
                return data < o.data || (data == o.data && ID < o.ID);
            }
        };

        struct Trans_Comp {
            Ticket::String<22> Train_ID_Sta;
            Ticket::String<22> Train_ID_End;
            Ticket::String<36> Cent;
            int num=0;
            int diff1=0;
            int diff2=0;
            int price1=0;
            int price2=0;
            int sta1=0;
            int sta2=0;
            int end1=0;
            int end2=0;
            Ticket::Date depart1;
            Ticket::Date depart2;
            bool operator<(const Trans_Comp &o) const {
                return num < o.num || (num == o.num && diff1<o.diff1);
            }
        };

    public:
        //Train_manager()=default;
        explicit Train_manager(const std::string &name, const std::string &station_name, const std::string &Rlname,
                               const std::string &seat_name, const std::string &count) : _BPT_Train(name),
                                                                                         _BPT_Station(station_name),
                                                                                         _BPT_Rl(Rlname),
                                                                                         _BPT_Seat(seat_name) {
            Count.open(count);
            if (Count.isFirstOpen()) cnt = 0, Count.write(Count.BEG, cnt);
            else Count.read(Count.BEG, cnt);
            //std::cout << cnt << '\n';
        };

        bool add_train(const Ticket::String<22> &SN, int &stanum, int &seatnum, Ticket::String<36> *stations,
                       const int *price, const Ticket::Date &sta_time, const int *traveltime, const int *stoppovertime,
                       const Ticket::Date *saleDate, char type) {//saleDate[0]为起始,1为终结
            if(_BPT_Train.find(SN)!=-1) return false;
            Train data;
            //int sumtime=0;
            //Train_Key data_key;
            // data_key.Train_SN
            data.Train_SN = SN;
            data.station_num = stanum;
            data.type = type;
            //设定起始时间相关
            data.start_time = sta_time;
            Ticket::Date Datetmp(data.start_time);
            data.start_day = saleDate[0] + Datetmp;
            data.end_day = saleDate[1] + Datetmp;
         //   data.start_station = stations[0];
            //设定首站，到达时间，停留时间,累计时间无用，
            //data.train_info[0].seat=seat;
            data.train_info[0].station = stations[0];
            data.train_info[0].prefix_price = price[0];
            //传入的stopover数组首尾为0;
            //传入的travel time 数组首为0;
            //传入的price 数组首位0;
            data.train_info[0].stopover = 0;
            //、、要改！
            Ticket::Date Statmp = data.train_info[0].Sta_Date = data.start_day;
            Ticket::Date Endtmp = data.train_info[0].End_Date = data.end_day;
            data.train_info[0].depart_time = sta_time+data.train_info[0].stopover;
            data.train_info[0].prefix_time = 0;
            //seat 最后一位没有用
            data.seat = seatnum;
            //设定后面
            //Ticket::Date Statmp,Endtmp;
            // seatArr.rl='N';
            //seatArr.seat_info[0]=seatnum;
            for (int i = 1; i < stanum; i++) {
                //data.seat[i]=seatnum;
                data.train_info[i].station = stations[i];
                //data.train_info[i].seat=seatnum;
                data.train_info[i].prefix_price = data.train_info[i - 1].prefix_price + price[i];
                Statmp += traveltime[i];
                Endtmp += traveltime[i];
                /*     data.train_info[i].arri_time.hr = Statmp.hr;
                     data.train_info[i].arri_time.mi = Statmp.mi;*/
                data.train_info[i].stopover = stoppovertime[i];
                Statmp += stoppovertime[i];
                Endtmp += stoppovertime[i];
                Ticket::pair<int,int> hrmi=Statmp.getHHMM();
                Ticket::Date depatmp(0,0,hrmi.first,hrmi.second);
                data.train_info[i].depart_time=depatmp;
                Ticket::pair<int,int> mmdd=Statmp.getMMDD();
                Ticket::Date Dtmp(mmdd.first,mmdd.second,0,0);
                Ticket::pair<int,int> mmdd2=Endtmp.getMMDD();
                Ticket::Date Dtmp2(mmdd2.first,mmdd2.second,0,0);
                data.train_info[i].Sta_Date = Dtmp;
                data.train_info[i].End_Date = Dtmp2;
                data.train_info[i].prefix_time =data.train_info[i - 1].prefix_time + traveltime[i] + stoppovertime[i];//
            }
            _BPT_Train.insert(SN,data);
            _BPT_Rl.insert(SN, 'N');
            return true;
            //终点站,price与出发时间停留时间无用
        }

        bool delete_train(const Ticket::String<22> &SN) {
            //这里好像有点鲁棒？判断是否发售？
            int pos = _BPT_Rl.find(SN);
            if (pos == -1) return false;
            else {
                //Seat_arr seatArr=_BPT_Seat.getVal(pos);
                char flag = _BPT_Rl.getVal(pos);
                if (flag == 'N') {
                    if (_BPT_Rl.erase(SN) && _BPT_Train.erase(SN)) return true;
                    else
                        Error("delete_train_2");
                    //std::cerr<<"delete_train_3",throw std::exception();
                } else if (flag == 'Y') return false;
                else
                    Error("delete_train_3");
                //std::cerr<<"delete_train_3",throw std::exception();
            }
        }

        bool release_train(const Ticket::String<22> &SN) {
            int pos = _BPT_Train.find(SN);
            //Otmp_pos为第二key为0的位置
            Station_Key data_key;
            if (pos == -1) return false;
            int rl_pos = _BPT_Rl.find(SN);
            if (rl_pos == -1) Error("release_train_2");//std::cerr<<"release_train_2",throw std::exception();
            char flag = _BPT_Rl.getVal(rl_pos);
            if (flag == 'Y') return false;
            _BPT_Rl.modifyVal(rl_pos, 'Y');
            Train data = _BPT_Train.getVal(pos);
            data_key.pos = ++cnt;
            for (int i = 0; i < data.station_num; i++) {
                data_key.Station_name = data.train_info[i].station;
                //要插入 station
                Station station(SN, i);
                //std::cout<<data_key.Station_name<<' '<<data_key.pos<<'\n';
                //std::cout<<station.Train_SN<<' '<<station.Train_pos<<'\n';
                int flag = _BPT_Station.insert(data_key, station);
                if (flag == -1) Error("release_train_4");
                Ticket::pair<int,int> mmdd=data.train_info[i].Sta_Date.getMMDD();
                Ticket::Date tmp(mmdd.first,mmdd.second,0,0);
                /*     tmp.dd = data.train_info[i].Sta_Date.dd;
                     tmp.mm = data.train_info[i].Sta_Date.mm;*/
                for (; data.train_info[i].Sta_Date.cmpDate(tmp) <= 0 &&
                       tmp.cmpDate(data.train_info[i].End_Date) <= 0; ++tmp) {
                    Seat_Key seatKey(SN, data.train_info[i].station, tmp.getDateStr());
                    if (_BPT_Seat.insert(seatKey, data.seat) == -1)Error("release_train_5");
                    //std::cerr<<"release_trian_2",throw wrong_operation();
                }
            }
            //data.seat=-1;
            return true;
            //_BPT_Train.modifyVal(pos,data);
        }

        bool query_train(const Ticket::String<22> &SN, const Ticket::Date &tDate, std::ostream &os) {
            int pos = _BPT_Train.find(SN);
            int pos2 = _BPT_Train.find(SN);
            int pos3 = _BPT_Train.find(SN);
            //int Rlpos=_BPT_Rl.find(SN);
            //int spos=_BPT_Seat.find(SN);
            if (pos == -1) return false;
            else {
                int Rlpos = _BPT_Rl.find(SN);
                if (Rlpos == -1) Error("query_train");
                //std::cerr<<"query_train",throw wrong_operation();
                Train data = _BPT_Train.getVal(pos);
                if (data.start_day.cmpDate(tDate) > 0 || tDate.cmpDate(data.end_day) >0 ) return false;
                int sz = data.station_num;
                char flag = _BPT_Rl.getVal(Rlpos);
                if (flag == 'N') {
                    Ticket::Date cal;
                    Ticket::Date Datetmp(data.start_time);
                    cal = tDate + Datetmp;
                    os << SN << ' ' << data.type << '\n';
                    os << data.train_info[0].station << ' ' << "xx-xx xx:xx ->" << ' ' << cal << ' '
                       << data.train_info[0].prefix_price << ' ' << data.seat << '\n';
                    for (int i = 1; i < sz - 1; i++) {
                        cal += data.train_info[i].prefix_time - data.train_info[i - 1].prefix_time -
                               data.train_info[i].stopover;
                        os << data.train_info[i].station << ' ' << cal << ' ' << '-' << '>' << ' ';
                        cal += data.train_info[i].stopover;
                        os << cal << ' ' << data.train_info[i].prefix_price << ' ' << data.seat << '\n';
                    }
                    cal += data.train_info[sz - 1].prefix_time - data.train_info[sz - 2].prefix_time -
                           data.train_info[sz - 1].stopover;
                    os << data.train_info[sz - 1].station << ' ' << cal << ' ' << "-> xx-xx xx:xx" << ' '
                       << data.train_info[sz - 1].prefix_price << ' ' << 'x' << '\n';
                } else if (flag == 'Y') {
                    Ticket::Date cal;
                    Ticket::Date Datetmp(data.start_time);
                    Ticket::pair<int,int> mmdd=tDate.getMMDD();
                    Ticket::Date Datekey(mmdd.first, mmdd.second, 0, 0);
                    cal = tDate + Datetmp;
                    os << SN << ' ' << data.type << '\n';
                    os << data.train_info[0].station << ' ' << "xx-xx xx:xx ->" << ' ' << cal << ' '
                       << data.train_info[0].prefix_price << ' '
                       << _get_seat(SN, data.train_info[0].station, Datekey.getDateStr()) << '\n';
                    for (int i = 1; i < sz - 1; i++) {
                        cal += data.train_info[i].prefix_time - data.train_info[i - 1].prefix_time -
                               data.train_info[i].stopover;
                        os << data.train_info[i].station << ' ' << cal << ' ' << '-' << '>' << ' ';
                        cal += data.train_info[i].stopover;
                        Ticket::pair<int,int> mmdd2=cal.getMMDD();
                        Ticket::Date Datekey2(mmdd2.first, mmdd2.second, 0, 0);
                        os << cal << ' ' << data.train_info[i].prefix_price << ' '
                           << _get_seat(SN, data.train_info[i].station, Datekey2.getDateStr()) << '\n';
                    }
                    cal += data.train_info[sz - 1].prefix_time - data.train_info[sz - 2].prefix_time -
                           data.train_info[sz - 1].stopover;
                    os << data.train_info[sz - 1].station << ' ' << cal << ' ' << "-> xx-xx xx:xx" << ' '
                       << data.train_info[sz - 1].prefix_price << ' ' << 'x' << '\n';
                } else
                    Error("query_ticket_2");
                return true;
            }
            //出错的话先检查spos和pos

        }

        bool
        query_ticket(const Ticket::String<36> &Sta, const Ticket::String<36> &Det, const Ticket::Date &date, char type,
                     std::ostream &os) {//type 'T'-time 'P'-price
            Backend::map<Ticket::String<22>, int> match;
            std::vector<Ticket::String<22>> aimIDvec;
            Station_Key Keytmp;
            Keytmp.Station_name = Sta;
            Keytmp.pos = 0;
            //先得到起始位置
            std::vector<int> stavec = _BPT_Station.route<Station_Comp>(Keytmp);
            //首站不存在
            if (stavec.empty()) { return false; }
            Keytmp.Station_name = Det;
            //终止位置
            std::vector<int> endvec = _BPT_Station.route<Station_Comp>(Keytmp);
            //尾站不存在
            if (endvec.empty()) { return false; }
            int sz = stavec.size();
            Station stationtmp;
            //起始站匹配
            for (int i = 0; i < sz; i++) {
                stationtmp = _BPT_Station.getVal(stavec[i]);
                Ticket::pair<const Ticket::String<22>, int> tmp(stationtmp.Train_SN, stationtmp.Train_pos);
                //    os<<stationtmp.Train_SN<<' '<<stationtmp.Train_pos<<' '<<'\n';
                if (match.insert(tmp)) {}
                else std::cerr << "query_ticket", throw std::exception();
            }
            //   os<<"----------------------"<<'\n';
            sz = endvec.size();
            //将结尾站符合要求的车塞进去
            for (int i = 0; i < sz; i++) {
                stationtmp = _BPT_Station.getVal(endvec[i]);
                //小心逆向查车，小心车没释放
                try {
                    //逆向查车不算
                    //  std::cerr<<"------000"<<stationtmp.Train_SN<<' '<<stationtmp.Train_pos<<' '<<date<<' '<<'\n';
                    if (stationtmp.Train_pos > match[stationtmp.Train_SN])
                        aimIDvec.push_back(stationtmp.Train_SN);
                } catch (NotFound) {}
            }
            //if(match.count(stationtmp.Train_SN)) aimIDvec.push_back(stationtmp.Train_SN);}
            if (match.empty()) { return false; }
            //    std::cout<<"------------------"<<'\n';
            std::vector<Train> Trainvec;
            //int pos;
            sz = aimIDvec.size();
            //把Train取出来

            //Ticket::Date Datetmp(data.start_time);
            Ticket::pair<int,int> mmdd=date.getMMDD();
            Ticket::Date DateKey(mmdd.first,mmdd.second,0,0);
            /*       DateKey.dd = date.dd;
                   DateKey.mm = date.mm;*/
            for (int i = 0; i < sz; i++) {
                //时间判断
                //   std::cerr<<aimIDvec[i]<<' '<<DateKey<<'\n';
                Seat_Key seatKey(aimIDvec[i], Sta, DateKey.getDateStr());
                if (_BPT_Seat.find(seatKey) != -1) {
                    //     std::cerr<<aimIDvec[i]<<'\n';
                    int trapos = _BPT_Train.find(aimIDvec[i]);
                    if (trapos == -1) std::cerr << "query_ticket_2", throw std::exception();
                    Trainvec.push_back(_BPT_Train.getVal(trapos));
                } else {}
            }
            sz = Trainvec.size();
            Comp arr[sz];
            std::pair<int, int> pospair[sz];
            //Seat_arr seatArrs[sz];
            int tmp_seat_pos;
            if (type == 'T') {
                for (int i = 0; i < sz; i++) {
                    arr[i].ID = Trainvec[i].Train_SN;
                    arr[i].pos = i;
                    int station_num = Trainvec[i].station_num;
                    for (int j = 0; j < station_num; j++)
                        if (Sta == Trainvec[i].train_info[j].station) {
                            pospair[i].first = j;
                            break;
                        }
                    for (int j = station_num - 1; j >= 0; j--)
                        if (Det == Trainvec[i].train_info[j].station) {
                            pospair[i].second = j;
                            break;
                        }
                    arr[i].data = Trainvec[i].train_info[pospair[i].second].prefix_time -
                                  Trainvec[i].train_info[pospair[i].first].prefix_time -
                                  Trainvec[i].train_info[pospair[i].second].stopover;
                    //    std::cout<<arr[i].data<<' '<<arr[i].ID<<'\n';
                }

            } else if (type == 'P') {
                for (int i = 0; i < sz; i++) {
                    arr[i].ID = Trainvec[i].Train_SN;
                    arr[i].pos = i;
                    int station_num = Trainvec[i].station_num;
                    for (int j = 0; j < station_num; j++)
                        if (Sta == Trainvec[i].train_info[j].station) {
                            pospair[i].first = j;
                            break;
                        }
                    for (int j = station_num - 1; j >= 0; j--)
                        if (Det == Trainvec[i].train_info[j].station) {
                            pospair[i].second = j;
                            break;
                        }
                    arr[i].data = Trainvec[i].train_info[pospair[i].second].prefix_price -
                                  Trainvec[i].train_info[pospair[i].first].prefix_price;
                }
            } else std::cerr << "query_ticket", throw std::exception();
            std::sort(arr, arr + sz);
            //  for(int i=0;i<sz;i++) std::cout<<arr[i].ID<<' '<<arr[i].data<<' '<<arr[i].pos<<'\n';
            Ticket::Date Dtmp;
            Ticket::pair<int,int> mmdd2;
            os << sz << '\n';
            for (int i = 0; i < sz; i++) {

                int Trainpos = arr[i].pos;
                Dtmp = date + Trainvec[Trainpos].train_info[pospair[Trainpos].first].depart_time;
                //Dtmp += Trainvec[Trainpos].train_info[pospair[Trainpos].first].stopover;
                int seat = _get_seat_range(Trainvec[Trainpos], Sta, Dtmp, pospair[Trainpos].first,
                                           pospair[Trainpos].second);
                os << arr[i].ID << ' ' << Sta << ' ' << Dtmp << ' ' << '-' << '>' << ' ';
                // Dtmp=Trainvec[Trainpos].train_info[pospair[i].first].arri_time;
                int diff = Trainvec[Trainpos].train_info[pospair[Trainpos].second].prefix_time -
                           Trainvec[Trainpos].train_info[pospair[Trainpos].first].prefix_time -
                           Trainvec[Trainpos].train_info[pospair[Trainpos].second].stopover;
                Dtmp += diff;
                /* std::cout<<"-----"<<'\n';
                 int diff=Trainvec[Trainpos].train_info[pospair[i].second].prefix_time-
                          Trainvec[Trainpos].train_info[pospair[i].first].prefix_time-
                          Trainvec[Trainpos].train_info[pospair[i].second].stopover;
                 std::cout<<diff<<' '<<Dtmp<<' '<<Dtmp+diff<<' '<<'\n';
                 std::cout<<"-----"<<'\n';*/
                mmdd2=Dtmp.getMMDD();
                Ticket::Date DateKet2(mmdd2.first,mmdd2.second,0,0);
                /* DateKet2.mm = Dtmp.mm;
                 DateKet2.dd = Dtmp.dd;*/
                os << Det << ' ' << Dtmp << ' '<< Trainvec[Trainpos].train_info[pospair[Trainpos].second].prefix_price
                                                  - Trainvec[Trainpos].train_info[pospair[Trainpos].first].prefix_price << ' ' << seat
                   << '\n';
            }
            return true;
        }

        bool query_transfer(const Ticket::String<36> &Sta, const Ticket::String<36> &Det, const Ticket::Date &date,
                            char type, std::ostream &os) {
            //为什么感觉时间处理这么麻烦?
            //先记住我在每一个info里存的prefix_time
            //都是该站离站时间到第一个站的离站时间
            //由于中间存在停靠，所以需要做额外的加减处理
            Station_Key stationKey;
            stationKey.Station_name = Sta;
            stationKey.pos = 0;
            std::vector<int> StaPosvec = _BPT_Station.route<Station_Comp>(stationKey);
            stationKey.Station_name = Det;
            std::vector<int> EndPosvec = _BPT_Station.route<Station_Comp>(stationKey);
            //std::vector<Station> Stavec;
            std::vector<Station> Endvec;
            //一切的核心这个map Key值是尾站的所有train名， vector是尾站相同的中转站的在从中转站到尾站中车的位置
            //但由于中转站在从起点出发的火车中位置基本上与从中转站出发到尾站中的位置不一样
            //所以用pair来存，其中first是在从起点出发的火车中的位置，second是站在转乘的火车中的位置
            std::priority_queue<Trans_Comp> PQ;
            //Ticket::String<25> Ret[2];
            //起始站
            int nums;
            Trans_Comp Ret;
            Ret.num = 88888888;
            //  std::cerr<<"transfer_0"<<'\n';
            //  std::cerr<<date<<'\n';

            map<Ticket::String<22>, std::vector<std::pair<int, int>>> Endmatch;
            for (int i = 0; i < EndPosvec.size(); i++) {
                Endvec.push_back(_BPT_Station.getVal(EndPosvec[i]));
                std::vector<std::pair<int, int>> tmpvec;
                map<Ticket::String<22>, std::vector<std::pair<int, int>>>::value_type valueType(Endvec[i].Train_SN,
                                                                                                tmpvec);
                Endmatch.insert(valueType);
            }
            for (int i = 0; i < StaPosvec.size(); i++) {

                for(int j=0;j<EndPosvec.size();j++) Endmatch[Endvec[j].Train_SN].clear();
                //   std::cerr<<"transfer_0.5"<<'\n';
                Station StaStation = _BPT_Station.getVal(StaPosvec[i]);
                Ticket::String<22> Train_ID = StaStation.Train_SN;
                int train_pos = _BPT_Train.find(Train_ID);
                Train data = _BPT_Train.getVal(train_pos);
                int statnum = data.station_num;
                //遍历起始站的每一辆车车的每一个节点
                for (int j = StaStation.Train_pos; j < statnum; j++) {
                    //遍历该节点的每一辆车，并将其插入map中
                    Station_Key CentKey;
                    CentKey.pos = 0;
                    CentKey.Station_name = data.train_info[j].station;
                    std::vector<int> CentPosvec = _BPT_Station.route<Station_Comp>(CentKey);
                    std::vector<Station> Centvec;
                    int Centpos1 = -1;
                    for (int k = 0; k < CentPosvec.size(); k++) {
                        Centvec.push_back(_BPT_Station.getVal(CentPosvec[k]));
                      //  std::cout<<"-------"<<'\n';
                        if (Centvec[k].Train_SN == StaStation.Train_SN){
                            Centpos1 = Centvec[k].Train_pos;
                       //     std::cout<<Centpos1<<"\n";
                        }
                    }
                    for (int k = 0; k < CentPosvec.size(); k++) {
                        if (Endmatch.count(Centvec[k].Train_SN)) {
                            std::pair<int, int> value(Centpos1, Centvec[k].Train_pos);
                            Endmatch[Centvec[k].Train_SN].push_back(value);
                        }
                    }
                }
                for (int j = 0; j < Endvec.size(); j++) {
                    if (StaStation.Train_SN != Endvec[j].Train_SN) {//换乘的不是同一辆车
                        //     std::cerr<<"transfer_1"<<'\n';
                        int train_pos2 = _BPT_Train.find(Endvec[j].Train_SN);
                        Train Candidate = _BPT_Train.getVal(train_pos2);
                        std::vector<std::pair<int, int>> CentTrainpos = Endmatch[Endvec[j].Train_SN];
                        for (int k = 0; k < CentTrainpos.size(); k++) {
                            //             std::cerr<<"transfer_1.5"<<'\n';
                            int StaPos = StaStation.Train_pos;
                            int CentPos1 = CentTrainpos[k].first;
                            int CentPos2 = CentTrainpos[k].second;
                            int EndPos = Endvec[j].Train_pos;
                            //            std::cerr<<StaPos<<' '<<CentPos1<<' '<<CentPos2<<' '<<EndPos<<'\n';
                            //            std::cerr<<data.train_info[StaPos].Sta_Date<<' '<<date<<' '<<data.train_info[StaPos].End_Date<<'\n';
                            if (StaPos < CentPos1 && CentPos2 < EndPos &&
                                data.train_info[StaPos].Sta_Date.cmpDate(date) <= 0 &&
                                date.cmpDate(data.train_info[StaPos].End_Date)<=0) {//查询的始发时间合法//保证正向
                                int diff = data.train_info[CentPos1].prefix_time -
                                           data.train_info[StaPos].prefix_time-data.train_info[CentPos1].stopover;
                                //time表示中转站时间
                                Ticket::Date time = date + data.train_info[StaPos].depart_time+ diff;
                                //只要到达中转时间小于最晚离开时间即可
                                //             std::cerr<<"transfer_2"<<'\n';
                                if (!(Candidate.train_info[CentPos2].End_Date +
                                         Candidate.train_info[CentPos2].depart_time<time)) {
                                    Trans_Comp Challenger;
                                    Challenger.Cent=Candidate.train_info[CentPos2].station;
                                    Challenger.diff1=diff;
                                    Challenger.depart1=date + data.train_info[StaPos].depart_time;
                                    Challenger.sta1=StaPos;
                                    Challenger.end1=CentPos1;
                                    Challenger.sta2=CentPos2;
                                    Challenger.end2=EndPos;
                                    Challenger.price1=data.train_info[CentPos1].prefix_price-data.train_info[StaPos].prefix_price;
                                    Challenger.price2=Candidate.train_info[EndPos].prefix_price-Candidate.train_info[CentPos2].prefix_price;
                                    //           std::cerr<<"transfer_3"<<'\n';
                                    Ticket::Date Timetmp = Candidate.train_info[CentPos2].depart_time;
                                    Ticket::pair<int,int> mmdd=time.getMMDD();
                                    Ticket::pair<int,int> hrmi=Timetmp.getHHMM();
                                    //Datetmp表示该车到站同天的离站时间
                                    Ticket::Date Datetmp(mmdd.first,mmdd.second,hrmi.first,hrmi.second);
                                    //Start表示该车最早离站时间
                                    mmdd=Candidate.train_info[CentPos2].Sta_Date.getMMDD();
                                    Ticket::Date Start(mmdd.first,mmdd.second,hrmi.first,hrmi.second);
                                    if (Datetmp < Start) {
                                        //同天离站时间小于起始离站，说明该天在发车起始以外，需要等到第一趟车
                                        diff += Start.diffMinute(time);
                                        Challenger.depart2=Start;
                                    } else {
                                        if (time < Datetmp) diff += Datetmp.diffMinute(time);
                                        else {
                                            //同天离站时间小于到站时间，那么要多等一天
                                            ++Datetmp;
                                            diff += Datetmp.diffMinute(time);
                                        }
                                        Challenger.depart2=Datetmp;
                                    }
                                    Challenger.diff2=Candidate.train_info[EndPos].prefix_time -
                                                     Candidate.train_info[CentPos2].prefix_time -
                                                     Candidate.train_info[EndPos].stopover;
                                    if (type == 'P') {
                                        nums = Challenger.price1+Challenger.price2;
                                    }
                                    else {
                                        diff += Challenger.diff2;
                                        nums = diff;
                                    }
                                    Challenger.num = nums;
                                    Challenger.Train_ID_End = Candidate.Train_SN;
                                    Challenger.Train_ID_Sta = data.Train_SN;
                                    Ret = std::min(Challenger, Ret);
                                }
                            }
                        }
                    }
                }
            }
            if(Ret.num==88888888) os<<'0'<<'\n';
            else{
                Ticket::Date tmp=Ret.depart1+Ret.diff1;
                int seat=_get_seat_range(Ret.Train_ID_Sta,Sta,Ret.depart1,Ret.sta1,Ret.end1);
                os<<Ret.Train_ID_Sta<<' '<<Sta<<' '<<Ret.depart1<<' '<<'-'<<'>'<<' '<<Ret.Cent<<' '<<tmp<<' '<<Ret.price1<<' '<<seat<<'\n';
                Ticket::Date tmp2=Ret.depart2+Ret.diff2;
                //   std::cerr<<Ret.depart2<<" "<<Ret.diff2<<'\n';
                seat=_get_seat_range(Ret.Train_ID_End,Ret.Cent,Ret.depart2,Ret.sta2,Ret.end2);
                os<<Ret.Train_ID_End<<' '<<Ret.Cent<<' '<<Ret.depart2<<' '<<'-'<<'>'<<' '<<Det<<' '<<tmp2<<' '<<Ret.price2<<' '<<seat<<'\n';
            }
            return true;
        }

        void GetSeat(const Ticket::String<22> &Train_ID, Ticket::Date &Start_Date, Ticket::Date &End_Date,
                     Ticket::String<36> &Sta, Ticket::String<36> &End, int &sta, int &end, int &seat, int &price,
                     int nums) {
            int pos = _BPT_Train.find(Train_ID);
            if (pos == -1) {
                seat = -2;
                return;
            }

            Train data = _BPT_Train.getVal(pos);
            if(nums>data.seat) {seat=-2;return;}
            int sz = data.station_num;
            bool is_exist=false;
            for (int i = 0; i < sz; i++) {
                if (data.train_info[i].station == Sta) {
                    is_exist=true;
                    sta = i;
                    break;
                }
            }
            if(!is_exist) {seat=-2;return;}
            //  std::cerr<<'4'<<'\n';
            //std::cout<<Start_Date.timeCnt<<"\n";
            //std::cout<<Start_Date.to_string()<<' '<<data.train_info[sta].Sta_Date.to_string()<<' '<<data.train_info[sta].End_Date.to_string();
            if (data.train_info[sta].Sta_Date.cmpDate(Start_Date) > 0 ||
                Start_Date.cmpDate(data.train_info[sta].End_Date) > 0) {
                seat = -2;
                return;
            }


            Ticket::pair<int,int> hrmi=data.train_info[sta].depart_time.getHHMM();
            Ticket::pair<int,int> mmdd=Start_Date.getMMDD();
            /*     Start_Date.mi = data.train_info[sta].arri_time.mi;
                 Start_Date.hr = data.train_info[sta].arri_time.hr;*/
            Ticket::Date Stmp(mmdd.first,mmdd.second,hrmi.first,hrmi.second);
            Start_Date=Stmp;
            mmdd=Start_Date.getMMDD();
            Ticket::Date Dtmp(mmdd.first,mmdd.second,0,0);
            Seat_Key seatKey;
            seatKey.station = Sta;
            seatKey.train = Train_ID;
            seatKey.time = Dtmp.getDateStr();
            Ticket::Date Datetmp = Start_Date;
            int seatpos;
            seat = data.seat;
            int seattmp;
            //    std::cerr<<'1'<<'\n';
            //  std::cerr<<nums<<'\n';
            is_exist=false;
            for (int i = sta; i < sz; i++) {
                if (data.train_info[i].station == End) {
                    end = i;
                    is_exist=true;
                    break;
                }
                seatKey.station=data.train_info[i].station;
                seatpos = _BPT_Seat.find(seatKey);
                seattmp = _BPT_Seat.getVal(seatpos);
                //       std::cerr<<'2'<<'\n';
                seat = std::min(seat, seattmp);
                Datetmp += (data.train_info[i + 1].prefix_time - data.train_info[i].prefix_time);
                Ticket::pair<int,int> mmdd2=Datetmp.getMMDD();
                Ticket::Date Datekey(mmdd2.first,mmdd2.second,0,0);
                /*      Datekey.mm = Datetmp.mm;
                      Datekey.dd = Datetmp.dd;*/
                seatKey.time = Datekey.getDateStr();
            }
            //   std::cerr<<'3'<<'\n';
            //   std::cerr<<data.station_num<<' '<<end<<"\n";
            if(!is_exist) {seat=-2;return;}
            if(sta>=end) {seat=-2;return;}
            if(seat<nums) seat=-1;
            End_Date = Datetmp+(-data.train_info[end].stopover);
            price = data.train_info[end].prefix_price - data.train_info[sta].prefix_price;
        }

        void RenewSeat(const Ticket::String<22> & Train_ID, const Ticket::Date &Sta_Time,
                       int sta,int end, int devi) {
            int pos = _BPT_Train.find(Train_ID);
            Train data = _BPT_Train.getVal(pos);
            int sz = data.station_num;

            // seatKey.time=Sta_Time.to_string();
            Ticket::Date Datetmp = Sta_Time;
            Ticket::pair<int,int> mmdd=Datetmp.getMMDD();
            Ticket::Date Datekey(mmdd.first,mmdd.second,0,0);
/*            DateKey.mm = Datetmp.mm;
            DateKey.dd = Datetmp.dd;*/
            Seat_Key seatKey;
            //seatKey.station = Sta;
            seatKey.train = Train_ID;
            seatKey.time = Datekey.getDateStr();
            int seatpos;
            int oriseat;
            for (int i = sta; i <end; i++) {
                seatKey.station = data.train_info[i].station;
                seatpos = _BPT_Seat.find(seatKey);
                oriseat = _BPT_Seat.getVal(seatpos);
                oriseat += devi;
                _BPT_Seat.modifyVal(seatpos, oriseat);
                //if (data.train_info[i].station == Sta) break;
                Datetmp += (data.train_info[i + 1].prefix_time - data.train_info[i].prefix_time);
                Ticket::pair<int,int> mmdd2=Datetmp.getMMDD();
                Ticket::Date DateKey2(mmdd2.first,mmdd2.second,0,0);
                /*   DateKey.mm = Datetmp.mm;
                   DateKey.dd = Datetmp.dd;*/
                seatKey.time = DateKey2.getDateStr();
            }
        }

        ~Train_manager() {
            Count.write(Count.BEG, cnt);
            Count.close();
        };
        void clean(){
            Count.write(Count.BEG, cnt);
            Count.close();
            _BPT_Train.clear();
            _BPT_Station.clear();
            _BPT_Rl.clear();
            _BPT_Seat.clear();
            Count.clear();
        }
    };
}
#endif