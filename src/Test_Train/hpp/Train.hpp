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
namespace Backend{
#define Error(x) throw Ticket::WrongOperation(x)
    class Train_manager{
    private:
        struct Train{
            //private:
            Ticket::String<20> Train_SN;
            Ticket::Date start_day;
            Ticket::Date end_day;
            Ticket::Time start_time;
            Ticket::String<40> start_station;
            int station_num=0;
            // char rl='N';
            char type;
            int seat;
            //int station;
            struct info{
                Ticket::String<40> station;
                //int prefix_time;
                Ticket::Date Sta_Date;//仅代表日期的起终
                Ticket::Date End_Date;//
                Ticket::Time arri_time;
                int prefix_price=0;
                int prefix_time=0;
                //prefix_time是离开时间
                int stopover=0;
                //int seat;
            };
            info train_info[100];
            //public:
            //Train()=default;
            //~Train()=default;
        };
        struct Station{
            Ticket::String<20> Train_SN;
            int Train_pos=0;
            //Ticket::Time arri_time;
            Station()=default;
            Station(const Ticket::String<20> & SN,const int & pos){
                Train_SN=SN;
                Train_pos=pos;
                //arri_time=o.train_info[pos].arri_time;
                //start=o.train_info[pos].arri_Date;
                //end=o.train_info[pos].depart_Date;
            }
        };
        struct Seat_Key{
            Ticket::String<10> time;
            Ticket::String<20> train;
            Ticket::String<40> station;
            Seat_Key()=default;
            Seat_Key(const Ticket::String<20> & Train,const Ticket::String<40> Station,const Ticket::String<10> & Time) {
                time = Time;
                train =Train;
                station=Station;
            }
            bool operator==(const Seat_Key & r)const{
                return time==r.time&&train==r.train&&station==r.station;
            }
            bool operator<(const Seat_Key & r)const{
                return time<r.time||(time==r.time&&train<r.train)||(time==r.time&&train==r.train&&station<r.station);
            }

        };
        /*struct Train_Key{
            Ticket::String Train_SN;
            int pos;
        };*/
        struct Station_Key{
            Ticket::String<40> Station_name;
            int pos=0;
            bool operator==(const Station_Key & r)const{
                return Station_name==r.Station_name&&pos==r.pos;
            }
            bool operator<(const Station_Key & r)const{
                return Station_name<r.Station_name||(Station_name==r.Station_name&&pos<r.pos);
            }
        };
        struct Station_Comp{
            bool operator()(const Station_Key & l,const Station_Key & r){
                return l.Station_name<r.Station_name;
            }
        };
        Ticket::BPlusTree<Seat_Key,int> _BPT_Seat;
        Ticket::BPlusTree<Ticket::String<20>,Train> _BPT_Train;
        Ticket::BPlusTree<Station_Key,Station> _BPT_Station;
        Ticket::BPlusTree<Ticket::String<20>,char> _BPT_Rl;
        inline int _get_seat(const Ticket::String<20> & train,const Ticket::String<40> & station,const Ticket::String<10> & date){
            Seat_Key seatKey(train,station,date);
            int pos=_BPT_Seat.find(seatKey);
            if(pos==-1) return -1;
            else return _BPT_Seat.getVal(pos);
        }
        inline int _get_seat_range(const Train& data,const Ticket::String<40> & Sta,const Ticket::Date & Start_Date,int sta,int end){
            Seat_Key seatKey;
            seatKey.station=Sta;
            seatKey.train=data.Train_SN;
            seatKey.time=Start_Date.to_string();

            Ticket::Date Datetmp=Start_Date;
            Ticket::Date Datekey;
            int seatpos;
            int seat=data.seat;
            int seattmp;
            for(int i=sta;i<end;i++){
                seatpos=_BPT_Seat.find(seatKey);
                seattmp=_BPT_Seat.getVal(seatpos);
                seat=std::min(seat,seattmp);
                Datetmp+=(data.train_info[i+1].prefix_time-data.train_info[i].prefix_time-data.train_info[i+1].stopover);
                Datekey.mm=Datetmp.mm;
                Datekey.dd=Datetmp.dd;
                seatKey.time=Datekey.to_string();
            }
            return seat;
        }

