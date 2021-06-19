//
// Created by Ubospica on 2021/5/28.
//

#ifndef TICKETSYSTEM_2021_VECTOR_HPP
#define TICKETSYSTEM_2021_VECTOR_HPP

#include "Exception.hpp"
#include "Algorithm.hpp"

#include <cstddef>
#include <iterator>

namespace Ticket {
	

	/**
	 * a data container like std::vector
	 * store data in a successive memory and support random access.
	 */
	template<typename T>
	class vector {
	private:
		T *arr = nullptr;
		int sz = 0, capacity = 0;
		/**
		 *
		 * a type for actions of the elements of a vector, and you should write
		 *   a class named const_iterator with same interfaces.
		 */
		/**
		 * you can see RandomAccessIterator at CppReference for help.
		 */
//		class const_iterator;
		template <typename Type, typename source_type>
		class iterator_base {
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = Type;
			using pointer = Type*;
			using reference = Type&;
			using iterator_category = std::output_iterator_tag;
		private:
			/**
			 * data members
			 *
			 */
			value_type *pos = nullptr;
			const source_type * source = nullptr;
		public:
			/**
			 * return a new iterator which pointer n-next elements
			 * as well as operator-
			 */
			iterator_base() = default;
			iterator_base(value_type *pos, const source_type * source) : pos(pos), source(source) { }
			
			iterator_base operator+(const int &n) const {
				std::vector<int>::const_iterator a;
				return iterator_base(pos + n, source);
			}
			iterator_base operator-(const int &n) const {
				return iterator_base(pos - n, source);
			}
			// return the distance between two iterators,
			// if these two iterators point to different vectors, throw invaild_iterator.
			int operator-(const iterator_base &rhs) const {
				if (source == nullptr || source != rhs.source) {
					throw invalid_iterator();
				}
				return pos - rhs.pos;
			}
			iterator_base& operator+=(const int &n) {
				pos += n;
				return *this;
			}
			iterator_base& operator-=(const int &n) {
				pos -= n;
				return *this;
			}
			/**
			 * iter++
			 */
			iterator_base operator++(int) {
				auto tmp = *this;
				++pos;
				return tmp;
			}
			/**
			 * ++iter
			 */
			iterator_base& operator++() {
				++pos;
				return *this;
			}
			/**
			 * iter--
			 */
			iterator_base operator--(int) {
				auto tmp = *this;
				--pos;
				return tmp;
			}
			/**
			 * --iter
			 */
			iterator_base& operator--() {
				--pos;
				return *this;
			}
			/**
			 * *it
			 */
			value_type& operator*() const{
				return *pos;
			}
			/**
			 * it -> member
			 */
			value_type* operator->() const{
				return pos;
			}
			/**
			 * a operator to check whether two iterators are same (pointing to the same memory address).
			 */
			template <typename another_value_type, typename another_source_type>
			bool operator==(const iterator_base<another_value_type, another_source_type> &rhs) const {
				return pos == rhs.pos;
			}
//			bool operator==(const const_iterator &rhs) const {
//				return pos == rhs.pos;
//			}
			/**
			 * some other operator for iterator_base.
			 */
			template <typename another_value_type, typename another_source_type>
			bool operator!=(const iterator_base<another_value_type, another_source_type> &rhs) const {
				return pos != rhs.pos;
			}
			template <typename another_value_type, typename another_source_type>
			bool operator<(const iterator_base<another_value_type, another_source_type> &rhs) const {
				return pos < rhs.pos;
			}
			template <typename another_value_type, typename another_source_type>
			bool operator>(const iterator_base<another_value_type, another_source_type> &rhs) const {
				return pos > rhs.pos;
			}
			template <typename another_value_type, typename another_source_type>
			bool operator<=(const iterator_base<another_value_type, another_source_type> &rhs) const {
				return pos <= rhs.pos;
			}
			template <typename another_value_type, typename another_source_type>
			bool operator>=(const iterator_base<another_value_type, another_source_type> &rhs) const {
				return pos <= rhs.pos;
			}
		};
		void destruct() {
			if (arr != nullptr) {
				for (int i = 0; i < sz; ++i) {
					arr[i].~T();
				}
				deallocate(arr);
				arr = nullptr;
				sz = capacity = 0;
			}
		}
		
		T* allocate(size_t len) {
			return static_cast<T*>(operator new(len * sizeof(T)));
		}
		
		void deallocate(T *ptr) {
			operator delete(ptr);
		}
	
	
	public:
		using iterator = iterator_base<T, vector<T>>;
		
		/**
		 *
		 * has same function as iterator, just for a const object.
		 */
		using const_iterator = iterator_base<const T, vector<T>>;
//		class const_iterator {
//
//		};
		/**
		 * Constructs
		 * Default constructor, copy constructor
		 */
		vector() : sz(0), capacity(1) {
			arr = allocate(1);
		}
		
