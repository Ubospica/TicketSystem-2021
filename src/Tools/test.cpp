//
// Created by Ubospica on 2021/4/23.
//


#include "Exception.hpp"
#include "String.hpp"
#include "Time.hpp"

#include <iostream>
#include <string>


int main() {
	using namespace Ticket;
	using namespace std;
	String a;
	cin>>a;
	String b;
	cin>>b;
	cout<<(a>b)<<' ';
	swap(a,b);
	cout<<a<<' '<<b<<'\n';
}