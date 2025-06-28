#include "mewlib.h"
#include "mewtypes.h"
#include "mewfs.hpp"
#include <string>

int main() {
	using namespace mew::fs;
	IOFile file("test.txt", "wri+");
	file.Open();
	if (!file.IsOpen()) {
		printf("Failed to open file\n");
		return 1;
	}
	file.Write("Hello, World!");
	file.Close();

	IOFile file2("test2.txt", "wr+");
	file2.Open();
	if (!file2.IsOpen()) {
		printf("Failed to open file2\n");
		return 1;
	}
	file2.Write("222 Hello, World! 222");
	file2.Close();
	
	
	IOFile::SaveIsolateToFile("isolate_dump.txt");
	return 0;
}