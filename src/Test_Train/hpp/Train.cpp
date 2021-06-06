#include<iostream>
#include"Time.hpp"
#include"String.hpp"
#include"BPlusTree.hpp"
#include"map.hpp"
#include<vector>
#include"exceptions.hpp"
namespace Backend{
    class Train_manager{
    private:
        struct Train{
        //private:
            //Ticket::String Train_SN;
            Ticket::Date start_day;
            Ticket::Date end_day;
            Ticket::Time start_time;
            Ticket::String start_station;
            int station_num;
           // char rl='N';
            char type;
            //int station;
            struct info{
                Ticket::String station;
                //int prefix_time;
                Ticket::Date Sta_Date;//仅代表日期的起终
                Ticket::Date End_Date;//
                Ticket::Time arri_time;
                int prefix_price;
                int prefix_time;
                //prefix_time是离开时间
                int stopover;
                //int seat;
            };
            info train_info[100];
        //public:
            //Train()=default;
            //~Train()=default;
        };
        struct Station{
            Ticket::String Train_SN;
            int Train_pos;
            Ticket::Date Start_Date;
            Ticket::Date End_Date;
            //Ticket::Time arri_time;
            Station(Ticket::String & SN,Train & o,int & pos){
                Train_SN=SN;
                Train_pos=pos;
                Start_Date=o.train_info[pos].Sta_Date;
                End_Date=o.train_info[pos].End_Date;
                //arri_time=o.train_info[pos].arri_time;
                //start=o.train_info[pos].arri_Date;
                //end=o.train_info[pos].depart_Date;
            }
        };
        /*struct Train_Key{
            Ticket::String Train_SN;
            int pos;
        };*/
        struct Seat_arr{
            int seat_info[100];
            char rl;
        };
        struct Station_Key{
            Ticket::String Station_name;
            int pos;
        };
        struct Station_Comp{
            bool operator()(Station_Key & l,Station_Key & r){
                return l.Station_name<r.Station_name;
            }
        };
        Ticket::BPlusTree<Ticket::String,Train> _BPT_Train;
        Ticket::BPlusTree<Station_Key,Station> _BPT_Station;
        Ticket::BPlusTree<Ticket::String,Seat_arr> _BPT_Seat;
        inline int _get_seat(const Seat_arr & seatArr,int & Sta,int & Det){
            int ret=seatArr.seat_info[Sta];
            for(int i=Sta;i<=Det;i++) ret=min(ret,seatArr.seat_info[i]);
            return min;
        }
        inline void _set_seat(Seat_arr & seatArr,int & Sta,int & Det,int & devi){
            for(int i=Sta;i<=Det;i++) seatArr.seat_info[i]+=devi;
        }
        void _get_seat_arr(Seat_arr & seatArr,const Ticket::String & SN,const Ticket::String & Sta,const Ticket::String & End){
            Station_Key cmp;
            cmp.pos=0;
            cmp.Station_name=Sta;
            std::vector<Station> Stavec=_BPT_Station.route<Station_Comp>(cmp);
            cmp.Station_name=End;
            std::vector<Station> Endvec=_BPT_Station.route<Station_Comp>(cmp);
            int sta,end,sz;
            sz=Stavec.size();
            for(int i=0;i<sz;i++) if(Stavec[i].Train_SN==SN) sta=Stavec[i].Train_pos;
            sz=Endvec.size();
            for(int i=0;i<sz;i++) if(Stavec[i].Train_SN==SN) end=Stavec[i].Train_pos;
            int seatpos=_BPT_Seat.find(SN);
            seatArr=_BPT_Seat.getVal(seatpos);
        }
        struct Comp{
            int data;
            int pos;
            Ticket::String ID;
            bool operator<(const Comp & o){
                return data<o.data||(data==o.data&&ID<o.ID);
            }
        };
    public:
        Train_manager()=default;
        bool add_train(const Ticket::String & SN,int & stanum,int seatnum,Ticket::String * stations, int * price,Ticket::Time sta_time,int * traveltime,int * stoppovertime,Ticket::Date * saleDate,char type){//saleDate[0]为起始,1为终结
            Train data;
            int sumtime=0;
            //Train_Key data_key;
           // data_key.Train_SN
           data.station=stanum;
           data.type=type;
           //设定起始时间相关
           data.start_time=sta_time;
           data.start_day=saleDate[0]+data.start_time;
           data.end_day=saleDate[1]+data.start_time;
           data.start_station=stations[0];
           //设定首站，到达时间，停留时间,累计时间无用，
           //data.train_info[0].seat=seat;
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
           //设定后面
           //Ticket::Date Statmp,Endtmp;
           Seat_arr seatArr;
           seatArr.rl='N';
           seatArr.seat_info[0]=seat;
           for(int i=1;i<stanum;i++){
               seatArr.seat_info[i]=seatnum;
               data.train_info[i].station=stations[i];
               //data.train_info[i].seat=seatnum;
               data.train_info[i].prefix_price=data.train_info[i-1].prefix_price+price[i];
               Statmp+=traveltime[i-1];
               Endtmp+=traveltime[i-1];
               data.train_info[i].Sta_Date=Statmp;
               data.train_info[i].End_Date=Endtmp;
               data.train_info[i].arri_time.hr=Statmp.hr;
               data.train_info[i].arri_time.mi=Statmp.mi;
               data.train_info[i].stopover=stoppovertime[i];
               Statmp+=stoppovertime[i];
               Endtmp+=stoppovertime[i];
               //data.train_info[i].arri_Date=data.train_info[i-1].depart_Date+traveltime[i-1];
               //data.train_info[i].depart_Date=data.train_info[i].arri_Date+stoppovertime[i];
               data.train_info[i].prefix_time=data.train_info[i-1].prefix_time+traveltime[i-1]+stoppovertime[i];//
           }
           if(_BPT_Train.insert(SN,data)==-1){
               _BPT_Seat.insert(SN,seatArr);
               return false;
           }
           else return true;
           //终点站,price与出发时间停留时间无用
        }
        bool delete_train(const Ticket::String & SN){
            //这里好像有点鲁棒？判断是否发售？
            int pos=_BPT_Seat.find(SN);
            if(pos==-1) throw NotFound("delete_train");
            else {
                Seat_arr seatArr=_BPT_Seat.find(SN);
                if(seatArr.rl=='N'){
                    if(_BPT_Seat.erase(SN)&&_BPT_Train.erase(SN)) return true;
                    else throw wrong_operation("delete_train");
                }
                else if(seatArr.rl=='Y') return false;
                else throw wrong_operation("delete_train_2");
            }
        }
        bool release_train(const Ticket::String & SN){
            int pos=_BPT_Train.find(SN);
            //Otmp_pos为第二key为0的位置
            int Otmp_pos;
            Station_Key Otmp;
            Station_Key data_key;
            if(pos==-1) throw out_of_bound("release_train");
            Train data=_BPT_Train.getVal(pos);
            Otmp.pos=0;
            for(int i=0;i<data.station_num;i++){
                Otmp.Station_name=data_key.Station_name=data.train_info[i].station;
                Otmp_pos=_BPT_Station.find(Otmp);
                Station station(SN,data,i);
                if(tmp_pos==-1){
                    Station head(SN,data,1);
                    _BPT_Station.insert(Otmp,head);
                    data_key.pos=1;
                    _BPT_Station.insert(data_key,station);
                }
                else{
                    Station Ontmp=_BPT_Station.getVal(Otmp_pos);
                    data_key.pos=++Ontmp.Train_pos;
                    _BPT_Station.insert(data_key,statoin);
                    //data_key.pos=++ntmp.;
                    //_BPT_Station.insert(Otmp,station);
                    _BPT_Station.modifyVal(Otmp_pos,Ontmp);
                }
            }
            //改状态
            Seat_arr seatArr;
            int seat_pos=_BPT_Seat.find(SN);
            if(seatArr==-1) throw out_of_bound("release_train_2");
            else{
                seatArr.rl='Y';
                _BPT_Seat.modifyVal(seat_pos,seatArr);
            }
            //_BPT_Train.modifyVal(pos,data);
        }
        bool query_train(const Ticket::String & SN,const Ticket::Date & tDate){
            int pos=_BPT_Train.find(SN);
            int spos=_BPT_Seat.find(SN);
            if(pos==-1&&spos==-1) return false;
            else{
                //出错的话先检查spos和pos
                Train data=_BPT_Train.getVal(pos);
                Seat_arr seatArr=_BPT_Seat.getVal(spos);
                //、、还有日期不对
                int sz=data.station_num;
                Ticket::Date cal;
                cal=tDate+data.start_time;
                cout<<SN<<' '<<data.type<<'\n';
                cout<<data.train_info[0].station<<' '<<"xx-xx xx:xx ->"<<' '<<cal<<' '<<data.train_info[0].prefix_price<<' '<<seatArr.seat_info[0];
                for(int i=1;i<sz;i++){
                    cal+=data.train_info[i].prefix_time-data.train_info[i-1].prefix_time-data.train_info[i].stopover;
                    cout<<data.train_info[i].station<<' '<<cal<<' '<<'-'<<'>';
                    cal+=data.train_info[i].stopover;
                    cout<<cal<<' '<<data.train_info[i].prefix_price<<' '<<seatArr.seat_info[i];
                }
                cal+=data.train_info[sz-1].prefix_time-data.train_info[sz-2].prefix_time-data.train_info[sz-1].stopover;
                cout<<data.train_info[sz-1].station<<' '<<cal<<' '<<"-> xx-xx xx:xx"<<' '<<data.train_info[sz-1].prefix_price<<'x'<<'\n';
            }
            return true;
        }
        void query_ticket(const Ticket::String & Sta,const Ticket::String &Det,const Ticket::Date & date,char type){//type 'T'-time 'P'-price
            Backend::map<Ticket::String,int> match;
            std::vector<Ticket::String> aimIDvec;
            Station_Key Keytmp;
            Keytmp.Station_name=Sta;
            Keytmp.pos=0;
            std::vector<Station> & stavec=_BPT_Station.route<Station_Comp>(Keytmp);
            Keytmp.Station_name=Det;
            std::vector<Station> & endvec=_BPT_Station.route<Station_Comp>(Keytmp);
            int sz=staIDvec.size();
            std::pair<Ticket::String,int> tmp;
            tmp.second=0;
            for(int i=1;i<sz;i++){
                if(stavec[i].Start_Date.day<=date.day&&date.day<=stavec[i].End_Date.day){
                    tmp.first=stavec[i].Train_SN;
                    if(match.insert(tmp)){}
                    else throw wrong_operation("query_ticket");
                }
            }
            sz=endIDvec.size();
            for(int i=1;i<sz;i++) if(match.count(endvec[i].Train_SN)) aimIDvec.push_back(endvec[i].Train_SN);
            std::vector<Train> Trainvec;
            int pos;
            sz=aimIDvec.size();
            for(int i=0;i<sz;i++){
                pos=_BPT_Station.find(aimIDvec[i]);
                //这里可能会有问题
                Station tmp=_BPT_Station.getVal(pos);
                //即时间判断
                if(tmp.Start_Date<=date&&date<=tmp.End_Date){
                    int trapos=_BPT_Train.find(tmp.Train_SN);
                    if(trapos==-1) throw wrong_operation("query_tiekcet_2");
                    Trainvec.push_back(_BPT_Train.getVal(trapos));
                }
            }
            sz=Trainvec.size();
            Comp arr[sz];
            Seat_arr seatArrs[sz];
            int tmp_seat_pos;
            if(type=='T'){
                for(int i=0;i<sz;i++){
                    tmp_seat_pos=_BPT_Seat.find(aimIDvec[i]);
                    seatArrs[i]=_BPT_Seat.find(tmp_seat_pos);
                    arr[i].ID=aimIDvec[i];
                    arr[i].pos=i;
                    arr[i].data=Trainvec[i].train_info[endvec[i].Train_pos].prefix_time-Trainvec[i].train_info[stavec[i].Train_pos].prefix_time+Trainvec[i].train_info[endvec[i].Train_pos].stopover;
                }
            }
            else if(type=='P'){
                for(int i=0;i<sz;i++){
                    tmp_seat_pos=_BPT_Seat.find(aimIDvec[i]);
                    seatArrs[i]=_BPT_Seat.find(tmp_seat_pos);
                    arr[i].ID=aimIDvec[i];
                    arr[i].pos=i;
                    arr[i].data=Trainvec[i].train_info[endvec[i].Train_pos].prefix_price-Trainvec[i].train_info[stavec[i].Train_pos].prefix_price;
                }
            }
            else throw wrong_operation("query_ticket_2");
            std::sort(arr,arr+sz);
            Ticket::Date Dtmp;
            for(int i=0;i<sz;i++)
            {
                Dtmp=date+Trainvec[i].train_info[stavec[i].Train_pos].arri_time+Trainvec[i].train_info[stavec[i].Train_pos].stopover;
                cout<<arr[i].ID<<' '<<Sta<<' '<<Dtmp<<' ';
                Dtmp=Trainvec[i].train_info[stavec[i].Train_pos].arri_time+(Trainvec[i].train_info[endvec[i].Train_pos].prefix_time-Trainvec[i].train_info[stavec[i].Train_pos].prefix_time-Trainvec[i].train_info[stavec[i].Train_pos].stopover-Trainvec[i].train_info[endvec[i].Train_pos].stopover);
                cout<<Det<<' '<<Dtmp<<' '<<Trainvec[i].train_info[endvec[i].Train_pos].prefix_price-Trainvec[i].train_info[stavec[i].Train_pos].prefix_price<<_get_seat(seatArrs[i],stavec[i].Train_pos,endvec[i].Train_pos);
            }
        }
        int get_seat(const Ticket::String & SN,const Ticket::String & Sta,const Ticket::String & End){
            Seat_arr Arr;
            _get_seat_arr(Arr,SN,Sta,End);
            return _get_seat(Arr,sta,end);
        }
        void set_seat(const Ticket::String & SN,const Ticket::String & Sta,const Ticket::String & End,int & devi){
            Seat_arr Arr;
            _get_seat_arr(Arr,SN,Sta,End);
            _set_seat(Arr,sta,end,devi);
        }
        
        ~Train_manager()=default;
    };
}