        struct Comp{
            int data=0;
            int pos=0;
            Ticket::String<20> ID;
            bool operator<(const Comp & o)const{
                return data<o.data||(data==o.data&&ID<o.ID);
            }
        };

        struct Trans_Comp{
            Ticket::String<20> Train_ID_Sta;
            Ticket::String<20> Train_ID_End;
            int num;
            bool operator<(const Trans_Comp & o) const{
                return num<o.num||(num==o.num&&Train_ID_Sta<o.Train_ID_Sta)||(num==o.num&&Train_ID_Sta==o.Train_ID_Sta&&Train_ID_End<o.Train_ID_End);
            }
        };

        struct Trans_Type{
            Ticket::String<20> Train_ID;
            int num;
            Ticket::Date Time;
        };
    public:
        //Train_manager()=default;
        explicit Train_manager(const std::string & name,const std::string & station_name,const std::string & Rlname,const std::string & seat_name):_BPT_Train(name),_BPT_Station(station_name),_BPT_Rl(Rlname),_BPT_Seat(seat_name){};
        bool add_train(const Ticket::String<20> & SN,int & stanum,int & seatnum,Ticket::String<40> * stations,const int * price,const Ticket::Time & sta_time,const int * traveltime,const int * stoppovertime,const Ticket::Date * saleDate,char type){//saleDate[0]为起始,1为终结
            Train data;
            //int sumtime=0;
            //Train_Key data_key;
            // data_key.Train_SN
            data.Train_SN=SN;
            data.station_num=stanum;
            data.type=type;
            //设定起始时间相关
            data.start_time=sta_time;
            Ticket::Date Datetmp(data.start_time);
            data.start_day=saleDate[0]+Datetmp;
            data.end_day=saleDate[1]+Datetmp;
            data.start_station=stations[0];
            //设定首站，到达时间，停留时间,累计时间无用，
            //data.train_info[0].seat=seat;
            data.train_info[0].station=stations[0];
            data.train_info[0].prefix_price=price[0];
            //传入的stopover数组首尾为0;
            //传入的travel time 数组首为0;
            //传入的price 数组首位0;
            data.train_info[0].stopover=0;
            //、、要改！
            Ticket::Date Statmp=data.train_info[0].Sta_Date=data.start_day;
            Ticket::Date Endtmp=data.train_info[0].End_Date=data.end_day;
            data.train_info[0].arri_time=sta_time;
            data.train_info[0].prefix_time=0;
            //seat 最后一位没有用
            data.seat=seatnum;
            //设定后面
            //Ticket::Date Statmp,Endtmp;
            // seatArr.rl='N';
            //seatArr.seat_info[0]=seatnum;
            for(int i=1;i<stanum;i++){
                //data.seat[i]=seatnum;
                data.train_info[i].station=stations[i];
                //data.train_info[i].seat=seatnum;
                data.train_info[i].prefix_price=data.train_info[i-1].prefix_price+price[i];
                Statmp+=traveltime[i];
                Endtmp+=traveltime[i];
                data.train_info[i].arri_time.hr=Statmp.hr;
                data.train_info[i].arri_time.mi=Statmp.mi;
                data.train_info[i].stopover=stoppovertime[i];
                Statmp+=stoppovertime[i];
                Endtmp+=stoppovertime[i];
                data.train_info[i].Sta_Date=Statmp;
                data.train_info[i].End_Date=Endtmp;
                data.train_info[i].prefix_time=data.train_info[i-1].prefix_time+traveltime[i]+stoppovertime[i];//
            }
            if(_BPT_Train.insert(SN,data)==-1) return false;
            else{_BPT_Rl.insert(SN,'N');return true;}
            //终点站,price与出发时间停留时间无用
        }
        bool delete_train(const Ticket::String<20> & SN){
            //这里好像有点鲁棒？判断是否发售？
            int pos=_BPT_Rl.find(SN);
            if(pos==-1) return false;
            else {
                //Seat_arr seatArr=_BPT_Seat.getVal(pos);
                char flag=_BPT_Rl.getVal(pos);
                if(flag=='N'){
                    if(_BPT_Rl.erase(SN)&&_BPT_Train.erase(SN)) return true;
                    else Error("delete_train_2");
                        //std::cerr<<"delete_train_3",throw std::exception();
                }
                else if(flag=='Y') return false;
                else  Error("delete_train_3");
                    //std::cerr<<"delete_train_3",throw std::exception();
            }
        }
        bool release_train(const Ticket::String<20> & SN){
            int pos=_BPT_Train.find(SN);
            //Otmp_pos为第二key为0的位置
            int Otmp_pos;
            Station_Key Otmp;
            Station_Key data_key;
            if(pos==-1) return false;
            int rl_pos=_BPT_Rl.find(SN);
            if(rl_pos==-1) Error("release_train_2");//std::cerr<<"release_train_2",throw std::exception();
            char flag=_BPT_Rl.getVal(rl_pos);
            if(flag=='Y') return false;
            _BPT_Rl.modifyVal(rl_pos,'Y');
            Train data=_BPT_Train.getVal(pos);
            Otmp.pos=0;
            for(int i=0;i<data.station_num;i++){
                Otmp.Station_name=data_key.Station_name=data.train_info[i].station;
                Otmp_pos=_BPT_Station.find(Otmp);
                //要插入 station
                Station station(SN,i);
                //没有station 插入新的
                if(Otmp_pos==-1){
                    Station head(SN,1);
                    int flag=_BPT_Station.insert(Otmp,head);
                    if(flag==-1) Error("release_train_2");
                        //std::cerr<<"release_trian_2",throw wrong_operation();
                    data_key.pos=1;
                    flag=_BPT_Station.insert(data_key,station);
                    if(flag==-1) Error("release_train_3");
                        //std::cerr<<"release_trian_2",throw wrong_operation();
                }
                else{
                    Station Ontmp=_BPT_Station.getVal(Otmp_pos);
                    data_key.pos=++Ontmp.Train_pos;
                    int flag=_BPT_Station.insert(data_key,station);
                    if(flag==-1) Error("release_train_4");
                    //std::cerr<<"release_trian_2",throw wrong_operation();
                    //data_key.pos=++ntmp.;
                    //_BPT_Station.insert(Otmp,station);
                    _BPT_Station.modifyVal(Otmp_pos,Ontmp);
                }
                Ticket::Date tmp;
                tmp.dd=data.train_info[i].Sta_Date.dd;
                tmp.mm=data.train_info[i].Sta_Date.mm;
                for(;data.train_info[i].Sta_Date.cmpDate(tmp)<=0&&tmp.cmpDate(data.train_info[i].End_Date)<=0;++tmp) {
                    Seat_Key seatKey(SN, data.train_info[i].station, tmp.to_string());
                    if (_BPT_Seat.insert(seatKey, data.seat) == -1)Error("release_train_5");
                    //std::cerr<<"release_trian_2",throw wrong_operation();
                }
            }
            //data.seat=-1;
            return true;
            //_BPT_Train.modifyVal(pos,data);
        }
        bool query_train(const Ticket::String<20> & SN,const Ticket::Date & tDate,std::ostream & os){
            int pos=_BPT_Train.find(SN);
            //int Rlpos=_BPT_Rl.find(SN);
            //int spos=_BPT_Seat.find(SN);
            if(pos==-1) return false;
            else{
                int Rlpos=_BPT_Rl.find(SN);
                if(Rlpos==-1) Error("query_train");
                //std::cerr<<"query_train",throw wrong_operation();
                Train data=_BPT_Train.getVal(pos);
                if(data.start_day.cmpDate(tDate)==1||tDate.cmpDate(data.end_day)==1) return false;
                int sz=data.station_num;
                char flag=_BPT_Rl.getVal(Rlpos);
                if(flag=='N'){
                    Ticket::Date cal;
                    Ticket::Date Datetmp(data.start_time);
                    cal=tDate+Datetmp;
                    os<<SN<<' '<<data.type<<'\n';
                    os<<data.train_info[0].station<<' '<<"xx-xx xx:xx ->"<<' '<<cal<<' '<<data.train_info[0].prefix_price<<' '<<data.seat<<'\n';
                    for(int i=1;i<sz-1;i++){
                        cal+=data.train_info[i].prefix_time-data.train_info[i-1].prefix_time-data.train_info[i].stopover;
                        os<<data.train_info[i].station<<' '<<cal<<' '<<'-'<<'>'<<' ';
                        cal+=data.train_info[i].stopover;
                        os<<cal<<' '<<data.train_info[i].prefix_price<<' '<<data.seat<<'\n';
                    }
                    cal+=data.train_info[sz-1].prefix_time-data.train_info[sz-2].prefix_time-data.train_info[sz-1].stopover;
                    os<<data.train_info[sz-1].station<<' '<<cal<<' '<<"-> xx-xx xx:xx"<<' '<<data.train_info[sz-1].prefix_price<<' '<<'x'<<'\n';
                }
                else if(flag=='Y'){
                    Ticket::Date cal;
                    Ticket::Date Datetmp(data.start_time);
                    Ticket::Date Datekey(tDate.mm,tDate.dd,0,0);
                    cal=tDate+Datetmp;
                    os<<SN<<' '<<data.type<<'\n';
                    os<<data.train_info[0].station<<' '<<"xx-xx xx:xx ->"<<' '<<cal<<' '<<data.train_info[0].prefix_price<<' '<<_get_seat(SN,data.train_info[0].station,Datekey.to_string())<<'\n';
                    for(int i=1;i<sz-1;i++){
                        cal+=data.train_info[i].prefix_time-data.train_info[i-1].prefix_time-data.train_info[i].stopover;
                        os<<data.train_info[i].station<<' '<<cal<<' '<<'-'<<'>'<<' ';
                        cal+=data.train_info[i].stopover;
                        os<<cal<<' '<<data.train_info[i].prefix_price<<' '<<_get_seat(SN,data.train_info[0].station,Datekey.to_string())<<'\n';
                    }
                    cal+=data.train_info[sz-1].prefix_time-data.train_info[sz-2].prefix_time-data.train_info[sz-1].stopover;
                    os<<data.train_info[sz-1].station<<' '<<cal<<' '<<"-> xx-xx xx:xx"<<' '<<data.train_info[sz-1].prefix_price<<' '<<'x'<<'\n';
                }
                else Error("query_ticket_2");
                return true;
                }
                //出错的话先检查spos和pos

        }
        bool query_ticket(const Ticket::String<40> & Sta,const Ticket::String<40> &Det,const Ticket::Date & date,char type,std::ostream & os){//type 'T'-time 'P'-price
            Backend::map<Ticket::String<20>,int> match;
            std::vector<Ticket::String<20>> aimIDvec;
            Station_Key Keytmp;
            Keytmp.Station_name=Sta;
            Keytmp.pos=0;
            //先得到起始位置
            std::vector<int>  stavec=_BPT_Station.route<Station_Comp>(Keytmp);
            //首站不存在
            if(stavec.empty()){return false;}
            Keytmp.Station_name=Det;
            //终止位置
            std::vector<int>  endvec=_BPT_Station.route<Station_Comp>(Keytmp);
            //尾站不存在
            if(endvec.empty()){return false;}
            int sz=stavec.size();
            Station stationtmp;
            //起始站匹配
            for(int i=1;i<sz;i++){
                stationtmp=_BPT_Station.getVal(stavec[i]);
                Ticket::pair<const Ticket::String<20>,int> tmp(stationtmp.Train_SN,stationtmp.Train_pos);
                if(match.insert(tmp)){}
                else std::cerr<<"query_ticket",throw std::exception();
            }
            sz=endvec.size();
            //将结尾站符合要求的车塞进去
            for(int i=1;i<sz;i++) {
                stationtmp = _BPT_Station.getVal(endvec[i]);
                //小心逆向查车，小心车没释放
                try {
                    //逆向查车不算
                    if (stationtmp.Train_pos > match[stationtmp.Train_SN])
                        aimIDvec.push_back(stationtmp.Train_SN);
                } catch (NotFound) {}
            }
                //if(match.count(stationtmp.Train_SN)) aimIDvec.push_back(stationtmp.Train_SN);}
            if(match.empty()){return false;}
            std::vector<Train> Trainvec;
            //int pos;

            sz=aimIDvec.size();
            //把Train取出来
            Ticket::Date DateKey;
            DateKey.dd=date.dd;
            DateKey.mm=date.mm;
            for(int i=0;i<sz;i++){
                //时间判断
                Seat_Key seatKey(aimIDvec[i],Sta,DateKey.to_string());
                if(_BPT_Seat.find(seatKey)!=-1) {
                    int trapos = _BPT_Train.find(aimIDvec[i]);
                    if (trapos == -1) std::cerr << "query_ticket_2", throw std::exception();
                    Trainvec.push_back(_BPT_Train.getVal(trapos));
                }
                else{}
            }
            sz=Trainvec.size();
            Comp arr[sz];
            std::pair<int,int> pospair[sz];
            //Seat_arr seatArrs[sz];
            int tmp_seat_pos;
            if(type=='T'){
                for(int i=0;i<sz;i++){
                    arr[i].ID=aimIDvec[i];
                    arr[i].pos=i;
                    int station_num=Trainvec[i].station_num;
                    for(int j=0;j<station_num;j++) if(Sta==Trainvec[i].train_info[j].station){pospair[i].first=j;break;}
                    for(int j=station_num-1;j>=0;j--) if(Det==Trainvec[i].train_info[j].station){pospair[i].second=j;break;}
                    arr[i].data=Trainvec[i].train_info[pospair[i].second].prefix_time-Trainvec[i].train_info[pospair[i].first].prefix_time-Trainvec[i].train_info[ pospair[i].second].stopover;
                }
            }
            else if(type=='P'){
                for(int i=0;i<sz;i++){
                    arr[i].ID=aimIDvec[i];
                    arr[i].pos=i;
                    int station_num=Trainvec[i].station_num;
                    for(int j=0;j<station_num;j++) if(Sta==Trainvec[i].train_info[j].station){pospair[i].first=j;break;}
                    for(int j=station_num-1;j>=0;j--) if(Det==Trainvec[i].train_info[j].station){pospair[i].second=j;break;}
                    arr[i].data=Trainvec[i].train_info[pospair[i].second].prefix_price-Trainvec[i].train_info[pospair[i].first].prefix_price;
                }
            }
            else std::cerr<<"query_ticket",throw std::exception();
            std::sort(arr,arr+sz);
            Ticket::Date Dtmp;
            Ticket::Date DateKet2;
            for(int i=0;i<sz;i++)
            {
                int Trainpos=arr[i].pos;
                Dtmp=date+Trainvec[Trainpos].train_info[pospair[i].first].arri_time;
                Dtmp+=Trainvec[Trainpos].train_info[pospair[i].first].stopover;
                int seat=_get_seat_range(Trainvec[Trainpos],Sta,Dtmp,pospair[i].first,pospair[i].second);
                std::cout<<arr[Trainpos].ID<<' '<<Sta<<' '<<Dtmp<<' ';
                Dtmp=Trainvec[Trainpos].train_info[pospair[i].first].arri_time;
                Dtmp+=(Trainvec[Trainpos].train_info[pospair[i].second].prefix_time-
                       Trainvec[Trainpos].train_info[pospair[i].first].prefix_time-
                       Trainvec[Trainpos].train_info[pospair[i].second].stopover);
                DateKet2.mm=Dtmp.mm;
                DateKet2.dd=Dtmp.dd;
                std::cout<<Det<<' '<<Dtmp<<' '<<Trainvec[Trainpos].train_info[pospair[i].second].prefix_price
                -Trainvec[Trainpos].train_info[pospair[i].first].prefix_price<<seat;
            }
            return true;
        }

