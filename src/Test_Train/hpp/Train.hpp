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
#include<unordered_map>
#include<queue>
namespace Backend {
#define Error(x) throw Ticket::WrongOperation(x)
    class My_Unordered_Map{
    private:
        struct Node{
            int _data=-1;
            size_t _key=0;
            Node *next=nullptr;
        };
        static const int factor=66919;
        Node data[factor];
        std::vector<int> indexvec;
    public:
        My_Unordered_Map()=default;
        void insert(size_t & tkey,int  tdata){
            size_t index=tkey%factor;
         //   std::cout<<"insert:"<<tkey<<'\n';
            if(data[index]._key!=0){
                Node * tmp=data[index].next;
                Node * prev=&(data[index]);
             //   std::cout<<prev->_key<<' '<<'\n';
                while(tmp!=nullptr){
                    prev=prev->next;
                    tmp=tmp->next;
                }
                tmp=new Node;
                tmp->next=nullptr;
                tmp->_data=tdata;
                tmp->_key=tkey;
                prev->next=tmp;
            }
            else{
                indexvec.push_back(index);
                data[index]._data=tdata;
                data[index]._key=tkey;
            }
        }
        int GetData(size_t & tKey){
            size_t index=tKey % factor;
        //    std::cout<<"GetData:"<<tKey<<'\n';
            if(data[index]._key!=0){
                Node * tmp=&(data[index]);
                while(tmp!=nullptr){
                 //   std::cout<<tmp->_key<<'\n';
               //     std::cout<<tmp->_data<<'\n';
                    if(tmp->_key==tKey) return tmp->_data;
                    tmp=tmp->next;
                }
                return -1;
            }
            else return -1;
        }
        ~My_Unordered_Map(){
            clear();
        }
        void clear(){
            Node * tmp;
            int pos;
            for(int i=0;i<indexvec.size();i++){
                pos=indexvec[i];
                tmp=data[pos].next;
                while(tmp!=nullptr){
                    data[pos].next=(data[pos].next)->next;
                    delete tmp;
                    tmp=data[pos].next;
                }
                data[pos].next=nullptr;
                data[pos]._data=-1;
                data[pos]._key=0;
            }
            indexvec.clear();
        }
    };
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
         //   Ticket::String<6> time;
         //   Ticket::String<24> train;
         size_t time;
         size_t train;
            Seat_Key() = default;
            Seat_Key(size_t & hashtime,size_t & hashtrain) {
                time = hashtime;
                train = hashtrain;
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
           // Ticket::String<36> Station_name;
           int hashStation;
            int pos = 0;
            bool operator==(const Station_Key &r) const {
                return hashStation == r.hashStation && pos == r.pos;
            }

            bool operator<(const Station_Key &r) const {
                return hashStation < r.hashStation || (hashStation == r.hashStation && pos < r.pos);
            }
        };

        struct Station_Comp {
            bool operator()(const Station_Key &l, const Station_Key &r) {
                return l.hashStation < r.hashStation;
            }
        };

        struct Seat{
            int seatarr[100]={0};
        //    int n;
        };

        Ticket::BPlusTree<Seat_Key,Seat> _BPT_Seat;
        Ticket::BPlusTree<size_t, Train> _BPT_Train;
        Ticket::BPlusTree<Station_Key, Station> _BPT_Station;
        Ticket::BPlusTree<size_t, char> _BPT_Rl;
        Ticket::FileIONoCache Count;
        int cnt = 0;

