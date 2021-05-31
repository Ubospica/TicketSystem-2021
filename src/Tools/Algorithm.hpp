//
// Created by Ubospica on 2021/5/30.
//

#ifndef TICKETSYSTEM_2021_ALGORITHM_HPP
#define TICKETSYSTEM_2021_ALGORITHM_HPP

#include <utility>

namespace Ticket {
	
	
	/**
	 * type traits
	 * @tparam Iter
	 */
	template <typename Iter>
	struct iterator_traits {
		using value_type = decltype(*Iter()); /// type of Iter::operator*()
	};
	
	template <typename Type>
	struct less {
		bool operator() (const Type &a, const Type &b) {
			return a < b;
		}
	};
	
	class Algorithm {
	public:
		template <typename Iter>
		static void iter_swap(const Iter &lhs, const Iter &rhs) {
			using T = typename iterator_traits<Iter>::value_type;
			T tmp = std::move(*lhs);
			*lhs = std::move(*rhs);
			*rhs = std::move(tmp);
		}
		
		template <typename T>
		const T& _median(const T &a, const T &b, const T &c) {
			if (a < b) {
				if (b <= c) {
					return b;
				}//a < b; b > c;
				else {
					if (a < c) return c;
					else return a;
				}
			}
			else {// a >= b
				if (b >= c) {
					return b;
				}
				else {//a >= b; b < c
					if (a < c) return a;
					else return c;
				}
			}
		}
		
		template <typename Iter, typename T>
		void _partition(Iter first, Iter last, T pivot) {
		
		}
		
		template <typename Iter,
				typename Comp = less<typename iterator_traits<Iter>::value_type>>
		static void sort(Iter beg, Iter end, Comp cmp = Comp()) {
		
		}
	};
	
}


#endif //TICKETSYSTEM_2021_ALGORITHM_HPP
