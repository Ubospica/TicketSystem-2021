#ifndef BACKEND_COMMAND_HPP
#define BACKEND_COMMAND_HPP
#include<iostream>
#include"String.hpp"
#include"Exception.hpp"
namespace Backend{
    class Cmd_Que{
    private:
        struct data {
            char type;
            std::string _command_part;
        };

        struct Node {
            Node *nxt;
            data _command;
            Node *prev;

            Node(char type, std::string &Val) {
                nxt = nullptr;
                prev = nullptr;
                _command.type = type;
                _command._command_part = Val;
            }

            Node() {
                nxt = prev = nullptr;
                _command._command_part = "";
                _command.type = ' ';
            }
        };

        Node *head;
        Node *rear;
        int _size;
    public:
        Cmd_Que(){
            head = new Node;
            rear = new Node;
            head->nxt = rear;
            (rear->_command.type) = '~';
            (rear->_command)._command_part = "^8l,*";
            rear->prev = head;
            _size = 0;
        };
        void insert(char type, std::string &Val);
        const std::string &top()const;
        const char &top_type();
        void pop();
        int size();
        void print();
        ~Cmd_Que();
        void clear();
    };
    void process(Backend::Cmd_Que *command_stream, std::string & Cmd);
}

namespace Backend {
#define ReStr(x, y, z) while(y[z]!=' '&&y[z]!='\0') x+=y[z++]

    class End{};

    void Cmd_Que::insert(char type, std::string &Val) {
        Node *new_Node = new Node(type, Val);
        new_Node->nxt = rear;
        new_Node->prev = rear->prev;
        new_Node->prev->nxt = new_Node->nxt->prev = new_Node;
        _size++;
    }

    const std::string & Cmd_Que::top()const {
        return (head->nxt->_command)._command_part;
    }

    void Cmd_Que::pop() {
        Node *tmp;
        tmp = head->nxt;
        head->nxt = tmp->nxt;
        tmp->nxt->prev = head;
        delete tmp;
        _size--;
    }

    int Cmd_Que::size() {
        return _size;
    }

    void Cmd_Que::print() {
        Node *tmp = head->nxt;
        while (tmp->nxt != nullptr) std::cout << (tmp->_command)._command_part << ' ', tmp = tmp->nxt;
        //std::cout<<"0000000000"<<'\n';
        std::cout<<'\n';
    }

    const char &Cmd_Que::top_type() {//' '为空 '~' 为结尾符 '!'为命令
        return (head->nxt->_command).type;
    }

    void Cmd_Que::clear(){
        Node *tmp;
        while (head->nxt != rear) {
            tmp = head->nxt;
            head->nxt = tmp->nxt;
            delete tmp;
        }
        head->nxt=rear;
        rear->prev=head;
        _size=0;
    }

    Cmd_Que::~Cmd_Que() {
        Node *tmp;
        while (head->nxt != nullptr) {
            tmp = head->nxt;
            head->nxt = tmp->nxt;
            delete tmp;
        }
        delete head;
    }

    void read_substr(Backend::Cmd_Que *stream, int n, std::string &command, int &pointer) {
        std::string parameter_list[n];
        std::string strtmp;
        char type;
        for (int i = 0; i < n; i++) {
            if (command[pointer] == '-') {
                pointer++;
                type = command[pointer++];
                pointer++;
            }
            ReStr(parameter_list[i], command, pointer);
            /*while(command[pointer]!=' '&&command[pointer]!='\n'){
                parameter_list[i]+=command[pointer++];}*/
            strtmp = parameter_list[i];
            pointer++;
            stream->insert(type, parameter_list[i]);
        }
    }

    void process(Backend::Cmd_Que *Cmd_Strm, std::string & Cmd) {
        int pointer = 0;
        int sz = Cmd.length();
        std::string tmp = "";
        ReStr(tmp, Cmd, pointer);
        Cmd_Strm->insert('!', tmp);
        pointer++;
        if (tmp == "add_user") {
            read_substr(Cmd_Strm, 6, Cmd, pointer);
        } else if (tmp == "login") {
            read_substr(Cmd_Strm, 2, Cmd, pointer);
        } else if (tmp == "logout") {
            read_substr(Cmd_Strm, 1, Cmd, pointer);
        } else if (tmp == "query_profile") {
            read_substr(Cmd_Strm, 2, Cmd, pointer);
        } else if (tmp == "modify_profile") {
            read_substr(Cmd_Strm, 2, Cmd, pointer);
            std::string parameter_list[4];
            int pos = 0;
            char type;
            //std::string strtmp;
            while (pointer < sz) {
                if (Cmd[pointer] == '-') {
                    pointer++;
                    type = Cmd[pointer++];
                    pointer++;
                }
                ReStr(parameter_list[pos], Cmd, pointer);
              //  strtmp = parameter_list[pos];
                pointer++;
                Cmd_Strm->insert(type, parameter_list[pos]);
                pos++;
            }
        } else if (tmp == "add_train") {
            read_substr(Cmd_Strm, 10,Cmd, pointer);
        } else if (tmp == "release_train") {
            read_substr(Cmd_Strm, 1, Cmd, pointer);
        } else if (tmp == "query_train") {
            read_substr(Cmd_Strm, 2, Cmd, pointer);
        } else if (tmp == "delete_train") {
            read_substr(Cmd_Strm, 1, Cmd, pointer);
        } else if (tmp == "query_ticket") {
            read_substr(Cmd_Strm, 3, Cmd, pointer);
            std::string in="time";
            if(pointer>=sz) Cmd_Strm->insert('p',in);
            else read_substr(Cmd_Strm, 1, Cmd, pointer);
        } else if (tmp == "query_transfer") {
            read_substr(Cmd_Strm, 3, Cmd, pointer);
            std::string in="time";
            if(pointer>=sz) Cmd_Strm->insert('p',in);
            else read_substr(Cmd_Strm, 1, Cmd, pointer);
        } else if (tmp == "buy_ticket") {
            read_substr(Cmd_Strm, 6, Cmd, pointer);
            std::string in="false";
            if(pointer>=sz) Cmd_Strm->insert('q',in);
            else read_substr(Cmd_Strm, 1, Cmd, pointer);
        } else if (tmp == "query_order") {
            read_substr(Cmd_Strm, 1, Cmd, pointer);
        } else if (tmp == "refund_ticket") {
            read_substr(Cmd_Strm, 1, Cmd, pointer);
            std::string in="1";
            if(pointer==sz) Cmd_Strm->insert('n',in);
            else read_substr(Cmd_Strm, 1, Cmd, pointer);
        } else if (tmp == "clean") {}
        else if (tmp == "exit") {}
        else throw Ticket::SyntaxError();
    }
}
#endif