//
// Created by Ubospica on 2021/4/28.
//

#ifndef TICKETSYSTEM_2021_TIME_HPP
#define TICKETSYSTEM_2021_TIME_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include "Utility.hpp"

namespace Ticket {
//	struct Time {
//		int hr = 0, mi = 0;
//
//		Time () = default;
//
//		Time (int h, int m) : hr(h), mi(m) { }
//
//		bool operator<(const Time &another) const {
//			return hr != another.hr ? hr < another.hr
//				: mi < another.mi;
//		}
//
//		[[nodiscard]] virtual std::string to_string() const {
//			char tmp[20];
//			sprintf(tmp, "%02d:%02d", hr, mi);
////			std::stringstream ss;
////			ss << std::setw(2) << std::setfill('0') << std::right;
////			ss << hr << ':' << mi;
//			return tmp;
//		}
//
//		friend std::ostream& operator<<(std::ostream &os, const Time &one) {
//			os << one.to_string();
//			return os;
//		}
//	};
//
//	constexpr int monthDays[] { 100'000, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
//	struct Date : public Time {
//		int mm = 0, dd = 0;
//
//
//		Date () = default;
//
//		Date (int mm, int dd, int h, int m) : mm(mm), dd(dd), Time(h, m) { }
//
//		Date (const Time &base) : Time(base) {}
//
//		bool operator<(const Date &another) const {
//			return mm != another.mm ? mm < another.mm :
//				dd != another.dd ? dd < another.dd :
//					Time::operator<(another);
//		}
//
//		Date operator+(const Date &another) const {
//			Date res = *this;
//			res.mi += another.mi;
//			res.hr += another.hr;
//			res.dd += another.dd;
//			res.mm += another.mm;
//			if (res.mi >= 60) {
//				res.mi -= 60;
//				++res.hr;
//			}
//			if (res.hr >= 24) {
//				res.hr -= 24;
//				++res.dd;
//			}
//			if (res.dd > monthDays[res.mm]) {
//				res.dd -= monthDays[res.mm];
//				++res.mm;
//			}
//			return res;
//		}
//
//		Date& operator+=(const Date &another) {
//			*this = *this + another;
//			return *this;
//		}
//
//		Date operator+(const int &minutes) const {
//			return *this + Date(0, minutes/1440, (minutes % 1440) / 60, minutes % 60);
//		}
//
//		Date& operator+=(const int &minutes) {
//			*this = *this + minutes;
//			return *this;
//		}
//
//		Date& operator++() {
//			*this += Date(0, 1, 0, 0);
//			return *this;
//		}
//
//		// mm == 6 or 7
//		[[nodiscard]] int calcMinute() const {
//			if (mm == 6) {
//				return (dd * 24 + hr) * 60 + mi;
//			}
//			else {
//				return ((monthDays[mm - 1] + dd) * 24 + hr) * 60 + mi;
//			}
//		}
//
//		[[nodiscard]] int diffMinute(const Date &b) const {
//			return calcMinute() - b.calcMinute();
//		}
//
//		[[nodiscard]] int cmpDate(const Date &another) const {
//			if (mm == another.mm) {
//				return dd < another.dd ? -1 : dd == another.dd ? 0 : 1;
//			}
//			else {
//				return mm < another.mm ? -1 : 1;
//			}
//		}
//
//		[[nodiscard]] std::string to_string() const override {
//			char tmp[20];
//			sprintf(tmp, "%02d-%02d", mm, dd);
////			std::stringstream ss;
////			ss << std::setw(2) << std::setfill('0') << std::right;
////			ss << mm << '-' << dd;
//			return std::string(tmp) + ' ' + Time::to_string();
//		}
//
//		friend std::ostream& operator<<(std::ostream &os, const Date &one) {
//			os << one.to_string();
//			return os;
//		}
//	};
	
	constexpr int monthDays[] { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	constexpr int totalDays[] { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	struct Date {
		int timeCnt = 0;
		
		Date () = default;
		
		explicit Date(int timeCnt) : timeCnt(timeCnt) { }
		
		Date (int h, int m) {
			timeCnt = h * 60 + m;
		}
		
		Date (int mm, int dd, int h, int m) {
			timeCnt = (totalDays[mm - 1] + dd) * 1440 + h * 60 + m;
		}
		
		bool operator<(const Date &another) const {
			return timeCnt < another.timeCnt;
		}
		
		Date operator+(const Date &another) const {
			return Date(timeCnt + another.timeCnt);
		}
		
		Date& operator+=(const Date &another) {
			*this = *this + another;
			return *this;
		}
		
		Date operator+(const int &minutes) const {
			return Date(timeCnt + minutes);
		}
		
		Date& operator+=(const int &minutes) {
			timeCnt += minutes;
			return *this;
		}
		
		Date operator-(const int &minutes) const {
			return Date(timeCnt - minutes);
		}
		
		Date& operator-=(const int &minutes) {
			timeCnt -= minutes;
			return *this;
		}
		
		Date& operator++() {
			timeCnt += 1440;
			return *this;
		}
		
		[[nodiscard]] int calcMinute() const {
			return timeCnt;
		}
		
		[[nodiscard]] int diffMinute(const Date &b) const {
			return timeCnt - b.timeCnt;
		}
		
		pair<int, int> getMMDD() const {
			int month = 1, tmp = timeCnt / 1440;
			while (tmp > monthDays[month]) {
				tmp -= monthDays[month];
				++month;
			}
			return make_pair(month, tmp);
		}
		
		pair<int, int> getHHMM() const {
			return make_pair((timeCnt % 1440) / 60, timeCnt % 60);
		}
		
		Date getDate() const {
			return Date(timeCnt / 1440 * 1440);
		}
		
		
		/**
		 * returns 1 if date of *this is less then another
		 * @param another
		 * @return
		 */
		[[nodiscard]] int cmpDate(const Date &another) const {
			if (timeCnt / 1440 < another.timeCnt / 1440) return -1;
			else if (timeCnt / 1440 == another.timeCnt / 1440) return 0;
			else return 1;
		}
		
		[[nodiscard]] std::string to_string() const {
			auto mmdd = getMMDD(), hhmm = getHHMM();
			char res[25];
			sprintf(res, "%02d-%02d %02d:%02d", mmdd.first, mmdd.second, hhmm.first, hhmm.second);
			return std::string(res);
		}
		
		friend std::ostream& operator<<(std::ostream &os, const Date &one) {
			os << one.to_string();
			return os;
		}
	};
}


#endif //TICKETSYSTEM_2021_TIME_HPP
