
#ifndef BINREADER_H
#define BINREADER_H
#include <stdint.h>

#include <stdio.h>
class BinReader {
private:
	FILE *file;
public:
	BinReader(FILE* f);
	uint8_t readByte();
	uint32_t filePosition();
	uint32_t readUInt32();
	uint32_t readFileSize();
};
#endif