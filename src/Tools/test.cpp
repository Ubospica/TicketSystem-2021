//
// Created by Ubospica on 2021/4/23.
//


#include "Exception.hpp"
#include "String.hpp"
#include "Time.hpp"
#include "FileIO.hpp"
#include "Vector.hpp"


int main() {
	using namespace Ticket;
	using std::cin;
	using std::cout;
	Date tmp (1,21,1,3);
	for (int i = 1; i <= 10; ++i) ++tmp;
	cout << tmp << '\n';
}


