#include <iostream>
#include"Mainclass.hpp"
#include<string>
using namespace std;
int main(){
 //   std::ios::sync_with_stdio(0), std::cin.tie(0), std::cout.tie(0);
//	freopen("a.in", "r", stdin);
//	freopen("a.out", "w", stdout);
//	for (int i = 1; i <= 10; ++i) {
    Backend::Main OP;
//    其中第一个参数为输入流，第二个参数为输出流
    OP.Run(std::cin);
//	}
    return 0;
}

