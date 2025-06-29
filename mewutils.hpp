/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_UTILS_LIB_SO2U
#define MEW_UTILS_LIB_SO2U
#include "mewlib.h"
#include "mewstack.hpp"
#include "mewstring.hpp"
#include <variant>
#include <filesystem>

namespace mew::utils {

  template<typename T>
  void faspp(T& lhr, T val) {
    delete &lhr;
    lhr = val;
  }

  template<typename T>
  void fas(T& lhr, T val) {
    free(&lhr);
    lhr = val;
  }

  template<typename T>
  void fas(T*& lhr, T* val) {
    free(lhr);
    lhr = val;
  }

const char* getUserHome() {
#ifdef _WIN32
  return getenv("USERPROFILE");
#else
  return getenv("HOME");
#endif
}
const char* getUserHome(const char* next_path) {
  const char* home = getUserHome();
  const char* a = mew::concatPath(home, next_path);
  return a;
}

mew::stack<const char*>* splitLines(const char* str, bool trim_lines = true) {
  using namespace mew::string;
  auto lines = new mew::stack<const char*>();
  size_t size = strlen(str);
  const char* line_begin = str;
  size_t line_size = 0;
  for (size_t i = 0; i < size; ++i) {
    if (CharInString("\n", str[i])) {
      while(CharInString("\n", str[i+1])) { ++i; ++line_size; }
      const char* cs = scopy(line_begin, line_size+1);
      if (trim_lines) {
        fas(cs, (const char*)strtrim(cs));
      }
      lines->push(cs);
      line_begin = str + i++ + 1;
      line_size = 0;
    } else {
      ++line_size;
    }
  }
  if (line_size > 0) {
    const char* cs = strtrim(line_begin, line_size);
    lines->push(cs);
  }
  return lines;
}

char* str_parse(const char* str, size_t size) {
  using namespace mew::string;
  StringBuilder sb;
  for (size_t i = 0; i < size; ++i) {
    if (i+1 < size && str[i] == '\\' && str[i+1] == '\\') {
      sb += '\\'; ++i;
    } else 
    if (i+1 < size && str[i] == '\\' && str[i+1] == 'n') {
      sb += '\n'; ++i;
    } else 
    if (i+1 < size && str[i] == '\\' && str[i+1] == 't') {
      sb += '\t'; ++i;
    } else 
    if (i+1 < size && str[i] == '\\' && str[i+1] == '0') {
      sb += '\0'; ++i;
    } else sb.Append(str[i]);
  }
  char* ostr = scopy((char*)sb.c_str(), sb.size()+1);
  return ostr;
}

char* str_separate(const char* str, size_t size) {
  using namespace mew::string;
  StringBuilder sb;
  char* result;
  while (std::isspace(*str)) { ++str; --size; }
  for (size_t i = 0; i < size; ++i) {
    // skip strings
    if (str[i] == '\"') {
      sb += str[i];
      ++i; // skip current
      for (; i < size && (str[i] != '\"' || str[i - 1] == '\\'); ++i) {
        sb += str[i];
      }
      sb += str[i];
    } else 
    if (std::ispunct(str[i]) && str[i] != '_') {
      sb.Append('\0', true);
      sb += str[i];
      sb.Append('\0', true);
    } else
    // mark spaces and symbols
    if (std::isspace(str[i])) {
      sb.Append('\0', true);
    } else {
      sb += str[i];
    }
  }
  sb += '\0';
  sb += '\1';
  char* ostr = scopy((char*)sb.c_str(), sb.size()+1);
  return ostr;
}

char* str_separate(const char* str) {
  return str_separate(str, strlen(str));
}

char* str_split(const char* str, size_t size, char s, bool trim = true) {
  using namespace mew::string;
  StringBuilder sb;
  for (size_t i = 0; i < size; ++i) {
    char ss = str[i];
    if (ss == s || (trim && std::isspace(ss))) {
      sb.Append('\0', true);
    } else {
      sb += ss;
    }
  }
  sb += '\0';
  sb += '\1';
  char* ostr = scopy((char*)sb.c_str(), sb.size()+1);
  return ostr;
}
char* str_split(const char* str, char s, bool trim = true) {
  return str_split(str, strlen(str), s, trim);
}

char* str_split(const char* str, size_t size, const char* s, bool trim = true) {
  using namespace mew::string;
  StringBuilder sb;
  for (size_t i = 0; i < size; ++i) {
    char ss = str[i];
    if (mew::iscmpstr(ss, s) || (trim && std::isspace(ss))) {
      sb.Append('\0', true);
    } else {
      sb += ss;
    }
  }
  sb += '\0';
  sb += '\1';
  char* ostr = scopy((char*)sb.c_str(), sb.size()+1);
  return ostr;
}
char* str_split(const char* str, const char* s, bool trim = true) {
  return str_split(str, strlen(str), s, trim);
}
wchar_t* str_split(const wchar_t* str, size_t size, wchar_t s, bool trim = true) {
  using namespace mew::string;
  WStringBuilder sb;
  for (size_t i = 0; i < size; ++i) {
    wchar_t ss = str[i];
    if (ss == s || (trim && std::isspace(ss))) {
      sb.Append('\0', true);
    } else {
      sb += ss;
    }
  }
  sb += L'\0';
  sb += L'\1';
  wchar_t* ostr = scopy((wchar_t*)sb.c_str(), sb.size()+1);
  return ostr;
}
wchar_t* str_split(const wchar_t* str, wchar_t s, bool trim = true) {
  return str_split(str, wcslen(str), s, trim);
}

wchar_t* str_split(const wchar_t* str, size_t size, const wchar_t* s, bool trim = true) {
  using namespace mew::string;
  WStringBuilder sb;
  for (size_t i = 0; i < size; ++i) {
    wchar_t ss = str[i];
    if (mew::iscmpstr(ss, s) || (trim && std::isspace(ss))) {
      sb.Append('\0', true);
    } else {
      sb += ss;
    }
  }
  sb += L'\0';
  sb += L'\1';
  wchar_t* ostr = scopy((wchar_t*)sb.c_str(), sb.size()+1);
  return ostr;
}
wchar_t* str_split(const wchar_t* str, const wchar_t* s, bool trim = true) {
  return str_split(str, wcslen(str), s, trim);
}


char* shift_word(char* separeted) {
  if (separeted == nullptr) return nullptr;
  char* begin = separeted;
  while (*begin != '\0') { ++begin; }
  ++begin;
  return *begin == '\1' ? nullptr: begin;
}

wchar_t* shift_word(wchar_t* separeted) {
  if (separeted == nullptr) return nullptr;
  wchar_t* begin = separeted;
  while (*begin != L'\0') { ++begin; }
  ++begin;
  return *begin == L'\1' ? nullptr: begin;
}


char* str_to_str(const char* separeted) {
  if (separeted[0] != '\"') {
    return nullptr;
  }
  char* begin = scopy(separeted+1);
  for (int i = 0; i < strlen(begin); ++i) {
    if (begin[i] == '\\' && begin[i+1] == '\"') {
      begin[i] = '\"';
      memmove(begin+i+1, begin+i+2, strlen(begin+i+1));
    }
    if (begin[i] == '\\' && begin[i+1] == 'n') {
      begin[i] = '\n';
      memmove(begin+i+1, begin+i+2, strlen(begin+i+1));
    }
    if (begin[i] == '\\' && begin[i+1] == 't') {
      begin[i] = '\t';
      memmove(begin+i+1, begin+i+2, strlen(begin+i+1));
    }
  }
  if (begin[strlen(begin)-1] == '\"') {
    begin[strlen(begin)-1] = '\0';
  } else {
    begin[strlen(begin)] = '\0';
  }
  return begin;
}


int hex_to_int(const char* str, bool& success) {
  char* end;
  long result = strtol(str, &end, 16);
  if (*end == '\0') {
    success = true;
    return static_cast<int>(result);
  } else {
    success = false;
    return 0;
  }
}

int str_to_int(const char* str, bool& success) {
  if (mew::starts_with(str, "0x")) {
    return hex_to_int(str+2, success);
  }
  char* end;
  long result = strtol(str, &end, 10);
  if (*end == '\0') {
    success = true;
    return static_cast<int>(result);
  } else {
    success = false;
    return 0;
  }
}

float str_to_float(const char* str, bool& success) {
  char* end;
  float result = strtof(str, &end);
  if (*end == '\0') {
    success = true;
    return result;
  } else {
    success = false;
    return 0.0f;
  }
}

double str_to_double(const char* str, bool& success) {
  char* end;
  double result = strtod(str, &end);
  if (*end == '\0') {
    success = true;
    return result;
  } else {
    success = false;
    return 0.0;
  }
}

using NumberVariant = std::variant<int, float, double, bool>;
NumberVariant str_to_number(const char* str) {
  bool success;
  if (str == nullptr || *str == '\0') {
    success = false;
    return 0; // Default value for int
  }

  if (strcmp(str, "true") == 0 || strcmp(str, "false") == 0) {
    success = true;
    return strcmp(str, "true") == 0;
  }

  if (strchr(str, '.')) {
    // Try to parse as float or double
    double d_value = str_to_double(str, success);
    if (success) {
      return d_value;
    }
    float f_value = str_to_float(str, success);
    if (success) {
      return f_value;
    }
  } else {
    // Try to parse as int
    int i_value = str_to_int(str, success);
    if (success) {
      return i_value;
    }
  }

  success = false;
  return 0; // Default value for int
}


class TokenRow {
private:
  // free before
  std::shared_ptr<char[]> m_sequence;
  char* c_ptr;
public: 
  TokenRow() {}
  TokenRow(const char* str): m_sequence(str_separate(str)),c_ptr(m_sequence.get()) {}
  TokenRow(const TokenRow& tr): m_sequence(tr.m_sequence), c_ptr(tr.c_ptr) {}

  void config(const char* str) {
    m_sequence.reset(str_separate(str));
    c_ptr = m_sequence.get();
  }

  void print() {
    TokenRow tmp(*this);
    char* w = *tmp;
    while((w = *tmp++) != nullptr) {
      printf("\"%s\" ", w);
    }
  }

  char* operator*() {
    return c_ptr;
  }

  TokenRow& operator++() {
    c_ptr = shift_word(c_ptr);
    return *this;
  }

  TokenRow operator++(int) {
    TokenRow tmp(*this);
    ++*this;
    return tmp;
  }

  bool same(const char* str) {
    return strcmp(c_ptr, str);
  }

  
};
std::filesystem::path GetAbsPath(const char* path) { 
  std::filesystem::path __path(path);
  if (!__path.is_absolute()) {
    __path = std::filesystem::absolute(__path.lexically_normal());
  }
  return __path;
}


};

#endif