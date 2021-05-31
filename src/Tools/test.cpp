//
// Created by Ubospica on 2021/4/23.
//


#include "Exception.hpp"
#include "String.hpp"
#include "Time.hpp"
#include "FileIO.hpp"
#include "Vector.hpp"
#include "Algorithm.hpp"


int main() {
	using namespace Ticket;
	using std::cin;
	using std::cout;
	vector<int> v;
	for (int i = 1; i <= 100; ++i) {
		v.push_back(std::rand());
	}
	Algorithm::sort(v.begin(), v.end());
	for (auto i : v) {
		cout << i << '\n';
	}
}


