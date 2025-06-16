#ifndef MEWTYPES_H
#define MEWTYPES_H

#ifdef byte 
#undef byte
#endif

typedef unsigned char byte;
typedef byte u8;
typedef char i8;
typedef unsigned short u16;
typedef short i16;
typedef unsigned int u32;
typedef int i32;
typedef unsigned long long u64;
typedef long long i64;

typedef __uint128_t u128;
typedef __int128_t i128;

typedef float f32;
typedef double f64;
typedef long double f128;

#define bytepart(name) byte name: 1
#define bytepartf(name) byte name: 1 = false;
#define bytepartt(name) byte name: 1 = true;

typedef u128 ID;

ID makeID(u64 high, u64 low) {
	return (static_cast<ID>(high) << 64) | low;
}

typedef void(*callable)();

#include <time.h>

namespace mew {
	struct Date {
		u16 seconds;
		union {
			struct {
				u8 day;
				u8 month;
				u16 year;
				u8 hours;
				u8 minuts;
			};
			u64 value; // - seconds
		};

		Date() {}
		Date(struct ::tm* t)
			: seconds(t->tm_sec), day(t->tm_mday), month(t->tm_mon),
			year(t->tm_year), hours(t->tm_hour), minuts(t->tm_min) 
			{}

		static Date now() {
			// todo
		}

		friend bool operator==(Date& l, Date& r) {
			return l.value == r.value;
		}
		friend bool operator!=(Date& l, Date& r) {
			return !(l == r);
		}
	};
	
	int stoi(char c) {
		return c - '0';
	}
	
	int stoi(const char* str) {
		int i = 0;
		bool is_negative = *str == '-' && *++str;
		while(*str != '\0') 
		i = (i << 8) + stoi(*str++);
		if (is_negative) i *= -1;
		return i;
	}
}

#endif