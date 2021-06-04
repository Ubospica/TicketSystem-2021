#ifndef BACKEND_LOGGING_HPP
#define BACKEND_LOGGING_HPP
#include<iostream>
#include "String.hpp"
#include "map.hpp"
#include "BPlusTree.hpp"
//#include "Logging.hpp"
//#include<iostream>
#include"Exception.hpp"

namespace Backend {
    enum class user_parameter{
        Passwd,
        Name,
        MailAddr,
        //Privilege,
        Username
    };
    class user{
       // friend   std::ostream & operator<<(std::ostream& os,const user& u);
    private:
        Ticket::String<25> username;
        Ticket::String<35> passwd;
        Ticket::String<25> name;
        Ticket::String<35> mailAddr;
        int privilege=0;
    public:
        user()=default;

        explicit user(const Ticket::String<25> & Name):username(Name){};

        Ticket::String<25> get_25(user_parameter kind);

        const Ticket::String<25> get_25(user_parameter kind)const;

        Ticket::String<35> get_35(user_parameter kind);

        const int & get_privilege()const;

        void set_str(user_parameter kind,const std::string & change);

        void set_pri(int pri);

        bool passwd_match(const Ticket::String<35> & password);

        void print(std::ostream & os);

        ~user()=default;
    };

    class BPT_insert_user{
    private:
        Ticket::BPlusTree<Ticket::String<25>,user> _BPT_user;
    public:
        explicit BPT_insert_user(std::string & filename):_BPT_user(filename){};

        bool add_user(user & data);

        user find_user(const Ticket::String<25> & username);

        //void erase_user(const Ticket::String<25> & username);

        void modify_user(const user& Nuser);

        int size();

        void clean();

        ~BPT_insert_user()=default;
    };
    class Log_op{
    private:
        //    std::string _filename;
        BPT_insert_user op_user;
        //、、不需要记登陆者
        //int 为privilege
        Backend::map<Ticket::String<25>,int> _logging_list;

    public:
        explicit Log_op(std::string  & filename):op_user(filename){};

        bool is_log(const Ticket::String<25> & username);

        bool login(const Ticket::String<25> & user_name,const Ticket::String<35> & passwd);

        bool logout(const Ticket::String<25> & username);

        bool show_user(const Ticket::String<25> & op_name,const Ticket::String<25> & username,std::ostream & os);

        bool modify(const Ticket::String<25> & op_name,const bool * kind,const Ticket::String<25> & change,std::string * strs,int pri,std::ostream & os);//0 是密码 1是名字 2是邮箱 3是权限

        bool add_user(const Ticket::String<25> & op_name,const Ticket::String<25> & user_name,const std::string & password,const std::string & name,const std::string & mailAddr,const int & pri);

        void clean();

        ~Log_op()=default;
    };

}
namespace Backend {

    Ticket::String<25> user::get_25(user_parameter kind){
        switch(kind) {
            case (user_parameter::Username):return username;
            case (user_parameter::Name):return name;
            default:
                std::cerr<<"get_25"<<'\n';
                throw Ticket::WrongOperation("get_25");
        }
    }

    const Ticket::String<25> user::get_25(user_parameter kind)const{
        switch(kind) {
            case (user_parameter::Username):return username;
            case (user_parameter::Name):return name;
            default:
                std::cerr<<"get_25"<<'\n';
                throw Ticket::WrongOperation("get_25");
        }
    }


    Ticket::String<35> user::get_35(user_parameter kind){
        switch(kind) {
            case (user_parameter::Username):return passwd;
            case (user_parameter::Name):return mailAddr;
            default:
                std::cerr<<"get_35"<<'\n';
                throw Ticket::WrongOperation("get_25");
        }
    }


    const int & user::get_privilege()const{
        return privilege;
    }

    void user::set_str(user_parameter kind,const std::string & change){
        switch(kind) {
            case (user_parameter::Passwd):passwd=change;break;
            case (user_parameter::Name):name=change;break;
            case (user_parameter::MailAddr):mailAddr=change;break;
            default: {
                std::cerr<<"get_user_parameter"<<'\n';
                Ticket::WrongOperation();
            }
        }
        //  else if(kind==user_parameter::Privilege)  privilege=change;
    }

    void user::set_pri(int pri){
        privilege=pri;
    }

    bool user::passwd_match(const Ticket::String<35> & password){
        return password==passwd;
    }

    void user::print(std::ostream & os) {
        os<<username<<' '<<name<<' '<<mailAddr<<' '<<privilege<<'\n';
        //return os;
    }
    /*std::ostream & operator<<(std::ostream& os,const user& u){
        os<<u.username<<' '<<u.passwd<<' '<<u.mailAddr<<' '<<u.name<<' '<<u.privilege;
        return os;
    }*/

    bool BPT_insert_user::add_user(user & data){
        //std::cout<<data.get_username()<<' '<<'\n';
        if(_BPT_user.insert(data.get_25(user_parameter::Username),data)==-1) return false;
        else {
           // std::cout<<_BPT_user.insert(data.get_username(),data);
            return true;
        }
    }

    user BPT_insert_user::find_user(const Ticket::String<25> & username){
        int pos=_BPT_user.find(username);
        if(pos==-1) throw NotFound();
        return _BPT_user.getVal(pos);
    }

