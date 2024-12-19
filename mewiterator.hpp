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
<<<<<<< HEAD
private:
  T* _M_begin, *_M_const_begin, *_M_end;
  bool _can_update(int offset) {
    return ((size_t)_M_begin+offset) <= (size_t)_M_end && ((size_t)_M_begin+offset) >= (size_t)_M_const_begin;
  }

  constexpr size_t _el_size() const noexcept {
    return sizeof(T);
  }

  constexpr size_t _nel_size() const noexcept {
    return -sizeof(T);
  }

public:
  ////////////////////////////////////////////////////////////
  iterator() {}

  ////////////////////////////////////////////////////////////
  iterator(T* begin): _M_begin(begin),_M_const_begin(begin), _M_end(begin) {}

  ////////////////////////////////////////////////////////////
  iterator(T* begin, size_t count): _M_begin(begin),_M_const_begin(begin), _M_end((T*)(begin+(count*_el_size()))) {}

  ////////////////////////////////////////////////////////////
  iterator(T* begin, T* end): _M_begin(begin),_M_const_begin(begin), _M_end(end) {}

  ////////////////////////////////////////////////////////////
  iterator<T>& operator++() {
    if (_can_update(_el_size())) 
      _M_begin = (T*)(_M_begin+_el_size());
=======
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
>>>>>>> b92be18e87ead7f49618c6b8951e01a329f41714
    return *this;
  }

  ////////////////////////////////////////////////////////////
<<<<<<< HEAD
  iterator<T> operator++(int) {
    iterator<T> tmp(*this);
    ++(*this);
    return tmp;
  }

  ////////////////////////////////////////////////////////////
  iterator<T>& operator--() {
    if (_can_update(_nel_size())) 
      _M_begin = (T*)(_M_begin+_nel_size());
    return *this;
  }

  ////////////////////////////////////////////////////////////
  iterator<T> operator--(int) {
    iterator<T> tmp(*this);
    --(*this);
    return tmp;
  }
  
  ////////////////////////////////////////////////////////////
  iterator<T>& operator+=(int offset) {
    if (_can_update(offset*_el_size())) 
      _M_begin = _M_begin + (T*)(offset*_el_size());
    return *this;
  }

  ////////////////////////////////////////////////////////////
  iterator<T>& operator-=(int offset) {
    if (_can_update(offset*_nel_size())) 
      _M_begin = _M_begin + (T*)(offset*_nel_size());
    return *this;
  }

  ////////////////////////////////////////////////////////////
  T& operator*() const {
    return *_M_begin;
  }
  ////////////////////////////////////////////////////////////
  T* operator->() const {
    return _M_begin;
  }

  ////////////////////////////////////////////////////////////
  friend iterator<T> operator+(const iterator<T>& l, int offset) {
    iterator<T> tmp(l);
    tmp += offset;
    return tmp;
  }

  ////////////////////////////////////////////////////////////
  friend iterator<T> operator-(const iterator<T>& l, int offset) {
    iterator<T> tmp(l);
    tmp -= offset;
    return tmp;
  }

  ////////////////////////////////////////////////////////////
  friend bool operator==(const iterator<T>& l, const iterator<T>& r) {
    return l._M_begin == r._M_begin;
  }
  ////////////////////////////////////////////////////////////
  friend bool operator!=(const iterator<T>& l, const iterator<T>& r) {
    return l._M_begin != r._M_begin;
  }

=======
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
>>>>>>> b92be18e87ead7f49618c6b8951e01a329f41714
};

}

#endif