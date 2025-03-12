/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_UTILS_LIB_SO2U
#define MEW_UTILS_LIB_SO2U
#include "mewlib.h"
#include "mewstack.hpp"

namespace mew::utils {

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

char* str_separate(const char* str, size_t size) {
  char* result = scopy(result, size+1);
  for (int i = 0; i < size; ++i) {
    // skip strings
    if (str[i] == '\"') {
      ++i; // skip current
      for (;i < size || (str[i] == '\"' && str[i-1] != '\\'); ++i) {}
    }
    // mark spaces
    if (std::isspace(str[i])) {
      result[i] = '\0';
    }
  }
  result[size] = '\1';
  return result;
}
char* str_separate(const char* str) {
  return str_separate(str, strlen(str));
}

char* shift_word(char* separeted) {
  char* begin = separeted;
  while (*++begin != '\0') { if (*begin == '\1') {return nullptr;} }
  return begin;
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
  return begin;
}

int str_to_int(const char* str, bool& success) {
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

};

#endif