  /*  void BPT_insert_user::erase_user(const Ticket::String<25> & username){
        //int pos=_BPT_user.find(username);
        _BPT_user.erase(username);
    }
*/
    void BPT_insert_user::modify_user(const user& Nuser){
        int pos=_BPT_user.find(Nuser.get_25(user_parameter::Username));
        _BPT_user.modifyVal(pos,Nuser);
    }

    int BPT_insert_user::size(){
        return _BPT_user.getSize();
    }

    void BPT_insert_user::clean() {
        _BPT_user.clear();
    }

    bool Log_op::is_log(const Ticket::String<25> & username){return _logging_list.count(username);}

    bool Log_op::login(const Ticket::String<25> & user_name,const Ticket::String<35> & passwd){
        user tmp;
        // int tmp_pri;
        try {
            //op_pri = _logging_list[op_name];
            // tmp_pri = _logging_list[user_name];
            tmp=op_user.find_user(user_name);
        }catch(NotFound){return false;}
        //if(op_pri<tmp.get_privilege()) return false;
        if(tmp.passwd_match(passwd)) {
            if(is_log(user_name)) return false;
            Backend::map<Ticket::String<25>,int>::value_type p(user_name,tmp.get_privilege());
           // std::cout<<tmp.get_privilege()<<'?'<<"\n";
            _logging_list.insert(p);
            return true;
        }
        return false;
    }

    bool Log_op::logout(const Ticket::String<25> & username){
        return _logging_list.erase(username);
    }

    bool Log_op::show_user(const Ticket::String<25> & op_name,const Ticket::String<25> & username,std::ostream & os){
        int op_pri;
        user tmp;
        try{
            op_pri=_logging_list[op_name];
            tmp=op_user.find_user(username);
        }catch(NotFound){return false;}
       // std::cout<<'?'<<'\n';
        if(op_pri<tmp.get_privilege()) return false;
        if(op_pri==tmp.get_privilege()&&op_name!=username) return false;
        else {
            tmp.print(os);
            return true;
        }
    }

    bool Log_op::modify(const Ticket::String<25> & op_name,const bool * kind,const Ticket::String<25> & change,std::string * strs,int pri,std::ostream & os){
        int op_pri;
        //int tmp_pri;
        user tmp;
     //   std::cout<<_logging_list["root"]<<"\n";
        //std::cout
     //   std::cout<<_logging_list.size()<<"\n";
        if(_logging_list.count(op_name)) {
           // std::cout<<'?'<<'\n';
            try {
                op_pri = _logging_list[op_name];
                //tmp_pri = _logging_list[change];
                tmp=op_user.find_user(change);
            }catch(NotFound){return false;}
          //  std::cout<<op_pri<<' '<<tmp.get_privilege()<<' '<<op_name<<' '<<change<<"\n";
            if(op_pri<tmp.get_privilege()) return false;
            if(op_pri==tmp.get_privilege()&&op_name!=change) return false;
            //std::cout<<op_pri<<"\n";
      //      std::cout<<'?'<<'\n';
            if (kind[0]) tmp.set_str(user_parameter::Passwd, strs[0]);
            if (kind[1]) tmp.set_str(user_parameter::Name, strs[1]);
            if (kind[2]) tmp.set_str(user_parameter::MailAddr, strs[2]);
            if (kind[3]) {
                if(op_pri<=pri) return false;
                if(is_log(change)) _logging_list[change]=pri;
                tmp.set_pri(pri);
            }
            //   std::cout<<'?'<<'\n';
            op_user.modify_user(tmp);
            tmp.print(os);
            return true;
        }
        else return false;
    }//0 是密码 1是名字 2是邮箱 3是权限

    bool Log_op::add_user(const Ticket::String<25> & op_name,const Ticket::String<25> & user_name,const std::string & password,const std::string & name,const std::string & mailAddr,const int & pri){
        if(_logging_list.count(op_name)) {
            //判断权限
            int c_pri;
            try {c_pri = _logging_list[op_name];}catch(NotFound){
                std::cerr<<"NotFound";
                return false;}
         //   std::cout<<pri<<' '<<c_pri<<'\n';
            if(c_pri<=pri){
                std::cerr<<"pri"<<'\n';
                return false;}
            user u(user_name);
            u.set_pri(pri);
            //u.set_str(user_parameter::Username, user_name);
            u.set_str(user_parameter::Passwd, password);
            u.set_str(user_parameter::Name, name);
            u.set_str(user_parameter::MailAddr, mailAddr);
            return op_user.add_user(u);
            //return true;
        }
        else {
            //不存在特判
            //try {user c = op_user.find_user(op_name);}catch(NotFound){
            if(!op_user.size())
            {
                user u(user_name);
                u.set_pri(10);
                //std::cout<<'?'<<'\n';
                //u.set_str(user_parameter::Usernamem, user_name);
                u.set_str(user_parameter::Passwd, password);
                u.set_str(user_parameter::Name, name);
                u.set_str(user_parameter::MailAddr, mailAddr);
                op_user.add_user(u);
                return true;
            }
            else return false;
        }
    }

    void Log_op::clean(){
        op_user.clean();
    }

}
#endif