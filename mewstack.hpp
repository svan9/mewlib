/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_STACK_LIB_SO2U
#define MEW_STACK_LIB_SO2U
#include "mewlib.h"
#include "mewmath.hpp"
#include "mewiterator.hpp"

namespace mew {

template<typename T, size_t alloc_size = 1U>
class stack {
private:
  size_t _M_size, _M_capacity;
  T* data = nullptr;
public:
  ////////////////////////////////////////////////////////////
  stack(): _M_capacity(sizeof(T)), _M_size(0) {
    data = (T*)malloc(_M_capacity);
  }

  ////////////////////////////////////////////////////////////
  ~stack() {
    free(data);
  }

  ////////////////////////////////////////////////////////////
  void resize(size_t new_size, bool ignore_size = false) {
    new_size *= alloc_size;
    data = (T*)realloc(data, new_size);
    if (!ignore_size && _M_size > new_size) {
      _M_size = new_size;
    }
    _M_capacity = new_size;
  }
  
  ////////////////////////////////////////////////////////////
  void reserve(size_t size) {
    resize((_M_capacity+size)*sizeof(T), true);
  }

  ////////////////////////////////////////////////////////////
  void upsize_if_needs() {
    if ((_M_size+1)*sizeof(T) > _M_capacity) {
      resize((_M_size+1)*sizeof(T));
    }
  }

  ////////////////////////////////////////////////////////////
  bool has(int idx) const noexcept {
    size_t real_idx = get_real_idx(idx);
    return real_idx < _M_size;
  }

  ////////////////////////////////////////////////////////////
  T& at(size_t idx) {
    MewAssert(has(idx));
    return data[idx];
  }

  ////////////////////////////////////////////////////////////
  size_t get_real_idx(int idx) const noexcept {
    size_t real_idx;
    real_idx = mod(idx, _M_size);
    real_idx = (_M_size + real_idx) % _M_size;
    return real_idx;
  }

  ////////////////////////////////////////////////////////////
  T& at(int idx) {
    return at(get_real_idx(idx));
  }

  ////////////////////////////////////////////////////////////
  T& operator[](int idx) {
    return at(idx);
  }

  ////////////////////////////////////////////////////////////
  size_t data_size() const noexcept {
    return (sizeof(T)*_M_size);
  }

  ////////////////////////////////////////////////////////////
  size_t size() const noexcept {
    return _M_size;
  }

  ////////////////////////////////////////////////////////////
  size_t push(T&& value) {
    upsize_if_needs();
    T* pointer = data+_M_size;
    memcpy(pointer, &value, sizeof(value));
    _M_size++;
    return _M_size-1;
  }

  ////////////////////////////////////////////////////////////
  size_t push(const T& value) {
    upsize_if_needs();
    memcpy(data+_M_size, &value, sizeof(value));
    _M_size++;
    return _M_size-1;
  }

  ////////////////////////////////////////////////////////////
  T top() {
    return at(-1);
  }

  ////////////////////////////////////////////////////////////
  T pop() {
    T t = top();
    _M_size -= 1;
    return t;
  }

  ////////////////////////////////////////////////////////////
  void erase(size_t start, size_t size = 1) {
    MewAssert(size <= _M_size);
    T* begin = (T*)(data+start);
    T* end = (T*)(data+start+size);
    memmove(begin, end, size*sizeof(T));
    _M_size -= size;
  }
  
  ////////////////////////////////////////////////////////////
  void erase(int idx) {
    erase(get_real_idx(idx));
  }

  ////////////////////////////////////////////////////////////
  bool empty() const noexcept {
    return _M_size == 0;
  }

  ////////////////////////////////////////////////////////////
  void clear() {
    _M_size = 0;
    memset(data, 0, _M_capacity);
  }

  ////////////////////////////////////////////////////////////
  size_t indexOf(T& value) {
    for (int i = 0; i < _M_size; ++i) {
      T& cur_el = data[i];
      if (0 == memcmp(&value, &cur_el, sizeof(T))) {
        return i;
      }
    }
    return (size_t)(-1);
  }

  ////////////////////////////////////////////////////////////
  iterator<T> begin() {
    iterator<T> it(data, _M_size);
    return it;
  }

  ////////////////////////////////////////////////////////////
  iterator<T> end() {
    iterator<T> it(data+(_M_size*sizeof(T*)), 0);
    return it;
  }
};

}

#endif