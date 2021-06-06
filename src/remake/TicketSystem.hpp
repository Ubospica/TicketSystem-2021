//
// Created by Ubospica on 2021/6/6.
//

#ifndef CODE_TICKETSYSTEM_HPP
#define CODE_TICKETSYSTEM_HPP

#include <iostream>
#include <string>
#include <cstring>

#include "Exception.hpp"

namespace Ticket {
	class System {
	public:
		
		Users users;
		
		void run (const std::string &input) {
			const char *s = input.c_str();
			char operation[20];
			int cnt;
			sscanf(s, "%s %n", operation, &cnt);
//			std::cerr << operation << ' ' << cnt << '\n';
			s += cnt;
			if (std::strcmp(operation, "add_user")) {
				add_user(s);
			}
			else if (std::strcmp(operation, "login")) {
				login(s);
			}
			else if (std::strcmp(operation, "logout")) {
				logout(s);
			}
			else if (std::strcmp(operation, "query_profile")) {
				query_profile(s);
			}
			else if (std::strcmp(operation, "modify_profile")) {
				modify_profile(s);
			}
			else if (std::strcmp(operation, "add_train")) {
				add_train(s);
			}
			else if (std::strcmp(operation, "release_train")) {
				release_train(s);
			}
			else if (std::strcmp(operation, "query_train")) {
				query_train(s);
			}
			else if (std::strcmp(operation, "delete_train")) {
				delete_train(s);
			}
			else if (std::strcmp(operation, "query_ticket")) {
				query_ticket(s);
			}
			else if (std::strcmp(operation, "query_transfer")) {
				query_transfer(s);
			}
			else if (std::strcmp(operation, "buy_ticket")) {
				buy_ticket(s);
			}
			else if (std::strcmp(operation, "query_order")) {
				query_order(s);
			}
			else if (std::strcmp(operation, "refund_ticket")) {
				refund_ticket(s);
			}
			else if (std::strcmp(operation, "clean")) {
				clean(s);
			}
			else if (std::strcmp(operation, "exit")) {
				exit(s);
			}
		}
		/*
		 * [N] add_user
			参数列表
			-c -u -p -n -m -g
			说明
			当前用户<cur_username>为-c，创建一个<username>为-u，<password>为-p，<name>为-n，<mailAddr>为-m，<privilege>为-g的用户。
			权限要求：-c已登录，且新用户的权限低于-c的权限。
			特别地，创建第一个用户时，新用户权限为10，不受上述权限规则约束。具体来讲，当创建第一个用户时，忽略-c和-g参数，并认为新用户优先级为10。
			返回值
			注册成功：0
			注册失败：-1
		 */
		
		void add_user(const char *s) {
			static char curname[35], username[35], passwd[35], name[35], mail[35];
			int pri = 0;
			
			char c;
			int cnt;
			
			while (sscanf(s, "-%c ", &c) == 1) {
				s += 3;
				if (c == 'c') {
					sscanf(s, "%s%n", curname, &cnt);
				}
				else if (c == 'u') {
					sscanf(s, "%s%n", username, &cnt);
				}
				else if (c == 'p') {
					sscanf(s, "%s%n", passwd, &cnt);
				}
				else if (c == 'n') {
					sscanf(s, "%s%n", name, &cnt);
				}
				else if (c == 'm') {
					sscanf(s, "%s%n", mail, &cnt);
				}
				else if (c == 'g') {
					sscanf(s, "%d%n", &pri, &cnt);
				}
				else {
					throw SyntaxError();
				}
				s += cnt + 1;
			}
			users.add_user(curname, username, passwd, name, mail, pri);
		}
		
