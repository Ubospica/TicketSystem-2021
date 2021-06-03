//
// Created by Ubospica on 2021/5/29.
//

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
	FileIO tmp("122.dat");
	for (int i = 0; i < 100; ++i)
		tmp.write(FileIO::END, i);
	for (int i = 0, cnt = 0; cnt < 100; i = (i + 9)%100, ++cnt) {
		int a;
		int i1 = (i + 2) % 100;
		cout << "i = " << i << " " << i1 << '\n';
		for (int j = 0; j < 5; ++j) {
			tmp.read(i * 4, a);
			cout << a << ' ';
			tmp.read(i1 * 4, a);
			cout << a << '\n';
		}
		tmp.write(i * 4, i + 1);
		tmp.write(i1 * 4, i1 + 3);
		
		for (int j = 0; j < 5; ++j) {
			tmp.read(i * 4, a);
			cout << a << ' ';
			tmp.read(i1 * 4, a);
			cout << a << '\n';
		}
	}
	for (int i = 0; i < 100; ++i) {
		int a;
		tmp.read(i*4, a);
		cout << i << ' ' << a << '\n';
	}
}


