
#include <iostream>
#include <fstream>

#include "./memory.hpp"

using str = std::string;

static constexpr size_t KB = 1'024;
static constexpr size_t MB = 1'048'576;
static constexpr size_t GB = 1'073'741'824;
const QWord MEGA = ((QWord*)"MEGACPUS")[0];
const QWord HLT = ((QWord*)"HALTMEGA")[0];

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

	Byte Instruction = 0x00;

	inline void reset(CPU& cpu, Memory memory) {
		cpu.IP = memory.fetch16(0xFFFC);
		cpu.SP = 0x0000;
		cpu.ACC = 0x00;
		cpu.X = 0x00;
		cpu.Y = 0x00;
		cpu.SR.C = 0;
        cpu.SR.Z = 0;
	    cpu.SR.I = 0;
		cpu.SR.D = 0;
		cpu.SR.B = 0;
		cpu.SR.V = 0;
		cpu.SR.N = 0;
	}
}

namespace MegaCPU {
	struct Register {
		Byte content[8];
		Byte& bl = content[0];
		Byte& bh = content[1];
		Word& wl = ((Word*)content)[0];
		Word& wh = ((Word*)content)[1];
		DWord& el = ((DWord*)content)[0];
		DWord& eh = ((DWord*)content)[1];
		QWord& rx = ((QWord*)content)[0];
	};

	struct Accumulator {
		Byte content[16];
		Byte& bl = content[0];
		Byte& bh = content[1];
		Word& wl = ((Word*)content)[0];
		Word& wh = ((Word*)content)[1];
		DWord& el = ((DWord*)content)[0];
		DWord& eh = ((DWord*)content)[1];
		QWord& r = ((QWord*)content)[0];
		QWord& o = ((QWord*)content)[1];
	};

	struct SpecilPointerReg {
		Byte content[8];
		Word& w = ((Word*)content)[0];
		DWord& e = ((DWord*)content)[0];
		QWord& r = ((QWord*)content)[0];
	};

    struct CPU {
	    Register a;
	    Register b;
	    Register c;
	    Register d;
	    Register e;
	    Register f;
	    Register g;
	    Register h;
		Register i;
		Register j;
		Register k;
		Register l;
		Register m;
		Register n;
		Register o;
		Register p;
	    Accumulator ac;
	    SpecilPointerReg ip;
	    SpecilPointerReg dsp;
		SpecilPointerReg fsp;
	};

	Word instruction = 0x0000;
}

MegaCPU::Register mode;

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

	Memory memory(getFileSize(rompath));
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

	MegaCPU::CPU cpu;

	bool on = true;
	mode.wh = 0x6502;

	
	W6502::reset(cpu6502, memory);

	cpu.a.rx = MEGA;

	std::cout << cpu.a.rx << '\n';

	while(on) {
		if(mode.wh == 0x6502) {
			W6502::cycle(cpu6502, memory);
		} else if(mode.rx == MEGA) {
			std::cout << "What? MEGAcpu, haven't heard of that!\n";
			exit(4);
		}
	}
}
