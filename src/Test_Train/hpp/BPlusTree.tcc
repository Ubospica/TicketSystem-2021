
#include "BPlusTree.hpp"

#include <iostream>
#include <vector>
#include "Utility.hpp"

namespace Ticket {
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	struct BPlusTree<Key, Value, NO_VALUE_FLAG, M>::Node{
		int cnt = 0;
		bool isRoot = false, isLeaf = false;
		int prev = -1, next = -1;
		Key vKey[M] {};
		int son[M + 1] {};
	};
	
	
//	//-2 cur
//	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
//	template <typename T>
//	inline void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::read(int pos, T &cur, FileIO &fs) {
//		if(pos >= 0){
//			fs.seekg(pos);
//		}
//		fs.read(reinterpret_cast<char*>(&cur), sizeof(cur));
//	}
//
//	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
//	template <typename T>
//	inline void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::read(int pos, T &cur) {
//		read(pos, cur, treeDt);
//	}
//
//	//pos: -1 end; -2 cur
//	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
//	template <typename T>
//	inline void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::write(int pos, const T &cur, std::fstream &fs) {
//		if(pos >= 0){
//			fs.seekp(pos);
//		}
//		else if(pos == Pos::END) {
//			fs.seekp(0, std::ios::end);
//		}
//		fs.write(reinterpret_cast<const char*>(&cur), sizeof(cur));
//	}
//
//	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
//	template <typename T>
//	inline void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::write(int pos, const T &cur) {
//		write(pos, cur, treeDt);
//	}
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	BPlusTree<Key, Value, NO_VALUE_FLAG, M>::BPlusTree(const std::string& name) {
		treeDt.open(name + "Index.dat");
		if (treeDt.isFirstOpen()) {
			init();
		}
		else {
			treeDt.read(Pos::POS_ROOT, root);
			treeDt.read(Pos::POS_SIZE, size);
			treeDt.read(Pos::POS_HEIGHT, height);
		}
		
		if (!NO_VALUE_FLAG) {
			valueDt.open(name + "Data.dat");
		}
//		std::string s1 = name + "Index.dat";
//		auto fl = std::ios::in | std::ios::out | std::ios::binary;
//		treeDt.open(s1, fl);
//		if (!treeDt) { //file not exists
//			//trashy cpp grammar! `std::ofstream(s1)` won't compile
//			std::ofstream(static_cast<std::string>(s1));
//			treeDt.open(s1, fl);
//			if (!treeDt) {
//				throw RuntimeError("File system error");
//			}
//			init();
//		}
//		else {
//			read(0, root);
//			read(Pos::POS_SIZE, size);
//			read(Pos::POS_HEIGHT, height);
//		}
//		if (!NO_VALUE_FLAG) {
//			std::string s2 = name + "Data.dat";
//			valueDt.open(s2, fl);
//			if (!valueDt) { //file not exists
//				//trashy cpp grammar! `std::ofstream(s1)` won't compile
//				std::ofstream(static_cast<std::string>(s2));
//				valueDt.open(s2, fl);
//				if (!valueDt) {
//					throw RuntimeError("File system error");
//				}
//			}
//		}
	}
	
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::init() {
		root = sizeof(root) * 3;
		height = size = 0;
		treeDt.write(Pos::POS_ROOT, root);
		treeDt.write(Pos::POS_SIZE, size);
		treeDt.write(Pos::POS_HEIGHT, height);
		Node newRt {0, 1, 1};
		treeDt.write(FileIO::END, newRt);
	}
	
	//returns node in the index
	//(-1, 0) if not found
	//(pos, p) if found; the val should be node[pos].vSon[p]
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	template <typename Comp>
	pair<int,int> BPlusTree<Key, Value, NO_VALUE_FLAG, M>::findIndex(int pos, const Key &vKey) {
		Node cur;
		treeDt.read(pos, cur);
		if (cur.cnt == 0) {
			return make_pair(-1, 0);
		}
		int pl = std::upper_bound(cur.vKey, cur.vKey + cur.cnt, vKey, Comp()) - cur.vKey;
		if (!cur.isLeaf) { //simple node
			return findIndex<Comp>(cur.son[pl], vKey);
		}
		else {
			if (pl == 0 || Comp()(cur.vKey[pl - 1], vKey)) {
				return make_pair(-1, 0);
			}
			else {
				return make_pair(pos, pl - 1);
			}
		}
	}
	
	//-1 if not found
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	template <typename Comp>
	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::find(int pos, const Key &vKey) {
		auto res = findIndex<Comp>(pos, vKey);
		if (res.first == -1) {
			return -1;
		}
		else {
			Node cur;
			treeDt.read(res.first, cur);
			return cur.son[res.second];
		}
	}

	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	template <typename Comp>
	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::find(const Key &vl) {
		return find<Comp>(root, vl);
	}
	
