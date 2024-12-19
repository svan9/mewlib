/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MewLib_IMPL
#define MewLib_IMPL

#include <stdlib.h>
#include <stdio.h>

#ifndef MEW_NO_RELEASE
	#define MEW_NO_RELEASE
#endif
#ifndef MEW_USE_THROWS
	#define MEW_USE_THROWS
#endif

#ifndef MEW_NO_RELEASE
	#ifndef DEBUG
		#define RELEASE
	#endif
#endif

#if defined(__cplusplus) && __cplusplus >= 202002L
	#define __CXX20
#endif

#if defined(__cplusplus) && defined(MEW_USE_THROWS) 
#undef MEW_USE_THROWS
#endif 

#ifndef NULLVAL
#define NULLVAL ((void)0)
#endif

#ifdef __cplusplus
	#include <exception>
#endif

#ifdef RELEASE
	#define __mewassert(strexpr, message, file, line, func) NULLVAL
	#define __mewassert_nm(strexpr, file, line, func) NULLVAL
	#define __mewassert_nm_t_bad(func, val, correct) NULLVAL
	#define __mewassert_nm_t_good(func, val, correct) NULLVAL
	#define __mewassert_r(strexpr, message, file, line, func) NULLVAL
	#define __mewassert_nm_r(strexpr, file, line, func) NULLVAL
#else
	#define __mewassert(strexpr, message, file, line, func) \
		printf("\nAssert failed at %s:%i, %s(...)\n  With expression (%s)\n  `%s`", file, line, func, strexpr, message);
	#define __mewassert_nm(strexpr, file, line, func) \
		printf("\nAssert failed at %s:%i, %s(...)\n  With expression (%s)\n", file, line, func, strexpr);
	#define __mewassert_r(strexpr, message, file, line, func) \
		"\nAssert failed at %s:%i, %s(...)\n  With expression (%s)\n  `%s`", file, line, func, strexpr, message
	#define __mewassert_nm_r(strexpr, file, line, func) \
		"\nAssert failed at %s:%i, %s(...)\n  With expression (%s)\n", file, line, func, strexpr
	#define __mewassert_nm_t_bad(func, val, correct) \
		putwchar(L'');printf("\033[31mTest failed %s(%i != %i) \033[0m\n", func, val, correct);
	#define __mewassert_nm_t_good(func, val, correct) \
		putwchar(L'');printf("\033[92mTest success %s(%i == %i)\033[0m\n", func, val, correct);

#endif

#define MewTest(value, correct) \
		if (!(value == correct)) { __mewassert_nm_t_bad(__func__, value, correct); }\
		else { __mewassert_nm_t_good(__func__, value, correct); }

// #define ProcMewStringEnum()
// #define MewStringEnum(...)

#if defined(MEW_USE_THROWS) && __cplusplus
	#define MewUserAssert(expr, message) \
		if (!(expr)) { throw std::runtime_error(message); }
	#define MewAssert(expr) \
		if (!(expr)) { throw std::runtime_error(#expr);}
#else
	#define MewUserAssert(expr, message) \
		if (!(expr)) {__mewassert(#expr, message, __FILE__, __LINE__, __func__); exit(1); }
	#define MewAssert(expr) \
		if (!(expr)) {__mewassert_nm(#expr, __FILE__, __LINE__, __func__); exit(1); }
#endif
#define MewNot() MewUserAssert(false, "not")
#define MewNoImpl() MewUserAssert(false, "not implemented")
#define MewNotImpl() MewUserAssert(false, "not implemented")
#define MewWarn(fmt, ...) printf("-- MEW WARN " fmt " --\n", __VA_ARGS__)

#ifndef DISABLE_MEW_PING
	#define __mew printf("-- mew at %s:%i, %s(...) --\n", __FILE__, __LINE__, __func__);
	#define _mew() __mew;
	#define MEWMEW __mew
	#define ____MEWMEW____ __mew
#endif

#ifndef DISABLE_MEW_MATH
	#define MEW_FIND_MASK(_val, _flag) (((_val) & (_flag)) == (_flag))
	#ifdef __cplusplus
		#define MEW_ADD_MASK(_val, _flag) (decltype(_val))((int)(_val) | (int)(_flag))
	#else
		#define MEW_ADD_MASK(_val, _flag) ((_val) | (_flag))
	#endif

	bool __mew_bvtr(bool s) { return true; }

	#define MEW_ONE_OR_NONE(_el) (__mew_bvtr(_el))
	#define MEW_POW2(expr) ((expr)*(expr))
	#define MEW_IN_RANGE(min, max, value) ((min) <= (value) && (value) <= (max))
	#define MEW_RM_ALIGN(value, align) ((value) - ((value) % (align)))
	#define MEW_RM_ALIGN_A(value, align) ((value) + ((value) % (align)))

	#define MEW_RANGE_EQ_PRT(lexpr, rexpr, range) ()

	#define MEW_RANGE_EQ(lexpr, rexpr, range) (  \
			(lexpr) == (rexpr) ||                    \
			MEW_IN_RANGE((lexpr)-(range), (lexpr)+(range), rexpr))
#endif

#ifdef __cplusplus
	#define MewPrintError(_error) printf("\nErrored from %s:%i at function `%s(...)`\n\twhat: %s", __FILE__, __LINE__, __func__, (_error).what());

	#include <string.h>
	#include <string>
	#include <filesystem>
	#include <wchar.h>
	#include <fstream>
#ifdef __CXX20
	#include <concepts>
#endif

typedef char* data_t;
typedef unsigned char byte;
typedef unsigned int uint;
#ifdef __cplusplus
namespace mew {
#ifdef __CXX20
	template<typename>
	struct ClearType;

	template<typename T>
	struct ClearType<T*> {
		typedef T type;
	};

	template<typename T, size_t size>
	struct ClearType<T[size]> {
		typedef T type;
	};

	template<typename T>
	struct ClearType<const T> {
		typedef T type;
	};

	template<typename T>
	struct ClearType<const T&> {
		typedef T type;
	};

	template<typename T>
	struct ClearType<const T*> {
		typedef T type;
	};

	template<typename T>
	struct ClearType {
		typedef T type;
	};
	
	template<typename Base, typename Derived>
	concept baseof = std::is_base_of_v<ClearType<Base>, ClearType<Derived>>;

	template<typename VF, typename VS>
	concept same_as = std::same_as<ClearType<VF>, ClearType<VS>>;

	std::string ReadFile(std::filesystem::path& path) {
		constexpr const size_t read_size = 4096;
		std::ifstream file(path, std::ios::in);
    MewAssert(file.is_open());
    file.seekg(std::ios::beg);
    file >> std::noskipws;
		std::string out;
		std::string buf(read_size, '\0');
		while (file.read(&buf[0], read_size)) {
			out.append(buf, 0, file.gcount());
    }
		out.append(buf, 0, file.gcount());
		return out;
	}

	std::string ReadFile(const char* path) {
		std::filesystem::path __path(path);
    if (!__path.is_absolute()) {
      __path = std::filesystem::absolute(__path.lexically_normal());
    }
    return ReadFile(__path);
	}

	int strcmp(char* s, char* m, size_t size) {
		for (int i = 0; i < size; ++i) {
			if (s[i] != m[i]) {
				return 0;
			}
		}
		return 1;
	}
	
	int strcmp(const char* s, const char* m, size_t size) {
		for (int i = 0; i < size; ++i) {
			if (s[i] != m[i]) {
				return 0;
			}
		}
		return 1;
	}
#endif	
}

#endif
#endif

#endif