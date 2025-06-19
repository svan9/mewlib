#ifndef MEW_FS_LIB_SO2U
#define MEW_FS_LIB_SO2U

#include "mewlib.h"
#include "mewtypes.h"
#include "mewutils.hpp"
#include "mewthread.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#ifdef _WIN32
#define stat _stat64
#endif

namespace mew::fs {

	struct FileStats {
		mew::Date last_update;

		FileStats() {}
	};
	class Path {
	private:
#ifdef _WIN32
		typedef wchar_t PathSymbol;
#elif defined(__linux) 
		typedef char PathSymbol;		
#endif
		PathSymbol* m_path;
		FileStats last_stats;
		PathSymbol* m_splited_path;
	public:
		Path() {}
		Path(const PathSymbol* path): m_splited_path(utils::str_split(path, '/')), m_path(scopy(path)) {}
		
#ifdef _WIN32
		Path(const char* path): Path(char_to_wchar(path)) {}		
#endif
		
		// Check if the path exists
		bool IsExist() { 
			struct stat buffer;
			return (stat(m_path, &buffer) == 0);
		}
		
		bool IsFile() { 
			MewUserAssert(IsExist(), "path not exist");
			struct stat buffer;
			stat(m_path, &buffer);
			return (buffer.st_mode & S_IFMT) == S_IFREG;
		}
		
		bool IsFolder() { 
			MewUserAssert(IsExist(), "path not exist");
			struct stat buffer;
			stat(m_path, &buffer);
			return (buffer.st_mode & S_IFMT) == S_IFDIR;
		}
		
		// todo
		bool IsDisk() { return true; }
		
		FileStats GetStats() {
			MewUserAssert(IsFile(), "is not file path");
			struct __stat64 result;
			if (0 == stat(m_path, &result)) {
				FileStats file_stat;
				struct tm _time;
				_localtime64_s(&result.st_mtime, &_time);
				file_stat.last_update = mew::Date(&_time);
			}
			return FileStats{};
		}
		
		// todo DEBUG
		void WatchUpdates(callable clb) {
			mew::watchBool([this]() {
				auto tpu = this->last_stats;
				auto tcu = this->GetStats();
				return tpu.last_update != tcu.last_update;
			}, clb);
		}		
				
	};
		
};

#endif