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
#include <chrono>
#include <conio.h>
#include <future>

namespace mew {

	////////////////////////////////////////////////////////////
	template<typename T>
	T* _copy_value(T& value) {
		return new T(value);
	}
	////////////////////////////////////////////////////////////
	template<typename T>
	T* _copy_value(const T& value) {
		return _copy_value(const_cast<T&>(value));
	}
	////////////////////////////////////////////////////////////
	template<typename T>
	T* _copy_value(T&& value) {
		return new T(value);
	}
	////////////////////////////////////////////////////////////
	template<typename T>
	T* _copy_value(T* value) {
		return new T(value);
	}
		
	template<typename T>
	void copy_to(T* ptr, T& copied) {
		new (ptr) T(copied);
	}
	template<typename T>
	void copy_to(T* ptr, const T& copied) {
		new (ptr) T(const_cast<T&>(copied));
	}
	template<typename T>
	void copy_to(T* ptr, T* copied) {
		new (ptr) T(*copied);
	}
	
	
	
	bool rmemcmp(byte* a, byte* b, size_t size) {
		for (int i = 0; i < size; ++i) {
			if (a[i] != b[i]) return false;
		}
		return true;
	}
	template<typename T, typename K>
	bool memcmp(T* a, K* b, size_t size) {
		return rmemcmp((byte*)a, (byte*)b, size);
	}

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

	int wait_char() {
		if (_kbhit()) return _getch();
		else return -1;
	}

	typedef int&(*asi)(int&);
	typedef float&(*asf)(float&);
	typedef lli&(*asl)(lli&);
	typedef double&(*asd)(double&);
	
	template<typename A>
	using asio = A&(*)(A&);
	template<typename A, typename B>
	using adio = A&(*)(A&, B&);

	enum struct ATYPE {
		INT, FLOAT, LONG, DOUBLE
	};
	using asgio = void(*)(void*, ATYPE);
	using adgio = void(*)(void*, ATYPE, void*, ATYPE);

	ATYPE getATYPE(int a) {
		return ATYPE::INT;
	}
	ATYPE getATYPE(float a) {
		return ATYPE::FLOAT;
	}
	ATYPE getATYPE(lli a) {
		return ATYPE::LONG;
	}
	ATYPE getATYPE(double a) {
		return ATYPE::DOUBLE;
	}

	void gen_asgio(asgio fn, int& a) {
		fn(&a, ATYPE::INT);
	}
	void gen_asgio(asgio fn, float& a) {
		fn(&a, ATYPE::FLOAT);
	}
	void gen_asgio(asgio fn, lli& a) {
		fn(&a, ATYPE::LONG);
	}
	void gen_asgio(asgio fn, double& a) {
		fn(&a, ATYPE::DOUBLE);
	}

	template<typename>
	struct is_in_adgio_t_helper
		: public std::false_type { };
	template<>
	struct is_in_adgio_t_helper<int>
		: public std::true_type { };
	template<>
	struct is_in_adgio_t_helper<float>
		: public std::true_type { };
	template<>
	struct is_in_adgio_t_helper<lli>
		: public std::true_type { };
	template<>
	struct is_in_adgio_t_helper<double>
		: public std::true_type { };

	template<typename _Tp>
	struct is_in_adgio_t
    : public is_in_adgio_t_helper<std::__remove_cv_t<_Tp>>::type 
    { };

	template<typename>
	struct is_in_nd_adgio_t_helper
		: public std::false_type { };
	template<>
	struct is_in_nd_adgio_t_helper<int>
		: public std::true_type { };
	template<>
	struct is_in_nd_adgio_t_helper<lli>
		: public std::true_type { };

	template<typename _Tp>
	struct adgio_numeric_t
    : public is_in_nd_adgio_t_helper<std::__remove_cv_t<_Tp>>::type 
    { };



	template<typename T, typename K>
	void gen_adgio(adgio fn, T& a, K& b) {
		static_assert(is_in_adgio_t<T>::value && is_in_adgio_t<K>::value);
		fn(&a, getATYPE(a), &b, getATYPE(b));
	}

	int& ainc(int& a) { return ++a; }
	float& ainc(float& a) { return ++a; }
	lli& ainc(lli& a) { return ++a; }
	double& ainc(double& a) { return ++a; }

	void aginc(void* a, ATYPE at) {
		switch(at) {
			case ATYPE::INT: 		ainc(*(int*)a); break;
			case ATYPE::FLOAT: 	ainc(*(float*)a); break;
			case ATYPE::LONG: 	ainc(*(lli*)a); break;
			case ATYPE::DOUBLE: ainc(*(double*)a); break;
		}
	}

	int& adec(int& a) { return --a; }
	float& adec(float& a) { return --a; }
	lli& adec(lli& a) { return --a; }
	double& adec(double& a) { return --a; }

