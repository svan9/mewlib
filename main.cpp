#include "mewlib.h"
#include "mewstring.hpp"
#include "mewstack.hpp"
#include "mewmath.hpp"

int main() {
	printf("\n----String iterator test----\n");
	mew::string::StringIterator sit("hellow, word");
	while (!sit.IsEnd()) {
		printf("%c_", *sit++);
	}
	printf("\n----Stack test----\n");
	mew::stack<int> __stack;
	__stack.push(9);
	__stack.push(5);
	__stack.push(4);
	__stack.push(2);
	__stack.push(13);
	__stack.push(12);
	int x = __stack.at(-2);
	MewTest(x, 13);
	__stack.erase(2, 3);
	MewTest(__stack.size(), 3);
	MewTest(__stack.empty(), (int)false);
	__stack.clear();
	MewTest(__stack.empty(), (int)true);
	__stack.push(12);
	__stack.push(0xffffffff);
	__stack.push(0);
	x = __stack.pop();
	MewTest(x, 0);
	x = __stack.pop();
	MewTest(x, 0xffffffff);
	x = __stack.pop();
	MewTest(x, 12);
}