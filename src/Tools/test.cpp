//
// Created by Ubospica on 2021/4/23.
//


#include "Exception.hpp"
#include "String.hpp"
#include "Time.hpp"
#include "FileIO.hpp"

#include <iostream>
#include <string>


int main() {
	using namespace Ticket;
	using namespace std;
//	FileIO a("1223");
	int v = 444, w;
//	a.write(FileIO::END, v);
//	a.close();
	FileIO b("1223");
	b.read(FileIO::BEG, w);
	cout<<w<<'\n';
}


