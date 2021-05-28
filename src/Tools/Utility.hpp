//
// Created by Ubospica on 2021/5/28.
//

#ifndef TICKETSYSTEM_2021_UTILITY_HPP
#define TICKETSYSTEM_2021_UTILITY_HPP

namespace Ticket {
	template<typename T1, typename T2>
	struct Pair {
		T1 first;
		T2 second;
		Pair() : first(), second() {}
		Pair(const T1 &a, const T2 &b) : first(a), second(b) {}
	};
	template<typename T1, typename T2>
	Pair<T1, T2> MakePair(const T1 &a, const T2 &b) {
		return Pair(a, b);
	}
	
	template<typename T1, typename T2>
	bool operator<(const Pair<T1, T2> &lhs, const Pair<T1, T2> &rhs) {
		return lhs.first != rhs.first ? lhs.first < rhs.first : lhs.second < rhs.second;
	}
	
	template<typename T1, typename T2>
	bool operator>(const Pair<T1, T2> &lhs, const Pair<T1, T2> &rhs) {
		return lhs.first != rhs.first ? lhs.first > rhs.first : lhs.second > rhs.second;
	}
	
	template<typename T1, typename T2>
	bool operator==(const Pair<T1, T2> &lhs, const Pair<T1, T2> &rhs) {
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}
	
	template<typename T1, typename T2>
	bool operator!=(const Pair<T1, T2> &lhs, const Pair<T1, T2> &rhs) {
		return !(lhs == rhs);
	}
	
	template<typename T1, typename T2>
	bool operator<=(const Pair<T1, T2> &lhs, const Pair<T1, T2> &rhs) {
		return !(lhs > rhs);
	}
	
	template<typename T1, typename T2>
	bool operator>=(const Pair<T1, T2> &lhs, const Pair<T1, T2> &rhs) {
		return !(lhs < rhs);
	}
}

#endif //TICKETSYSTEM_2021_UTILITY_HPP
