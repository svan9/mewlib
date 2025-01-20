/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/

#ifndef MEW_MATH_LIB_SO2U
#define MEW_MATH_LIB_SO2U
#include "mewlib.h"
#include "mewvector.hpp"
#include <unordered_map>
#include <inttypes.h>
#include <stdio.h>
#include <time.h>

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

	int64_t getTime() {
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    return ((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000;
	}

	double getTimeSeconds() {
		return getTime()/1000.0;
	}

	float deltaTime(float lastSeconds) {
		return getTimeSeconds() - lastSeconds;
	}

	float deltaTime(double* lastSeconds) {
		double current_time = getTimeSeconds();
		float result = current_time - *lastSeconds;
		*lastSeconds = current_time;
		return result;
	}


	template<typename T, typename Scalar>
	T quadraticBezierCurve(const T& p1, const T& p2, const T& p3, Scalar t) {
		const auto reverseT = (1 - t);
		const auto a = p1 * reverseT * reverseT;
		const auto b = 2 * reverseT * t * p2;
		const auto c = t * t * p3;
		return a + b + c;
	}

	template<typename T, typename Scalar>
	T cubicBezierCurve(const T& p1, const T& p2, const T& p3, const T& p4, Scalar t) {
    const auto reverseT = (1 - t);
    const auto a = reverseT * reverseT * reverseT * p1;
    const auto b = 3 * reverseT * reverseT * t * p2;
    const auto c = 3 * reverseT * t * t * p3;
    const auto d = t * t * t * p4;
    return a + b + c + d;
	}

	class Lerp {
	private:
		float current, target;
		double last_time;
	public:
		Lerp(): last_time(getTimeSeconds()) {}
		Lerp(float _current): current(_current), last_time(getTimeSeconds()) {}

		void setTarget(float _target) {
			target = _target;
		}

		void start(float _current, float _target) {
			target = _target;
			current = _current;
			last_time = getTimeSeconds();
		} 

		float lerp(float _current, float _target, float factor = 1.0f) {
			target = _target; 
			current = _current;
			float dt = factor * deltaTime(&last_time);
			current = lerp_value(current, target, dt);
			return current;
		}

		float lerp(float factor = 1.0f) {
			float dt = factor * deltaTime(&last_time);
			current = lerp_value(current, target, dt);
			return current;
		}
		
		float value() const noexcept {
			return current;
		}

	};

	class Bezier {
	private:
		float f1, f2, f3, f4;
		float current, target;
		// float c1, c2, c3, c4;
		// float t1, t2, t3, t4;
		double last_time;
	public:
		Bezier(): last_time(getTimeSeconds()) {}
		Bezier(float f1, float f2, float f3, float f4): 
			f1(f1), f2(f2), f3(f3), f4(f4), last_time(getTimeSeconds()) {}
		
		void setTarget(float _target) {
			target = _target;
		}

		void start(float _current, float _target) {
			target = _target;
			current = _current;
			last_time = getTimeSeconds();
		} 
		
		float lerp(float _current, float _target, float factor = 1.0f) {
			target = _target; 
			current = _current;
			return lerp(factor);
		}

		float lerp(float factor = 1.0f) {
			float dt = factor * deltaTime(&last_time);
			current = cubicBezierCurve((1+f1)*current, (1+f2)*current, (1+f3)*target, (1+f4)*target, dt);
			return current;
		}

		float value() const noexcept {
			return current;
		}
	};

}

#endif