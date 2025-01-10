/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_UTILS_LIB_SO2U
#define MEW_UTILS_LIB_SO2U
#include "mewlib.h"

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

};

#endif