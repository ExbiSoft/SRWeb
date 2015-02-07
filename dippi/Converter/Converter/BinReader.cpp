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
	return uint32_t(bytes[0]) | (uint32_t(bytes[1]) << 8) | (uint32_t(bytes[2]) << 16) | (uint32_t(bytes[3]) << 24);
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
// hope it works, lol
uint32_t BinReader::read7BitEncInt() {
	// http://en.wikipedia.org/wiki/Variable-length_quantity
	// http://stackoverflow.com/questions/1550560/encoding-an-integer-in-7-bit-format-of-c-sharp-binaryreader-readstring
	uint32_t read = 0;
	uint32_t val;
	uint32_t r = 0;
	
	do {
		val = readByte();
		r |= (val & 0x7f) << read;
		read += 7;
	} while (val & 0x80);
	return r;
}

std::wstring BinReader::readString() {
	
	uint32_t len = read7BitEncInt();
	// end of string, i hope
	uint32_t eos = filePosition() + len;

	// http://stackoverflow.com/questions/402283/stdwstring-vs-stdstring
	// TODO how about linux? it uses only char*
#ifdef _WIN32

	std::wstring result;
	while (filePosition() < eos) {
		result += readChar();
	}
	return result;
#else
	throw std::logic_error("Not implemted");
#endif

}

wchar_t BinReader::readChar() {
	wchar_t result = readByte();
	// decode utf??? http://stackoverflow.com/questions/3911536/utf-8-unicode-whats-with-0xc0-and-0x80
	if (result & 0x80) {
		int count = 1;
		// if it starts with 10, it belongs to multi byte sequence...
		// lets try to parse it then, shall we?
		while (result & (0x80 >> count)) {
			count++;
		}

		// copy paste from somewhere, i have no idea WTF this does
		result &= ( 1 << (8 - count)) - 1;
		while (--count) { result <<= 6; result |= (readByte() & 0x3F); }
	}
	return result;
}

std::vector<uint8_t> BinReader::readBytes(uint32_t count) {
	std::vector<uint8_t> result;
	result.reserve(count);
	while (count--) result.push_back(readByte());
	return result;
}
