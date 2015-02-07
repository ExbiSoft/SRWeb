
#ifndef BINREADER_H
#define BINREADER_H
#include <stdint.h>

#include <stdio.h>
#include <string>
#include <vector>
class BinReader {
private:
	FILE *file;
public:
	BinReader(FILE* f);
	uint8_t readByte();
	uint32_t filePosition();
	uint32_t readUInt32();
	uint32_t readFileSize();
	uint32_t read7BitEncInt();
	std::wstring readString();
	std::vector<uint8_t> readBytes(uint32_t count);
#ifdef _WIN32
	wchar_t readChar();
#else
	char* readChar();
#endif
};
#endif