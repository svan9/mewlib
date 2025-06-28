#ifndef MEW_FS_LIB_SO2U
#define MEW_FS_LIB_SO2U

#include "mewlib.h"
#include "mewtypes.h"
#include "mewutils.hpp"
#include "mewthread.hpp"
#include "mewisolate.hpp"
#include <sys/types.h>
#include <sys/stat.h>

#include "mewpack"
#ifndef _WIN32
#include <unistd.h>
#endif
#ifdef _WIN32
#define stat _stat64
#include <fileapi.h>
// extern dword GetFullPathNameA(cstr lpFileName, dword nBufferLength, cstr lpBuffer, cstr *lpFilePart);
// extern dword GetFullPathNameW(cwstr lpFileName, dword nBufferLength, cwstr lpBuffer, wstr *lpFilePart);

#endif

#ifndef PATH_MAX

#ifdef _WIN32
#define PATH_MAX 260
#else
#define PATH_MAX 512
#endif



#endif

namespace mew::fs {

	#ifdef _WIN32
		const char* GetAbsolutePath(const char* path) {
			char* abs_path = new char[PATH_MAX];
			if (GetFullPathNameA((char*)path, PATH_MAX, abs_path, nullptr) == 0) {
				delete[] abs_path;
				return nullptr; // Failed to get absolute path
			}
			return abs_path;
		}

		const wchar_t* GetAbsolutePath(const wchar_t* path) {
			wchar_t* abs_path = new wchar_t[PATH_MAX];
			if (GetFullPathNameW(path, PATH_MAX, abs_path, nullptr) == 0) {
				delete[] abs_path;
				return nullptr; // Failed to get absolute path
			}
			return abs_path;
		}
	
		struct _stat64 unsafe_GetStat(const char* path) {
			struct _stat64 buffer;
			if (_stat64(path, &buffer) == 0) {
				return buffer;
			} else {
				MewUserAssert(false, "Failed to get file stats");
				return {};
			}
		}
		struct _stat64 unsafe_GetStat(const wchar_t* path) {
			struct _stat64 buffer;
			if (_wstat64(path, &buffer) == 0) {
				return buffer;
			} else {
				MewUserAssert(false, "Failed to get file stats");
				return {};
			}
		}
		struct _stat64 GetStat(const char* path, bool* is_exist = nullptr) {
			struct _stat64 buffer;
			if (_stat64(path, &buffer) == 0) {
				if (is_exist) {
					*is_exist = true;
				}
				return buffer;
			}
			if (is_exist) {
				*is_exist = false;
			}
			return {};
		}
		struct _stat64 GetStat(const wchar_t* path, bool* is_exist = nullptr) {
			struct _stat64 buffer;
			if (_wstat64(path, &buffer) == 0) {
				if (is_exist) {
					*is_exist = true;
				}
				return buffer;
			}
			if (is_exist) {
				*is_exist = false;
			}
			return {};
		}
		
		bool IsExist(const char* path) {
			struct _stat64 buffer;
			return (_stat64(path, &buffer) == 0);
		}
		bool IsExist(const wchar_t* path) {
			struct _stat64 buffer;
			return (_wstat64(path, &buffer) == 0);
		}
	#else
		
		const char* GetAbsolutePath(const char* path) {
			char* abs_path = new char[PATH_MAX];
			if (realpath(path, abs_path) == nullptr) {
				delete[] abs_path;
				return nullptr; // Failed to get absolute path
			}
			return abs_path;
		}

		const wchar_t* GetAbsolutePath(const wchar_t* path) {
			return char_to_wchar(GetAbsolutePath(wchar_to_char(path)));
		}

		struct stat GetStat(const char* path) {
			struct stat buffer;
			if (stat(path, &buffer) == 0) {
				return buffer;
			} else {
				MewUserAssert(false, "Failed to get file stats");
				return {};
			}
		}
		bool IsExist(const wchar_t* path) {
			struct stat buffer;
			return (stat(path, &buffer) == 0);
		}
		
	#endif

	struct FileStats {
		mew::Date last_update;

		FileStats() {}
	};

	
	
