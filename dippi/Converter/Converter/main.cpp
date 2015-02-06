#include "ContentReader.h"
int main(int argc, char** argv) {
	FILE* handle = fopen("data\\test.xnb", "r");
	if (handle == 0) return 1;
	ContentReader reader(handle);
	reader.readXNB();
	return 0;
}