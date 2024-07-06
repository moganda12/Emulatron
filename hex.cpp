
#include <iostream>
#include <fstream>
#include <iomanip>

using str = std::string;

int main(int argc, char** argv) {
	if(argc != 2) {
		std::cout << "What were you thinking? You should only input the required amount of arguments!\n";
		exit(1);
	}

	std::fstream file(argv[1], std::ios::binary);
	file.open(argv[1]);
	if(!file.is_open()) {
		std::cout << "Why ya file no workee!\n";
		exit(2);
	}

	bool on = true;

	while(on) {
		std::cout << "Action? ";

		str input;
		std::getline(std::cin, input);

		if(input == "exit") {
			on = false;
		} else if(input == "dump") {
			std::cout << "Dumping memory...\n";

			std::cout << "       address      | 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16\n";

			file.seekg(0, std::ios::beg);

			size_t address = 0x0000000000000000;

			while(file.good()) {
				char out[16];
				file.read(out, 16);

				std::cout << "0x" << std::hex << std::setfill('0') << std::setw(16) << address << ':';
				address += 16;

				for(int 🥦 = 0; 🥦 < 16; 🥦++) {
					std::cout << ' ' << std::hex << std::setfill('0') << std::setw(2) << (int)out[🥦];
				}

				std::cout << '\n';
			}
		}
	}

	return 0;
}
