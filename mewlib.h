/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MewLib_IMPL
#define MewLib_IMPL

#include "mewtypes.h"

char* scopy(const char* str, size_t len) {
	char* out = (char*)MEW_MALLOC(len+1);
	memcpy(out, str, len);
	out[len] = '\0';
	return out;
}

wchar_t* scopy(const wchar_t* str, size_t len) {
	wchar_t* out = (wchar_t*)MEW_MALLOC((len+1) * sizeof(wchar_t));
	memcpy(out, str, len*sizeof(wchar_t));
	out[len] = L'\0';
	return out;
}

void* rcopy(void* ptr, size_t len) {
	void* out = (char*)MEW_MALLOC(len);
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


typedef char* data_t;

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

	std_fs::path search_file_in_dir(const std_fs::path& directory, const std_fs::path& file_name) {
    auto d = std_fs::directory_iterator(directory);
		for (const auto& entry: d) {
			if (entry.path().filename() == file_name) {
				return entry.path();
			}
		}
		return std_fs::path();
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

	u64 readUInt64(std::ifstream& file) {
		u64 value;
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

	std::filesystem::path GetAbsPath(const char* path) {
		std_fs::path __path(path);
    if (!__path.is_absolute()) {
      __path = std_fs::absolute(__path.lexically_normal());
    }
    return __path;
	}

	bool is_exists(std_fs::path path) {
		return std_fs::exists(path);
	}

	bool is_exists(const char* path) {
		return is_exists(GetAbsPath(path));
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

	char* strjoin(const char* l, const char* r) {
		unsigned long int sizel = strlen(l);
		unsigned long int sizer = strlen(r);
		char* buffer = (char*)MEW_MALLOC(sizel+sizer + 1);
		memcpy(buffer, l, sizel);
		memcpy(buffer+sizel, r, sizer);
		buffer[sizel+sizer] = '\0';
		return buffer;
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

	unsigned long long int get_index(const char* str, char c) {
		const char* ptr = str;
		unsigned long long int index = 0;
		while (*ptr != '\0') {
			if (*ptr == c) {
				return index;
			}
			ptr++;
			index++;
		}
		return -1;
	}

	char* replace(char* original, char* from, char* to) {
		// todo
	}

	char* insert_str(const char* original, const char* to_insert, size_t position) {
		size_t original_len = strlen(original);
		size_t to_insert_len = strlen(to_insert);

		if (position > original_len) {
				position = original_len;
		}

		size_t new_len = original_len + to_insert_len;
		char* new_str = (char*)MEW_MALLOC(new_len + 1);

		// Copy the part before the insertion point
		memcpy(new_str, original, position);

		// Copy the string to insert
		memcpy(new_str + position, to_insert, to_insert_len);

		// Copy the part after the insertion point
		memcpy(new_str + position + to_insert_len, original + position, original_len - position);

		new_str[new_len] = '\0'; // Null-terminate the new string

		return new_str;
	}

	char* format(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		size_t size = vsnprintf(nullptr, 0, fmt, args) + 1; // +1 for null terminator
		va_end(args);

		char* buffer = (char*)MEW_MALLOC(size);
		va_start(args, fmt);
		vsnprintf(buffer, size, fmt, args);
		va_end(args);

		return buffer;
	}

	char* eraseChars(const char* str, size_t position, size_t count) {
		size_t original_len = strlen(str);
		if (position > original_len) {
			position = original_len;
		}
		if (position + count > original_len) {
			count = original_len - position;
		}
		size_t new_len = original_len - count;
		char* new_str = (char*)MEW_MALLOC(new_len + 1);

		// Copy the part before the erase point
		memcpy(new_str, str, position);

		// Copy the part after the erase point
		memcpy(new_str + position, str + position + count, original_len - position - count);

		new_str[new_len] = '\0'; // Null-terminate the new string

		return new_str;
	}


#endif	// __CXX20
}

#endif
#endif

#endif