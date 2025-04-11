/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_PRINT_LIB_SO2U
#define MEW_PRINT_LIB_SO2U
#include <initializer_list>
#include <iostream>
#include "mewlib.h"
#include "mewmath.hpp"
#include "mewlist.hpp"
#include "mewstack.hpp"
#include "mewiterator.hpp"


////////////////////////////////////////////////////////////
template<typename T>
std::ostream& operator<<(std::ostream& os, const mew::stack<T> &as) {
  os << "{ ";
  for (size_t i = 0U; i < as.size(); ++i) {
    os << as[i] << ", ";
  }
  os << "}";
  return os;
}

////////////////////////////////////////////////////////////
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::initializer_list<T> &is) {
  os << "{ ";
  for (size_t i = 0U; i < is.size(); ++i) {
    os << is.begin()[i] << ", ";
  }
  os << "}";
  return os;
}

#endif