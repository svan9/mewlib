#include "mewlib.h"
#include "mewtypes.h"
#include "mewstack.hpp"
#include "mewallocator.hpp"
#include "mewmalloc.hpp"

#include <iostream>
#include <chrono>

using namespace std::chrono;
using std::chrono::_V2::system_clock;

auto get_time_start() {
	return high_resolution_clock::now();
}

template <typename T>
size_t get_time_end(T start_time) {
	auto end_time = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end_time - start_time).count();
	return duration;
}

static std::chrono::_V2::system_clock::time_point g_start_time;
static size_t g_measure_ms;

void begin_measure() {
	g_start_time = get_time_start();
}

size_t end_measure() {
	return (g_measure_ms = get_time_end(g_start_time));
}

float getSeconds(size_t ms) {
	return ms/1000.0f;
}

float get_test_time() {
	return getSeconds(g_measure_ms);
}


typedef mew::stack<int> lvl_1;
typedef mew::stack<lvl_1> lvl_2;

void test_1(lvl_2& stack) {

	for (int i = 0; i < 40; ++i) {
		lvl_1 _st;
		for (int x = i; x < i+20; ++x) {
			_st.push(x);
		}
		// printf("%i\n", i);
		stack.push(_st);
		// printf("%i\n", i);
	}
}


struct Foo {
	mew::stack<int> _st;
	int _x;

	Foo(){}
	Foo(int x): _x(x) {}
	Foo(const Foo& other) {
		_x = other._x;
		_st = other._st;
	}
	Foo(Foo&& other) noexcept {
		_x = other._x;
		_st = std::move(other._st);
	}
};

double fractal(double x) {
	return x*x + x;
}

int main() {
	lvl_2 stack;
	printf("Stack test\n");
	printf("----PRIMITIVE TEST----\n");
	test_1(stack);

	for (auto& st: stack) {
		for (auto& el: st) {
			printf("%i ", el);
		}
	}
	
	printf("\n----END PRIMITIVE TEST----\n");
	printf("----CUSTOM OBJECT TEST----\n");
	mew::stack<Foo> foo_stack;
	Foo foo;
	foo._x = 10;
	foo._st.push(1);
	foo._st.push(2);
	foo_stack.push(foo);
	Foo foo2;
	foo2._x = 12;
	foo2._st.push(4);
	foo2._st.push(5);
	foo_stack.push(foo2);
	foo_stack.emplace(44);
	for (auto& f: foo_stack) {
		printf("Foo x: %i\n", f._x);
		for (auto& el: f._st) {
			printf("%i ", el);
		}
		printf("\n");
	}
	printf("----END CUSTOM OBJECT TEST----\n");

	printf("----FRACTAL TEST----\n");

	constexpr const int count = 1000000;
	mew::stack<double, mew::AllocatorBase<double, count/4>> big_stack;
	big_stack.push(fractal(0.1f));
	for (int i = 0; i < count; ++i) {
		big_stack.push(fractal(big_stack.top()));
		if (big_stack.top() >= INFINITE) {
			big_stack.pop();
			break;
		}
	}

	// for (auto& el: big_stack) {
	// }
	printf("END: %llf ", big_stack.top());

	printf("\n----END FRACTAL TEST----\n");
}