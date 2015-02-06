#include "BinReader.h"
#include <stdexcept>


BinReader::BinReader(FILE* f) : file(f){}

uint32_t BinReader::filePosition() {
	return ftell(file);
}

uint8_t BinReader::readByte() {
	// http://www.cplusplus.com/reference/cstdio/fgetc/
	int x = fgetc(file);
	if (x == EOF) throw std::runtime_error("error reading file");
	return (uint8_t)x;
}

uint32_t BinReader::readUInt32() {
	// uint32 is 4 bytes
	// lets read 4 bytes
	uint8_t bytes[] = {
		readByte(),
		readByte(),
		readByte(),
		readByte()
	};
	// http://stackoverflow.com/questions/12240299/convert-bytes-to-int-uint-in-c
	// byte is 8 bits each, lets shift them
	return uint32_t(bytes[0]) | uint32_t(bytes[1]) << 8 | uint32_t(bytes[2]) << 16 | uint32_t(bytes[3]) << 24;
}

uint32_t BinReader::readFileSize() {
	uint32_t cur = filePosition();

	// seek to eof
	if (fseek(file, 0, SEEK_END)) {
		throw std::runtime_error("seeking failed!");
	}
	uint32_t fsize = filePosition();
	// rewind
	if (fseek(file, cur, SEEK_SET)) {
		throw std::runtime_error("seeking back failed!");
	}
	return fsize;
}
