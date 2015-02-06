
#ifndef CONTENTREADER_H
#define CONTENTREADER_H
// disable warning
#define _CRT_SECURE_NO_DEPRECATE
#include <stdint.h>
#include "BinReader.h"
class ContentReader {
private:
	BinReader binary;
public:
	ContentReader(FILE* f);
	~ContentReader() { };
	void readXNB();
	uint32_t readHeader();
};

#endif