	class Path {
	public:
		#ifdef _WIN32
				typedef wchar_t PathSymbol;
		#elif defined(__linux) 
				typedef char PathSymbol;		
		#endif
	private:
		PathSymbol* m_path;
		FileStats last_stats;
		PathSymbol* m_splited_path;
	public:
		Path() {}
		Path(const PathSymbol* path): m_splited_path(utils::str_split(path, '/')), m_path(scopy(path)) {}
		
#ifdef _WIN32
		Path(const char* path): Path(char_to_wchar(path)) {
			auto _path = char_to_wchar(path);
			m_path = scopy(_path);
		}		
#endif
		
		// Check if the path exists
		bool IsExist() { 
			return ::mew::fs::IsExist(m_path);
		}
		
		bool IsFile() { 
			bool is_exist;
			auto stat_ = ::mew::fs::GetStat(m_path, &is_exist);
			MewUserAssert(is_exist, "path not exist");
			return (stat_.st_mode & S_IFMT) == S_IFREG;
		}
		
		bool IsFolder() { 
			bool is_exist;
			auto stat_ = ::mew::fs::GetStat(m_path, &is_exist);
			MewUserAssert(is_exist, "path not exist");
			return (stat_.st_mode & S_IFMT) == S_IFDIR;
		}
		
		// todo
		bool IsDisk() { return true; }
		
		FileStats GetStats() {
			bool is_exist;
			auto stat_ = ::mew::fs::GetStat(m_path, &is_exist);
			MewUserAssert(is_exist, "path not exist");
			MewUserAssert((stat_.st_mode & S_IFMT) == S_IFREG, "is not a file");
			FileStats file_stat;
			struct tm _time;
			MewUserAssert(_localtime64_s(&_time, &stat_.st_mtime) != 0, "Failed to convert time");
			file_stat.last_update = mew::Date(&_time);
			return file_stat;
		}
		
		// todo DEBUG
		// void WatchUpdates(callable clb) {
		// 	mew::watchBool([this]() {
		// 		auto tpu = this->last_stats;
		// 		auto tcu = this->GetStats();
		// 		return tpu.last_update != tcu.last_update;
		// 	}, clb);
		// }

		const PathSymbol* GetPath() const {
			return m_path;
		}
		const PathSymbol* GetSplitedPath() const {
			return m_splited_path;
		}
		const PathSymbol* GetFileName() const {
			if (!m_splited_path) return nullptr;
			const PathSymbol* last_slash = m_splited_path;
			while (*last_slash) {
				if (*last_slash == '/') {
					last_slash++;
				} else {
					break;
				}
			}
			return last_slash;
		}
		const PathSymbol* GetFileNameWithoutExt() const {
			const PathSymbol* file_name = GetFileName();
			if (!file_name) return nullptr;
			const PathSymbol* dot = file_name;
			while (*dot) {
				if (*dot == '.') {
					break;
				}
				dot++;
			}
			size_t len = dot - file_name;
			return scopy(file_name, len);
		}

		const PathSymbol* GetExtension() const {
			const PathSymbol* file_name = GetFileName();
			if (!file_name) return nullptr;
			const PathSymbol* dot = file_name;
			while (*dot) {
				if (*dot == '.') {
					dot++;
					return dot;
				}
				dot++;
			}
			return nullptr; // No extension found
		}

		const PathSymbol* GetDirectory() const {
			if (!m_path) return nullptr;
			const PathSymbol* last_slash = m_path;
			while (*last_slash) {
				if (*last_slash == '/') {
					last_slash++;
				} else {
					break;
				}
			}
			size_t len = last_slash - m_path;
			return scopy(m_path, len);
		}

		static const char* GetAbsolutePath(const char* path) {
			fs::Path p(path);
			if (!p.IsExist()) {
				return nullptr; // Path does not exist
			}
			return ::mew::fs::GetAbsolutePath(path);
		}

		static const wchar_t* GetAbsolutePath(const wchar_t* path) {
			fs::Path p(path);
			if (!p.IsExist()) {
				return nullptr; // Path does not exist
			}
			return ::mew::fs::GetAbsolutePath(path);
		}

		Path& Absolute() {
			if (m_path) {
				m_path = (PathSymbol*)GetAbsolutePath(m_path);
			}
			return *this;
		}

