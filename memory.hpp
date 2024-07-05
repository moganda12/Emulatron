#pragma once

#include <cstddef>

using Byte = unsigned char;
using Word = unsigned short;
using DWord = unsigned int;
using QWord = size_t;

template <size_t length>
class Memory {
private:
	Byte data[length];
public:
	Byte fetch8(size_t address) {
		return data[address];
	}
	unsigned short fetch16(size_t address) {
		return data[address] | (data[address + 1] << 8);
	}
	unsigned int fetch32(size_t address) {
		return data[address] | (data[address + 1] << 8) | (data[address + 2] << 16) | (data[address + 3] << 24);
	}
	size_t fetch64(size_t address) {
		return data[address] | (data[address + 1] << 8) | (data[address + 2] << 16) | (data[address + 3] << 24) | (data[address + 4] << 32) | (data[address + 5] << 40) | (data[address + 6] << 48) | (data[address + 7] << 56);
	}
	void store8(size_t address, Byte value) {
		data[address] = value;
	}
	void store16(size_t address, unsigned short value) {
		data[address] = value & 0xFF;
		data[address + 1] = value >> 8;
	}
	void store32(size_t address, unsigned int value) {
		data[address] = value & 0xFF;
		data[address + 1] = (value >> 8) & 0xFF;
		data[address + 2] = (value >> 16) & 0xFF;
		data[address + 3] = (value >> 24) & 0xFF;
	}
	void store64(size_t address, size_t value) {
		data[address] = value & 0xFF;
		data[address + 1] = (value >> 8) & 0xFF;
		data[address + 2] = (value >> 16) & 0xFF;
		data[address + 3] = (value >> 24) & 0xFF;
		data[address + 4] = (value >> 32) & 0xFF;
		data[address + 5] = (value >> 40) & 0xFF;
		data[address + 6] = (value >> 48) & 0xFF;
		data[address + 7] = (value >> 56) & 0xFF;
	}
	size_t getlen() {
		return length;
	}
	Byte* getdata() {
		return data;
	}
};

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