		vector(const vector &other) : sz(other.sz), capacity(other.capacity) {
			arr = allocate(other.capacity);
			for (int i = 0; i < other.sz; ++i) {
				arr[i] = other.arr[i];
			}
		}
		/**
		 * Destructor
		 */
		~vector() {
			destruct();
		}
		/**
		 * Assignment operator
		 */
		vector &operator=(const vector &other) {
			if (this == &other) {
				return *this;
			}
			destruct();
			sz = other.sz, capacity = other.capacity;
			arr = allocate(other.capacity);
			for (int i = 0; i < other.sz; ++i) {
				arr[i] = other[i];
			}
			return *this;
		}
		/**
		 * assigns specified element with bounds checking
		 * throw index_out_of_bound if pos is not in [0, sz)
		 */
		T & at(const size_t &pos) {
			if (pos >= sz) {
				throw index_out_of_bound();
			}
			return arr[pos];
		}
		const T & at(const size_t &pos) const {
			if (pos >= sz) {
				throw index_out_of_bound();
			}
			return arr[pos];
		}
		/**
		 * assigns specified element with bounds checking
		 * throw index_out_of_bound if pos is not in [0, sz)
		 * !!! Pay attentions
		 *   In STL this operator does not check the boundary but I want you to do.
		 */
		T & operator[](const size_t &pos) {
			return at(pos);
		}
		const T & operator[](const size_t &pos) const {
			return at(pos);
		}
		/**
		 * access the first element.
		 * throw container_is_empty if sz == 0
		 */
		const T & front() const {
			if (sz == 0) {
				throw container_is_empty();
			}
			return arr[0];
		}
		/**
		 * access the last element.
		 * throw container_is_empty if sz == 0
		 */
		const T & back() const {
			if (sz == 0) {
				throw container_is_empty();
			}
			return arr[sz - 1];
		}
		/**
		 * returns an iterator to the beginning.
		 */
		iterator begin() {
			return iterator(arr, this);
		}
		const_iterator cbegin() const {
			return const_iterator(arr, this);
		}
		/**
		 * returns an iterator to the end.
		 */
		iterator end() {
			return iterator(arr + sz, this);
		}
		const_iterator cend() const {
			return const_iterator(arr + sz, this);
		}
		/**
		 * checks whether the container is empty
		 */
		bool empty() const {
			return sz == 0;
		}
		/**
		 * returns the number of elements
		 */
		size_t size() const {
			return sz;
		}
		/**
		 * clears the contents
		 */
		void clear() {
			for (int i = 0; i < sz; ++i) {
				arr[i].~T();
			}
			sz = 0;
		}
		/**
		 * inserts value before pos
		 * returns an iterator pointing to the inserted value.
		 */
		iterator insert(iterator pos, const T &value) {
			int dis = pos - begin();
			if (sz == capacity) {
				capacity *= 2;
				T *newVal = allocate(capacity);
				for (int i = 0; i < dis; ++i) {
					new (&newVal[i]) T(std::move(arr[i]));
				}
				for (int i = dis; i < sz; ++i) {
					new (&newVal[i + 1]) T(std::move(arr[i]));
				}
				new (&newVal[dis]) T(value);
				for (int i = 0; i < sz; ++i) {
					arr[i].~T();
				}
				deallocate(arr);
				arr = newVal;
			}
			else {
				for (int i = sz - 1; i >= dis; --i) {
					if (i == sz - 1) new (&arr[i + 1]) T(std::move(arr[i]));
					else arr[i + 1] = arr[i];
				}
				if (dis == sz) new (&arr[dis]) T(value);
				else arr[dis] = value;
			}
			++sz;
			return begin() + dis;
		}
		/**
		 * inserts value at index ind.
		 * after inserting, this->at(ind) == value
		 * returns an iterator pointing to the inserted value.
		 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
		 */
		iterator insert(const size_t &ind, const T &value) {
			if (ind > sz) {
				throw index_out_of_bound();
			}
			return insert(begin() + ind, value);
		}
		/**
		 * removes the element at pos.
		 * return an iterator pointing to the following element.
		 * If the iterator pos refers the last element, the end() iterator is returned.
		 */
		iterator erase(iterator pos) {
			int dis = pos - begin();
			for (int i = dis; i < sz - 1; ++i) {
				arr[i] = std::move(arr[i + 1]);
			}
			arr[sz - 1].~T();
			--sz;
			return begin() + dis;
		}
		/**
		 * removes the element with index ind.
		 * return an iterator pointing to the following element.
		 * throw index_out_of_bound if ind >= size
		 */
		iterator erase(const size_t &ind) {
			if (ind >= sz) {
				throw index_out_of_bound();
			}
			return erase(begin() + ind);
		}
		/**
		 * adds an element to the end.
		 */
		void push_back(const T &value) {
			insert(end(), value);
		}
		/**
		 * remove the last element from the end.
		 * throw container_is_empty if size() == 0
		 */
		void pop_back() {
			if (sz == 0) {
				throw container_is_empty();
			}
			erase(end() - 1);
		}
	};
	
	template <typename Iter>
	void reverse(Iter begin, Iter end) {
		while (begin != end) {
			--end;
			if (begin == end) break;
			swap(*begin, *end);
			++begin;
		}
	}
	
}

#endif //TICKETSYSTEM_2021_VECTOR_HPP
