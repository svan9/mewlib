#ifndef MEW_VECTOR_SO2U
#define MEW_VECTOR_SO2U

#include <math.h>
#include "mewlib.h"

namespace mew {
	float lerp_value(float a, float b, float t) {
		return a + (t * (b - a));
	}
	
	template<uint L, typename T> struct vec;

	template<typename T>
	struct vec<2, T> {
		typedef vec<2, T> self_type;
		union {
			struct { T x, y; };
		};

		self_type& operator+=(const self_type& l) {
			x += l.x; 
			y += l.y;
			return *this;
		}
		self_type& operator-=(const self_type& l) {
			x -= l.x; 
			y -= l.y;
			return *this;
		}
		self_type& operator*=(const self_type& l) {
			x *= l.x; 
			y *= l.y;
			return *this;
		}
		self_type& operator/=(const self_type& l) {
			x /= l.x; 
			y /= l.y;
			return *this;
		}
		self_type& operator+=(T l) {
			x += l; 
			y += l;
			return *this;
		}
		self_type& operator-=(T l) {
			x -= l; 
			y -= l;
			return *this;
		}
		self_type& operator*=(T l) {
			x *= l; 
			y *= l;
			return *this;
		}
		self_type& operator/=(T l) {
			x /= l; 
			y /= l;
			return *this;
		}

		self_type& lerp(self_type target, T t) {
			while (t > 1) { t /= 10; }
			x += t * (target.x - x);
			y += t * (target.y - y);
			return *this;
		}
		

		friend self_type operator+(const self_type& r, const self_type& l) {
			return self_type(r.x+l.x, r.y+l.y);
		}
		friend self_type operator-(const self_type& r, const self_type& l) {
			return self_type(r.x-l.x, r.y-l.y);
		}
		friend self_type operator*(const self_type& r, const self_type& l) {
			return self_type(r.x*l.x, r.y*l.y);
		}
		friend self_type operator/(const self_type& r, const self_type& l) {
			return self_type(r.x/l.x, r.y/l.y);
		}
		friend self_type operator+(const self_type& r, T l) {
			return self_type(r.x+l, r.y+l);
		}
		friend self_type operator-(const self_type& r, T l) {
			return self_type(r.x-l, r.y-l);
		}
		friend self_type operator*(const self_type& r, T l) {
			return (self_type){(T)r.x*l, (T)r.y*l};
		}
		friend self_type operator/(const self_type& r, T l) {
			return (self_type){(T)r.x/l, (T)r.y/l};
		}
		friend self_type operator+(T l, const self_type& r) {
			return self_type(r.x+l, r.y+l);
		}
		friend self_type operator-(T l, const self_type& r) {
			return self_type(r.x-l, r.y-l);
		}
		friend self_type operator*(T l, const self_type& r) {
			return self_type(r.x*l, r.y*l);
		}
		friend self_type operator/(T l, const self_type& r) {
			return self_type(r.x/l, r.y/l);
		}

		static self_type Zero;

		static self_type lerp(const self_type& a, const self_type& b, T t) {
			return self_type(a.x + t * (b.x - a.x), a.y + t * (b.y - a.y));
		}
		
		bool isZero(float limit = 0.001f) {
			return fabsf(x) < limit && fabsf(y) < limit;
		}
		
		bool isZero(uint zero) {
			return x == 0 && y == 0;
		}

		bool isZero() {
			return isZero((T)(0));
		}


		self_type& zero() {
			x = 0;
			y = 0;
			return *this;
		}
	};
	typedef vec<2, float> vec2;
	template<>
	vec2 vec2::Zero{0.0f,0.0f};
	typedef vec<2, uint> vec2u;
	template<>
	vec2u	vec2u::Zero{0,0};
}

#endif