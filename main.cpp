#include "mewall.h"

int main() {
	printf("\n----String iterator test----\n");
	mew::string::StringIterator sit("hellow, word");
	while (!sit.IsEnd()) {
		printf("%c_", *sit++);
	}
	printf("\n----Stack test----\n");
	Tests::test_mew_stack();
}