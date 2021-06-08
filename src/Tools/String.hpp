//
// Created by Ubospica on 2021/4/23.
//

#ifndef BOOKSTORE_2021_DATA_HPP
#define BOOKSTORE_2021_DATA_HPP


#include <cstring>
#include <string>
#include <iostream>

namespace Ticket {
	/**
	 * String with fixed length
	 * @tparam LENGTH maximum length of string
	 */
	
	template <size_t LENGTH = 35>
	struct String {
		char data[LENGTH]{0};
		
		String() = default;
		
		String(char *str) {
			strcpy(data, str);
		}
		
		String(const std::string &s1) {
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
		
		template <size_t LEN>
		friend size_t hash(const String<LEN> &s);
	};
	
	using LString = String<35>;
	using SString = String<25>;
	
	size_t hash (const char *s) {
		static const size_t base = 257, mod = (size_t) 1e13 + 61;
		size_t res = 0;
		while (*s) {
			res = (res * base + (unsigned char) *s) % mod;
			++s;
		}
		return res;
	}
	
	size_t hash (const std::string &s) {
		return hash(s.c_str());
	}
	
	template <size_t LENGTH>
	size_t hash(const String<LENGTH> &s) {
		return hash(s.data);
	}
}


#endif //BOOKSTORE_2021_DATA_HPP