	void agdec(void* a, ATYPE at) {
		switch(at) {
			case ATYPE::INT: 		adec(*(int*)a); break;
			case ATYPE::FLOAT: 	adec(*(float*)a); break;
			case ATYPE::LONG: 	adec(*(lli*)a); break;
			case ATYPE::DOUBLE: adec(*(double*)a); break;
		}
	}

	int& anot(int& a) { return a = ~a; }
	float& anot(float& a) { MewUserAssert(false, "NOT SUPPORTED ANOT TYPE (float)"); }
	lli& anot(lli& a) { return a = ~a; }
	double& anot(double& a) { MewUserAssert(false, "NOT SUPPORTED ANOT TYPE (double)"); }

	void agnot(void* a, ATYPE at) {
		switch(at) {
			case ATYPE::INT: 		anot(*(int*)a); break;
			case ATYPE::FLOAT: 	anot(*(float*)a); break;
			case ATYPE::LONG: 	anot(*(lli*)a); break;
			case ATYPE::DOUBLE: anot(*(double*)a); break;
		}
	}

	#define agg_adgt()	static_assert(is_in_adgio_t<T>::value && is_in_adgio_t<K>::value, "unsupported type"); 
	#define agg_adgt_nn()	static_assert(adgio_numeric_t<T>::value && adgio_numeric_t<K>::value, "unsupported type"); 

	#pragma region _agg_gen
#define agg_gen(_fn, _a, _at, _b, _bt, _no_float) \
	switch (_at)                                    \
	{                                               \
	case ATYPE::INT:                                \
	{                                               \
		switch (_bt)                                  \
		{                                             \
		case ATYPE::INT:                              \
		{                                             \
			_fn(*(int *)_a, *(int *)_b);                \
		}                                             \
		break;                                        \
		case ATYPE::FLOAT:                            \
		{                                             \
			if (!_no_float)                             \
				_fn(*(int *)_a, *(float *)_b);            \
		}                                             \
		break;                                        \
		case ATYPE::LONG:                             \
		{                                             \
			_fn(*(int *)_a, *(lli *)_b);                \
		}                                             \
		break;                                        \
		case ATYPE::DOUBLE:                           \
		{                                             \
			if (!_no_float)                             \
				_fn(*(int *)_a, *(double *)_b);           \
		}                                             \
		break;                                        \
		}                                             \
	}                                               \
	break;                                          \
	case ATYPE::FLOAT:                              \
	{                                               \
		if (_no_float)                                \
			break;                                      \
		switch (_bt)                                  \
		{                                             \
		case ATYPE::INT:                              \
		{                                             \
			_fn(*(float *)_a, *(int *)_b);              \
		}                                             \
		break;                                        \
		case ATYPE::FLOAT:                            \
		{                                             \
			_fn(*(float *)_a, *(float *)_b);            \
		}                                             \
		break;                                        \
		case ATYPE::LONG:                             \
		{                                             \
			_fn(*(float *)_a, *(lli *)_b);              \
		}                                             \
		break;                                        \
		case ATYPE::DOUBLE:                           \
		{                                             \
			_fn(*(float *)_a, *(double *)_b);           \
		}                                             \
		break;                                        \
		}                                             \
	}                                               \
	break;                                          \
	case ATYPE::LONG:                               \
	{                                               \
		switch (_bt)                                  \
		{                                             \
		case ATYPE::INT:                              \
		{                                             \
			_fn(*(lli *)_a, *(int *)_b);                \
		}                                             \
		break;                                        \
		case ATYPE::FLOAT:                            \
		{                                             \
			if (!_no_float)                             \
				_fn(*(lli *)_a, *(float *)_b);            \
		}                                             \
		break;                                        \
		case ATYPE::LONG:                             \
		{                                             \
			_fn(*(lli *)_a, *(lli *)_b);                \
		}                                             \
		break;                                        \
		case ATYPE::DOUBLE:                           \
		{                                             \
			if (!_no_float)                             \
				_fn(*(lli *)_a, *(double *)_b);           \
		}                                             \
		break;                                        \
		}                                             \
	}                                               \
	break;                                          \
	case ATYPE::DOUBLE:                             \
	{                                               \
		if (_no_float)                                \
			break;                                      \
		switch (_bt)                                  \
		{                                             \
		case ATYPE::INT:                              \
		{                                             \
			_fn(*(double *)_a, *(int *)_b);             \
		}                                             \
		break;                                        \
		case ATYPE::FLOAT:                            \
		{                                             \
			_fn(*(double *)_a, *(float *)_b);           \
		}                                             \
		break;                                        \
		case ATYPE::LONG:                             \
		{                                             \
			_fn(*(double *)_a, *(lli *)_b);             \
		}                                             \
		break;                                        \
		case ATYPE::DOUBLE:                           \
		{                                             \
			_fn(*(double *)_a, *(double *)_b);          \
		}                                             \
		break;                                        \
		}                                             \
	}                                               \
	break;                                          \
	}

#define agg_gen_no_float(_fn, _a, _at, _b, _bt) \
	switch (_at)                                  \
	{                                             \
	case ATYPE::INT:                              \
	{                                             \
		switch (_bt)                                \
		{                                           \
		case ATYPE::INT:                            \
		{                                           \
			_fn(*(int *)_a, *(int *)_b);              \
		}                                           \
		break;                                      \
		case ATYPE::LONG:                           \
		{                                           \
			_fn(*(int *)_a, *(lli *)_b);              \
		}                                           \
		break;                                      \
		}                                           \
	}                                             \
	break;                                        \
	case ATYPE::LONG:                             \
	{                                             \
		switch (_bt)                                \
		{                                           \
		case ATYPE::INT:                            \
		{                                           \
			_fn(*(lli *)_a, *(int *)_b);              \
		}                                           \
		break;                                      \
		case ATYPE::LONG:                           \
		{                                           \
			_fn(*(lli *)_a, *(lli *)_b);              \
		}                                           \
		break;                                      \
		}                                           \
	}                                             \
	break;                                        \
	}
#pragma endregion _add_gen

