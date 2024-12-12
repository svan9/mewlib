/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/

#ifndef MEW_MATH_LIB_SO2U
#define MEW_MATH_LIB_SO2U
#include "mewlib.h"
#include <unordered_map>

namespace mew {
	void _itoa10(int _Value, char *_Dest) {
		snprintf(_Dest, sizeof(_Dest), "%d", _Value);
	}

	int mod(int a, int b) {
		if(b < 0) 
			return -mod(-a, -b);
		int ret = a % b;
		if(ret < 0)
			ret+=b;
		return ret;
	}

	template<typename T> 
	T operator%(const T& a, const T& b) {
		return mod(a, b);
	}

	template<typename T>
	size_t get_hash(T& s) {
		return reinterpret_cast<size_t>(s);
	}

	template<typename T>
	size_t get_hash(T* s) {
		return reinterpret_cast<size_t>(s);
	}

	size_t get_cstr_hash(const char* s) {
		return reinterpret_cast<size_t>(s);
	}

	size_t get_cstr_hash(const wchar_t* s) {
		return reinterpret_cast<size_t>(s);
	}
}

#endif