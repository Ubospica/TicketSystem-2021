//
// Created by Ubospica on 2021/2/12.
//


#include "BPlusTree.hpp"
#include "Tools/String.cpp"

#include <iostream>
#include <string>


struct Comp{
	bool operator() (Ticket::String a, Ticket::String b) {
		return a[0] < b[0];
	}
};
struct Comp1{
	bool operator() (Ticket::String a, Ticket::String b) {
		return false;
	}
};
int main() {
//	freopen("Test2.in", "r", stdin);
	using namespace Ticket;
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
			cin>>s;
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
	}
}