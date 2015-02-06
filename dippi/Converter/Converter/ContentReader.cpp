#include "ContentReader.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <wchar.h>
ContentReader::ContentReader(FILE* f) : binary(f) {}

void ContentReader::readXNB() {
	uint32_t endPos = readHeader();
	std::cout << "Filesize == " << endPos << std::endl;

	// reader count is 7bit encoded int
	uint32_t readerCount = binary.read7BitEncInt();

	for (uint32_t i = 0; i < readerCount; i++) {
		// strings are also 7bit encoded ints...
		std::wstring name = binary.readString();
		int32_t ver = (int32_t)binary.readUInt32();
		
		// throw exp if it isn't Texture2DReader
		if (wcsstr(name.c_str(), L"Texture2DReader") == 0) {
			throw std::logic_error("Not supported reader!");
		}
	}

	uint32_t resourceCount = binary.read7BitEncInt();
	std::cout << "Resource count: " << readerCount << std::endl;

}

uint32_t ContentReader::readHeader()
{
	uint32_t start = binary.filePosition();

	// read X,N and B to identify if it's really is a legit XNB file
	uint8_t x = binary.readByte();
	uint8_t n = binary.readByte();
	uint8_t b = binary.readByte();

	if (x != 'X' || n != 'N' || b != 'B') {
		throw std::runtime_error("Invalid XNB FILE");
	}

	// target platform, no need to save
	binary.readByte();
	// format
	uint8_t format = binary.readByte();
	// we only support XNA4, so it should be 5
	if (format != 5) {
		throw std::runtime_error("Not an XNA4 -xnb file!");
	}
	
	/*
	Bit 0x01 = content is for HiDef profile (otherwise Reach)
	Bit 0x80 = asset data is compressed
	If the file is compressed (flag bit 0x80 is set), data from this point on is packed using the Xbox XMemCompress API
	IMPORTANT!!!
	*/
	bool compressed = (binary.readByte() & 0x80) != 0;
	
	// how large it's in filesystem
	uint32_t filesize = binary.readUInt32();
	// lets compare it to actual binary filesize
	if (start + filesize > binary.readFileSize()) {
		throw std::runtime_error("XNB has been altered");
	}

	if (compressed) {
		 //TODO how does Xbox XmemCompress work?
		// or do we even need to support them?
		// only XBOX uses them?
	}

	return start + filesize;
}


