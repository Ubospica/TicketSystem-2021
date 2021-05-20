

```c++
那么又到了喜闻乐见的文档环节
version 0.1
1.MainClass
2.Log_Op
3.Order_Op
4.Train_Manager
5.Cmd_Que
```

```c++
2.Log_Op 文件
    (1)
    //参数指示
    enum class user_parameter{
        Passwd,
        Name,
        MailAddr,
        Username,
        Privilege
    }
	(2)
	// user 类,记录user 的基础信息
	class user{
        //重载输出符
        friend std::ostream & operator<<(std::ostream& os,const user& u);
    private:
        Ticket::String username;
        Ticket::String passwd;
        Ticket::String name;
        Ticket::String mailAddr;
        int privilege=0;
    public:
        user()=default;
        
        user(Ticket::String Name):username(Name){};
        
        const Ticket::String get_username()const;
        
        //按照user_parameter类型 返回所需str
        Ticket::String get_user_parameter(user_parameter kind);
        
        int get_privilege();
        
        //按照user_parameter,一次修改一种str为change
        void set_str(user_parameter kind,Ticket::String & change);
        
        void set_pri(int pri);
        
        //输入password,判断是否匹配
        bool passwd_match(const Ticket::String & password);
        
        ~user()=default;
    }

	 std::ostream & operator<<(std::ostream& os,const user& u){
        os<<u.username<<' '<<u.passwd<<' '<<u.mailAddr<<' '<<u.name<<' '<<u.privilege<<'\n';
        return os;
    }

	class BPT_insert_user{
    private:
        //封装BPT
        Ticket::BPlusTree<Ticket::String,user> _BPT_user;
    public:
        explicit BPT_insert_user(std::string & filename):_BPT_user(filename){};

        bool add_user(user & data);

        //没找到会抛出NotFound() (抛出好丑，我想改了)
        user find_user(const Ticket::String & username);

        void erase_user(const Ticket::String & username);

        void modify_user(const user& Nuser);

        ~BPT_insert_user()=default;
    };

	class Log_Op{
         BPT_insert_user op_user;
        
        //不需要记登陆者具体信息
        //登录记录的set
        Backend::map<Ticket::String,int> _logging_list;//int为0

        bool is_log(const Ticket::String & username);
    public:
        explicit Log_op(std::string  & filename):op_user(filename){};

        bool login(const Ticket::String user_name,Ticket::String passwd);

        bool logout(const Ticket::String& username);

        void show_user(const Ticket::String& username);//相当于query

        //bool* 中,0 是密码 1是名字 2是邮箱 3是权限，按照bool* 决定是否修改,strs中
        //0为密码 1是名字 2是邮箱
        //先判断op_name是否登录；再判断是否存在该username,然后判断权限大小
        bool modify(Ticket::String op_name const bool * kind,const Ticket::String & change,Ticket::String * strs,int pri);
        
        //先判断op_name是否登录;再判断是否存在该op_name(好像没有必要);再比pri和op_pri大小
        //若op_name未登录;再判断是否存在该op_name;若不存在则为初始状态；若存在则
        void add_user(user & u);

        ~Log_op()=default;
    };
```



```c++
5.Cmd_Que 文件
    (1)
    //模拟stringstream实现的队列类，其内部含有处理过的string 与参数类型 type
    class Cmd_Que{
    private:
        struct Node{
            char Type;
            std::string Str;
            Node* nxt;
            Node* prev;
        }
    public:
        Cmd_Que();
        void pop();
        void insert(const Ticket::String & data);
        const std::string & top()const;
        const char & top_type()const;
        ~Cmd_Que();
    }
    (2)
    //函数Substr,将命令按照空格拆解为不同成分,并返回Cmd_Que*;
	//会在函数内部开辟动态内存 ,因此需要在函数外删除该指针指向内容;
	#define Restr(x,y,z) while(x[y]!=' '&&x[y]!='\0') z+=x[y++];
	Cmd_Que* Substr(std::string & cmd);
```











