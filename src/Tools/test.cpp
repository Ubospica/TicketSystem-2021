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
	String s = "afvsffasd";
	cout << hash(s) << '\n';
}


