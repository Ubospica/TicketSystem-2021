//
// Created by Ubospica on 2021/4/23.
//


#include "Exception.hpp"
#include "String.hpp"
#include "Time.hpp"
#include "FileIO.hpp"
#include "Vector.hpp"
#include "Algorithm.hpp"
#include <algorithm>


int main() {
	using namespace Ticket;
	using std::cin;
	using std::cout;
	vector<int> v;
	for (int i = 1; i <= 5; ++i) {
		v.push_back(std::rand()%5);
	}
	std::sort(v.begin(), v.end(), std::greater<int>());
	for (auto i : v) {
		cout << i << '\n';
	}
}


