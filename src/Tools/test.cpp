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
	Date a(6,7,8,10);
	for (int i = 0; i < 40; ++i) ++a;
	cout << a << '\n';
}


