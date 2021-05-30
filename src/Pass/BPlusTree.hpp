//
// Created by Ubospica on 2021/2/12.
//

#ifndef BOOKSTORE_2021_BPLUSTREE_HPP
#define BOOKSTORE_2021_BPLUSTREE_HPP

//#include "Exception.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <tuple>

namespace Ticket{

	template <typename Key, typename Value, size_t M = 100>
	class BPlusTree {
	private:
		struct Node;
	public:
		explicit BPlusTree(const std::string& name);
		~BPlusTree() = default;
		template <typename Comp = std::less<Key> > int find(const Key &vl);
		Value getVal(int pValue);
		void modifyVal(int pValue, const Value &newVal);
		int insert(const Key &vl, const Value &vr);
		int insertIndex(const Key &vl, int pos);
		bool erase(const Key &vl);
//		std::vector<std::tuple<Key, int>> route();
        int Size();
		template <typename Comp = std::less<Key> > std::vector<int> route(const Key &val);
		
		//debug
		void print (const Node &p);
		void print (int pos);
		void print ();
		
	private:
		std::fstream treeDt, valueDt;
		int root, height, size;
		
		template <typename T> inline void read(int pos, T &cur, std::fstream &fs);
		template <typename T> inline void read(int pos, T &cur);
		
		template <typename T> inline void write(int pos, const T &cur, std::fstream &fs);
		template <typename T> inline void write(int pos, const T &cur);
		
		void init();
		template <typename Comp = std::less<Key> > int find(int pos, const Key &vKey);
		template <typename Comp = std::less<Key> > std::tuple<int,int> findIndex(int pos, const Key &vKey);
		int insert(int pos, Key &vKey, int &vSon);
		bool erase(int pos, const Key &vKey);
		
		class Pos {
		public:
			static const int END = -1, CUR = -2;
			static const int POS_ROOT = 0, POS_SIZE = sizeof(root), POS_HEIGHT = sizeof(int) * 2;
		};
	};

} // namespace Bookstore

//template implementation
#include "BPlusTree.tcc"

#endif //BOOKSTORE_2021_BPLUSTREE_HPP
