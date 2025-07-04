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

#include <cctype>

#ifndef MEW_NO_RELEASE
	#ifndef DEBUG
		#define RELEASE
	#endif
#endif

#if defined(__cplusplus) && __cplusplus >= 202002L
	#define __CXX20
#endif

// #if defined(__cplusplus) && defined(MEW_USE_THROWS) 
// #undef MEW_USE_THROWS
// #endif 

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

#if !defined(MEW_NOTUSE_THROWS) && __cplusplus
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
#define MewWarnMessage(fmt) printf("-- MEW WARN " fmt " --\n")

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
	#define MEW_CEIL(val) ((float)((int)(val)))
	#define MEW_ROUND(val) ((float)((int)((val)>0?(val)+0.5f:(val)-0.5f)))

	#define MEW_RANGE_EQ_PRT(lexpr, rexpr, range) ()

	#define MEW_RANGE_EQ(lexpr, rexpr, range) (  \
			(lexpr) == (rexpr) ||                    \
			MEW_IN_RANGE((lexpr)-(range), (lexpr)+(range), rexpr))
#endif
#define MEW_SBOOL(expr) ((expr)? "true": "false")

#define MEW_UNUSE(expr) (void)(expr)

#include <string.h>

char* scopy(const char* str, size_t len) {
	char* out = (char*)malloc(len+1);
	memcpy(out, str, len);
	out[len] = '\0';
	return out;
}

wchar_t* scopy(const wchar_t* str, size_t len) {
	wchar_t* out = (wchar_t*)malloc((len+1) * sizeof(wchar_t));
	memcpy(out, str, len*sizeof(wchar_t));
	out[len] = L'\0';
	return out;
}

void* rcopy(void* ptr, size_t len) {
	void* out = (char*)malloc(len);
	memcpy(out, ptr, len);
	return out;
}

char getLastChar(const char* str) {
	size_t len = strlen(str);
	if (len == 0) {
		return '\0'; // or handle empty string case as needed
	}
	return str[len - 1];
}

char* strtrim(const char* str, size_t size) {
	const char* end;
	size_t out_size;

	// Trim leading space
	while (isspace((unsigned char)*str)) str++;

	if (*str == 0)  // All spaces?
		return scopy("", 0);

	// Trim trailing space
	end = str + size - 1;
	while (end > str && isspace((unsigned char)*end)) end--;
	end++;

	// Set output size to minimum of trimmed string length and buffer size minus 1
	out_size = end - str;

	return scopy(str, out_size);
}

char* strtrim(const char* str) {
	return strtrim(str, strlen(str));
}

char* scopy(const char* str) {
	return scopy(str, strlen(str));
}

wchar_t* scopy(const wchar_t* str) {
	return scopy(str, wcslen(str));
}

const char* btoi(bool b) {
	return b ? "true" : "false";
}

const char* ValueOrDefault(const char* v, const char* d) {
	if (v) return v;
	return d;
}

#ifdef __cplusplus
#define readonly(__type) const (__type) &
#define nonull(__v) ((__v)!=nullptr)
#else
#define readonly(__type) (__type)
#define nonull(__v) ((__v)!=NULL)
#endif

#ifdef __cplusplus
template<typename T>
T* tmalloc() {
	return (T*)(malloc(sizeof(T)));
}
	#define MewPrintError(_error) printf("\nErrored from %s:%i at function `%s(...)`\n\twhat: %s", __FILE__, __LINE__, __func__, (_error).what());
#if defined(_WIN32)
# define _GLIBCXX_FILESYSTEM_IS_WINDOWS 1
#endif
	#include <string>
	#include <filesystem>
	#include <wchar.h>
	#include <fstream>
#ifdef __CXX20
	#include <concepts>
#endif

	#include <codecvt>

typedef char* data_t;
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned int uint;
typedef long long int lli;
namespace mew {
	namespace std_fs = std::filesystem;
	typedef char* data_t;
	#ifdef __cplusplus
	typedef unsigned char byte;
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

	bool is_valid_utf8(const std::string& str) {
	  try {
    	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    	converter.from_bytes(str);
    	return true;
  	} catch (...) {
    	return false;
  	}
	}
	bool is_valid_utf8(const char* str) {
	  try {
    	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    	converter.from_bytes(str);
    	return true;
  	} catch (...) {
    	return false;
  	}
	}

	std::wstring stringToWstring(const std::string& str) {
		// std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring out;
		for(char c: str) {
			out += (wchar_t)c;
		}
		return ;
	}


	std::string ReadFile(std_fs::path& path) {
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
		file.close();
		return out;
	}

	const char* ReadFileAbs(const char* path) {
		constexpr const size_t read_size = 4096UL;
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
		file.close();
		return scopy(out.c_str());
	}

	bool is_safe_path_char(char c) {
    // Allow basic ASCII plus some Windows special chars
    return (c >= 32 && c <= 126) || c == '\\' || c == ':' || c == '.';
	}

	std::string sanitize_path(const std::string& input) {
    std::string output;
    for (char c : input) {
        if (is_safe_path_char(c)) {
            output += c;
        }
    }
    return output;
	}
	std::string ReadFile(const char* path) {
		std::string str(path);
		str = sanitize_path(str);
		// MewUserAssert(is_valid_utf8(str), "Invalid Path");
		std_fs::path __path(str);
		if (!__path.is_absolute()) {
			__path = std_fs::absolute(__path.lexically_normal());
		}
		return ReadFile(__path);
	}
	
	std::string ReadFile(const wchar_t* path) {
		std_fs::path __path(path);
		if (!__path.is_absolute()) {
			__path = std_fs::absolute(__path.lexically_normal());
		}
		return ReadFile(__path);
	}

