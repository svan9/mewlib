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
typedef unsigned long dword;
typedef char* cstr;
typedef wchar_t* wstr;
typedef const char* ccstr;
typedef const wchar_t* cwstr;

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
#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

#include <time.h>
#include <string.h>
#include <typeinfo>

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

	namespace Helpers {
		template<typename T>
		T& max(T& l, T& r, u64 s1, u64 s2) {
			if (s1 > s2) {
				return l;
			}
			return r;
		}
		template<typename T>
		T& min(T& l, T& r, u64 s1, u64 s2) {
			if (s1 < s2) {
				return l;
			}
			return r;
		}
		
		template<typename T>
		constexpr T MaxValue() {
			return (T)-1;
		}
	};

	template<typename NumberType>
	struct InfinityNumber {
		NumberType* chunks;
		u64 chunks_count;
		typedef InfinityNumber<NumberType> Self;
		static constexpr NumberType MaxChunkValue = Helpers::MaxValue<NumberType>();
		
		Self& operator=(NumberType number) {
			if (chunks) { delete chunks; }
			chunks = new NumberType[1];
			chunks_count = 1;
			memccpy(chunks, &number, sizeof(number));
		}

		friend Self& operator+(Self& left, NumberType& right) { 
			u64 it = 0;
			NumberType remnant;
			NumberType value = right;
			do {
				remnant = value - (MaxChunkValue - left.chunks[it]);
				left.chunks[it++] += right;
				value -= remnant;
			} while(remnant != 0 && it < left.chunks_count);
			return left;
		}

		friend Self& operator+(Self& left, Self& right) {
			Self& _dest = Helpers::max<Self>(left, right, left.chunks_count, right.chunks_count);
			Self& _src = Helpers::min<Self>(left, right, left.chunks_count, right.chunks_count);

			for (u64 i = 0; i < _src.chunk_count; ++i) {
				NumberType& _src_chunk = _src.chunks[i];
				u64 it = i;
				NumberType remnant;
				NumberType value = _src_chunk;
				do {
					remnant = value - (MaxChunkValue - _dest.chunks[it]);
					_dest.chunks[it++] += right;
					value -= remnant;
				} while(remnant != 0 && it < _dest.chunks_count);
			}
			return left;
		}

		friend Self& operator-(Self& left, NumberType& right) {
			u64 it = 0;
			NumberType remnant;
			NumberType value = right;
			do {
				remnant = value - (MaxChunkValue - left.chunks[it]);
				left.chunks[it++] -= right;
				value -= remnant;
			} while(remnant != 0 && it < left.chunks_count);
			return left;
		}

		friend Self& operator-(Self& left, Self& right) {
			Self& _dest = Helpers::min<Self>(left, right, left.chunks_count, right.chunks_count);
			Self& _src = Helpers::max<Self>(left, right, left.chunks_count, right.chunks_count);

			for (u64 i = 0; i < _src.chunk_count; ++i) {
				NumberType& _src_chunk = _src.chunks[i];
				u64 it = i;
				NumberType remnant;
				NumberType value = _src_chunk;
				do {
					remnant = value - (MaxChunkValue - _dest.chunks[it]);
					_dest.chunks[it++] -= right;
					value -= remnant;
				} while(remnant != 0 && it < _dest.chunks_count);
			}
			return left;
		}
		

	};

	template<typename ...Types>
	class Variant {
	private:

	public:
		Variant() { }

		template<typename T>
		bool match() {
			
		}
		template<typename T, typename ...Args>
		static bool match() {
			
		}
		
		
	};
	

	namespace math {
		static constexpr double pi = 3.141592653589793;
	}

	void sleep_ms(u64 ms) {
		::_sleep(ms);
	}

	void sleep_s(f64 s) {
		::_sleep(s * 1000);
	}
}



#endif