	//pValue should >= 0
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	Value BPlusTree<Key, Value, NO_VALUE_FLAG, M>::getVal(int pValue) {
		Value res;
		valueDt.read(pValue, res);
		return res;
	}
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::modifyVal(int pValue, const Value &newVal) {
		valueDt.write(pValue, newVal);
	}
	
	
	//vSon == -1: don't have to insert nodes
	//return -1 if fails, 1 if succeeded
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::insert(int pos, Key &vKey, int &vSon) {
		Node cur;
		treeDt.read(pos, cur);
		if(cur.isLeaf) {
			int pl = std::lower_bound(cur.vKey, cur.vKey + cur.cnt, vKey) - cur.vKey;
			if (pl < cur.cnt && cur.vKey[pl] == vKey) {
				if (cur.son[pl] >= 0) {
					return -1;
				}
				else {
					cur.son[pl] = vSon;
					treeDt.write(pos, cur);
					vSon = -1;
					return 1;
				}
			}
			else {
				for (int i = cur.cnt - 1; i >= pl; --i) {
					cur.vKey[i + 1] = cur.vKey[i];
					cur.son[i + 1] = cur.son[i];
				}
				cur.vKey[pl] = vKey;
				cur.son[pl] = vSon;
				++cur.cnt;
				if (cur.cnt < M) {
					treeDt.write(pos, cur);
					vSon = -1;
					return 1;
				}
				else { //split
					Node newNd {M - M / 2, 0, 1, pos, cur.next};
					//1-M 1-m/2, m/2+1-m
					//0 - m/2-1, m/2 - m-1
					std::memcpy(newNd.vKey, cur.vKey + M / 2, newNd.cnt * sizeof(Key));
					std::memcpy(newNd.son, cur.son + M / 2, newNd.cnt * sizeof(int));
					cur.cnt = M / 2;
					
					//treeDt.seekp(0, std::ios::end);
					treeDt.movePos(FileIO::END);
					int newP = treeDt.tellPos();
					treeDt.write(FileIO::CUR, newNd);
					
					if (cur.next != -1) {
						Node nextNd;
						treeDt.read(cur.next, nextNd);
						nextNd.prev = newP;
						treeDt.write(cur.next, nextNd);
					}
					cur.next = newP;
					
					if (!cur.isRoot) {
						treeDt.write(pos, cur);
						vKey = newNd.vKey[0], vSon = newP;
					} else {
						cur.isRoot = 0;
						Node newRoot{1, 1, 0, -1, -1, {newNd.vKey[0]}, {pos, newP}};
						//treeDt.seekp(0, std::ios::end);
						//root = treeDt.tellp();
						treeDt.movePos(FileIO::END);
						root = treeDt.tellPos();
						treeDt.write(FileIO::END, newRoot);
						treeDt.write(pos, cur);
						treeDt.write(Pos::POS_ROOT, root); //
						++height;
						treeDt.write(Pos::POS_HEIGHT, height);
						vSon = -1;
					}
					return 1;
				}
			}
		}
		else { // non leaf
			int pl = std::upper_bound(cur.vKey, cur.vKey + cur.cnt, vKey) - cur.vKey;
			if (insert(cur.son[pl], vKey, vSon) == -1) {
				return -1;
			}
			else if (vSon == -1) {
				return 1;
			}
			else {//insert: son[pl+1] = get1 vkey[pl]=get2
				for (int i = cur.cnt - 1; i >= pl; --i) {
					cur.vKey[i + 1] = cur.vKey[i];
					cur.son[i + 2] = cur.son[i + 1];
				}
				cur.vKey[pl] = vKey;
				cur.son[pl + 1] = vSon;
				++cur.cnt;
				if (cur.cnt < M) {
					treeDt.write(pos, cur); //
					vSon = -1;
					return 1;
				}
				else {
					Node newNd {M - M / 2 - 1, 0, 0};
					//1-M 1-m/2, m/2+2-m
					//vkey:0 - m/2-1, m/2, m/2+1 - m-1
					//son: 0, m/2; m/2+1-m
					std::memcpy(newNd.vKey, cur.vKey + M / 2 + 1, newNd.cnt * sizeof(Key));
					std::memcpy(newNd.son, cur.son + M / 2 + 1, (newNd.cnt + 1) * sizeof(int));
					cur.cnt = M / 2;
					if (!cur.isRoot) {
						treeDt.write(pos, cur);
//						treeDt.seekp(0, std::ios::end);
//						int newP = treeDt.tellp();
						treeDt.movePos(FileIO::END);
						int newP = treeDt.tellPos();
						treeDt.write(FileIO::END, newNd);
						vKey = cur.vKey[cur.cnt];
						vSon = newP;
					} else {
						cur.isRoot = 0;
						treeDt.write(pos, cur);
						Node newRoot{1, 1, 0, -1, -1, {cur.vKey[cur.cnt]}, {pos}}; //
//						treeDt.seekp(0, std::ios::end);
//						int newP = treeDt.tellp();
						treeDt.movePos(FileIO::END);
						int newP = treeDt.tellPos();
						newRoot.son[1] = newP;
						treeDt.write(FileIO::END, newNd);
						root = treeDt.tellPos();
						treeDt.write(FileIO::END, newRoot);
						treeDt.write(Pos::POS_ROOT, root); //
						++height;
						treeDt.write(Pos::POS_HEIGHT, height);
						vSon = -1;
					}
					return 1;
				}
			}
		}
	}
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::insertIndex(const Key &vl, int pos) {
		Key vKey(vl);
		int pos1 = pos;
		if (insert(root, vKey, pos1) == 1) {
			++size;
			treeDt.write(Pos::POS_SIZE, size);
			return pos;
		}
		else {
			return -1;
		}
	}
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::insert(const Key &vl, const Value &vr) {
//		valueDt.seekp(0, std::ios::end); //
		valueDt.movePos(FileIO::END);
		Key vKey(vl);
		int pValue = valueDt.tellPos(), vSon = pValue;
		if (insert(root, vKey, vSon) == 1) {
			valueDt.write(FileIO::END, vr);
			++size;
			valueDt.write(Pos::POS_SIZE, size);
			return pValue;
		}
		else {
			return -1;
		}
	}
	

	
	// an idiotic & easily-implemented version
	// cuz i cannot erase the data (RValve) in "Data.dat"
	// so erasing index only is useless
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::erase(int pos, const Key &vKey) {
		Node cur;
		treeDt.read(pos, cur);
		if (cur.cnt == 0) {
			return -1;
		}
		int pl = std::upper_bound(cur.vKey, cur.vKey + cur.cnt, vKey) - cur.vKey;
		if (!cur.isLeaf) { //simple node
			return erase(cur.son[pl], vKey);
		}
		else {
			if (pl == 0 || cur.vKey[pl - 1] != vKey || cur.son[pl - 1] == -1) {
				return -1;
			}
			else {
				cur.son[pl - 1] = -1;
				treeDt.write(pos, cur);
				return 1;
			}
		}
	}
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::erase(const Key &vl) {
		int tmp = erase(root, vl);
//		if (erase(root, vl) == 1) {
		if (tmp == 1) {
			--size;
			treeDt.write(Pos::POS_SIZE, size);
//			treeDt.seekp(sizeof(int), std::ios::beg);
//			write(-2, size);
			return 1;
		}
		else {
			return -1;
		}
	}
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	template <typename Comp>
	auto BPlusTree<Key, Value, NO_VALUE_FLAG, M>::route(const Key &val) ->
		std::vector<int> {
		std::vector<int> res;
		auto pos0 = findIndex<Comp>(root, val);
		if (pos0.first == -1) {
			return res;
		}
		Node cur;
		treeDt.read(pos0.first, cur);
		auto checkAndAdd = [&res, &val] (const Node &cur) {
			for (int i = 0; i < cur.cnt; ++i) {
				bool a = Comp()(cur.vKey[i], val), b = Comp()(val, cur.vKey[i]);
				if (cur.son[i] != -1 && (!Comp()(cur.vKey[i], val)) && (!Comp()(val, cur.vKey[i]))) {
					res.push_back(cur.son[i]);
				}
			}
		};
		checkAndAdd(cur);
		auto tmp = pos0.first;//std::get<0>(pos0);
		while (cur.next != -1 && !Comp()(cur.vKey[cur.cnt], val) && !Comp()(val, cur.vKey[cur.cnt])) {
			tmp = cur.next;
			treeDt.read(tmp, cur);
			checkAndAdd(cur);
		}
		tmp = pos0.first;// = std::get<0>(pos0);
		while (cur.prev != -1 && !Comp()(cur.vKey[0], val) && !Comp()(val, cur.vKey[0])) {
			tmp = cur.prev;
			treeDt.read(tmp, cur);
			checkAndAdd(cur);
		}
		return res;
	}

	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::print (const BPlusTree::Node &p) {
		using namespace std;
		cerr << "cnt=" << p.cnt << " isroot=" << p.isRoot << " isLeaf=" << p.isLeaf
				<< " prev=" << p.prev << " next=" << p.next<<"\n";
		cerr << "vKey, son: ";
		for (int i = 0; i < p.cnt; ++i) {
			cerr << "(" << p.vKey[i] << ", " << p.son[i] << ") ";
		}
		cerr << p.son[p.cnt] << '\n';
//		cerr << "\nson: ";
//		for (int i = 0; i <= p.cnt; ++i) {
//			cerr << p.son[i] << ' ';
//		}
//		cerr << '\n';
	}
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::print (int pos) {
		using namespace std;
		Node cur;
		treeDt.read(pos, cur);
		cerr << "pos = " << pos << " ";
		print(cur);
		if (cur.isLeaf) {
			Value vr;
			cerr << "real value is ";
			for (int i = 0; i < cur.cnt; ++i) {
				if (cur.son[i] >= 0) {
					if (NO_VALUE_FLAG) {
						cerr << cur.son[i] << ' ';
					}
					else {
						valueDt.read(cur.son[i], vr);
						cerr << vr << ' ';
					}
				}
				else {
					cerr << "-1 ";
				}
			}
			cerr << '\n';
		}
		else {
			for (int i = 0; i <= cur.cnt; ++i) {
				print(cur.son[i]);
			}
		}
	}
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::print () {
		std::cerr << "root=" << root << " has value = " << !NO_VALUE_FLAG << "\n";
		print(root);
		std::cerr<<"\n\n";
	}
}