
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
	BinReader& getBinary() {
		return binary;
	}
};

// abstractions if we want to support more parsers?
class TypeReader {
public:
	virtual ~TypeReader() {}
	virtual std::wstring readerName() const = 0;
	virtual void read(ContentReader* reader) = 0;
};

class Texture2DReader : public TypeReader {
public:
	std::wstring readerName() const { return L"Microsoft.Xna.Framework.Content.Texture2DReader"; };
	void read(ContentReader* reader);
};

#endif