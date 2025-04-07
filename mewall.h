/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/

#ifndef __MEWLIB_ALL__
#define __MEWLIB_ALL__

#ifdef RAYLIB_H
#define MEWALL_NO_WIN_ALIASES
#endif

#include "mewlib.h"
#include "mewmath.hpp"
#include "mewstack.hpp"
#include "mewstring.hpp"
#include "mewmap.hpp"
#include "mewlist.hpp"
#include "mewmatrix.hpp"
#include "mewsmap.hpp"
#include "mewiterator.hpp"
#include "mewprint.hpp"
#ifndef MEWALL_NO_WIN_ALIASES
#include "mewargs.hpp"
#include "mewdll.hpp"
#endif
#include "mewutils.hpp"
#include "mewfeature.hpp"
#include "mewbinary.hpp"
#include "mewvector.hpp"
#include "mewnmath.hpp"
#include "mewarena.hpp"

namespace mew::test {
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
		__stack.push(1);
		__stack.push(2);
		__stack.push(3);
		__stack.push(4);
		auto rev = __stack.reversed();
		auto mrev = std::initializer_list({4, 3, 2, 1});
		x = (rev == mrev);
		MewTest(x, (int)true);
		if (!x) {
			std::cout << "stack: " << rev << std::endl;
			std::cout << "ref: " << mrev << std::endl;
		}
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

	inline void test_mew_smap() {
		mew::simple_map<int, float, 8U> __stack;
		__stack.insert({1, 10.f});
		__stack.insert({14, 13.f});
		__stack.insert({16, 14.4f});
		__stack.insert({13, 1.14f});
		__stack.insert({2, .55f});
		float val = __stack.at(2);
		MewTest(val, .55f);
		val = __stack.at(14);
		MewTest(val, 13.f);
		val = __stack.at(16);
		MewTest(val, 14.4f);
		MewTest(__stack.size(), 5U);
		__stack.clear();
		MewTest(__stack.size(), 0U);
	}

	inline void test_mew_arena() {

	}
};

#endif