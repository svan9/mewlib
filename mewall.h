/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/

#ifndef __MEWLIB_ALL__
#define __MEWLIB_ALL__

#include "mewlib.h"
#include "mewmath.hpp"
#include "mewstack.hpp"
#include "mewstring.hpp"
#include "mewmap.hpp"
#include "mewlist.hpp"

namespace Tests {
	inline void test_mew_stack() {
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
	inline void test_mew_list() {
		mew::list<int> __stack;
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
};

#endif