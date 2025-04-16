#include "mewlib.h"
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

int main() {
	int* i = (int*)mew::mem::alloc(sizeof(int));
	*i = 10;
	printf("1: %i\n", *i);
	mew::mem::dealloc(i);
	printf("2: %i\n", *i);

	// typedef mew::stack<int> lvl_1;
	// typedef mew::stack<lvl_1> lvl_2;
	// lvl_2 stack;
	// for (int i = 0; i < 40; ++i) {
	// 	lvl_1 _st;
	// 	for (int x = i; x < i+20; ++x) {
	// 		_st.push(x);
	// 	}
	// 	printf("%i\n", i);
	// 	stack.push(_st);
	// 	printf("%i\n", i);
	// }
	// printf("--- Mew Stack Test ---\n");
	// constexpr const size_t count = 100'000;
	// float time_s; 

	// std::cout << "Standart allocator test: ";
	// if (count < 500'000) {
	// 	mew::stack<size_t> standart_alloc;
	// 	begin_measure();
	// 		for (size_t i = 0; i < count; ++i) {
	// 			standart_alloc.push(i);
	// 		}
	// 	end_measure();
	// 	time_s = get_test_time();
	// 	std::cout << time_s << "ms" << std::endl;	
	// } else {
	// 	printf("skipped\n");
	// }
	// std::cout << "Big allocator test: ";
	// if (count <= 1'000'000) {
	// 	mew::stack<size_t, mew::BigAllocator<size_t>> big_alloc;
	// 	begin_measure();
	// 		for (size_t i = 0; i < count; ++i) {
	// 			big_alloc.push(i);
	// 		}
	// 	end_measure();
	// 	time_s = get_test_time();
	// 	std::cout << time_s << "ms" << std::endl;
	// } else {
	// 	printf("skipped\n");
	// }
	// std::cout << "Mid allocator test: ";
	// if (count < 500'000) {
	// 	mew::stack<size_t, mew::MidAllocator<size_t>> mid_alloc;
	// 	begin_measure();
	// 		for (size_t i = 0; i < count; ++i) {
	// 			mid_alloc.push(i);
	// 		}
	// 	end_measure();
	// 	time_s = get_test_time();
	// 	std::cout << time_s << "ms" << std::endl;
	// } else {
	// 	printf("skipped\n");
	// }
	// std::cout << "Large allocator test: ";
	// mew::stack<size_t, mew::LargeAllocator<size_t>> large_alloc;
	// begin_measure();
	// 	for (size_t i = 0; i < count; ++i) {
	// 		large_alloc.push(i);
	// 	}
	// end_measure();
	// time_s = get_test_time();
	// std::cout << time_s << "ms" << std::endl;
}