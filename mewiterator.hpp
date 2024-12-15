/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_ITERATOR_LIB_SO2U
#define MEW_ITERATOR_LIB_SO2U
#include "mewlib.h"
#include "mewmath.hpp"

namespace mew {

template<typename T>
class iterator {
public:
  typedef iterator<T> iterator_t;
  typedef const iterator<T>& r_iterator_t;
private:
  size_t size;
  T* data = nullptr;
public:
  ////////////////////////////////////////////////////////////
  iterator() { }

  ////////////////////////////////////////////////////////////
  iterator(T* _data, size_t _size): size(_size), data(_data) { }

  ////////////////////////////////////////////////////////////
  iterator_t& operator++() {
    if (size == 0) {
      return *this;
    }
    size--;
    data += sizeof(T);
    return *this;
  }

  ////////////////////////////////////////////////////////////
  iterator_t operator++(int) {
    iterator_t it(*this);
    ++(*this);
    return *this; 
  }

  ////////////////////////////////////////////////////////////
  T& begin() {
    return data;
  } 

  ////////////////////////////////////////////////////////////
  friend bool operator==(r_iterator_t r, r_iterator_t l) {
    return r.data == l.data;
  }

  ////////////////////////////////////////////////////////////
  friend bool operator!=(r_iterator_t r, r_iterator_t l) {
    return r.data != l.data;
  }
};

}

#endif