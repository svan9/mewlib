#include "mewall.h"
#include "mewalloc.hpp"
#include "mewxml.hpp"

int main() {
	using namespace mew;
	struct A {
		size_t size;
		uint s;
		char c;
		char p[3];
	};
	Alloc1MB gal;
	int* num1 = gal.alloc<int>();
	*num1 = 10;
	A* num2 = gal.alloc<A>();
	printf("-- i: %i\n", *num1);
	printf("-- A: { %u, %u, %u, [%u,%u,%u] }\n", num2->size, num2->s, 
		num2->c, num2->p[0], num2->p[1], num2->p[2]);
	gal.dealloc(num2);
	printf("-- M: %s\n", MEW_SBOOL(gal.exist(num2)));
	gal.dealloc(num1);
	printf("-- M2: %s\n", MEW_SBOOL(gal.exist(num1)));

	
 	// printf("\n----String iterator test----\n");
	// mew::string::StringIterator sit("hellow, word");
	// while (!sit.IsEnd()) {
	// 	printf("%c_", *sit++);
	// }
	// printf("\n----Stack test----\n");
	// Tests::test_mew_stack();
	
}