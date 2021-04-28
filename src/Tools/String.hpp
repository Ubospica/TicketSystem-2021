//
// Created by Ubospica on 2021/4/23.
//

#ifndef BOOKSTORE_2021_DATA_HPP
#define BOOKSTORE_2021_DATA_HPP


#include <cstring>
#include <string>
#include <iostream>

namespace Ticket {
	struct String {
		char data[50]{0};
		
		String() = default;
		
		explicit String(char *str) {
			strcpy(data, str);
		}
		
		explicit String(const std::string &s1) {
			strcpy(data, s1.c_str());
		}
		
		String(const String &ano) {
			strcpy(data, ano.data);
		}
		
		String &operator=(const String &ano) {
			if (this == &ano) {
				return *this;
			}
			strcpy(data, ano.data);
			return *this;
		}
		
		bool operator<(const String &ano) const {
			return std::strcmp(data, ano.data) < 0;
		}
		
		bool operator<=(const String &ano) const {
			return std::strcmp(data, ano.data) <= 0;
		}
		
		bool operator>(const String &ano) const {
			return std::strcmp(data, ano.data) > 0;
		}
		
		bool operator>=(const String &ano) const {
			return std::strcmp(data, ano.data) >= 0;
		}
		
		bool operator==(const String &ano) const {
			return std::strcmp(data, ano.data) == 0;
		}
		
		bool operator!=(const String &ano) const {
			return std::strcmp(data, ano.data) != 0;
		}
		
		friend std::ostream &operator<<(std::ostream &os, const String &s) {
			os << s.data;
			return os;
		}
		
		friend std::istream &operator>>(std::istream &is, String &s) {
			is >> s.data;
			return is;
		}
		
		void clear() {
			data[0] = 0;
		}
		
		bool empty() const {
			return data[0] == 0;
		}
		
		char& operator[] (int p) {
			return data[p];
		}
		
		const char& operator[] (int p) const {
			return data[p];
		}
	};
}


#endif //BOOKSTORE_2021_DATA_HPP