	template<typename T, typename K>
	T& aadd(T& a, K& b) {
		agg_adgt();
		a += b;
		return a;
	}

	void agadd(void* a, ATYPE at, void* b, ATYPE bt) {
		agg_gen(aadd, a, at, b, bt, false);
	}

	template<typename T, typename K>
	T& asub(T& a, K& b) {
		agg_adgt();
		a -= b;
		return a;
	}

	void agsub(void* a, ATYPE at, void* b, ATYPE bt) {
		agg_gen(asub, a, at, b, bt, false);
	}

	template<typename T, typename K>
	T& amul(T& a, K& b) {
		agg_adgt();
		a *= b;
		return a;
	}

	void agmul(void* a, ATYPE at, void* b, ATYPE bt) {
		agg_gen(amul, a, at, b, bt, false);
	}

	template<typename T, typename K>
	T& adiv(T& a, K& b) {
		agg_adgt();
		a /= b;
		return a;
	}

	void agdiv(void* a, ATYPE at, void* b, ATYPE bt) {
		agg_gen(adiv, a, at, b, bt, false);
	}

	template<typename T, typename K>
	T& aand(T& a, K& b) {
		agg_adgt_nn();
		a &= b;
		return a;
	}

	void agand(void* a, ATYPE at, void* b, ATYPE bt) {
		agg_gen_no_float(aand, a, at, b, bt);
	}

	template<typename T, typename K>
	T& aor(T& a, K& b) {
		agg_adgt_nn();
		a |= b;
		return a;
	}

	void agor(void* a, ATYPE at, void* b, ATYPE bt) {
		agg_gen_no_float(aor, a, at, b, bt);
	}

	template<typename T, typename K>
	T& axor(T& a, K& b) {
		agg_adgt_nn();
		a ^= b;
		return a;
	}

	void agxor(void* a, ATYPE at, void* b, ATYPE bt) {
		agg_gen_no_float(axor, a, at, b, bt);
	}

	template<typename T, typename K>
	T& als(T& a, K& b) {
		agg_adgt_nn();
		a <<= b;
		return a;
	}

	void agls(void* a, ATYPE at, void* b, ATYPE bt) {
		agg_gen_no_float(als, a, at, b, bt);
	}

	template<typename T, typename K>
	T& ars(T& a, K& b) {
		agg_adgt_nn();
		a >>= b;
		return a;
	}

	void agrs(void* a, ATYPE at, void* b, ATYPE bt) {
		agg_gen_no_float(ars, a, at, b, bt);
	}

	template<typename T, typename K>
	T& amov(T& a, K& b) {
		agg_adgt();
		void* __a = &b;
		void* __b = &a;
		size_t __less = sizeof(T);
		if (sizeof(T) >= sizeof(K)) {
			__a = &a;
			__b = &b;
			__less = sizeof(K);
		}
		memcpy(__a, __b, __less);
		return a;
	}

	void agmov(void* a, ATYPE at, void* b, ATYPE bt) {
		agg_gen(amov, a, at, b, bt, false);
	}

	template<typename T, typename K>
	T& aswap(T& a, K& b) {
		agg_adgt();
		T templ = a;
		a = b;
		b = templ;
		return a;
	}

	void agswap(void* a, ATYPE at, void* b, ATYPE bt) {
		agg_gen(aswap, a, at, b, bt, false);
	}
}

#endif