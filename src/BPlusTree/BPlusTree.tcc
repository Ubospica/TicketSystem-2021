
//author: ubospica
//source: http://www.mathcs.emory.edu/~cheung/Courses/554/Syllabus/3-index/B-tree=delete.html



#include "BPlusTree.hpp"

#include <iostream>
#include <vector>
#include "Tools/Utility.hpp"

namespace Ticket {
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	struct BPlusTree<Key, Value, NO_VALUE_FLAG, M>::Node{
		int cnt = 0;
		bool isRoot = false, isLeaf = false;
		int prev = -1, next = -1;
		Key vKey[M] {};
		int son[M + 1] {};
		friend std::ostream& operator<<(std::ostream &os, const Node &nd) {
			print(nd);
			return os;
		}
	};
	
	
	
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
	}
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	BPlusTree<Key, Value, NO_VALUE_FLAG, M>::~BPlusTree() {
		treeDt.write(Pos::POS_ROOT, root);
		treeDt.write(Pos::POS_SIZE, size);
		treeDt.write(Pos::POS_HEIGHT, height);
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
	
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::newNodePos() {
		treeDt.movePos(FileIO::END);
		return treeDt.tellPos();
	}
	
	//returns node in the index
	//(-1, 0) if not found
	//(pos, p) if found; the val should be node[pos].vSon[p]
	//if many: returns the last one
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
	
	//leaf:pl no-leaf:pl + 1
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	template <typename Comp>
	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::findStack (int pos,
			const Key &vKey, StackType &sta) {
		Node cur;
		treeDt.read(pos, cur);
		if (cur.cnt == 0) {
			return -1;
		}
		int pl = std::upper_bound(cur.vKey, cur.vKey + cur.cnt, vKey, Comp()) - cur.vKey;
		sta.push_back(make_pair(make_pair(pos, pl - 1), cur));
		if (!cur.isLeaf) { //simple node
			return findStack<Comp>(cur.son[pl], vKey, sta);
		}
		else {
			if (pl == 0 || Comp()(cur.vKey[pl - 1], vKey)) {
				return -1;
			}
			else {
				return 1;
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
				else {//fake erase
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
					
					int newP = newNodePos();
					treeDt.write(newP, newNd);
					
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
						root = newNodePos();
						treeDt.write(root, newRoot);
						treeDt.write(pos, cur);
						++height;
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
					Node newNd {M - M / 2 - 1, 0, 0, pos, cur.next};
					//1-M 1-m/2, m/2+2-m
					//vkey:0 - m/2-1, m/2, m/2+1 - m-1
					//son: 0, m/2; m/2+1-m
					std::memcpy(newNd.vKey, cur.vKey + M / 2 + 1, newNd.cnt * sizeof(Key));
					std::memcpy(newNd.son, cur.son + M / 2 + 1, (newNd.cnt + 1) * sizeof(int));
					cur.cnt = M / 2;
					
					int newP = newNodePos();
					treeDt.write(newP, newNd);
					
					if (cur.next != -1) {
						Node nextNd;
						treeDt.read(cur.next, nextNd);
						nextNd.prev = newP;
						treeDt.write(cur.next, nextNd);
					}
					cur.next = newP;
					
					if (!cur.isRoot) {
						treeDt.write(pos, cur);
						vKey = cur.vKey[cur.cnt];
						vSon = newP;
					} else {
						cur.isRoot = 0;
						treeDt.write(pos, cur);
						Node newRoot{1, 1, 0, -1, -1, {cur.vKey[cur.cnt]}, {pos, newP}}; //
						root = newNodePos();
						treeDt.write(root, newRoot);
						++height;
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
//			treeDt.write(Pos::POS_SIZE, size);
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
//			treeDt.write(Pos::POS_SIZE, size);
			return pValue;
		}
		else {
			return -1;
		}
	}
	
//
//
//	// an idiotic & easily-implemented version
//	// cuz i cannot erase the data (RValve) in "Data.dat"
//	// so erasing index only is useless
//	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
//	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::erase(int pos, const Key &vKey) {
//		Node cur;
//		treeDt.read(pos, cur);
//		if (cur.cnt == 0) {
//			return -1;
//		}
//		int pl = std::upper_bound(cur.vKey, cur.vKey + cur.cnt, vKey) - cur.vKey;
//		if (!cur.isLeaf) { //simple node
//			return erase(cur.son[pl], vKey);
//		}
//		else {
//			if (pl == 0 || cur.vKey[pl - 1] != vKey || cur.son[pl - 1] == -1) {
//				return -1;
//			}
//			else {
//				cur.son[pl - 1] = -1;
//				treeDt.write(pos, cur);
//				return 1;
//			}
//		}
//	}
//
//	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
//	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::erase(const Key &vl) {
//		if (erase(root, vl) == 1) {
//			--size;
//			return 1;
//		}
//		else {
//			return -1;
//		}
//	}
//
	//vKey:
	//erase: vKey[pl], son[pl + 1]
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::eraseInternal(int posInStk, StackType &sta, int pl) {
		auto &pos = sta[posInStk].first.first;
		auto &node = sta[posInStk].second;
		//vkey: 0 - node.cnt - 1
		//son: 0 - node.cnt
		for (int i = pl; i < node.cnt - 1; ++i) {
			node.vKey[i] = node.vKey[i + 1];
			node.son[i + 1] = node.son[i + 2];
		}
		--node.cnt;
		if (node.cnt >= M / 2 - 1 || node.isRoot) {
			treeDt.write(pos, node);
			return;
		}
		else {
			auto &faPos = sta[posInStk - 1].first.first;
			auto &faPl = sta[posInStk - 1].first.second;
			auto &fa = sta[posInStk - 1].second;
			if (node.next != -1) {
				Node next;
				treeDt.read(node.next, next);
				if (next.cnt > M / 2 - 1) {
					node.vKey[node.cnt] = fa.vKey[faPl + 1];
					node.son[node.cnt + 1] = next.son[0];
					fa.vKey[faPl + 1] = next.vKey[0];
					for (int i = 0; i < next.cnt - 1; ++i) {
						next.vKey[i] = next.vKey[i + 1];
						next.son[i] = next.son[i + 1];
					}
					next.son[next.cnt - 1] = next.son[next.cnt];
					++node.cnt;
					--next.cnt;
					
					treeDt.write(pos, node);
					treeDt.write(node.next, next);
					treeDt.write(faPos, fa);
				}
				else { //merge
					node.vKey[node.cnt] = fa.vKey[faPl + 1];
					node.son[node.cnt + 1] = next.son[0];
					++node.cnt;
					for (int i = 0; i < next.cnt; ++i) {
						node.vKey[node.cnt] = next.vKey[i];
						node.son[node.cnt + 1] = next.son[i + 1];
						++node.cnt;
					}
					
					if (next.next != -1) {
						Node nnext;
						treeDt.read(next.next, nnext);
						next.prev = pos;
						treeDt.write(next.next, nnext);
					}
					node.next = next.next;
					
					if (fa.cnt == 1) {
						root = pos;
						--height;
						node.isRoot = 1;
						treeDt.write(pos, node);
					}
					else {
						treeDt.write(pos, node);
						eraseInternal(posInStk - 1, sta, sta[posInStk - 1].first.second + 1);
					}
				}
			}//update prev&next!!!
			else if (node.prev != -1) {
				Node prev;
				treeDt.read(node.prev, prev);
				if (prev.cnt > M / 2 - 1) {
					for (int i = node.cnt - 1; i >= 0; --i) {
						node.vKey[i + 1] = node.vKey[i];
						node.son[i + 2] = node.son[i + 1];
					}
					node.son[1] = node.son[0];// !!
					node.vKey[0] = fa.vKey[faPl];
					node.son[0] = prev.son[prev.cnt];
					fa.vKey[faPl] = prev.vKey[prev.cnt - 1];
					++node.cnt;
					--prev.cnt;
					
					treeDt.write(pos, node);
					treeDt.write(node.prev, prev);
					treeDt.write(faPos, fa);
				}
				else {
					prev.vKey[prev.cnt] = fa.vKey[faPl];
					prev.son[prev.cnt + 1] = node.son[0];
					++prev.cnt;
					for (int i = 0; i < node.cnt; ++i) {
						prev.vKey[prev.cnt] = node.vKey[i];
						prev.son[prev.cnt + 1] = node.son[i + 1];
						++prev.cnt;
					}
					
					if (node.next != -1) {
						Node next;
						treeDt.read(node.next, next);
						next.prev = node.prev;
						treeDt.write(node.next, next);
					}
					prev.next = node.next;
					
					
					if (fa.cnt == 1) {
						root = node.prev;
						--height;
						prev.isRoot = 1;
						treeDt.write(node.prev, prev);
					}
					else {
						treeDt.write(node.prev, prev);
						eraseInternal(posInStk - 1, sta, sta[posInStk - 1].first.second);
					}
				}
			}
		}
	}
	
	

	//x return -1 when dont need to erase node in parent node
	//x otherwise return the index to be removed
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::eraseLeaf(int posInStk, StackType &sta) {
		auto &pos = sta[posInStk].first.first;
		auto &pl = sta[posInStk].first.second;
		auto &node = sta[posInStk].second;
		for (int i = pl; i < node.cnt - 1; ++i) {
			node.vKey[i] = node.vKey[i + 1];
			node.son[i] = node.son[i + 1];
		}
		--node.cnt;
		if (node.cnt >= M / 2 - 1 || node.isRoot) {//at least half full
			treeDt.write(pos, node);
			return;
		}
		else {
			if (node.next != -1) {
				Node next;
				treeDt.read(node.next, next);
				if (next.cnt > M / 2 - 1) {
					node.vKey[node.cnt] = next.vKey[0];
					node.son[node.cnt] = next.son[0];
					++node.cnt;
					for (int i = 0; i < next.cnt - 1; ++i) {
						next.vKey[i] = next.vKey[i + 1];
						next.son[i] = next.son[i + 1];
					}
					--next.cnt;
					Node &fa = sta[posInStk - 1].second;
					fa.vKey[sta[posInStk - 1].first.second + 1] = next.vKey[0];
					
					treeDt.write(sta[posInStk - 1].first.first, fa);
					treeDt.write(node.next, next);
					treeDt.write(pos, node);
					return;
				}
				else { // merge
					for (int i = 0; i < next.cnt; ++i) {
						node.vKey[node.cnt] = next.vKey[i];
						node.son[node.cnt] = next.son[i];
						++node.cnt;
					}
					
					//delete next
					
					if (next.next != -1) {
						Node nnext;
						treeDt.read(next.next, nnext);
						next.prev = pos;
						treeDt.write(next.next, nnext);
					}
					node.next = next.next;
					
					if (sta[posInStk - 1].second.cnt == 1) { //new root
						//erase root
						root = pos;
						--height;
						node.isRoot = 1;
						treeDt.write(pos, node);
						return;
					}
					else {
						treeDt.write(pos, node);
						eraseInternal(posInStk - 1, sta, sta[posInStk - 1].first.second + 1);
					}
				}
			}
			else if (node.prev != -1) {
				Node prev;
				treeDt.read(node.prev, prev);
				if (prev.cnt > M / 2 - 1) {
					for (int i = node.cnt - 1; i >= 0; --i) {
						node.vKey[i + 1] = node.vKey[i];
						node.son[i + 1] = node.son[i];
					}
					node.vKey[0] = prev.vKey[prev.cnt - 1];
					node.son[0] = prev.son[prev.cnt - 1];
					++node.cnt;
					--prev.cnt;
					Node &fa = sta[posInStk - 1].second;
					fa.vKey[sta[posInStk - 1].first.second] = node.vKey[0];
					
					treeDt.write(sta[posInStk - 1].first.first, fa);
					treeDt.write(node.prev, prev);
					treeDt.write(pos, node);
					return;
				}
				else { // merge
					for (int i = 0; i < node.cnt; ++i) {
						prev.vKey[prev.cnt] = node.vKey[i];
						prev.son[prev.cnt] = node.son[i];
						++prev.cnt;
					}
					
					if (node.next != -1) {
						Node next;
						treeDt.read(node.next, next);
						next.prev = node.prev;
						treeDt.write(node.next, next);
					}
					prev.next = node.next;
					
					//delete prev
					if (sta[posInStk - 1].second.cnt == 1) {
						root = node.prev;
						--height;
						prev.isRoot = 1;
						treeDt.write(node.prev, prev);
						return;
					}
					else {
						treeDt.write(node.prev, prev);
						eraseInternal(posInStk - 1, sta, sta[posInStk - 1].first.second);
					}
				}
			}
		}
	}


	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::eraseIndex(const Key &vl) {
		StackType sta;
		if (findStack(root, vl, sta) == -1) {
			return -1;
		}
		//found
		--size;
		eraseLeaf(sta.size() - 1, sta);
		return 1;
	}
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	int BPlusTree<Key, Value, NO_VALUE_FLAG, M>::erase(const Key &vl) {
		if (eraseIndex(vl) == 1) {
			//erase value
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
		Comp cmp;
		std::vector<int> res;
		auto pos0 = findIndex<Comp>(root, val);
		if (pos0.first == -1) {
			return res;
		}
		
		Node cur;
		treeDt.read(pos0.first, cur);
		
		while (true) { //loop iterator: pos0
			if (cur.son[pos0.second] != -1) {
				if (cmp(cur.vKey[pos0.second], val)) {
					break;
				}
				else {
					res.push_back(cur.son[pos0.second]);
				}
			}
			--pos0.second;
			if (pos0.second == -1) {
				if (cur.prev == -1) {
					break;
				}
				pos0.first = cur.prev;
				treeDt.read(pos0.first, cur);
				pos0.second = cur.cnt - 1;
			}
		}
		std::reverse(res.begin(), res.end());
		return res;
	}
	
	template <typename Key, typename Value, int NO_VALUE_FLAG, size_t M>
	void BPlusTree<Key, Value, NO_VALUE_FLAG, M>::clear() {
		treeDt.clear();
		init();
		if (!NO_VALUE_FLAG) {
			valueDt.clear();
		}
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