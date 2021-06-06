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
    public:
        struct Train {
            //private:
            Ticket::String<24> Train_SN;
            Ticket::Date start_day;
            Ticket::Date end_day;
            Ticket::Date start_time;
            // Ticket::String<40> start_station;
            int station_num = 0;
            int Pos=0;
            char type='\0';
            int seat=0;
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

    private:


        struct Station {
            int index=0;
            int Pos = 0;

            //Ticket::Time arri_time;
            Station() = default;

            Station(const int & Index, const int &pos) {
                index =Index ;
                Pos = pos;
                //arri_time=o.train_info[pos].arri_time;
                //start=o.train_info[pos].arri_Date;
                //end=o.train_info[pos].depart_Date;
            }

        };

        struct Seat_Key {
            Ticket::String<6> time;
            Ticket::String<24> train;
            Seat_Key() = default;

            Seat_Key(const Ticket::String<24> &Train,
                     const Ticket::String<6> &Time) {
                time = Time;
                train = Train;
            }

            bool operator==(const Seat_Key &r) const {
                return time == r.time && train == r.train;
            }

            bool operator<(const Seat_Key &r) const {
                return train < r.train||(train==r.train&&time<r.time);
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

        struct Seat{
            int seatarr[100]={0};
        //    int n;
        };

        Ticket::BPlusTree<Seat_Key,Seat> _BPT_Seat;
        Ticket::BPlusTree<Ticket::String<24>, Train> _BPT_Train;
        Ticket::BPlusTree<Station_Key, Station> _BPT_Station;
        Ticket::BPlusTree<Ticket::String<24>, char> _BPT_Rl;
        Ticket::FileIONoCache Count;
        int cnt = 0;

        int
        _get_seat_range(const Ticket::String<24> &Train_ID, const Ticket::Date &Start_Date, int sta,
                        int end,int sup) {

            //Ticket::pair<int,int> mmdd=Start_Date.getMMDD();
            Ticket::Date Datekey(Start_Date.transToDate());
            /*   Datekey.mm=Start_Date.mm;
               Datekey.dd=Start_Date.dd;*/
            Seat_Key seatKey;
            //seatKey.station = Sta;
            seatKey.train = Train_ID;
            seatKey.time = Datekey.getDateStr();
            int seat = sup;
         //   Seat_Key seatKey1(Train_ID,Start_Date.getDateStr());
            int seatpos=_BPT_Seat.find(seatKey);
            Seat seatarr=_BPT_Seat.getVal(seatpos);
            for (int i = sta; i < end; i++)  {
              //  std::cout<<seatarr.seatarr[i]<<'\n';
                seat = std::min(seat, seatarr.seatarr[i]);
            }
            //  std::cout<<"__________________"<<'\n';
            return seat;
        }


       /* int _get_seat_range(const Ticket::String<22> &Train_ID, const Ticket::String<36> &Sta, const Ticket::Date &Start_Date, int sta,
                                   int end) {
            int pos=_BPT_Train.find(Train_ID);
            Train data=_BPT_Train.getVal(pos);
           // Ticket::pair<int,int> mmdd=Start_Date.getMMDD();
            Ticket::Date Datekey(Start_Date.transToDate());
            *//*   Datekey.mm=Start_Date.mm;
               Datekey.dd=Start_Date.dd;*//*
            Seat_Key seatKey;
            seatKey.station = Sta;
            seatKey.train = data.Train_SN;
            seatKey.time = Datekey.getDateStr()
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
                //mmdd=Datetmp.getMMDD();
                Ticket::Date Datekey2(Datetmp.transToDate());
                *//*      Datekey.mm = Datetmp.mm;
                      Datekey.dd = Datetmp.dd;*//*
                seatKey.time = Datekey2.getDateStr();
            }
            //  std::cout<<"__________________"<<'\n';
            return seat;
        }*/
        struct Comp {
            int data = 0;
            int pos = 0;
            Ticket::String<24> ID;
            bool operator<(const Comp &o) const {
                return data < o.data || (data == o.data && ID < o.ID);
            }
        };

        struct Trans_Comp {
            Ticket::String<24> Train_ID_Sta;
            Ticket::String<24> Train_ID_End;
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
            Ticket::Date Start_Date1;
            Ticket::Date Start_Date2;
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

        bool add_train(const Ticket::String<24> &SN, int &stanum, int &seatnum, Ticket::String<36> *stations,
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
            data.start_day = saleDate[0];
            data.end_day = saleDate[1];
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
            data.train_info[0].Sta_Date = data.start_day;
            data.train_info[0].End_Date = data.end_day;
            Ticket::Date Statmp = data.start_day+Datetmp;
            Ticket::Date Endtmp = data.end_day+Datetmp;
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
               // Ticket::pair<int,int> mmdd=Statmp.getMMDD();
                Ticket::Date Dtmp(Statmp.transToDate());
               // Ticket::pair<int,int> mmdd2=Endtmp.getMMDD();
                Ticket::Date Dtmp2(Endtmp.transToDate());
                data.train_info[i].Sta_Date = Dtmp;
                data.train_info[i].End_Date = Dtmp2;
                data.train_info[i].prefix_time =data.train_info[i - 1].prefix_time + traveltime[i] + stoppovertime[i];//
            }
            _BPT_Train.insert(SN,data);
            _BPT_Rl.insert(SN, 'N');
            return true;
            //终点站,price与出发时间停留时间无用
        }

        bool delete_train(const Ticket::String<24> &SN) {
            //这里好像有点鲁棒？判断是否发售？
            int pos = _BPT_Rl.find(SN);
            if (pos == -1) return false;
            else {
                //Seat_arr seatArr=_BPT_Seat.getVal(pos);
                char flag = _BPT_Rl.getVal(pos);
                if (flag == 'N') {
                    if (_BPT_Rl.erase(SN) && _BPT_Train.erase(SN)) return true;
                    else {
                        std::cerr<<"heher7"<<'\n';
                        Error("delete_train_2");
                    }
                    //std::cerr<<"delete_train_3",throw std::exception();
                } else if (flag == 'Y') return false;
                else {
                    std::cerr<<"hehe6"<<'\n';
                    Error("delete_train_3");
                }
                //std::cerr<<"delete_train_3",throw std::exception();
            }
        }

        bool release_train(const Ticket::String<24> &SN) {
            int pos = _BPT_Train.find(SN);
            //Otmp_pos为第二key为0的位置
            Station_Key data_key;
            if (pos == -1) return false;
            int rl_pos = _BPT_Rl.find(SN);
            if (rl_pos == -1) {
                std::cerr<<"heher5"<<'\n';
                Error("release_train_2");}//std::cerr<<"release_train_2",throw std::exception();
            char flag = _BPT_Rl.getVal(rl_pos);
            if (flag == 'Y') return false;
            _BPT_Rl.modifyVal(rl_pos, 'Y');
            Train data = _BPT_Train.getVal(pos);
            data_key.pos = ++cnt;
            for (int i = 0; i < data.station_num; i++) {
                data_key.Station_name = data.train_info[i].station;
                Station station(i,pos);
                int flag = _BPT_Station.insert(data_key, station);
                if (flag == -1)  Error("release_train_4");
            }
            Seat Seatdata;
           // Seatdata.n=data.station_num;
            for(int i=0;i<data.station_num;i++) Seatdata.seatarr[i]=data.seat;
            for (Ticket::Date tmp=data.start_day; data.start_day.cmpDate(tmp) <= 0 &&
                   tmp.cmpDate(data.end_day) <= 0; ++tmp) {
                Seat_Key seatKey(SN,tmp.getDateStr());
                if (_BPT_Seat.insert(seatKey,Seatdata) == -1) {
                    Error("release_train_5");
                }
            }
            return true;
            //_BPT_Train.modifyVal(pos,data);
        }

        bool query_train(const Ticket::String<24> &SN, const Ticket::Date &tDate, std::ostream &os) {
            int pos = _BPT_Train.find(SN);
            if (pos == -1) return false;
            else {
                int Rlpos = _BPT_Rl.find(SN);
                if (Rlpos == -1) {
                    std::cerr<<"hehe2"<<'\n';
                    Error("query_train");}
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
                //    Ticket::pair<int,int> mmdd=tDate.getMMDD();
                    Ticket::Date Datekey(tDate.transToDate());
                    Seat_Key seatKey;
                    seatKey.train=SN;
                    seatKey.time=tDate.getDateStr();
                    int SeatPos=_BPT_Seat.find(seatKey);
                    Seat Seatdata=_BPT_Seat.getVal(SeatPos);
                    cal = tDate + Datetmp;
                    os << SN << ' ' << data.type << '\n';
                    os << data.train_info[0].station << ' ' << "xx-xx xx:xx ->" << ' ' << cal << ' '
                       << data.train_info[0].prefix_price << ' '
                       << Seatdata.seatarr[0] << '\n';
                    for (int i = 1; i < sz - 1; i++) {
                        cal += data.train_info[i].prefix_time - data.train_info[i - 1].prefix_time -
                               data.train_info[i].stopover;
                        os << data.train_info[i].station << ' ' << cal << ' ' << '-' << '>' << ' ';
                        cal += data.train_info[i].stopover;
                        os << cal << ' ' << data.train_info[i].prefix_price << ' '
                           << Seatdata.seatarr[i] << '\n';
                    }
                    cal += data.train_info[sz - 1].prefix_time - data.train_info[sz - 2].prefix_time -
                           data.train_info[sz - 1].stopover;
                    os << data.train_info[sz - 1].station << ' ' << cal << ' ' << "-> xx-xx xx:xx" << ' '
                       << data.train_info[sz - 1].prefix_price << ' ' << 'x' << '\n';
                } else {
                    std::cerr<<"heher"<<'\n';
                    throw Ticket::SyntaxError();
                }
                return true;
            }
            //出错的话先检查spos和pos

        }

        bool
        query_ticket(const Ticket::String<36> &Sta, const Ticket::String<36> &Det, const Ticket::Date &date, char type,
                     std::ostream &os) {//type 'T'-time 'P'-price
            Backend::map<int, int> match;
            std::vector<std::pair<int,int>> aimPosvec;
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
                Ticket::pair<const int, int> tmp(stationtmp.Pos, stationtmp.index);
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
                    int StaIndex=match[stationtmp.Pos];
                    if (stationtmp.index >StaIndex )
                        aimPosvec.push_back(std::pair<int,int>(stationtmp.Pos,StaIndex));
                } catch (NotFound) {}
            }
            //if(match.count(stationtmp.Train_SN)) aimIDvec.push_back(stationtmp.Train_SN);}
            if (match.empty()) { return false; }
            //    std::cout<<"------------------"<<'\n';
            std::vector<Train> Trainvec;
            //int pos;
            sz = aimPosvec.size();
            //把Train取出来
            Ticket::Date DateKey(date);
            /*       DateKey.dd = date.dd;
                   DateKey.mm = date.mm;*/
            //std::cout<<"-------------"<<'\n';
            for (int i = 0; i < sz; i++) {
                //时间判断
                Train Candidate=_BPT_Train.getVal(aimPosvec[i].first);
             //   std::cout<<Candidate.Train_SN<<' '<<' '<<Candidate.train_info[aimPosvec[i].second].Sta_Date<<' '<<date<<' '<<aimPosvec[i].second<<'\n';
                if (Candidate.train_info[aimPosvec[i].second].Sta_Date.cmpDate(date)<=0&&date.cmpDate(Candidate.train_info[aimPosvec[i].second].End_Date)<=0) {
                    Trainvec.push_back(Candidate);
                } else {}
            }
            sz = Trainvec.size();
            Comp arr[sz];
            std::pair<int, int> pospair[sz];
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
            Ticket::Date Dtmp;
            os << sz << '\n';
            for (int i = 0; i < sz; i++) {
                int Trainpos = arr[i].pos;
                Dtmp = date + Trainvec[Trainpos].train_info[pospair[Trainpos].first].depart_time;
                Ticket::Date DateKey=Dtmp-Trainvec[Trainpos].train_info[pospair[Trainpos].first].prefix_time;
               // std::cout<<'-'<<DateKey<<'\n';
                int seat = _get_seat_range(Trainvec[Trainpos].Train_SN, DateKey, pospair[Trainpos].first,
                                           pospair[Trainpos].second,Trainvec[Trainpos].seat);
                os << arr[i].ID << ' ' << Sta << ' ' << Dtmp << ' ' << '-' << '>' << ' ';
                int diff = Trainvec[Trainpos].train_info[pospair[Trainpos].second].prefix_time -
                           Trainvec[Trainpos].train_info[pospair[Trainpos].first].prefix_time -
                           Trainvec[Trainpos].train_info[pospair[Trainpos].second].stopover;
                Dtmp += diff;
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
            if(StaPosvec.empty()) return false;
            stationKey.Station_name = Det;
            std::vector<int> EndPosvec = _BPT_Station.route<Station_Comp>(stationKey);
            if(EndPosvec.empty()) return false;
            std::vector<Station> Endvec;
            //std::vector<Station> Stavec;
            //一切的核心这个map Key值是尾站的所有train名， vector是尾站相同的中转站的在从中转站到尾站中车的位置
            //但由于中转站在从起点出发的火车中位置基本上与从中转站出发到尾站中的位置不一样
            //所以用pair来存，其中first是在从起点出发的火车中的位置，second是站在转乘的火车中的位置
           // std::priority_queue<Trans_Comp> PQ;
            //Ticket::String<25> Ret[2];
            //起始站
            int nums;
            Trans_Comp Ret;
            Ret.num = 888888888;
          /*  for(int i=0;i<StaPosvec.size();i++){
            //    std::cerr<<"transfer_0"<<'\n';
                Station Start=_BPT_Station.getVal(StaPosvec[i]);
                Train train1=_BPT_Train.getVal(Start.Pos);
                map<Ticket::String<36>,int> Stamatch;
                for(int j=0;j<train1.station_num;j++){
                    map<Ticket::String<36>,int>::value_type Value(train1.train_info[j].station,j);
                    Stamatch.insert(Value);
                }
                for(int j=0;j<EndPosvec.size();j++){
                //    std::cerr<<"transfer_0.5"<<'\n';
                    Station End=_BPT_Station.getVal(EndPosvec[j]);
                    std::vector<std::pair<int,int>> Centvec;
                    if(End.Pos!=Start.Pos) {
                       // std::cerr<<"transfer_1"<<'\n';
                        Train train2 = _BPT_Train.getVal(End.Pos);
                        for (int k = 0; k < train2.station_num; k++) {
                            if (Stamatch.count(train2.train_info[k].station))
                                Centvec.push_back(std::pair<int, int>(Stamatch[train2.train_info[k].station], k));
                        }
                        for(int k=0;k<Centvec.size();k++) {
                            std::cerr<<"transfer_1.5"<<'\n';
                            int StaIndex=Start.index;
                            int CentIndex1=Centvec[k].first;
                            int CentIndex2=Centvec[k].second;
                            int EndIndex=End.index;
                            std::cerr<<StaIndex<<' '<<CentIndex1<<' '<<CentIndex2<<' '<<EndIndex<<'\n';
                            std::cerr<<train1.train_info[StaIndex].Sta_Date<<' '<<date<<' '<<train1.train_info[StaIndex].End_Date<<'\n';
                            if(StaIndex<CentIndex1&&CentIndex2<EndIndex
                            &&train1.train_info[StaIndex].Sta_Date.cmpDate(date)<=0
                            &&date.cmpDate(train1.train_info[StaIndex].End_Date)<=0){
                                std::cerr<<"transfer_2"<<'\n';
                                std::cerr<<train1.Train_SN<<' '<<train2.Train_SN<<'\n';
                                int diff=train1.train_info[CentIndex1].prefix_time-train1.train_info[StaIndex].prefix_time-train1.train_info[CentIndex1].stopover;
                                Ticket::Date Time=date+train1.train_info[StaIndex].depart_time+diff;
                                std::cerr<<train1.train_info[CentIndex1].End_Date+train1.train_info[CentIndex1].depart_time<<' '<<Time<<'\n';
                                if(!(train2.train_info[CentIndex1].End_Date+train2.train_info[CentIndex1].depart_time<Time)){
                                    Ticket::Date StartTime=train2.train_info[CentIndex2].Sta_Date+train2.train_info[CentIndex2].depart_time;
                                    Ticket::Date PossiTime=Time.transToDate()+train2.train_info[CentIndex2].depart_time;
                                    Trans_Comp Challenger;
                                    Challenger.Train_ID_Sta=train1.Train_SN;
                                    Challenger.Train_ID_End=train2.Train_SN;
                                    Challenger.sta1=StaIndex;
                                    Challenger.end1=CentIndex1;
                                    Challenger.sta2=CentIndex2;
                                    Challenger.end2=EndIndex;
                                    Challenger.diff1=diff;
                                    Challenger.Cent=train1.train_info[CentIndex1].station;
                                    Challenger.price1=train1.train_info[CentIndex1].prefix_price-train1.train_info[StaIndex].prefix_price;
                                    Challenger.price2=train2.train_info[EndIndex].prefix_price-train2.train_info[CentIndex2].prefix_price;
                                    std::cerr<<Challenger.price1<<" "<<Challenger.price2<<" "<<'\n';
                                    Challenger.depart1=date+train1.train_info[StaIndex].depart_time;
                                    Challenger.Start_Date1=date+train1.train_info[StaIndex].depart_time-train1.train_info[StaIndex].prefix_time;
                                    if(Time<StartTime){
                                        diff+=StartTime.diffMinute(Time);
                                        Challenger.depart2=StartTime;
                                        Challenger.Start_Date2=train2.train_info[CentIndex2].Sta_Date-train2.train_info[CentIndex2].prefix_time;
                                    }
                                    else{
                                        if(!(PossiTime<Time)) diff+= PossiTime.diffMinute(Time);
                                        else{
                                            ++PossiTime;
                                            diff+=PossiTime.diffMinute(Time);
                                        }
                                        Challenger.depart2=PossiTime;
                                        Challenger.Start_Date2=Challenger.depart2-train2.train_info[CentIndex2].prefix_time;
                                    }
                                    Challenger.diff2=train2.train_info[EndIndex].prefix_time-train2.train_info[CentIndex2].prefix_time-train2.train_info[EndIndex].stopover;
                                    diff+=Challenger.diff2;
                                    if(type=='P') Challenger.num=Challenger.price1+Challenger.price2;
                                    else Challenger.num=diff;
                                    Ret=std::min(Ret,Challenger);
                                }
                            }
                        }
                    }
                }
            }*/
           // std::cerr<<"transfer_0"<<'\n';
            //std::cerr<<"-----------"<<'\n';
            for (int i = 0; i < StaPosvec.size(); i++) {
                map<int, std::vector<std::pair<int, int>>> Endmatch;
                for (int i = 0; i < EndPosvec.size(); i++) {
                    Endvec.push_back(_BPT_Station.getVal(EndPosvec[i]));
                    std::vector<std::pair<int, int>> tmpvec;
                    map<int, std::vector<std::pair<int, int>>>::value_type valueType(Endvec[i].Pos,tmpvec);
                    Endmatch.insert(valueType);
                }
              //  for(int j=0;j<EndPosvec.size();j++) Endmatch[Endvec[j].Pos].clear();
                //   std::cerr<<"transfer_0.5"<<'\n';
                Station StaStation = _BPT_Station.getVal(StaPosvec[i]);
                Train data = _BPT_Train.getVal(StaStation.Pos);
                int statnum = data.station_num;
                //遍历起始站的每一辆车车的每一个节点
                for (int j = StaStation.index; j < statnum; j++) {
                    //遍历该节点的每一辆车，并将其插入map中
                    Station_Key CentKey;
                    CentKey.pos = 0;
                    CentKey.Station_name = data.train_info[j].station;
                    std::vector<int> CentPosvec = _BPT_Station.route<Station_Comp>(CentKey);
                    std::vector<Station> Centvec;
                    int Centpos1 = j;
                    for (int k = 0; k < CentPosvec.size(); k++) {
                      //  std::cout<<"-------"<<'\n';
                        if (Centvec[k].Pos == StaStation.Pos){
                            Centpos1 = Centvec[k].index;
                          //  std::cerr<<Centpos1<<' '<<j<<'\n';
                       //     std::cout<<Centpos1<<"\n";
                        }
                    }
                    for (int k = 0; k < CentPosvec.size(); k++) {
                        Centvec.push_back(_BPT_Station.getVal(CentPosvec[k]));
                        if (Endmatch.count(Centvec[k].Pos)) {
                            std::pair<int, int> value(Centpos1, Centvec[k].index);
                            Endmatch[Centvec[k].Pos].push_back(value);
                        }
                    }
                }
                for (int j = 0; j < Endvec.size(); j++) {
                    if (StaStation.Pos != Endvec[j].Pos) {//换乘的不是同一辆车
                          //   std::cerr<<"transfer_1"<<'\n';
                        Train Candidate = _BPT_Train.getVal(Endvec[j].Pos);
                        std::vector<std::pair<int, int>> CentTrainpos = Endmatch[Endvec[j].Pos];
                        for (int k = 0; k < CentTrainpos.size(); k++) {
                                    //     std::cerr<<"transfer_1.5"<<'\n';
                            int StaPos = StaStation.index;
                            int CentPos1 = CentTrainpos[k].first;
                            int CentPos2 = CentTrainpos[k].second;
                            int EndPos = Endvec[j].index;
                                   //     std::cerr<<StaPos<<' '<<CentPos1<<' '<<CentPos2<<' '<<EndPos<<'\n';
                                   //     std::cerr<<data.train_info[StaPos].Sta_Date<<' '<<date<<' '<<data.train_info[StaPos].End_Date<<'\n';
                            if (StaPos < CentPos1 && CentPos2 < EndPos &&
                                data.train_info[StaPos].Sta_Date.cmpDate(date) <= 0 &&
                                date.cmpDate(data.train_info[StaPos].End_Date)<=0) {//查询的始发时间合法//保证正向
                                int diff = data.train_info[CentPos1].prefix_time -
                                           data.train_info[StaPos].prefix_time-data.train_info[CentPos1].stopover;
                                //time表示中转站时间
                                Ticket::Date time = date + data.train_info[StaPos].depart_time+ diff;
                                //只要到达中转时间小于最晚离开时间即可
                                  //           std::cerr<<"transfer_2"<<'\n';
                                if (!(Candidate.train_info[CentPos2].End_Date +
                                         Candidate.train_info[CentPos2].depart_time<time)) {
                                    Trans_Comp Challenger;
                                    Challenger.Cent=data.train_info[CentPos1].station;
                                   // std::cerr<<CentPos1<<' '<<Challenger.Train_ID_Sta<<"\n";
                                   // std::cerr<<CentPos2<<' '<<Challenger.Train_ID_End<<'\n';
                                    Challenger.diff1=diff;
                                    Challenger.depart1=date + data.train_info[StaPos].depart_time;
                                    Challenger.sta1=StaPos;
                                    Challenger.end1=CentPos1;
                                    Challenger.sta2=CentPos2;
                                    Challenger.end2=EndPos;
                                    Challenger.Start_Date1=date + data.train_info[StaPos].depart_time-data.train_info[StaPos].prefix_time;
                                    Challenger.price1=data.train_info[CentPos1].prefix_price-data.train_info[StaPos].prefix_price;
                                    Challenger.price2=Candidate.train_info[EndPos].prefix_price-Candidate.train_info[CentPos2].prefix_price;
                                    Ticket::Date Timetmp = Candidate.train_info[CentPos2].depart_time;
                                    //Datetmp表示该车到站同天的离站时间
                                    Ticket::Date Datetmp(time.transToDate()+Timetmp);
                                    //Start表示该车最早离站时间
                                    Ticket::Date Start(Candidate.train_info[CentPos2].Sta_Date+Timetmp);
                                    if (Datetmp < Start) {
                                        //同天离站时间小于起始离站，说明该天在发车起始以外，需要等到第一趟车
                                        diff += Start.diffMinute(time);
                                        Challenger.depart2=Start;
                                        Challenger.Start_Date2=Start;
                                    } else {
                                        if (!(Datetmp<time)) diff += Datetmp.diffMinute(time);
                                        else {
                                            //同天离站时间小于到站时间，那么要多等一天
                                            ++Datetmp;
                                            diff += Datetmp.diffMinute(time);
                                        }
                                        Challenger.depart2=Datetmp;
                                        Challenger.Start_Date2=Datetmp-Candidate.train_info[CentPos2].prefix_time;
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
            if(Ret.num==888888888) return false;
            else{
                Ticket::Date tmp=Ret.depart1+Ret.diff1;
                int seat=_get_seat_range(Ret.Train_ID_Sta,Ret.Start_Date1,Ret.sta1,Ret.end1,100001);
                os<<Ret.Train_ID_Sta<<' '<<Sta<<' '<<Ret.depart1<<' '<<'-'<<'>'<<' '<<Ret.Cent<<' '<<tmp<<' '<<Ret.price1<<' '<<seat<<'\n';
                Ticket::Date tmp2=Ret.depart2+Ret.diff2;
                //   std::cerr<<Ret.depart2<<" "<<Ret.diff2<<'\n';
                seat=_get_seat_range(Ret.Train_ID_End,Ret.Start_Date2,Ret.sta2,Ret.end2,100001);
                os<<Ret.Train_ID_End<<' '<<Ret.Cent<<' '<<Ret.depart2<<' '<<'-'<<'>'<<' '<<Det<<' '<<tmp2<<' '<<Ret.price2<<' '<<seat<<'\n';
            }
            return true;
        }

        bool GetTrain(Train & data,const Ticket::String<24> & Train_ID){
            int pos=_BPT_Train.find(Train_ID);
            if(pos==-1) return false;
            else {data=_BPT_Train.getVal(pos);return true;}
        }

        void GetSeat(Train & data, Ticket::Date &Start_Date, Ticket::Date &End_Date,
                     const Ticket::String<36> &Sta, const Ticket::String<36> &End, int &sta, int &end, int &seat, int &price,
                     int nums) {
           // int pos = _BPT_Train.find(Train_ID);
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
            //此时Start_Date 不含分秒
            //Ticket::Date Stmp();
        //    Seat_Key seatKey;
       //     seatKey.train = data.Train_SN;
       //     seatKey.time = Start_Date.getDateStr();
            Start_Date=Start_Date+data.train_info[sta].depart_time;
         //   std::cout<<seatKey.time<<"\n";
            Ticket::Date Dtmp(Start_Date);
            Dtmp-=data.train_info[sta].prefix_time;
         //   std::cout<<Dtmp<<"\n";
            //mmdd=Start_Date.getMMDD();
            //Ticket::Date Datetmp = Start_Date;
            int seatpos;
            seat = data.seat;
            int seattmp;
            //    std::cerr<<'1'<<'\n';
            //  std::cerr<<nums<<'\n';
            is_exist=false;
            Seat_Key seatKey1(data.Train_SN,Dtmp.getDateStr());
            Seat seat1;
            int pos=_BPT_Seat.find(seatKey1);
           // std::cout<<pos<<'\n';
            seat1=_BPT_Seat.getVal(pos);
         //   for(int i=0;i<sz;i++) std::cout<<seat1.seatarr[i]<<'\n';
            for (int i = sta; i < sz; i++) {
                if (data.train_info[i].station == End) {
                    end = i;
                    is_exist=true;
                    break;
                }
                //       std::cerr<<'2'<<'\n';
                seat = std::min(seat, seat1.seatarr[i]);
            }
            //   std::cerr<<'3'<<'\n';
            //   std::cerr<<data.station_num<<' '<<end<<"\n";
            if(!is_exist) {seat=-2;return;}
            if(sta>=end) {seat=-2;return;}
            if(seat<nums) seat=-1;
            End_Date = Start_Date+data.train_info[end].prefix_time-data.train_info[sta].prefix_time-data.train_info[end].stopover;
            price = data.train_info[end].prefix_price - data.train_info[sta].prefix_price;
        }

        void RenewSeat(Train & data, const Ticket::Date &Sta_Time,
                       int sta,int end, int devi) {
            int sz = data.station_num;
            //这里Sta_Time包含分秒
            Ticket::Date Datetmp = Sta_Time-data.train_info[sta].prefix_time;
            Ticket::Date Datekey=(Datetmp.transToDate());
            Seat_Key seatKey;
            seatKey.train = data.Train_SN;
            seatKey.time = Datekey.getDateStr();
            int seatpos=_BPT_Seat.find(seatKey);
            if(seatpos==-1) std::cerr<<"fhere"<<'\n';
        //    std::cout<<seatpos<<'\n';
            Seat seat=_BPT_Seat.getVal(seatpos);
        //    std::cout<<seatKey.time<<'\n';
            for (int i = sta; i <end; i++) {
                seat.seatarr[i]+=devi;
            }
            _BPT_Seat.modifyVal(seatpos, seat);
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