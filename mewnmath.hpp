#ifndef MEWLIB_NMATH
#define MEWLIB_NMATH

#include "mewlib.h"
#define MAX32_INT (int)(0xffffffff/2)
#define MAX32_UINT (unsigned)(0xffffffff)
#define MIN32_INT (int)-(0xffffffff/2)

namespace mew::nmath {
	template<size_t size, typename T>
	struct nvec {
		T value[size];
	};

	typedef nvec<4, unsigned> ui128;
	typedef unsigned ui32;

	ui32 smax(ui32 a, ui32 b) {
		return a > b? a: b;
	}
	ui32 smin(ui32 a, ui32 b) {
		return a < b? a: b;
	}

	template<typename T>
	T smod(const T& a, const T& b) {
		return (a % b + b) % b;
	}

	ui32 sadd(ui32 a, ui32 b, ui32* r) {
		if (MAX32_UINT == a) {
			*r = b;
			return MAX32_UINT;
		}
		if (MAX32_UINT == b) {
			*r = a;
			return MAX32_UINT;
		}
		ui32 result = a+b;
		*r = smod(a, b);
		if (*r != 0) { 
			return MAX32_UINT;
		}		
		return result;
	}
	

	template<size_t size, typename T>
	nvec<size, T> add(const nvec<size, T>& l, const nvec<size, T>& r) {
		nvec<size, T> num = {0};
		ui32 rem = 0;
		for (size_t i = 0; i < size; ++i) {
			num.value[i] = (T)sadd((ui32)l.value[i], (ui32)r.value[i], &rem);
			num.value[i] += rem;
			rem = 0;
		}
	}

	template<size_t size, typename T>
	nvec<size, T> operator+(const nvec<size, T>& l, const nvec<size, T>& r) { 
		return add(l, r);
	}

}


#endif