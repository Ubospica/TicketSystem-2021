//
// Created by Ubospica on 2021/5/30.
//

#ifndef TICKETSYSTEM_2021_ALGORITHM_HPP
#define TICKETSYSTEM_2021_ALGORITHM_HPP

#include <iostream>
#include <functional>
#include <utility>

namespace Ticket {
	
	
	/**
	 * type traits
	 * @tparam Iter
	 */
	template <typename Iter>
	struct iterator_traits {
		using value_type = typename std::remove_reference<decltype(*(Iter()))>::type; /// type of Iter::operator*()
	};
	
	template <typename Type>
	struct less {
		bool operator() (const Type &a, const Type &b) {
			return a < b;
		}
	};
	
	class Algorithm {
	public:
		template <typename T>
		static void swap(T &lhs, T &rhs) {
			T tmp = std::move(lhs);
			lhs = std::move(rhs);
			rhs = std::move(tmp);
		}
		
	private:
		template <typename T, typename Comp>
		static const T& _median(const T &a, const T &b, const T &c, Comp cmp) {
			if (cmp(a, b)) { // a < b
				if (cmp(b, c)) { // b < c
					return b;
				}//a < b; b >= c;
				else {
					if (cmp(a, c)) return c;
					else return a;
				}
			}
			else {// a >= b
				if (cmp(c, b)) { // c < b <= a
					return b;
				}
				else {//a >= b; b < c
					if (cmp(a, c)) return a; //a < c
					else return c;
				}
			}
		}
		
		template <typename Iter, typename T, typename Comp>
		static Iter _partition(Iter first, Iter last, T pivot, Comp cmp) {
			while (true) {
				while (cmp(*first, pivot)) ++first;
				--last;
				while (cmp(pivot, *last)) --last;
				if (!(first < last)) {
					return first;
				}
				swap(*first, *last);
				++first;
			}
		}
		
		template <typename Iter, typename Comp>
		static void _sort(Iter beg, Iter end, Comp cmp) {
			while (end - beg > 1) {
//				std::cerr << "end - beg = " << end - beg << '\n';
//				if (end - beg == 2) {
//					if (cmp(*(end - 1), *beg)) swap(*beg, *(end - 1));
//					return;
//				}
				Iter cut = _partition(beg, end,
						  _median(*beg, *(beg + (end - beg) / 2), *(end - 1), cmp), cmp);
				if (cut - beg >= end - cut) {
					_sort(cut, end, cmp);
					end = cut;
				}
				else {
					_sort(beg, cut, cmp);
					beg = cut;
				}
			}
		}
	
	public:
		/**
		 * Quick sort
		 * @tparam Iter type of iterator
		 * @tparam Comp type of compare function
		 * @param beg
		 * @param end
		 * @param cmp
		 * @
		 */
		template <typename Iter,
				typename Comp = less<typename iterator_traits<Iter>::value_type>>
		static void sort(Iter beg, Iter end, Comp cmp = Comp()) {
			_sort(beg, end, cmp);
		}
	};
	
}


#endif //TICKETSYSTEM_2021_ALGORITHM_HPP
