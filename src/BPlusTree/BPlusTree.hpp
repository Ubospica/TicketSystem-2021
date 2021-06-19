//
// Created by Ubospica on 2021/2/12.
//

#ifndef BOOKSTORE_2021_BPLUSTREE_HPP
#define BOOKSTORE_2021_BPLUSTREE_HPP


#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>

#include "Tools/Exception.hpp"
#include "Tools/FileIO.hpp"
#include "Tools/Utility.hpp"
#include "Tools/String.hpp"

namespace Ticket{
	
	/**
	 * @brief 数据库 <br>
	 * 数据保存在两个文件中：treeDt（保存树节点以及指向Value文件中位置的指针），valueDt（保存所有Value）
	 *
	 * @note Index only mode. Usage:
	 * @code
	 * BPlusTree<int,int,1> bpt;
	 * //usable func:
	 * insertIndex(Key, int)
	 * erase(Key)
	 * find(Key)
	 * print()
	 * @endcode
	 *
	 * @tparam Key 键
	 * @tparam Value 值
	 * @tparam NO_VALUE_FLAG 如果是1表示Index only mode
	 * @tparam M 块的大小，默认100
	 */
	template <typename Key, typename Value, int NO_VALUE_FLAG = 0, size_t M = 200>
	class BPlusTree {
	protected:
		struct Node;
	public:
		/**
		 * 构造函数
		 * @param name 数据库名字，一个字符串，决定保存的文件名
		 */
		explicit BPlusTree(const std::string& name);
		~BPlusTree();
		/**
		 * 查找某个值
		 * @tparam Comp 请保持默认即可
		 * @param vl 值
		 * @return 不存在返回-1，否则返回valueDt文件中value的位置
		 * @par Example
		 * @code
		 * int pos = find(make_pair(1,2);
		 * @endcode
		 */
		template <typename Comp = std::less<Key> > int find(const Key &vl);
		/**
		 * 返回valueDt文件中pValue位置的值，和find()配合使用
		 * @param pValue
		 * @return 值
		 */
		Value getVal(int pValue);
		/**
		 * 修改valueDt文件中pValue位置的值为newVal
		 * @param pValue
		 * @param newVal
		 */
		void modifyVal(int pValue, const Value &newVal);
		/**
		 * 向数据库中插入一个新值，同时插入B+树中和valueDt中
		 * @param vl key
		 * @param vr value
		 * @return 如果失败返回-1，否则返回valueDt中插入vr的位置
		 */
		int insert(const Key &vl, const Value &vr);
		/**
		 * 向数据库中插入一个新值，仅仅修改B+树，并使得叶子节点中指向value文件的位置为Pos <br>
		 * 这个是为了实现一个需求：一个valueDt对应多种索引。此时仅在第一棵B+树中插入，其余树中保存指向第一棵树中valueDt的索引即可
		 * @param vl key
		 * @param pos 位置
		 * @return 如果失败返回-1，否则返回pos
		 */
		int insertIndex(const Key &vl, int pos);
		/**
		 * 删除某个值
		 * @param vl 删除的值
		 * @return 删除成功返回1，否则（如vl不存在）返回-1； （-1是为了和其他函数保持一致）
		 */
		int erase(const Key &vl);
		int eraseIndex(const Key &vl);
		/**
		 * 模糊查找
		 *
		 * 传入Comp运算符（Comp需要是比<弱的比较函数），然后返回所有在Comp意义下等于val的值
		 * @tparam Comp 重载小于号。Comp需要是一个仿函数类
		 * @param val
		 * @return 一个vector，保存所有模糊查找的结果
		 * @par Example
		 * @code
		 * struct Comp {
		 * 	bool operator()(pair<int,int> a, pair<int,int> b) {
		 * 		return a.first<b.first;
		 * 	}
		 * }
		 * auto res = bpt.route<Comp>(make_pair(1,0)); //查找所有第一维是1的值
		 * @endcode
		 */
		template <typename Comp = std::less<Key> > std::vector<int> route(const Key &val);
		
		/**
		 * @return size of keys in bpt
		 */
		int getSize() const {
			return size;
		}
		bool isEmpty() const {
			return size == 0;
		}
		
		void clear();
		
		//debug
		static void print (const Node &p);
		void print (int pos);
		/**
		 * 打印整棵树
		 */
		void print ();
	
	protected:
		FileIO treeDt, valueDt;
		int root, height, size;

//		template <typename T> inline void read(int pos, T &cur, FileIO &fs);
//		template <typename T> inline void read(int pos, T &cur);
//
//		template <typename T> inline void write(int pos, const T &cur, FileIO &fs);
//		template <typename T> inline void write(int pos, const T &cur);
		
		int newNodePos();
		void init();
		
		
		template <typename Comp = std::less<Key> > int find(int pos, const Key &vKey);
		template <typename Comp = std::less<Key> > pair<int, int> findIndex(int pos, const Key &vKey);
		
		using StackType = std::vector<pair<pair<int, int>, Node>>;
		template <typename Comp = std::less<Key>> int findStack (int pos, const Key &vKey, StackType &sta);
		
		int insert(int pos, Key &vKey, int &vSon);
		
		void eraseLeaf(int posInStk, StackType &sta);
		void eraseInternal(int posInStk, StackType &sta, int pl);
		
		
		struct Pos {
//			static const int END = -1, CUR = -2;
			static const int POS_ROOT = 0, POS_SIZE = sizeof(root), POS_HEIGHT = sizeof(int) * 2;
		};
	};
	
//	template <typename Key, typename Value, int NO_VALUE_FLAG = 0, size_t M = 200>
//	class BPlusTree : BPlusTree<Key, Value, NO_VALUE_FLAG, M> {
//	public:
//		using BPlusTree::BPlusTree(const std::string&);
//	};
//
//
//	template <String, typename Value, int NO_VALUE_FLAG = 0, size_t M = 200>
//	class BPlusTree : BPlusTree<size_t, Value, NO_VALUE_FLAG, M> {
//	public:
//		using BPlusTree::BPlusTree(const std::string&);
//		template <typename Comp = std::less<Key> > int find(const String &vl) {
//			return
//		}
//	};
	
} // namespace Bookstore

//template implementation
#include "BPlusTree.tcc"

#endif //BOOKSTORE_2021_BPLUSTREE_HPP
