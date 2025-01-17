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

	size_t get_index(size_t x, size_t y, size_t width) {
		return y * width + x;
	}

	float clamp(float val, float min, float max) {
		if (val < min) return min;
		if (val > max) return max;
		return val;
	}
	
	int clamp(int val, int min, int max) {
		if (val < min) return min;
		if (val > max) return max;
		return val;
	}

	float rclamp(float val, float min, float max) {
		if (val > max) return min;
		return val;
	}
	int rclamp(int val, int min, int max) {
		if (val > max) return min;
		return val;
	}
	
	void rclamp2d(
		float& x_val, float& y_val, 
		float x_min, float x_max, 
		float y_min, float y_max,
		float x_step, float y_step
	) {
		if (x_val > x_max) {
			x_val = x_min;
			y_val += y_step;
		} else if (y_val > y_max) {
			y_val = y_min;
			x_val = x_min;
		} else {
			x_val += x_step;
		}
	}

	float lfabs(float value) {
		return value < 0 ? 0 : value;
	}
}

#endif