		Path& operator=(const PathSymbol* path) {
			if (m_path) {
				free(m_path);
			}
			m_path = scopy(path);
			if (m_splited_path) {
				free(m_splited_path);
			}
			m_splited_path = utils::str_split(m_path, '/');
			return *this;
		}
		Path& operator=(const char* path) {
			return operator=(char_to_wchar(path));
		}
#ifndef _WIN32
		Path& operator=(const wchar_t* path) {
			if (m_path) {
				free(m_path);
			}
			m_path = scopy(path);
			if (m_splited_path) {
				free(m_splited_path);
			}
			m_splited_path = utils::str_split(m_path, '/');
			return *this;
		}
#endif
		Path& operator=(const Path& other) {
			if (this == &other) return *this; // self-assignment check
			if (m_path) {
				free(m_path);
			}
			m_path = scopy(other.m_path);
			if (m_splited_path) {
				free(m_splited_path);
			}
			m_splited_path = scopy(other.m_splited_path);
			return *this;
		}

		~Path() {
			if (m_path) {
				free(m_path);
			}
			if (m_splited_path) {
				free(m_splited_path);
			}
		}

		friend bool operator==(const Path& l, const Path& r) {
			return mew::strcmp(l.m_path, r.m_path) == 0;
		}

		friend bool operator!=(const Path& l, const Path& r) {
			return !(l == r);
		}
		friend bool operator<(const Path& l, const Path& r) {
			return mew::strcmp(l.m_path, r.m_path) < 0;
		}
		friend bool operator>(const Path& l, const Path& r) {
			return mew::strcmp(l.m_path, r.m_path) > 0;
		}
		friend bool operator<=(const Path& l, const Path& r) {
			return !(l > r);
		}
		friend bool operator>=(const Path& l, const Path& r) {
			return !(l < r);
		}
		#ifdef _WIN32
		friend std::ostream& operator<<(std::ostream& os, const Path& path) {
			os << wchar_to_char(path.m_path);
			return os;
		}
		friend std::wostream& operator<<(std::wostream& os, const Path& path) {
			os << path.m_path;
			return os;
		}
		#elif defined(__linux)
		friend std::ostream& operator<<(std::ostream& os, const Path& path) {
			os << path.m_path;
			return os;
		}
		friend std::wostream& operator<<(std::wostream& os, const Path& path) {
			os << char_to_wchar(path.m_path);
			return os;
		}
		#endif



		Path& append(const wchar_t* other) {
			if (!other || !*other) return *this; // No change if other is empty
			size_t len = wcslen(m_path);
			size_t other_len = wcslen(other);
			wchar_t* new_path = (wchar_t*)malloc(len + other_len + 2); // +1 for '/' and +1 for '\0'
			wcscpy(new_path, m_path);
			if (new_path[len - 1] != '/') {
				new_path[len++] = '/';
			}
			wcscpy(new_path + len, other);
			free(m_path);
			m_path = new_path;
			if (m_splited_path) {
				free(m_splited_path);
			}
			m_splited_path = utils::str_split(m_path, '/');
			return *this;
		}
#ifdef _WIN32
		Path& append(const char* other) {
			return append(char_to_wchar(other));
		}
#else
		Path& append(const char* other) {
			if (!other || !*other) return *this; // No change if other is empty
			size_t len = strlen(m_path);
			size_t other_len = strlen(other);
			char* new_path = (char*)malloc(len + other_len + 2); // +1 for '/' and +1 for '\0'
			strcpy(new_path, m_path);
			if (new_path[len - 1] != '/') {
				new_path[len++] = '/';
			}
			strcpy(new_path + len, other);
			free(m_path);
			m_path = new_path;
			if (m_splited_path) {
				free(m_splited_path);
			}
			m_splited_path = utils::str_split(m_path, '/');
			return *this;
		}
#endif

		Path& operator/(const PathSymbol* other) {
			return append(other);
		}
#ifdef _WIN32
		Path& operator/(const char* other) {
			return append(other);
		}
#endif

		
		
	};

	class IOFile {
	private:
		Path m_path;
		IOFlags m_flags;
		size_t m_size = 0;
		size_t m_cursor = 0;
		bool m_is_open = false;
		RawFile m_raw_file;

		static Isolate* GetIsolate() {
			static Isolate* m_isolate = nullptr;
			if (!m_isolate) {
				m_isolate = new Isolate();
			}
			return m_isolate;
		}

		
	public:
		IOFile() {}
		
