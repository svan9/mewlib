#include "mewlib.h"
#include "mewtypes.h"
#include "mewfs.hpp"

int main() {
	using namespace mew::fs;
	IOFile file("test.txt", "r+");
	file.Open();
	if (!file.IsOpen()) {
		printf("Failed to open file\n");
		return 1;
	}


	return 0;
}