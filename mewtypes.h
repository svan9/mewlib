/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEWTYPES_H
#define MEWTYPES_H

// #define MEW_NOTUSE_THROWS

#ifndef MEW_NO_RELEASE
	#define MEW_NO_RELEASE
#endif

#ifndef MEW_NO_RELEASE
	#ifndef DEBUG
		#define RELEASE
	#endif
#endif

#if defined(__cplusplus) && __cplusplus >= 202002L
	#define __CXX20
#endif

#ifndef NULLVAL
#define NULLVAL ((void)0)
#endif

#ifdef byte 
#undef byte
#endif

#ifdef __cplusplus
	#define MEW_MALLOC(size) new u8[(size)]
	#define MEW_NEW(type) new type()
#else
	#define MEW_MALLOC(size) malloc((size))
	#define MEW_NEW(type) (type*)malloc(sizeof(type))
#endif

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
	#define MEW_RANGE_EQ(lexpr, rexpr, range) ((lexpr) == (rexpr) || MEW_IN_RANGE((lexpr)-(range), (lexpr)+(range), rexpr))

#endif // DISABLE_MEW_MATH

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

#if !defined(MEW_NOTUSE_THROWS) && __cplusplus
	#define MewUserAssert(expr, message) \
		if (!(expr)) { throw mew::throws::simple(message); }
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
#define MewWarn(fmt, ...) printf(fmt, __VA_ARGS__)
#define MewForUserAssert(expr, fmt, ...) if(!(expr)) {printf("-- MEW WARN " fmt " --\n", __VA_ARGS__);}
#define MewWarnMessage(fmt) printf("-- MEW WARN " fmt " --\n")
#define MEW_FORMAT(fmt, ...) 

#define MEW_SBOOL(expr) ((expr) ? "true": "false")
#define MEW_UNUSE(expr) (void)(expr)

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <cctype>
#ifdef __cplusplus
	#include <exception>
	#include <string>
	#include <filesystem>
	#include <wchar.h>
	#include <fstream>
	#ifdef __CXX20
		#include <concepts>
	#endif
	#include <codecvt>
#endif // _cplusplus
#include <string.h>


typedef unsigned char byte;
typedef unsigned char u8;
typedef char i8;
typedef char s8;
typedef unsigned short u16;
typedef unsigned int uint;
typedef short i16;
typedef short s16;
typedef unsigned int u32;
typedef int i32;
typedef int s32;
typedef unsigned long long u64;
typedef long long i64;
typedef long long s64;
typedef unsigned long dword;
typedef char* cstr;
typedef wchar_t* wstr;
typedef const char* ccstr;
typedef const wchar_t* cwstr;

typedef __uint128_t u128;
typedef __int128_t i128;

typedef float f32;
typedef double f64;
typedef long double f128;

#define itoba(val) ((u8*)(u64)(val))
#define batou64(val) ((u64)(val))
#define batos32(val) ((s32)(u64)(val))
#define batou16(val) ((u16)(u64)(val))

#define bytepart(name) byte name: 1
#define bytepartf(name) byte name: 1 = false;
#define bytepartt(name) byte name: 1 = true;

typedef u128 ID;
#ifdef __cplusplus
ID makeID(u64 high, u64 low) {
	return (static_cast<ID>(high) << 64) | low;
}

typedef void(*callable)();
#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

#include <time.h>
#include <string.h>
#include <typeinfo>

