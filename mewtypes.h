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
	return (static_cast<u128>(high) << 64) | low;
}


#endif