		/*
		 * [F] login
			参数列表
			-u -p
			说明
			以-u为<username>，-p为<password>登录。若登录成功，当前用户列表增加此用户。第一次进入系统时，当前用户为空。
			返回值
			登录成功：0
			登录失败：-1
			[F] logout
			参数列表
			-u
			说明
			<username>为-u的用户退出登录。若退出成功，将此用户移出当前用户列表。
			返回值
			登出成功：0
			登出失败：-1
		 */
		void login(const char *s) {
			static char username[35], passwd[35];
			
			char c;
			int cnt;
			
			while (sscanf(s, "-%c ", &c) == 1) {
				s += 3;
				if (c == 'u') {
					sscanf(s, "%s%n", username, &cnt);
				}
				else if (c == 'p') {
					sscanf(s, "%s%n", passwd, &cnt);
				}
				else {
					throw SyntaxError();
				}
				s += cnt + 1;
			}
			users.login(username, passwd);
		}
		void logout(const char *s) {
			static char username[35];
			
			char c;
			int cnt;
			
			while (sscanf(s, "-%c ", &c) == 1) {
				s += 3;
				if (c == 'u') {
					sscanf(s, "%s%n", username, &cnt);
				}
				else {
					throw SyntaxError();
				}
				s += cnt + 1;
			}
			users.logout(username);
		}
		/*
		 * [SF] query_profile
			参数列表
			-c -u
			说明
			<username>为-c的用户查询<username>为-u的用户信息。
			权限要求：-c已登录，且「-c的权限大于-u的权限，或是-c和-u相同」。
			返回值
			查询成功：一行字符串，依次为被查询用户的<username>，<name>，<mailAddr>和<privilege>，用一个空格隔开。
			查询失败：-1
		 */
		void query_profile(const char *s) {
			static char curname[35], username[35];
			int pri = 0;
			
			char c;
			int cnt;
			
			while (sscanf(s, "-%c ", &c) == 1) {
				s += 3;
				if (c == 'c') {
					sscanf(s, "%s%n", curname, &cnt);
				}
				else if (c == 'u') {
					sscanf(s, "%s%n", username, &cnt);
				}
				else {
					throw SyntaxError();
				}
				s += cnt + 1;
			}
			users.query_profile(curname, username);
		}
		/*
		 * [F] modify_profile
			参数列表
			-c -u (-p) (-n) (-m) (-g)
			说明
			<username>为-c的用户修改<username>为-u的用户信息。修改参数同注册参数，且均可以省略。
			权限要求：-c已登录，且「-c的权限大于-u的权限，或是-c和-u相同」，且-g需低于-c的权限。
			返回值
			修改成功：返回被修改用户的信息，格式同query_profile。
			修改失败：-1
		 */
		void modify_profile(const char *s) {
			static char curname[35], username[35], passwd[35], name[35], mail[35];
			int pri = -1;
			curname[0] = username[0] = passwd[0] = name[0] = mail[0] = 0;
			
			char c;
			int cnt;
			
			while (sscanf(s, "-%c ", &c) == 1) {
				s += 3;
				if (c == 'c') {
					sscanf(s, "%s%n", curname, &cnt);
				}
				else if (c == 'u') {
					sscanf(s, "%s%n", username, &cnt);
				}
				else if (c == 'p') {
					sscanf(s, "%s%n", passwd, &cnt);
				}
				else if (c == 'n') {
					sscanf(s, "%s%n", name, &cnt);
				}
				else if (c == 'm') {
					sscanf(s, "%s%n", mail, &cnt);
				}
				else if (c == 'g') {
					sscanf(s, "%d%n", &pri, &cnt);
				}
				else {
					throw SyntaxError();
				}
				s += cnt + 1;
			}
			users.modify_profile(curname, username, passwd, name, mail, pri);
		}
		void add_train(const char *s) {
		
		}
		void release_train(const char *s) {
		
		}
		void query_train(const char *s) {
		
		}
		void delete_train(const char *s) {
		
		}
		void query_ticket(const char *s) {
		
		}
		void query_transfer(const char *s) {
		
		}
		void buy_ticket(const char *s) {
		
		}
		void query_order(const char *s) {
		
		}
		void clean(const char *s) {
		
		}
		
		
		void refund_ticket(const char *s) {
		
		}
		
		void exit(const char *s) {
			std::cout << "bye\n";
			throw End();
		}
	};
}

#endif //CODE_TICKETSYSTEM_HPP
