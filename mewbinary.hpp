#ifndef MEW_ALLOC_LIB_SO2U
#define MEW_ALLOC_LIB_SO2U

#include "mewlib.h"
#include "mewstack.hpp"
#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace mew::binary {

typedef char*(*proc_t)(void* data, size_t* size);
typedef bool(*asserter_t)(void*, size_t);

struct SBT_Part {
	const char* type;
	asserter_t assrt;
	proc_t proc;
};

class SBT {
private:
	mew::stack<SBT_Part, 8U> _M_proccess;
public:	
	////////////////////////////////////////////////////////////
	SBT() {
		MewNoImpl();
	}

	////////////////////////////////////////////////////////////
	bool write(std::ofstream& file, void* data, size_t size, SBT_Part& part) {
		if (!part.assrt(data, size)) { return false; }
		char* prt = part.proc(data, &size);
		file.write(prt, size);
		return true;
	}

	////////////////////////////////////////////////////////////
	void write(std::filesystem::path path, void* data) {
		std::ofstream file(path, std::ios::out);
		MewAssert(file.is_open());
		
	}

	////////////////////////////////////////////////////////////
	void write(const char* path, void* data) {
		std::filesystem::path __path(path);
    if (!__path.is_absolute()) {
      __path = std::filesystem::absolute(__path.lexically_normal());
    }
    return write(__path, data);
	}
	
	////////////////////////////////////////////////////////////
	friend SBT& operator<<(SBT& sbt, SBT_Part& part) {
		sbt._M_proccess.push(part);
		return sbt;
	}
};

};

namespace Tests {
	void test_mew_binary_sbg() {
		using namespace mew::binary;
		// sbt << (SBT_Part){ };
	}
};

#endif