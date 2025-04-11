/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_MATRIX_LIB_SO2U
#define MEW_MATRIX_LIB_SO2U
#include "mewlib.h"
#include "mewmath.hpp"
#include "mewstack.hpp"

namespace mew {

template<typename T>
class matrix {
private:
  mew::stack<T> _M_data;
  size_t _M_width, _M_height;
public:
  ////////////////////////////////////////////////////////////
  matrix(size_t _width, size_t _height): _M_width(_width), _M_height(_height) { 
    _M_data.reserve(_width*_height);
  }

  ////////////////////////////////////////////////////////////
  size_t width() const noexcept {
    return _M_width;
  }

  ////////////////////////////////////////////////////////////
  size_t height() const noexcept {
    return _M_height;
  }

  ////////////////////////////////////////////////////////////
  T& at(size_t x, size_t y) {
    int idx = (y*height())+x;
    return _M_data.at(idx);
  }

  ////////////////////////////////////////////////////////////
  T& operator[] (int idx) {
    return _M_data.at(idx);
  }
};

}

#endif