#define MewLog(level, fmt, ...) \
  mew::LogManager::get().push(level, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

// Просто записать
#define MewInfo(fmt, ...) \
  MewLog(mew::LogLevel::INFO, fmt, ##__VA_ARGS__)

#define MewWarn(fmt, ...) \
  MewLog(mew::LogLevel::WARN, fmt, ##__VA_ARGS__)

#define MewCritical(fmt, ...) \
  MewLog(mew::LogLevel::CRITICAL, fmt, ##__VA_ARGS__)

// Условные
#define MewCriticalIf(cond, fmt, ...) \
  do { if (cond) { MewCritical(fmt, ##__VA_ARGS__); } } while(0)

#define MewWarnIf(cond, fmt, ...) \
  do { if (cond) { MewWarn(fmt, ##__VA_ARGS__); } } while(0)

#define MewInfoIf(cond, fmt, ...) \
  do { if (cond) { MewInfo(fmt, ##__VA_ARGS__); } } while(0)

// Assert-стиль (критично если НЕ выполняется)
#define MewAssertMsg(expr, fmt, ...) \
  MewCriticalIf(!(expr), fmt, ##__VA_ARGS__)

	#define MewFlush() \
  mew::LogManager::get().flush()

#include <vector>


namespace mew {
	namespace throws {
		class exception {
		public:
			exception() noexcept { }
			~exception() noexcept {}
			const char* what() const noexcept {return 0;}
		};

		class simple: public exception {
		private:
			const char* message = 0;
		public:
			simple(const char* message) noexcept
				: message(message)  {}
			~simple() noexcept {}

			const char* what() const noexcept {
				return message;
			}
		};
	}

	namespace mem {
		inline byte* dealloc(byte* ptr) {
			if (ptr) {
				delete[] ptr;
			}
			return nullptr;
		}
	
		inline byte* alloc(u64 size) {
			return new byte[size];
		}
		
		inline byte* realloc(byte* ptr,  u64 old_size, u64 size) {
			byte* new_ptr = alloc(size);
			memcpy(new_ptr, ptr, old_size);
			dealloc(ptr);
			return new_ptr;
		}

		class Manager {
		private:
			byte* memory = nullptr;
			u64 m_size = 0;
		public:	
			Manager() {}
			Manager(byte* ptr, u64 size): memory(ptr), m_size(size) {}
			Manager(Manager&& old): memory(old.memory), m_size(old.m_size) {}
			Manager(Manager& old): memory(old.memory), m_size(old.m_size) {old.memory = nullptr;}
			~Manager() {
				delete[] memory;
			}

			Manager& alloc(u64 size) {
				if (memory) {
					delete[] memory;
				}
				this->m_size = size;
				memory = new byte[m_size];
				memset(memory, 0, m_size);
				return *this;
			}

			Manager& resize(u64 new_size) {
				if (new_size == m_size) return *this;	
				byte* new_memory = new byte[new_size];
				if (!memory) {
					this->memory = new_memory;
					m_size = new_size;
					return *this;
				}
				if (new_size < m_size) {
					memcpy(new_memory, memory, new_size);
				} else {
					memcpy(new_memory, memory, m_size);
				}
				delete[] memory;
				m_size = new_size;
				memory = new_memory;
				return *this;
			}
			
			byte* operator*() {
				return memory;
			}
			byte* operator*(int) {
				return memory;
			}

			u64 size() const noexcept {
				return this->m_size;
			}
		};
	}
}



namespace mew {

// ─── Уровни ─────────────────────────────────────────────────
enum class LogLevel : uint8_t {
  INFO     = 0,
  WARN     = 1,
  CRITICAL = 2,
};

// ─── Запись лога ─────────────────────────────────────────────
struct LogEntry {
  LogLevel    level;
  std::string message;
  const char* file;
  int         line;
  const char* func;
};

// ─── Менеджер ────────────────────────────────────────────────
class LogManager {
private:
  LogManager() = default;
  std::vector<LogEntry> entries_;

  static const char* label(LogLevel l) {
    switch (l) {
      case LogLevel::INFO:     return "INFO";
      case LogLevel::WARN:     return "WARN";
      case LogLevel::CRITICAL: return "CRITICAL";
      default:                 return "?";
    }
  }

  static const char* color(LogLevel l) {
    switch (l) {
      case LogLevel::INFO:     return "\033[36m";  // cyan
      case LogLevel::WARN:     return "\033[33m";  // yellow
      case LogLevel::CRITICAL: return "\033[31m";  // red
      default:                 return "\033[0m";
    }
  }
public:
  static LogManager& get() {
    static LogManager instance;
    return instance;
  }

  void push(LogLevel level, const char* file, int line, const char* func, const char* fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    entries_.push_back({ level, buf, file, line, func });

    // INFO и WARN — только пишем в буфер
    // CRITICAL — немедленно печатаем всё накопленное и бросаем
    if (level == LogLevel::CRITICAL) {
      flush();
      throw mew::throws::simple(buf);
    }
  }

  // Вывести все накопленные записи (например при завершении)
  void flush() {
    if (entries_.empty()) return;
    printf("\n╔══════════════ MEW LOG DUMP ══════════════╗\n");
    for (auto& e : entries_) {
      const char* tag  = label(e.level);
      const char* col  = color(e.level);
      printf("║ %s[%-8s]%s %s\n", col, tag, "\033[0m", e.message.c_str());
      printf("║   at %s:%d  (%s)\n", e.file, e.line, e.func);
    }
    printf("╚══════════════════════════════════════════╝\n\n");
    entries_.clear();
  }

  ~LogManager() {
    flush(); // при завершении программы — вывести всё что осталось
  }


};

}


namespace mew {
	struct Date {
		u16 seconds;
		union {
			struct {
				u8 day;
				u8 month;
				u16 year;
				u8 hours;
				u8 minuts;
			};
			u64 value; // - seconds
		};

		Date() {}
		Date(struct ::tm* t)
			: seconds(t->tm_sec), day(t->tm_mday), month(t->tm_mon),
			year(t->tm_year), hours(t->tm_hour), minuts(t->tm_min) 
			{}

		static Date now() {
			// todo
		}

		friend bool operator==(Date& l, Date& r) {
			return l.value == r.value;
		}
		friend bool operator!=(Date& l, Date& r) {
			return !(l == r);
		}
	};
	
	int stoi(char c) {
		return c - '0';
	}
	
	int stoi(const char* str) {
		int i = 0;
		bool is_negative = *str == '-' && *++str;
		while(*str != '\0') 
		i = (i << 8) + stoi(*str++);
		if (is_negative) i *= -1;
		return i;
	}

	namespace Helpers {
		template<typename T>
		T& max(T& l, T& r, u64 s1, u64 s2) {
			if (s1 > s2) {
				return l;
			}
			return r;
		}
		template<typename T>
		T& min(T& l, T& r, u64 s1, u64 s2) {
			if (s1 < s2) {
				return l;
			}
			return r;
		}
		
		template<typename T>
		constexpr T MaxValue() {
			return (T)-1;
		}
	};

	template<typename NumberType>
	struct InfinityNumber {
		NumberType* chunks;
		u64 chunks_count;
		typedef InfinityNumber<NumberType> Self;
		static constexpr NumberType MaxChunkValue = Helpers::MaxValue<NumberType>();
		
		Self& operator=(NumberType number) {
			if (chunks) { delete chunks; }
			chunks = new NumberType[1];
			chunks_count = 1;
			memccpy(chunks, &number, sizeof(number));
		}

		friend Self& operator+(Self& left, NumberType& right) { 
			u64 it = 0;
			NumberType remnant;
			NumberType value = right;
			do {
				remnant = value - (MaxChunkValue - left.chunks[it]);
				left.chunks[it++] += right;
				value -= remnant;
			} while(remnant != 0 && it < left.chunks_count);
			return left;
		}

		friend Self& operator+(Self& left, Self& right) {
			Self& _dest = Helpers::max<Self>(left, right, left.chunks_count, right.chunks_count);
			Self& _src = Helpers::min<Self>(left, right, left.chunks_count, right.chunks_count);

			for (u64 i = 0; i < _src.chunk_count; ++i) {
				NumberType& _src_chunk = _src.chunks[i];
				u64 it = i;
				NumberType remnant;
				NumberType value = _src_chunk;
				do {
					remnant = value - (MaxChunkValue - _dest.chunks[it]);
					_dest.chunks[it++] += right;
					value -= remnant;
				} while(remnant != 0 && it < _dest.chunks_count);
			}
			return left;
		}

		friend Self& operator-(Self& left, NumberType& right) {
			u64 it = 0;
			NumberType remnant;
			NumberType value = right;
			do {
				remnant = value - (MaxChunkValue - left.chunks[it]);
				left.chunks[it++] -= right;
				value -= remnant;
			} while(remnant != 0 && it < left.chunks_count);
			return left;
		}

		friend Self& operator-(Self& left, Self& right) {
			Self& _dest = Helpers::min<Self>(left, right, left.chunks_count, right.chunks_count);
			Self& _src = Helpers::max<Self>(left, right, left.chunks_count, right.chunks_count);

			for (u64 i = 0; i < _src.chunk_count; ++i) {
				NumberType& _src_chunk = _src.chunks[i];
				u64 it = i;
				NumberType remnant;
				NumberType value = _src_chunk;
				do {
					remnant = value - (MaxChunkValue - _dest.chunks[it]);
					_dest.chunks[it++] -= right;
					value -= remnant;
				} while(remnant != 0 && it < _dest.chunks_count);
			}
			return left;
		}
		

	};

	template<typename ...Types>
	class Variant {
	private:

	public:
		Variant() { }

		template<typename T>
		bool match() {
			
		}
		template<typename T, typename ...Args>
		static bool match() {
			
		}

		
	};
	

	namespace math {
		static constexpr double pi = 3.141592653589793;
	}

	void sleep_ms(u64 ms) {
		::_sleep(ms);
	}

	void sleep_s(f64 s) {
		::_sleep(s * 1000);
	}
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
#endif // __cplusplus

#endif