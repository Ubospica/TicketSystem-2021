//
// Created by Ubospica on 2021/5/31.
//

#include <iostream>

#include "BPlusTree.hpp"
#include "Tools/String.hpp"

int main() {
	using namespace Ticket;
	BPlusTree<pair<int,int>,String> bpt("123");
	
	bpt.insert(make_pair(1,3), "www");
	std::cout << bpt.getVal(bpt.find(make_pair(1,3)));
}