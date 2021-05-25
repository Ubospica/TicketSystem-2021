//
// Created by Ubospica on 2021/5/19.
//

#ifndef TICKETSYSTEM_2021_LIST_HPP
#define TICKETSYSTEM_2021_LIST_HPP

template<typename T>
class list {
private:
	struct Node {
		T value;
		Node *prev = nullptr, *next = nullptr;
	};
	Node *head = nullptr, *end = nullptr;
public:
//	struct iterator {
//		Node *node;
//		iterator() : node(nullptr) {}
//		iterator(Node *oneNode) : node(oneNode) {}
//		iterator(const iterator &another) : node(another.node) {}
//		iterator(iterator &&another) : node (another.node) {}
//
//		bool operator==(const iterator &another) const {
//			return node == another.node;
//		}
//		bool operator!=(const iterator &another) const {
//			return node != another.node;
//		}
//		T& operator*() const {
//			return node -> value;
//		}
//		T* operator->() const {
//			return &(operator*());
//		}
//		iterator& operator++() {
//			if (node != nullptr) {
//				node = node -> next;
//			}
//			return *this;
//		}
//		iterator operator++(int) {
//			auto tmp = *this;
//			if (node != nullptr) {
//				node = node -> next;
//			}
//			return tmp;
//		}
//		iterator& operator--() {
//			if (node != nullptr) {
//				node = node -> prev;
//			}
//			return *this;
//		}
//		iterator operator--(int) {
//			auto tmp = *this;
//			if (node != nullptr) {
//				node = node -> prev;
//			}
//			return tmp;
//		}
//	};
	
	void insert(Node *&val, T v) { //insert after val
		Node *newNd = new Node(v);
		if (val == nullptr) {
			val = newNd;
		}
		else {
			newNd -> prev = val;
			newNd -> next = val -> next;
			val -> next = newNd;
		}
		if (newNd -> next == nullptr) {
			end = newNd;
		}
	}
	
};


#endif //TICKETSYSTEM_2021_LIST_HPP
