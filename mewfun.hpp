#ifndef MEW_FUN_LIB_SO2U
#define MEW_FUN_LIB_SO2U

#include "mewlib.h"
#include "mewtypes.h"

namespace mew::fun {

	template<size_t N>
	struct uint_n {
		typedef uint_n<N> self;
		typedef u8 part;
		typedef u32 big_part;
		constexpr size_t bit_size = N;
		constexpr size_t size = N/8;
		part value[size] = {0};
		uint_n() {}

		self& operator=(part prt) {
			memcpy(&value, &prt, sizeof(prt));
			return *this;
		}
		
		self& operator=(big_part prt) {
			memcpy(&value, &prt, sizeof(prt));
			return *this;
		}
		
		friend self& operator+(self& l, self& m) {
			// todo
			return l;
		}
		
	};
};

#endif