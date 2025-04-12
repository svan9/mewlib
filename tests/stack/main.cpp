#include "mewlib.h"
#include "mewstack.hpp"
#include "mewallocator.hpp"

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
	printf("--- Mew Stack Test ---\n");
	constexpr const size_t count = 1'000'000;
	float time_s; 

	std::cout << "Standart allocator test: ";
	if (count < 500'000) {
		mew::stack<size_t> standart_alloc;
		begin_measure();
			for (size_t i = 0; i < count; ++i) {
				standart_alloc.push(i);
			}
		end_measure();
		time_s = get_test_time();
		std::cout << time_s << "ms" << std::endl;	
	} else {
		printf("skipped\n");
	}
	std::cout << "Big allocator test: ";
	if (count <= 1'000'000) {
		mew::stack<size_t, mew::BigAllocator<size_t>> big_alloc;
		begin_measure();
			for (size_t i = 0; i < count; ++i) {
				big_alloc.push(i);
			}
		end_measure();
		time_s = get_test_time();
		std::cout << time_s << "ms" << std::endl;
	} else {
		printf("skipped\n");
	}
	std::cout << "Mid allocator test: ";
	if (count < 500'000) {
		mew::stack<size_t, mew::MidAllocator<size_t>> mid_alloc;
		begin_measure();
			for (size_t i = 0; i < count; ++i) {
				mid_alloc.push(i);
			}
		end_measure();
		time_s = get_test_time();
		std::cout << time_s << "ms" << std::endl;
	} else {
		printf("skipped\n");
	}
	std::cout << "Large allocator test: ";
	mew::stack<size_t, mew::LargeAllocator<size_t>> large_alloc;
	begin_measure();
		for (size_t i = 0; i < count; ++i) {
			large_alloc.push(i);
		}
	end_measure();
	time_s = get_test_time();
	std::cout << time_s << "ms" << std::endl;
}