//
// Created by Ubospica on 2021/2/12.
//


#include "BPlusTree.hpp"
#include "Tools/String.hpp"

#include <iostream>
#include <string>

//using namespace Ticket;
using String = Ticket::String<>;
using Ticket::BPlusTree;

int cmpLen = 1;
struct Comp{
	bool operator() (const String &a, const String &b) {
		for (int i = 0; i < cmpLen; ++i) {
			if (a[i] < b[i]) return true;
			else if (a[i] > b[i] || (a[i] == 0 && b[i] == 0)) return false;
		}
		return false;
	}
};
struct Comp1{
	bool operator() (const String &a, const String &b) {
		return false;
	}
};

int main() {
//	freopen("Test2.in", "r", stdin);
	using namespace std;
	BPlusTree<String, int, 0, 3> bps("ww");
	int o, v;
	String s;
	cout<<boolalpha;
	while (cin>>o) {
		if (o == 1) {
			cin>>s;
			cin>>v;
			cout << bps.insert(s, v) << '\n';
//			bps.print();
		}
		else if (o == 2){
			cin>>s;
			int tmp = bps.find(s);
//			cerr << "find pos = " << tmp << '\n';
			if (tmp >= 0) cout << bps.getVal(tmp) << '\n';
			else cout << "Not found\n";
		}
		else if (o == 3) {
			cin>>s;
			cout << bps.erase(s) << '\n';
//			bps.print();
		}
		else if (o == 4) {
			cin>>s>>v;
			cmpLen = v;
			auto rt = bps.route(s);
			for (const auto &i : rt) {
				cout << bps.getVal(i) << ' ';
//				auto tmp = get<0>(i);
//				cout << get<0>(i) << ' ';
			}
			cout << '\n';
			rt = bps.route<Comp>(s);
			for (const auto &i : rt) {
				cout << bps.getVal(i) << ' ';
			}
			cout << '\n';
			rt = bps.route<Comp1>(s);
			for (const auto &i : rt) {
				cout << bps.getVal(i) << ' ';
			}
			cout << '\n';
			cout.flush();
		}
		else if (o == 5) {
			bps.clear();
		}
		else if (o == 6) {
			return 0;
		}
	}
}