        int
        _get_seat_range(const Ticket::String<24> &Train_ID, const Ticket::Date &Start_Date, int sta,
                        int end,int sup) {
            Ticket::Date Datekey(Start_Date.transToDate());
            Seat_Key seatKey;
            seatKey.train = hash(Train_ID);
            seatKey.time=Ticket::hash(Datekey.getDateStr());
            int seat = sup;
            int seatpos=_BPT_Seat.find(seatKey);
            Seat seatarr=_BPT_Seat.getVal(seatpos);
            for (int i = sta; i < end; i++)  {
                seat = std::min(seat, seatarr.seatarr[i]);
            }
            return seat;
        }

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
            size_t hashSN=hash(SN);
            if(_BPT_Train.find(hashSN)!=-1) return false;
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
            Ticket::Date Dtmp,Dtmp2;
            for (int i = 1; i < stanum; i++) {
                data.train_info[i].station = stations[i];
                data.train_info[i].prefix_price = data.train_info[i - 1].prefix_price + price[i];
                Statmp += traveltime[i];
                Endtmp += traveltime[i];
                data.train_info[i].stopover = stoppovertime[i];
                Statmp += stoppovertime[i];
                Endtmp += stoppovertime[i];
                Ticket::pair<int,int> hrmi=Statmp.getHHMM();
                Ticket::Date depatmp(0,0,hrmi.first,hrmi.second);
                data.train_info[i].depart_time=depatmp;
               // Ticket::pair<int,int> mmdd=Statmp.getMMDD();
               Dtmp=Statmp.transToDate();
               // Ticket::pair<int,int> mmdd2=Endtmp.getMMDD();
                Dtmp2=Endtmp.transToDate();
                data.train_info[i].Sta_Date = Dtmp;
                data.train_info[i].End_Date = Dtmp2;
                data.train_info[i].prefix_time =data.train_info[i - 1].prefix_time + traveltime[i] + stoppovertime[i];//
            }
            _BPT_Train.insert(hashSN,data);
            _BPT_Rl.insert(hashSN, 'N');
            return true;
            //终点站,price与出发时间停留时间无用
        }

        bool delete_train(const size_t & hashSN) {
            //这里好像有点鲁棒？判断是否发售？
            int pos = _BPT_Rl.find(hashSN);
            if (pos == -1) return false;
            else {
                //Seat_arr seatArr=_BPT_Seat.getVal(pos);
                char flag = _BPT_Rl.getVal(pos);
                if (flag == 'N') {
                    if (_BPT_Rl.erase(hashSN)==1&& _BPT_Train.erase(hashSN)==1) return true;
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

        bool release_train(const size_t & hashSN) {
            int pos = _BPT_Train.find(hashSN);
            //Otmp_pos为第二key为0的位置
            Station_Key data_key;
            if (pos == -1) return false;
            int rl_pos = _BPT_Rl.find(hashSN);
            if (rl_pos == -1) {
                std::cerr<<"heher5"<<'\n';
                Error("release_train_2");}//std::cerr<<"release_train_2",throw std::exception();
            char flag = _BPT_Rl.getVal(rl_pos);
            if (flag == 'Y') return false;
            _BPT_Rl.modifyVal(rl_pos, 'Y');
            Train data = _BPT_Train.getVal(pos);
            data_key.pos = ++cnt;
            int posflag;
            Station station;
            for (int i = 0; i < data.station_num; i++) {
                data_key.hashStation = hash(data.train_info[i].station);
               station.index=i;
               station.Pos=pos;
                posflag = _BPT_Station.insert(data_key, station);
                if (posflag == -1)  Error("release_train_4");
            }
            Seat Seatdata;
           // Seatdata.n=data.station_num;
            for(int i=0;i<data.station_num;i++) Seatdata.seatarr[i]=data.seat;
            size_t hashtrain,hashtime;
            Seat_Key seatKey;
            for (Ticket::Date tmp=data.start_day; data.start_day.cmpDate(tmp) <= 0 &&
                   tmp.cmpDate(data.end_day) <= 0; ++tmp) {
               hashtrain=hashSN;
               hashtime=Ticket::hash(tmp.getDateStr());
                seatKey.time=hashtime;
                seatKey.train=hashtrain;
                _BPT_Seat.insert(seatKey,Seatdata);
            }
            return true;
            //_BPT_Train.modifyVal(pos,data);
        }

        bool query_train(const size_t & hashSN, const Ticket::Date &tDate, std::ostream &os) {
            int pos = _BPT_Train.find(hashSN);
            if (pos == -1) return false;
            else {
                int Rlpos = _BPT_Rl.find(hashSN);
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
                    os << data.Train_SN << ' ' << data.type << '\n';
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
                    seatKey.train=hashSN;
                    seatKey.time=Ticket::hash(tDate.getDateStr());
                    int SeatPos=_BPT_Seat.find(seatKey);
                    Seat Seatdata=_BPT_Seat.getVal(SeatPos);
                    cal = tDate + Datetmp;
                    os << data.Train_SN << ' ' << data.type << '\n';
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
         //   Backend::map<int, int> match;
            const int Trainsz=sizeof(Train);
            int Uo[20001];
            bool Uob[20001]={0};
            std::vector<std::pair<int,int>> aimPosvec;
            Station_Key Keytmp;
            Keytmp.hashStation = hash(Sta);
            Keytmp.pos = 0;
            std::vector<int> stavec = _BPT_Station.route<Station_Comp>(Keytmp);
            if (stavec.empty()) { return false; }
            Keytmp.hashStation = hash(Det);
            std::vector<int> endvec = _BPT_Station.route<Station_Comp>(Keytmp);
            if (endvec.empty()) { return false; }
            int sz = stavec.size();
            Station stationtmp;
            int Uindex;
            for (int i = 0; i < sz; i++) {
                stationtmp = _BPT_Station.getVal(stavec[i]);
                Uindex=stationtmp.Pos/Trainsz;
                Uo[Uindex]=stationtmp.index;
                Uob[Uindex]=true;
              //  Ticket::pair<const int, int> tmp(stationtmp.Pos, stationtmp.index);
              //  if (match.insert(tmp)) {}
              //  else std::cerr << "query_ticket", throw std::exception();
            }
            sz = endvec.size();
            int StaIndex;

            for (int i = 0; i < sz; i++) {
                stationtmp = _BPT_Station.getVal(endvec[i]);
               // try {
               Uindex=stationtmp.Pos/Trainsz;
                    StaIndex=Uo[stationtmp.Pos/Trainsz];
                    if (stationtmp.index >StaIndex&&Uob[Uindex])
                        aimPosvec.emplace_back(std::pair<int,int>(stationtmp.Pos,StaIndex));
             //   } catch (NotFound) {}
            }
            //if(match.count(stationtmp.Train_SN)) aimIDvec.push_back(stationtmp.Train_SN);}
            if (aimPosvec.empty()) { return false; }
            //    std::cout<<"------------------"<<'\n';
            std::vector<Train> Trainvec;
            //int pos;
            sz = aimPosvec.size();
            //把Train取出来
            Ticket::Date DateKey(date);
            /*       DateKey.dd = date.dd;
                   DateKey.mm = date.mm;*/
            //std::cout<<"-------------"<<'\n';
            Train Candidate;
            for (int i = 0; i < sz; i++) {
                //时间判断
                Candidate=_BPT_Train.getVal(aimPosvec[i].first);
                if (Candidate.train_info[aimPosvec[i].second].Sta_Date.cmpDate(date)<=0&&date.cmpDate(Candidate.train_info[aimPosvec[i].second].End_Date)<=0) {
                    Trainvec.push_back(Candidate);
                } else {}
            }
            sz = Trainvec.size();
            Comp arr[sz];
            std::pair<int, int> pospair[sz];
          //  int tmp_seat_pos;
            if (type == 'T') {
                int station_num;
                for (int i = 0; i < sz; i++) {
                    arr[i].ID = Trainvec[i].Train_SN;
                    arr[i].pos = i;
                    station_num = Trainvec[i].station_num;
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
                int station_num;
                for (int i = 0; i < sz; i++) {
                    arr[i].ID = Trainvec[i].Train_SN;
                    arr[i].pos = i;
                    station_num = Trainvec[i].station_num;
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
            Ticket::Date Dtmp, DateKey2;
            os << sz << '\n';
            int seat,diff,Trainpos;
            for (int i = 0; i < sz; i++) {
                Trainpos = arr[i].pos;
                Dtmp = date + Trainvec[Trainpos].train_info[pospair[Trainpos].first].depart_time;
                DateKey2=Dtmp-Trainvec[Trainpos].train_info[pospair[Trainpos].first].prefix_time;
               // std::cout<<'-'<<DateKey<<'\n';
                seat = _get_seat_range(Trainvec[Trainpos].Train_SN, DateKey2, pospair[Trainpos].first,
                                           pospair[Trainpos].second,Trainvec[Trainpos].seat);
                os << arr[i].ID << ' ' << Sta << ' ' << Dtmp << ' ' << '-' << '>' << ' ';
                diff = Trainvec[Trainpos].train_info[pospair[Trainpos].second].prefix_time -
                           Trainvec[Trainpos].train_info[pospair[Trainpos].first].prefix_time -
                           Trainvec[Trainpos].train_info[pospair[Trainpos].second].stopover;
                Dtmp += diff;
                os << Det << ' ' << Dtmp << ' '<< Trainvec[Trainpos].train_info[pospair[Trainpos].second].prefix_price
                                                  - Trainvec[Trainpos].train_info[pospair[Trainpos].first].prefix_price << ' ' << seat
                   << '\n';
            }
            return true;
        }

        bool query_transfer(const Ticket::String<36> & Sta, const Ticket::String<36> &Det, const Ticket::Date &date,
                            char type, std::ostream &os) {
            //为什么感觉时间处理这么麻烦?
            //先记住我在每一个info里存的prefix_time
            //都是该站离站时间到第一个站的离站时间
            //由于中间存在停靠，所以需要做额外的加减处理
            Station_Key stationKey;
            stationKey.hashStation = hash(Sta);
            stationKey.pos = 0;
            std::vector<int> StaPosvec = _BPT_Station.route<Station_Comp>(stationKey);
            if(StaPosvec.empty()) return false;
            stationKey.hashStation = hash(Det);
            std::vector<int> EndPosvec = _BPT_Station.route<Station_Comp>(stationKey);
            if(EndPosvec.empty()) return false;
            //std::vector<Station> Stavec;
            std::vector<Station> Endvec;
            //一切的核心这个map Key值是尾站的所有train名， vector是尾站相同的中转站的在从中转站到尾站中车的位置
            //但由于中转站在从起点出发的火车中位置基本上与从中转站出发到尾站中的位置不一样
            //所以用pair来存，其中first是在从起点出发的火车中的位置，second是站在转乘的火车中的位置
           // std::priority_queue<Trans_Comp> PQ;
            //Ticket::String<25> Ret[2];
            //起始站
            int nums;
            Trans_Comp Ret;
            Ret.num = 888888888;
           // std::cerr<<"transfer_0"<<'\n';
            //std::cerr<<"-----------"<<'\n';
           // map<Ticket::String<36>, int> Endmatch;
           // const int Trainsize=sizeof(Train);
            Station Start,End;Train train1,train2;
            int CentPos1,CentPos2,StaPos,EndPos;
            Trans_Comp Challenger;
            My_Unordered_Map Match;
            for(int i=0;i<StaPosvec.size();i++){
                //map<Ticket::String<36>,int> Match;
                Match.clear();
                Start=_BPT_Station.getVal(StaPosvec[i]);
                train1=_BPT_Train.getVal(Start.Pos);
                for(int j=0;j<EndPosvec.size();j++) {
                    End = _BPT_Station.getVal(EndPosvec[j]);
                    if (End.Pos != Start.Pos) {
                        train2 = _BPT_Train.getVal(End.Pos);
                        for (int k = 0; k < train1.station_num; k++) {
                           // map<Ticket::String<36>, int>::value_type p(train1.train_info[k].station, k);
                           // Match.insert(p);
                           size_t hashnum=hash(train1.train_info[k].station);
                           Match.insert(hashnum,k);
                        }
                        for (int k = 0; k < train2.station_num; k++) {
                            size_t hashnum=hash(train2.train_info[k].station);
                            int data=Match.GetData(hashnum);
                            if (data!=-1) {
                                CentPos1 = data;
                                CentPos2 = k;
                                StaPos = Start.index;
                                EndPos = End.index;
                                if (StaPos < CentPos1 && CentPos2 < EndPos &&
                                    train1.train_info[StaPos].Sta_Date.cmpDate(date) <= 0 &&
                                    date.cmpDate(train1.train_info[StaPos].End_Date)<=0) {
                                    int diff = train1.train_info[CentPos1].prefix_time -
                                               train1.train_info[StaPos].prefix_time -
                                               train1.train_info[CentPos1].stopover;
                                    Challenger.depart1 = date + train1.train_info[StaPos].depart_time;
                                    Ticket::Date Time = Challenger.depart1 + diff;
                                    if (!(train2.train_info[CentPos2].End_Date +
                                          train2.train_info[CentPos2].depart_time < Time)) {
                                        Ticket::Date StartTime = train2.train_info[CentPos2].Sta_Date +
                                                                 train2.train_info[CentPos2].depart_time;
                                        Ticket::Date PossiTime =
                                                Time.transToDate() + train2.train_info[CentPos2].depart_time;
                                        Challenger.Start_Date1 = date + train1.train_info[StaPos].depart_time -
                                                                 train1.train_info[StaPos].prefix_time;
                                        Challenger.Cent = train2.train_info[CentPos2].station;
                                        Challenger.diff1 = diff;
                                        Challenger.Train_ID_Sta = train1.Train_SN;
                                        Challenger.Train_ID_End = train2.Train_SN;
                                        Challenger.price1 = train1.train_info[CentPos1].prefix_price -
                                                            train1.train_info[StaPos].prefix_price;
                                        Challenger.price2 = train2.train_info[EndPos].prefix_price -
                                                            train2.train_info[CentPos2].prefix_price;
                                        Challenger.sta1 = StaPos;
                                        Challenger.end1 = CentPos1;
                                        Challenger.sta2 = CentPos2;
                                        Challenger.end2 = EndPos;
                                        Challenger.diff2 = train2.train_info[EndPos].prefix_time -
                                                           train2.train_info[CentPos2].prefix_time -
                                                           train2.train_info[EndPos].stopover;
                                        if (Time < StartTime) {
                                            diff += StartTime.diffMinute(Time);
                                            Challenger.Start_Date2 =
                                                    StartTime - train2.train_info[CentPos2].prefix_time;
                                            Challenger.depart2 = StartTime;
                                        } else {
                                            if (!(PossiTime < Time)) {}
                                            else ++PossiTime;
                                            diff += PossiTime.diffMinute(Time);
                                            Challenger.depart2 = PossiTime;
                                            Challenger.Start_Date2 =
                                                    PossiTime - train2.train_info[CentPos2].prefix_time;
                                        }
                                        diff+=Challenger.diff2;
                                        if (type == 'P') Challenger.num = Challenger.price2 + Challenger.price1;
                                        else Challenger.num = diff;
                                        Ret = std::min(Ret, Challenger);
                                    }
                                }
                            }
                        }
                    }
                }
            }
       /*     for (int i = 0; i < EndPosvec.size(); i++) {
                Endvec.push_back(_BPT_Station.getVal(EndPosvec[i]));
                std::vector<std::pair<int, int>> tmpvec;
                map<int, std::vector<std::pair<int, int>>>::value_type valueType(Endvec[i].Pos, tmpvec);
                Endmatch.insert(valueType);
            }
            for (int i = 0; i < StaPosvec.size(); i++) {
                for(int j=0;j<EndPosvec.size();j++) Endmatch[Endvec[j].Pos].clear();
                //   std::cerr<<"transfer_0.5"<<'\n';
                Station StaStation = _BPT_Station.getVal(StaPosvec[i]);
                Train data = _BPT_Train.getVal(StaStation.Pos);
                int statnum = data.station_num;
                //遍历起始站的每一辆车车的每一个节点
                for (int j = StaStation.index; j < statnum; j++) {
                    //遍历该节点的每一辆车，并将其插入map中
                    Station_Key CentKey;
                    CentKey.pos = 0;
                    CentKey.hashStation = hash(data.train_info[j].station);
                    std::vector<int> CentPosvec = _BPT_Station.route<Station_Comp>(CentKey);
                    std::vector<Station> Centvec;
                    int Centpos1 = j;
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
                                 //   std::cerr<<CentPos1<<' '<<Challenger.Train_ID_Sta<<"\n";
                                 //   std::cerr<<CentPos2<<' '<<Challenger.Train_ID_End<<'\n';
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
                                        Challenger.Start_Date2=Start-Candidate.train_info[CentPos2].prefix_time;
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
            }*/
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

        bool GetTrain(Train & data,const Ticket::String<24> & ID){
            int pos=_BPT_Train.find(hash(ID));
            if(pos==-1) return false;
            else {data=_BPT_Train.getVal(pos);return true;}
        }

        void BuyTicekt(Train & data,Ticket::Date & Start_Date,Ticket::Date & End_Date,const Ticket::String<36> &Sta,const Ticket::String<36>End,
                    int & sta,int & end,int &seat,int &price,int nums){
            if(nums>data.seat) {seat=-2;return;}
            int sz=data.station_num;
            bool is_exist=false;
            for (int i = 0; i < sz; i++) {
                if (data.train_info[i].station == Sta) {
                    is_exist=true;
                    sta = i;
                    break;
                }
            }
            if(!is_exist) {seat=-2;return;}
            if (data.train_info[sta].Sta_Date.cmpDate(Start_Date) > 0 ||
                Start_Date.cmpDate(data.train_info[sta].End_Date) > 0) {
                seat = -2;
                return;
            }
            Start_Date=Start_Date+data.train_info[sta].depart_time;
            Ticket::Date Dtmp(Start_Date);
            Dtmp-=data.train_info[sta].prefix_time;
            int seatpos;
            seat = data.seat;
            int seattmp;
            is_exist=false;
            size_t hashtrain=hash(data.Train_SN);
            size_t hashtime=Ticket::hash(Dtmp.getDateStr());
            Seat_Key seatKey1(hashtime,hashtrain);
            Seat seat1;
            int pos=_BPT_Seat.find(seatKey1);
            seat1=_BPT_Seat.getVal(pos);
            for (int i = sta; i < sz; i++) {
                if (data.train_info[i].station == End) {
                    end = i;
                    is_exist=true;
                    break;
                }
                seat = std::min(seat, seat1.seatarr[i]);
            }
        //    std::cout<<sta<<' '<<end<<' '<<nums<<' '<<seat<<'\n';
            if(!is_exist) {seat=-2;return;}
            if(sta>=end) {seat=-2;return;}
            if(seat<nums) seat=-1;
            else {
               // type = 'S';
                for(int i=sta;i<end;i++)  seat1.seatarr[i]-=nums;
                _BPT_Seat.modifyVal(pos,seat1);
            }
            End_Date = Start_Date+data.train_info[end].prefix_time-data.train_info[sta].prefix_time-data.train_info[end].stopover;
            price = data.train_info[end].prefix_price - data.train_info[sta].prefix_price;
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

        void RenewN(Train & Aim,std::vector<order> & TrainOrdervec,std::vector<OrderKey> & Renewvec,order & Success ){
           // int aimPos=_BPT_Train.find(Success.get_str(order_parameter::Train_ID));
           // Train Aim=_BPT_Train.getVal(aimPos);
            Seat_Key seatKey;
            seatKey.train=hash(Success.get_str(order_parameter::Train_ID));
            int sta=Success.get_num(order_parameter::Start_Position);
            int end=Success.get_num(order_parameter::End_Position);
            int devi=Success.get_num(order_parameter::Num);
            Ticket::Date StartTime=Success.get_Date(order_parameter::Start_Date)-Aim.train_info[sta].prefix_time;
            seatKey.time=Ticket::hash(StartTime.getDateStr());
            int pos=_BPT_Seat.find(seatKey);
            Seat seat=_BPT_Seat.getVal(pos);
            for(int i=sta;i<end;i++) seat.seatarr[i]+=devi;
            int statmp;
            int endtmp;
            OrderKey orderKey;
            int num;
            Ticket::Date Timetmp;
            for(int i=0;i<TrainOrdervec.size();i++){
                statmp=TrainOrdervec[i].get_num(order_parameter::Start_Position);
                Timetmp=TrainOrdervec[i].get_Date(order_parameter::Start_Date);
                Timetmp-=Aim.train_info[statmp].prefix_time;
             //  std::cout<<Timetmp<<' '<<StartTime<<'\n';
             //   TrainOrdervec[i].print(std::cout);
                if(Timetmp.timeCnt!=StartTime.timeCnt) continue;
                endtmp=TrainOrdervec[i].get_num(order_parameter::End_Position);
                int seattmp=Aim.seat;
                for(int j=statmp;j<endtmp;j++) seattmp=std::min(seattmp,seat.seatarr[j]);
                num=TrainOrdervec[i].get_num(order_parameter::Num);
                if(seattmp<num) continue;
                for(int j=statmp;j<endtmp;j++) seat.seatarr[j]-=num;
                orderKey.SN=TrainOrdervec[i].get_num(order_parameter::SN);
                orderKey.hashstr=hash(TrainOrdervec[i].get_str(order_parameter::Username));
                Renewvec.push_back(orderKey);
            }
            _BPT_Seat.modifyVal(pos,seat);
        }
    };
}
#endif