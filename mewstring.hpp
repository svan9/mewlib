/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_STRING_LIB_SO2U
#define MEW_STRING_LIB_SO2U
#include "mewlib.h"

#ifndef __cplusplus
#error cpp only
#endif

namespace mew {
  
namespace string {
  char *ansi(wchar_t *unicode) {
    size_t size = wcslen(unicode);
    char* buffer = new char[size+1]; 
    for (int i = 0; i < size; i++) {
      buffer[i] = wctob(unicode[i]);
    }
    buffer[size] = '\0';
    return buffer;
  }

	class StringIterator {
	public:
		const char *cbegin, *begin, *end;
		typedef StringIterator iterator;
		typedef const StringIterator const_iterator;

		////////////////////////////////////////////////////////////
		StringIterator() {}

		////////////////////////////////////////////////////////////
		StringIterator(const char* source): cbegin(source), begin(source), end(source+strlen(source)) {}

		////////////////////////////////////////////////////////////
		iterator Begin() {
			return iterator(*this);
		}

		////////////////////////////////////////////////////////////
		iterator End() {
			return iterator(*this)+(end-begin);
		}
		
		////////////////////////////////////////////////////////////
		bool IncCheck(int v = 1) {
			return begin+(v-1) != end;
		}
				
		////////////////////////////////////////////////////////////
		bool DecCheck(int v = 1) {
			return begin-(v+1) != cbegin;
		}

		////////////////////////////////////////////////////////////
		iterator& operator++() {
			if (IncCheck()) { ++begin; }
			return *this;
		}
		
		////////////////////////////////////////////////////////////
		iterator operator++(int) {
			iterator tmp(*this);
      ++(*this);
			return tmp;
		}
		
		////////////////////////////////////////////////////////////
		iterator& operator--() {
			if (DecCheck()) { begin--; }
			return *this;
		}

		////////////////////////////////////////////////////////////
		iterator operator--(int) {
			iterator tmp(*this);
			operator--();
			return tmp;
		}  

		////////////////////////////////////////////////////////////
		iterator& operator+=(const int v) noexcept {
			if (IncCheck(v)) { begin += v; }
			return *this;
		}

		///////////////////////////////////////////////////////////.
		iterator operator+(const int _Off) const noexcept {
			iterator _Tmp = *this;
			_Tmp += _Off;
			return _Tmp;
		}

		////////////////////////////////////////////////////////////
		friend iterator operator+(const int _Off, iterator _Next) noexcept {
			_Next += _Off;
			return _Next;
		}

		////////////////////////////////////////////////////////////
		iterator& operator-=(const int v) noexcept {
			if (DecCheck(v)) { begin -= v; }
			return *this;
		}

		////////////////////////////////////////////////////////////
		iterator operator-(const int _Off) const noexcept {
			iterator _Tmp = *this;
			_Tmp -= _Off;
			return _Tmp;
		}

		////////////////////////////////////////////////////////////
		friend iterator operator-(const int _Off, iterator _Next) noexcept {
			_Next -= _Off;
			return _Next;
		}

		////////////////////////////////////////////////////////////
		friend bool operator==(const iterator& l, const iterator& r) noexcept {
			return l.end == r.end && l.begin == r.begin;
		}

		////////////////////////////////////////////////////////////
		friend bool operator!=(const iterator& l, const iterator& r) noexcept {
			return l.end != r.end || l.begin != r.begin;
		}

		////////////////////////////////////////////////////////////
		bool IsEnd() {
			return begin == end;
		}

		////////////////////////////////////////////////////////////
		char operator*() {
			return *begin;
		}

		////////////////////////////////////////////////////////////
		static char BeginChar(const char* source) {
			return *(source);
		}

		////////////////////////////////////////////////////////////
		static char EndChar(const char* source) {
			return *(strlen(source)+source-1);
		}

		////////////////////////////////////////////////////////////
		static bool HasChar(const char* source, char c) {
			for (int i = 0; i < strlen(source); i++) {
				if (source[i] == c) {
					return true;
				}
			}
			return false;
		}

		////////////////////////////////////////////////////////////
		static bool HasCharNoCase(const char* source, char c) {
			for (int i = 0; i < strlen(source); i++) {
				if (tolower(source[i]) == tolower(c)) {
					return true;
				}
			}
			return false;
		}

		////////////////////////////////////////////////////////////
		static bool HasChars(const char* source, char* c) {
			bool value = false;
			for (int i = 0; i < strlen(c); i++) {
				value &= HasChar(source, c[i]);
			}
			return value;
		}

		////////////////////////////////////////////////////////////
		static bool HasCharsNoCase(const char* source, char* c) {
			bool value = false;
			for (int i = 0; i < strlen(c); i++) {
				value &= HasCharNoCase(source, c[i]);
			}
			return value;
		}
	};

	////////////////////////////////////////////////////////////
	size_t CountRightBefore(const char* source, char c) {
		size_t count = 0;
		const char* begin = source;
		const char* end = begin+strlen(source);
		while (begin != end) {
			if (*(begin++) == c) { break; }
			count++;
		}
		return count;
	}

	////////////////////////////////////////////////////////////
	bool CharInString(const char* str, char c) {
		const char* begin = str;
		const char* end = begin+strlen(str);
		while (begin != end) {
			if (*(begin++) == c) {
				return true;
			}
		}
		return false;
	}
	
	////////////////////////////////////////////////////////////
	size_t CountRightBefore(const char* source, const char* c) {
		size_t count = 0;
		const char* begin = source;
		const char* end = begin+strlen(source);
		while (begin != end) {
			if (CharInString(c, *(begin++))) { break; }
			count++;
		}
		return count;
	}	

	////////////////////////////////////////////////////////////
	size_t CountRightFor(const char* source, const char* c) {
		size_t count = 0;
		const char* begin = source;
		const char* end = begin+strlen(source);
		while (begin != end) {
			if (!CharInString(c, *(begin++))) { break; }
			count++;
		}
		return count;
	}


	////////////////////////////////////////////////////////////
	bool EndWith(const char* source, char c) {
		size_t size = strlen(source);
		return source[size-1] == c;
	}

	////////////////////////////////////////////////////////////
	bool SameStr(const char* str1, const char* str2) {
		size_t s1 = strlen(str1);
		size_t s2 = strlen(str2);
		if (s1 != s2) { return false; }
		return 0 == memcmp(str1, str2, s1);
	}
}
}

#endif