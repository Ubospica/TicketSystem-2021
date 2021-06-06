//
// Created by Ubospica on 2021/6/6.
//

#include <iostream>
#include <string>

#include "TicketSystem.hpp"
#include "Tools/Exception.hpp"

using namespace Ticket;
int main() {
	std::string str;
	System system;
	while(getline(std::cin, str)) {
		try {
			system.run(str);
		} catch (End) {
			break;
		}
	}
	return 0;
}