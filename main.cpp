#include <iostream>
#include <fstream>

#include "./memory.hpp"

using str = std::string;

static constexpr size_t KB = 1'024;
static constexpr size_t MB = 1'048'576;
static constexpr size_t GB = 1'073'741'824;

static constexpr int minArgs = 1;

namespace W6502 {
	struct StatusRegister {
	    Byte C : 1;
        Byte Z : 1;
        Byte I : 1;
        Byte D : 1;
	    Byte B : 1;
	    Byte V : 1;
	    Byte N : 1;
    };

	struct CPU {
        Word IP, SP;

	    Byte ACC, X, Y;
	    StatusRegister SR;
    };

}

namespace MegaCPU {
}

size_t getFileSize(const str& path) {
	std::ofstream file(path, std::ios::app);
	return file.tellp();
}

template <typename T>
inline void bindump(std::ofstream& file, T value) {
	void* bindata = &value;
	file.write((char*)bindata, sizeof(value));
	return;
}

template <typename T>
inline void binup(std::ifstream& file, T& value) {
	file.read((char*)&value, sizeof(value));
	return;
}

int main(int argc, char** argv) {
	if(argc <= minArgs) {
		std::cout << "Missing inputs!\n Usage: <path> <romfile>\n";
		exit(1);
	}

	str rompath = argv[1];

	Memory<KB*64> memory;
	{
		std::ifstream file(argv[1], std::ios::binary);
		if(!file.is_open()) {
			std::cout << "It is doom! Failed to open file! Error code 2\n";
			exit(2);
		}
		const size_t size = getFileSize(rompath);
		if(size != sizeof(memory)) {
			std::cout << "It is doom! File size is not equal to memory size! Error code 3\n";
			std::cout << "Would you like to nuke rom (y/n)? ";
			char c;
			std::cin >> c;
			if(c == 'y' || c == 'Y') {
				std::ofstream nuke(argv[1], std::ios::binary);
				std::cout << "Nuking rom ☢️ 💣\n";
				bindump(nuke, memory);
			    nuke << std::flush;
				nuke.close();
				std::cout << "rom nuked ☠️ ☢️ 💥\n";
			} else if(c == 'n' || c == 'N') {
				exit(3);
			}
		} else {
			file.seekg(0, std::ios::beg);
			binup(file, memory);
		}
	}

	W6502::CPU cpu6502;

	Register a;
	a.bl = 10;
	std::cout << (int)a.bl << '\n';
}
