//
// Created by Ubospica on 2021/5/19.
//

#ifndef TICKETSYSTEM_2021_LIST_HPP
#define TICKETSYSTEM_2021_LIST_HPP

#include <iostream>
template<typename T>
class List {
public:
	struct Node {
		T value = {};
		Node *prev = nullptr, *next = nullptr;
	};
	Node *head = nullptr, *end = nullptr;
	List() {
		head = new Node;
		end = new Node;
		head -> next = end;
		end -> prev = head;
	}
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
	
	Node* insert(Node *val, T v) { //insert after val
		Node *newNd = new Node{v};
		newNd -> prev = val;
		newNd -> next = val -> next;
		val -> next -> prev = newNd;
		val -> next = newNd;
		return newNd;
	}
	void erase(Node *val) {
		val -> prev -> next = val -> next;
		val -> next -> prev = val -> prev;
//		val -> prev = val -> next = nullptr;
		delete val;
	}
	Node *insertNode(Node *val, Node *newNd) {
		newNd -> prev = val;
		newNd -> next = val -> next;
		val -> next -> prev = newNd;
		val -> next = newNd;
		return newNd;
	}
	void eraseNoDelete(Node *val) {
		val -> prev -> next = val -> next;
		val -> next -> prev = val -> prev;
	}
	void deallocate(Node *&val) {
		if (val != nullptr) {
			delete val;
			val = nullptr;
		}
	}
	~List() {
		Node *current = head;
		while (current != nullptr) {
			auto tmp = current;
			current = current -> next;
			delete tmp;
		}
		head = end = nullptr;
	}
	
	void print() {
		Node *current = head;
		std::cerr << "head = " << head << " end = " << end << '\n';
		while (current != nullptr) {
			std::cerr << "cur = " << current << " pr = " << current -> prev << " ne = " << current -> next << '\n';
			std::cerr << "val: " << current -> value << '\n';
			current = current -> next;
		}
		std::cerr << '\n';
	}
};


#endif //TICKETSYSTEM_2021_LIST_HPP