        bool query_transfer(const Ticket::String<40> & Sta,const Ticket::String<40> &Det,const Ticket::Date & date,char type,std::ostream & os){
            //为什么感觉时间处理这么麻烦?
            //先记住我在每一个info里存的prefix_time
            //都是该站离站时间到第一个站的离站时间
            //由于中间存在停靠，所以需要做额外的加减处理
            Station_Key stationKey;
            stationKey.Station_name=Sta;
            stationKey.pos=0;
            int sz;
            std::vector<int> Stavec=_BPT_Station.route(stationKey);
            stationKey.Station_name=Det;
            std::vector<int> Endvec=_BPT_Station.route(stationKey);
            std::vector<Station> StaStatvec;
            std::vector<Station> EndStatvec;
            map<Ticket::String<20>,int> Stamatch;
            map<Ticket::String<20>,int> Endmatch;
            sz=Endvec.size();
            for(int i=0;i<sz;i++) {
                EndStatvec.push_back(_BPT_Station.getVal(Endvec[i]));
                map<Ticket::String<20>,int>::value_type valueType(EndStatvec[i].Train_SN,i);
                Endmatch.insert(valueType);
            }
            sz=Stavec.size();
            for(int i=0;i<sz;i++) {
                StaStatvec.push_back(_BPT_Station.getVal(Stavec[i]));
                map<Ticket::String<20>,int>::value_type valueType(StaStatvec[i].Train_SN,i);
                Stamatch.insert(valueType);
            }
            std::priority_queue<Trans_Comp> PQ;
            //Ticket::String<20> Ret[2];
            //起始站
            for(int i=0;i<sz;i++){
                int train_pos=_BPT_Train.find(StaStatvec[i].Train_SN);
                Train data=_BPT_Train.getVal(train_pos);
                int statnum=data.station_num;
                //遍历起始站的每一辆车车的每一个节点
                for(int j=StaStatvec[i].Train_pos;j<statnum;j++) {
                    //将j遍历到的节点设为中央节点
                    stationKey.Station_name = data.train_info[j].station;
                    std::vector<int> Centvec = _BPT_Station.route(stationKey);
                    std::vector<Station> CentStatvec;
                    std::vector<Trans_Type> StaTransVec;
                    std::vector<Trans_Type> EndTransVec;
                    //std::vector<Train> CandiTrainvec;
                    //遍历中央节点的每一辆车
                    for (int k = 0; k < Centvec.size(); k++) CentStatvec.push_back(_BPT_Station.getVal(Centvec[k]));
                    if(type=='P') {
                        Train Candidate;
                        for (int k = 0; k < Centvec.size(); k++) {
                            //把中间站的所有train取出来
                                int trainpos = _BPT_Train.find(CentStatvec[k].Train_SN);
                                Candidate = _BPT_Train.getVal(trainpos);
                                int Stapos = StaStatvec[i].Train_pos;
                                //中间节点一，表示从Sta来到的车
                                int Centpos1 = CentStatvec[k].Train_pos;
                                Trans_Comp cmptmp;
                                Ticket::Date time;
                                //查看询问的date是否合法并且Centpos1大于Stapos说明列车行驶方向正确
                                if (Candidate.train_info[Stapos].Sta_Date.cmpDate(date) <= 0 &&
                                    date.cmpDate(Candidate.train_info[Stapos].End_Date) <= 0 &&
                                    Stapos < Centpos1) {//表示正向
                                    int diff = Candidate.train_info[Centpos1].prefix_price -
                                               Candidate.train_info[Stapos].prefix_price;
                                    //time表示到达中转站时间
                                    time = date + Candidate.train_info[Stapos].arri_time +
                                           Candidate.train_info[Stapos].stopover + diff;
                                    cmptmp.Train_ID_Sta = CentStatvec[k].Train_SN;
                                    Train Candidate2;
                                    //确定好起点到中转站后
                                    //枚举中转站到终点
                                    for (int l = 0; l < Centvec.size(); l++) {
                                        if (Endmatch.count(CentStatvec[l].Train_SN) &&
                                            CentStatvec[l].Train_SN != CentStatvec[k].Train_SN) {
                                            int trainpos2 = _BPT_Train.find(CentStatvec[l].Train_SN);
                                            Candidate2 = _BPT_Train.getVal(trainpos2);
                                            cmptmp.Train_ID_End = CentStatvec[l].Train_SN;
                                            int Endpos = EndStatvec[Endmatch[CentStatvec[l].Train_SN]].Train_pos;
                                            int Centpos2 = CentStatvec[i].Train_pos;
                                            //保证时间与方向正确
                                            //小于号右边为最晚该车离站时间，即保证来到这里后只要等待一定会遇到车
                                            //这里有问题，希望它没有小于等于的情况
                                            if (time < Candidate2.train_info[Centpos2].End_Date +
                                                       Candidate2.train_info[Centpos2].arri_time+Candidate2.train_info[Centpos2].stopover && Centpos2 < Endpos) {
                                                diff += Candidate2.train_info[Endpos].prefix_price -
                                                        Candidate.train_info[Centpos2].prefix_price;
                                                //计算
                                                cmptmp.num = diff;
                                                //丢入优先队列,我觉得这里可以优化，即出了这层循环再丢进单调队列
                                                PQ.push(cmptmp);
                                            }
                                        }
                                    }
                                }
                        }
                    }
                    else if(type=='T'){
                        Train Candidate;
                        for (int k = 0; k < Centvec.size(); k++) {
                            //Trans_Type type;
                            if (Stamatch.count(CentStatvec[k].Train_SN)) {
                                //把train取出来
                                int trainpos = _BPT_Train.find(CentStatvec[k].Train_SN);
                                Candidate = _BPT_Train.getVal(trainpos);
                                int Stapos = StaStatvec[i].Train_pos;
                                int Centpos1 = CentStatvec[k].Train_pos;
                                Trans_Comp cmptmp;
                                Ticket::Date time;
                                if (Candidate.train_info[Stapos].Sta_Date.cmpDate(date) <= 0 &&
                                    date.cmpDate(Candidate.train_info[Centpos1].End_Date) <= 0 &&
                                    Stapos < Centpos1){//表示正向
                                    int diff = Candidate.train_info[Centpos1].prefix_time -
                                               Candidate.train_info[Stapos].prefix_time-Candidate.train_info[Centpos1].stopover;
                                    //time表示到达中转站时间
                                    time = date+Candidate.train_info[Stapos].arri_time+Candidate.train_info[Stapos].stopover+diff;
                                    cmptmp.Train_ID_Sta=CentStatvec[k].Train_SN;
                                    Train Candidate2;
                                    //确定好起点到中转站后
                                    //枚举中转站到终点
                                    for(int l=0;l<Centvec.size();l++) {
                                        if(Endmatch.count(CentStatvec[l].Train_SN)&&CentStatvec[l].Train_SN!=CentStatvec[k].Train_SN) {
                                            int trainpos2=_BPT_Train.find(CentStatvec[l].Train_SN);
                                            Candidate2=_BPT_Train.getVal(trainpos2);
                                            cmptmp.Train_ID_End=CentStatvec[l].Train_SN;
                                            int Endpos=EndStatvec[Endmatch[CentStatvec[l].Train_SN]].Train_pos;
                                            int Centpos2=CentStatvec[i].Train_pos;
                                            //保证时间与方向正确
                                            //小于号右边为最晚该车离站时间，即保证来到这里后只要等待一定会遇到车
                                            //这里有问题，希望它没有小于等于的情况
                                            if(time<Candidate2.train_info[Centpos2].End_Date+Candidate2.train_info[Centpos2].arri_time+Candidate2.train_info[Centpos2].stopover&&Centpos2<Endpos){
                                                Ticket::Time Timetmp=Candidate2.train_info[Centpos2].arri_time;
                                                Ticket::Date Datetmp(Timetmp);
                                                Ticket::Date Start(Timetmp);
                                                Datetmp.dd=time.dd;
                                                Datetmp.mm=time.mm;
                                                //Start表示该车最早离站时间
                                                Start.dd=Candidate2.train_info[Centpos2].Sta_Date.dd;
                                                Start.mm=Candidate2.train_info[Centpos2].Sta_Date.mm;
                                                //Datetmp表示该车到站同天的离站时间
                                                Datetmp+=Candidate2.train_info[Centpos2].stopover;
                                                Start+=Candidate2.train_info[Centpos2].stopover;
                                                //同天离站时间小于起始离站，说明该天在发车起始以外，需要等到第一趟车
                                                if(Datetmp<Start){
                                                    diff+=Start.diffMinute(time);
                                                }
                                                else{
                                                    if(time<Datetmp) diff+=Datetmp.diffMinute(time);
                                                    //同天离站时间小于到站时间，那么要多等一天
                                                    else{
                                                        ++Datetmp;
                                                        diff+=Datetmp.diffMinute(time);
                                                    }
                                                }
                                                diff+=Candidate2.train_info[Endpos].prefix_time -
                                                      Candidate.train_info[Centpos2].prefix_time-Candidate.train_info[Endpos].stopover;
                                                cmptmp.num=diff;
                                                PQ.push(cmptmp);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        //Trans_Comp cmptmp;
                        //PQ.push(cmptmp);
                    }
                }
            }
            os<<PQ.top().Train_ID_Sta<<'\n'<<PQ.top().Train_ID_End<<'\n';
            return true;
        }
      void GetSeat(const Ticket::String<20> & Train_ID,Ticket::Date & Start_Date,Ticket::Date & End_Date,Ticket::String<40> & Sta,Ticket::String<40> & End,int & sta,int & end,int & seat,int & price,int nums){
          int pos=_BPT_Train.find(Train_ID);
          if(pos==-1) {seat=-1;return;}
          Train data =_BPT_Train.getVal(pos);
          int sz=data.station_num;
          for(int i=0;i<sz;i++) if(data.train_info[i].station==Sta){ sta=i;break;}
          if(data.train_info[sta].Sta_Date.cmpDate(Start_Date)>0||Start_Date.cmpDate(data.train_info[sta].End_Date)>0) {seat=-1;return;}
          Seat_Key seatKey;
          seatKey.station=Sta;
          seatKey.train=Train_ID;
          seatKey.time=Start_Date.to_string();

          Start_Date.mi=data.train_info[sta].arri_time.mi;
          Start_Date.hr=data.train_info[sta].arri_time.hr;
          Start_Date+=data.train_info[sta].stopover;
          Ticket::Date Datetmp=Start_Date;
          Ticket::Date Datekey;
          int seatpos;
          seat=data.seat;
          int seattmp;
          for(int i=sta;i<sz;i++){
              seatpos=_BPT_Seat.find(seatKey);
              seattmp=_BPT_Seat.getVal(seatpos);
              if(nums>seattmp) {seat=-1;return;}
              seat=std::min(seat,seattmp);
              if(data.train_info[i].station==End){end=i;break;}
              Datetmp+=(data.train_info[i+1].prefix_time-data.train_info[i].prefix_time-data.train_info[i+1].stopover);
              Datekey.mm=Datetmp.mm;
              Datekey.dd=Datetmp.dd;
              seatKey.time=Datekey.to_string();
          }
          End=data.train_info[end].station;
          price=data.train_info[end].prefix_price-data.train_info[sta].prefix_price;
      }
      void RenewSeat(const Ticket::String<20> Train_ID,const Ticket::Date & Sta_Time,const Ticket::String<40> & Sta,const Ticket::String<40> End,int devi){
          int pos=_BPT_Train.find(Train_ID);
          Train data=_BPT_Train.getVal(pos);
          int sz=data.station_num;
          int sta,end;
          for(int i=0;i<sz;i++) if(data.train_info[i].station==Sta){ sta=i;break;}

         // seatKey.time=Sta_Time.to_string();
          Ticket::Date Datetmp=Sta_Time;
          Ticket::Date DateKey;
          DateKey.mm=Datetmp.mm;
          DateKey.dd=Datetmp.dd;
          Seat_Key seatKey;
          seatKey.station=Sta;
          seatKey.train=Train_ID;
          seatKey.time=DateKey.to_string();
          int seatpos;
          int oriseat;
          for(int i=sta;i<sz;i++){
              seatKey.station=data.train_info[i].station;
              seatpos=_BPT_Seat.find(seatKey);
              oriseat=_BPT_Seat.getVal(seatpos);
              oriseat+=devi;
              _BPT_Seat.modifyVal(seatpos,oriseat);
              if(data.train_info[i].station==Sta) break;
              Datetmp+=(data.train_info[i+1].prefix_time-data.train_info[i].prefix_time-data.train_info[i+1].stopover);
              DateKey.mm=Datetmp.mm;
              DateKey.dd=Datetmp.dd;
              seatKey.time=DateKey.to_string();
          }
      }

      void QuerySeat(){

      }
        ~Train_manager()=default;
    };
}
#endif