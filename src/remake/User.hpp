//
// Created by Ubospica on 2021/6/6.
//

#ifndef CODE_USER_HPP
#define CODE_USER_HPP


#include <iostream>
#include <string>
#include <cstring>
#include <map>

#include "Tools/String.hpp"
#include "BPlusTree/BPlusTree.hpp"


namespace Ticket {
	using std::map;
	
	class Users {
		
		/*
		 * username：用户的唯一标识符，由字母开头，字母、数字和下划线组成的字符串，长度不超过20。
			password：由字母、数字和下划线组成的字符串，长度不低于6，不超过30。
			name：由二至五个汉字组成的字符串。
			mailAddr：同一般邮箱地址格式，且仅含数字、大小写字母，@和.，长度不超过30。（无需检验邮箱格式的合法性）
			privilege：所处用户组优先级，为一个0~10的整数。
		 */
		struct User {
			SString username, name;
			LString passwd, mail;
			int pri;
		};
		BPlusTree<SString, User> userList;
		map<SString, int> loginUser; // username -> pri
		
		void add_user(const char *curname, const char *username, const char *passwd, const char *name, const char *mail, int pri) {
			if (userList.isEmpty()) {
				userList.insert(username, (User) {username, name, passwd, mail, 10});
				std::cout << "0\n";
				return;
			}
			else {
				auto it = loginUser.find(curname);
				if (it == loginUser.end() || *it <= pri || userList.find(username) != -1) {
					std::cout << "-1\n";
					return;
				}
				userList.insert()
			}
		}
		
	};
}


#endif //CODE_USER_HPP
