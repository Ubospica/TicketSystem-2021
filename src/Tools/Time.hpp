//
// Created by Ubospica on 2021/4/28.
//

#ifndef TICKETSYSTEM_2021_TIME_HPP
#define TICKETSYSTEM_2021_TIME_HPP

#include <iostream>
#include <iomanip>
#include <string>

namespace Ticket {
	struct Time {
		int hr = 0, mi = 0;
		
		Time () = default;
		
		Time (int h, int m) : hr(h), mi(m) { }
		
		bool operator<(const Time &another) const {
			return hr != another.hr ? hr < another.hr
				: mi < another.mi;
		}
		
		[[nodiscard]] virtual std::string to_string() const {
			std::stringstream ss;
			ss << std::setw(2) << std::setfill('0') << std::right;
			ss << hr << ':' << mi;
			return ss.str();
		}
		
		friend std::ostream& operator<<(std::ostream &os, const Time &one) {
			os << one.to_string();
			return os;
		}
	};
	
	struct Date : public Time {
		int mm = 0, dd = 0;
		
		constexpr static int monthDays[] {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		
		Date () = default;
		
		Date (int mm, int dd, int h, int m) : mm(mm), dd(dd), Time(h, m) { }
		
		Date (const Time &base) : Time(base) {}
		
		bool operator<(const Date &another) const {
			return mm != another.mm ? mm < another.mm :
				dd != another.dd ? dd < another.dd :
					Time::operator<(another);
		}
		
		Date operator+(const Date &another) const {
			Date res = *this;
			res.mi += another.mi;
			res.hr += another.hr;
			res.dd += another.dd;
			res.mm += another.mm;
			if (res.mi >= 60) {
				res.mi -= 60;
				++res.hr;
			}
			if (res.hr >= 24) {
				res.hr -= 24;
				++res.dd;
			}
			if (res.dd >= monthDays[res.mm]) {
				res.dd -= monthDays[res.mm];
				++res.mm;
			}
			return res;
		}
		
		Date& operator+=(const Date &another) {
			*this = *this + another;
			return *this;
		}
		
		Date operator+(const int &minutes) const {
			return *this + Date(0, minutes/1440, (minutes % 1440) / 60, minutes % 60);
		}
		
		Date& operator+=(const int &minutes) {
			*this = *this + minutes;
			return *this;
		}
		
		Date& operator++() {
			*this += Date(0, 1, 0, 0);
			return *this;
		}
		
		[[nodiscard]] int cmpDate(const Date &another) const {
			if (mm == another.mm) {
				return dd < another.dd ? -1 : dd == another.dd ? 0 : 1;
			}
			else {
				return mm < another.mm ? -1 : 1;
			}
		}
		
		[[nodiscard]] std::string to_string() const override {
			std::stringstream ss;
			ss << std::setw(2) << std::setfill('0') << std::right;
			ss << mm << '-' << dd;
			return ss.str() + Time::to_string();
		}
		
		friend std::ostream& operator<<(std::ostream &os, const Date &one) {
			os << one.to_string();
			return os;
		}
	};
}


#endif //TICKETSYSTEM_2021_TIME_HPP