	std_fs::path get_path(const char* path) {
		std_fs::path __path(path);
		if (!__path.is_absolute()) {
			__path = std_fs::absolute(__path.lexically_normal());
		}
		return __path;
	}

	std_fs::path get_path(const wchar_t* path) {
		std_fs::path __path(path);
		if (!__path.is_absolute()) {
			__path = std_fs::absolute(__path.lexically_normal());
		}
		return __path;
	}

	char* wchar_to_char(const wchar_t* wstr) {
		size_t len = wcslen(wstr);
		size_t convertedLen = wcstombs(nullptr, wstr, 0);
		if (convertedLen == (size_t)-1) {
			return nullptr; // Conversion failed
		}
		char* cstr = (char*)malloc(convertedLen + 1);
		wcstombs(cstr, wstr, convertedLen + 1);
		return cstr;
	}

	wchar_t* char_to_wchar(const char* wstr) {
		size_t len = strlen(wstr);
		wchar_t* cstr = new wchar_t[len+1];
		for (int i = 0; i < len; ++i) {
			cstr[i] = (wchar_t)wstr[i];
		}
		cstr[len] = L'\0';
		return cstr;
	}


	const char* get_file_name(std_fs::path& path) {
		return scopy(wchar_to_char(path.filename().c_str()));
	}

	const char* ReadFullFile(const char* path) {
		std::string content = ReadFile(path);
		return scopy(content.c_str(), content.size());
	}

	const char* ReadFullFileAbs(const char* path) {
		const char* content = ReadFileAbs(path);
		return content;
	}

	const char* ReadFullFile(const wchar_t* path) {
		std::string content = ReadFile(path);
		return scopy(content.c_str(), content.size());
	}

	const char* ReadFullFile(std_fs::path& path) {
		std::string content = ReadFile(path);
		return scopy(content.c_str(), content.size());
	}

	const char* ReadString(std::ifstream& file) {
		std::string result;
		char ch;
		while (file.get(ch)) {
			if (ch == '\0') {
				break;
			}
			result += ch;
		}
		return scopy(result.c_str());
	}

	std::ifstream getIfStream(std_fs::path& path) {
		std::ifstream file(path, std::ios::in);
    MewAssert(file.is_open());
    file.seekg(std::ios::beg);
    file >> std::noskipws;
		return file;
	}
	int readInt4Bytes(std::ifstream& file) {
		int value;
		file.read(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}
	template<typename T>
	void readBytes(std::ifstream& file, T& out) {
		file.read(reinterpret_cast<char*>(&out), sizeof(out));
	}

	template<typename T>
	void readSeqBytes(std::ifstream& file, T* out, size_t size) {
		file.read((char*)(out), size);
	}

	template<typename T>
	void writeBytes(std::ofstream& file, T& content, size_t size) {
		file.write((char*)(&content), size);
	}

	template<typename T>
	void writeBytes(std::ofstream& file, const T& content) {
		file.write((char*)(&content), sizeof(content));
	}

	template<typename T>
	void writeBytes(std::ofstream& file, T& content) {
		file.write((char*)(&content), sizeof(content));
	}

	void writeString(std::ofstream& file, const char* str) {
		file.write(str, strlen(str)+1);
	}

	template<typename T>
	void writeSeqBytes(std::ofstream& file, T* content, size_t size) {
		for (int i = 0; i < size; ++i) {
			writeBytes(file, content[i]);
		}
	}

	std::ifstream getIfStream(const char* dir, const char* file) {
		std_fs::path __path(dir);
		__path /= file;
		if (!__path.is_absolute()) {
			__path = std_fs::absolute(__path.lexically_normal());
		}
		return getIfStream(__path);
	}
	
	std::ifstream getIfStream(const char* path) {
		std_fs::path __path(path);
    if (!__path.is_absolute()) {
      __path = std_fs::absolute(__path.lexically_normal());
    }
    return getIfStream(__path);
	}

	const char* concatPath(const char* first, const char* second) {
		std_fs::path __path(first);
		__path = __path / (second[0] == '/' ? second+1 : second);
		auto ref = __path.string();
		return scopy(ref.c_str(), ref.size());
	}
	
	bool strcmp(const char* s, const char* m, size_t size) {
		for (int i = 0; i < size; ++i) {
			if (s[i] != m[i]) {
				return false;
			}
		}
		return true;
	}
	bool strcmp(const char* s, const char* m) {
		size_t s1 = strlen(s);
		size_t s2 = strlen(m);
		if (s1 < s2) return false;
		for (int i = 0; i < s2; ++i) {
			if (s[i] != m[i]) {
				return false;
			}
		}
		return true;
	}
	bool strcmp(const wchar_t* s, const wchar_t* m, size_t size) {
		for (int i = 0; i < size; ++i) {
			if (s[i] != m[i]) {
				return false;
			}
		}
		return true;
	}
	bool strcmp(const wchar_t* s, const wchar_t* m) {
		size_t s1 = wcslen(s);
		size_t s2 = wcslen(m);
		if (s1 < s2) return false;
		for (int i = 0; i < s2; ++i) {
			if (s[i] != m[i]) {
				return false;
			}
		}
		return true;
	}

	bool starts_with(const char* l, const char* m) {
		return strcmp(l, m, strlen(m));
	}

	bool iscmpstr(char l, const char* m) {
		while (*m != 0) if (l == *m++) return true;
		return false;
	}

	bool iscmpstr(wchar_t l, const wchar_t* m) {
		while (*m != 0) if (l == *m++) return true;
		return false;
	}

#endif	
}

#endif
#endif

#endif