/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_STACK_LIB_SO2U
#define MEW_STACK_LIB_SO2U
#include <initializer_list>
#include "mewlib.h"
#include "mewmath.hpp"
#include "mewiterator.hpp"

namespace mew {

template<typename T, size_t alloc_size = 1U>
class stack {
private:
  size_t _M_size, _M_capacity;
  T* _M_data = nullptr;
public:
  ////////////////////////////////////////////////////////////
  stack(): _M_capacity(sizeof(T)), _M_size(0) {
    _M_data = (T*)malloc(_M_capacity);
  }

  ////////////////////////////////////////////////////////////
  stack(T* _array, size_t _length): _M_capacity(sizeof(T)), _M_size(_length) {
    _M_data = (T*)malloc(_M_capacity);
    memcpy(_M_data, _array, sizeof(T)*_length);
  }

  ////////////////////////////////////////////////////////////
  stack(stack<T, alloc_size>& ref): _M_capacity(ref._M_capacity), _M_size(ref._M_size) {
    _M_data = (T*)malloc(_M_capacity);
    memcpy(_M_data, ref._M_data, _M_capacity);
  }

  ////////////////////////////////////////////////////////////
  stack(size_t size): _M_capacity(sizeof(T)*size), _M_size(0) {
    _M_data = (T*)malloc(_M_capacity);
  }

  ////////////////////////////////////////////////////////////
  ~stack() {
    free(_M_data);
  }

  ////////////////////////////////////////////////////////////
  T* data() const noexcept {
    return _M_data;
  }

  ////////////////////////////////////////////////////////////
  void resize(size_t new_size, bool ignore_size = false) {
    new_size *= alloc_size;
    _M_data = (T*)realloc(_M_data, new_size);
    if (!ignore_size && _M_size > new_size) {
      _M_size = new_size;
    }
    _M_capacity = new_size;
  }
  
  ////////////////////////////////////////////////////////////
  void reserve(size_t size) {
    if (size == 0) { return; }
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
  T& at(size_t idx) const {
    MewAssert(has(idx));
    return _M_data[idx];
  }

  ////////////////////////////////////////////////////////////
  size_t get_real_idx(int idx) const noexcept {
    if (idx == 0) {
      return 0;
    }
    size_t real_idx;
    real_idx = mod(idx, _M_size);
    real_idx = (_M_size + real_idx) % _M_size;
    return real_idx;
  }

  ////////////////////////////////////////////////////////////
  T& at(int idx) const {
    return at(get_real_idx(idx));
  }

  ////////////////////////////////////////////////////////////
  T& operator[](int idx) {
    return at(idx);
  }

  ////////////////////////////////////////////////////////////
  const T& operator[](int idx) const {
    return at(idx);
  }

  ////////////////////////////////////////////////////////////
  size_t _M_data_size() const noexcept {
    return (sizeof(T)*_M_size);
  }

  ////////////////////////////////////////////////////////////
  size_t size() const noexcept {
    return _M_size;
  }

  ////////////////////////////////////////////////////////////
  size_t push(T&& value) {
    upsize_if_needs();
    T* pointer = _M_data+(_M_size*sizeof(T));
    memcpy(pointer, &value, sizeof(value));
    _M_size++;
    return _M_size-1;
  }
  ////////////////////////////////////////////////////////////
  size_t push(T* value) {
    upsize_if_needs();
    T* pointer = _M_data+(_M_size*sizeof(T));
    memcpy(pointer, value, sizeof(T));
    _M_size++;
    return _M_size-1;
  }
  ////////////////////////////////////////////////////////////
  size_t push(const T& value) {
    upsize_if_needs();
    T* pointer = _M_data+(_M_size*sizeof(T));
    memcpy(pointer, &value, sizeof(value));
    _M_size++;
    return _M_size-1;
  }

  ////////////////////////////////////////////////////////////
  template<typename ...Args>
  size_t emplace(Args... args) {
    upsize_if_needs();
    T* value = new T(args...);
    memcpy(_M_data+(_M_size*sizeof(T)), value, sizeof(T));
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
  void erase() {
    _M_size -= 1;
  }

  ////////////////////////////////////////////////////////////
  void erase(size_t start, size_t size = 1) {
    MewAssert(size <= _M_size);
    T* begin = (T*)(_M_data+start);
    T* end = (T*)(_M_data+start+size);
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
    memset(_M_data, 0, _M_capacity);
  }

  ////////////////////////////////////////////////////////////
  size_t indexOf(const T& value) const {
    for (int i = 0; i < _M_size; ++i) {
      T& cur_el = _M_data[i];
      if (0 == memcmp(&value, &cur_el, sizeof(T))) {
        return i;
      }
    }
    return (size_t)(-1);
  }

  ////////////////////////////////////////////////////////////
  size_t indexOf(T&& value) const {
    for (int i = 0; i < _M_size; ++i) {
      T& cur_el = _M_data[i];
      if (0 == memcmp(&value, &cur_el, sizeof(T))) {
        return i;
      }
    }
    return (size_t)(-1);
  }

  ////////////////////////////////////////////////////////////
  void pushIfNotExists(T&& value) {
    if (indexOf(value) != (size_t)(-1)) {
      push(value);
    }
  }

  ////////////////////////////////////////////////////////////
  void pushIfNotExists(const T& value) {
    if (indexOf(value) != (size_t)(-1)) {
      push(value);
    }
  }

  ////////////////////////////////////////////////////////////
  iterator<T> begin() {
    iterator<T> tmp(_M_data, _M_size);
    return tmp;
  }

  ////////////////////////////////////////////////////////////
  iterator<T> end() {
    iterator<T> tmp(_M_data+(_M_size*sizeof(T)));
    return tmp;
  }

  ////////////////////////////////////////////////////////////
  stack<T,alloc_size> reversed() {
    stack<T,alloc_size> nst(_M_size);
    for (int i = _M_size-1; i >= 0; --i) {
      auto el = at(i);
      nst.push(el);
    }
    return nst;
  }

  ////////////////////////////////////////////////////////////
  stack<T,alloc_size> clone() {
    stack<T,alloc_size> nst(*this);
    return nst;
  }

  ////////////////////////////////////////////////////////////
  friend bool operator==(stack<T,alloc_size>& st, std::initializer_list<T> list) {
    auto a = memcmp(st._M_data, list.begin(), st._M_data_size());
    return (st.size() == list.size()) && 
      (0 == memcmp(st._M_data, list.begin(), st._M_data_size()));
  }

  ////////////////////////////////////////////////////////////
  friend bool operator!=(stack<T,alloc_size>& st, std::initializer_list<T> list) {
    return (st.size() != list.size()) || 
      (0 != memcmp(st._M_data, list.begin(), st._M_data_size()));
  }

  ////////////////////////////////////////////////////////////
  stack<T,alloc_size>& operator++() {
    memmove(_M_data, _M_data+sizeof(T), _M_data_size()-sizeof(T));
    _M_size -= 1;
    return *this;
  }

  ////////////////////////////////////////////////////////////
  stack<T,alloc_size> operator++(int) {
    stack<T,alloc_size> tmp(*this);
    ++(*this);
    return tmp;
  }

};

}

#endif