		IOFile(const Path& path): m_path(path) {}
		IOFile(Path&& path): m_path(path) {}
		IOFile(const Path::PathSymbol* path): m_path(path) {}
#ifdef _WIN32
		IOFile(const char* path): m_path(path) {}
#endif
		IOFile(const Path& path, const char* flags): m_path(path), m_flags(flags) {}
		IOFile(Path&& path, const char* flags): m_path(path), m_flags(flags) {}
		IOFile(const Path::PathSymbol* path, const char* flags): m_path(path), m_flags(flags) {}
#ifdef _WIN32
		IOFile(const char* path, const char* flags): m_path(path), m_flags(flags) {}
#endif

		IOFile(const Path& path, IOFlags&& flags): m_path(path), m_flags(flags) {}
		IOFile(const Path& path, const IOFlags& flags): m_path(path), m_flags(flags) {}
		IOFile(Path&& path, IOFlags&& flags): m_path(path), m_flags(flags) {}
		IOFile(Path&& path, const IOFlags& flags): m_path(path), m_flags(flags) {}
		IOFile(const Path::PathSymbol* path, const IOFlags& flags): m_path(path), m_flags(flags) {}
#ifdef _WIN32
		IOFile(const char* path, IOFlags&& flags): m_path(path), m_flags(flags) {}
		IOFile(const char* path, const IOFlags& flags): m_path(path), m_flags(flags) {}
#endif

		const Path& GetPath() const { return m_path; }
		const IOFlags& GetFlags() const { return m_flags; }
		const bool IsOpen() const { return m_is_open; }
		
		void Open() {
			if (m_flags.IsIsolate) {
				GetIsolate()->on();
			}
			m_raw_file = GetIsolate()->Open(m_path.GetPath(), m_flags.Raw());
			if (m_flags.IsIsolate) {
				GetIsolate()->off();
			}
			m_is_open = true;
		}

		void Open(const char* flags) {
			m_flags = IOFlags(flags);
			if (m_flags.IsIsolate) {
				GetIsolate()->on();
			}
			m_raw_file = GetIsolate()->Open(m_path.GetPath(), m_flags.Raw());
			if (m_flags.IsIsolate) {
				GetIsolate()->off();
			}
			m_is_open = true;
		}

		size_t Write(const char* content) {
			if (!IsOpen()) { Open(); }
			if (!m_flags.Write) {
				MewUserAssert(false, "File is not opened for writing");
				return 0;
			}
			if (m_flags.IsIsolate) {
				GetIsolate()->on();
			}
			size_t ws = GetIsolate()->Write(m_raw_file, content, strlen(content), m_cursor);
			if (m_flags.IsIsolate) {
				GetIsolate()->off();
			}
			return ws;
		}

		const char* Read(size_t size) {
			if (!IsOpen()) { Open(); }
			if (!m_flags.Read) {
				MewUserAssert(false, "File is not opened for reading");
				return nullptr;
			}
			if (m_flags.IsIsolate) {
				GetIsolate()->on();
			}
			char* buffer = new char[size + 1];
			GetIsolate()->Read(m_raw_file, buffer, size, m_cursor);
			if (m_flags.IsIsolate) {
				GetIsolate()->off();
			}
			return buffer;
		}

		size_t GetSize() {
			if (!IsOpen()) { Open(); }
			if (m_flags.IsIsolate) {
				GetIsolate()->on();
			}
			m_size = GetIsolate()->GetFileSize(m_raw_file);
			if (m_flags.IsIsolate) {
				GetIsolate()->off();
			}
			return m_size;
		}

		void Close() {
			if (m_flags.IsIsolate) {
				GetIsolate()->on();
			}
			GetIsolate()->Close(m_raw_file);
			if (m_flags.IsIsolate) {
				GetIsolate()->off();
			}
			m_is_open = false;
		}
 
		static void SaveIsolateToFile(const char* path) {
			GetIsolate()->on();
			auto raw = GetIsolate()->ToString();
			FILE* fp = fopen(path, "wb+");
			if (fp == nullptr) {
				MewUserAssert(false, "Failed to open file for writing");
				return;
			}
			size_t written = fwrite(raw.data(), 1, raw.size(), fp);
			fclose(fp);
			GetIsolate()->off();
		}
#ifdef _WIN32
		static void SaveIsolateToFile(const wchar_t* path) {
			auto raw = GetIsolate()->ToString();
			FILE* fp = _wfopen(path, L"wb+");
			if (fp == nullptr) {
				MewUserAssert(false, "Failed to open file for writing");
				return;
			}
			size_t written = fwrite(raw.data(), 1, raw.size(), fp);
			fclose(fp);
		}
#endif
	};
};

#include "mewpop"

#endif