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
#include "mewallocator.hpp"
#include <string.h>

namespace mew {

template<typename T, typename _Alloc = mew::Allocator<T>>
class stack {
private:
  _Alloc _M_allocator;
public:
  stack(): _M_allocator(1) { } // Initialize _M_capacity
  
  ////////////////////////////////////////////////////////////
  size_t size() const noexcept {
    return _M_allocator.size();
  }

  ////////////////////////////////////////////////////////////
  size_t count() const noexcept {
    return _M_allocator.count();
  }

  ////////////////////////////////////////////////////////////
  T* begin() const noexcept {
    return _M_allocator.begin();
  }
  
  ////////////////////////////////////////////////////////////
  T* end() const noexcept {
    return _M_allocator.end();
  }
  
  ////////////////////////////////////////////////////////////
  const T* cbegin() const { return (const T*)_M_allocator.begin(); }

  ////////////////////////////////////////////////////////////
  const T* cend() const { return (const T*)_M_allocator.end(); }

  ////////////////////////////////////////////////////////////
  stack(T* _array, size_t _length): _M_allocator(1U) {
    _M_allocator.copy(_array, _length);
  }

  ////////////////////////////////////////////////////////////
  stack(stack<T>& ref): _M_allocator(ref._M_allocator) { }
  
  ////////////////////////////////////////////////////////////
  stack(size_t size): _M_allocator(size) { }

  ////////////////////////////////////////////////////////////
  T* data() const noexcept {
    return _M_allocator.begin();
  }

  ////////////////////////////////////////////////////////////
  void resize(size_t new_size, bool ignore_size = false) {
    if (ignore_size) {
      _M_allocator.reserve(new_size);
    } else {
      _M_allocator.resize(new_size);
    }
  }
  
  ////////////////////////////////////////////////////////////
  void reserve(size_t size) {
    _M_allocator.reserve(size);
  }

  ////////////////////////////////////////////////////////////
  bool has(int idx) const noexcept {
    size_t real_idx = get_real_idx(idx);
    return real_idx < size();
  }

  ////////////////////////////////////////////////////////////
  bool has(T& idx) const noexcept {
    for (int i = 0; i < count(); ++i) {
      if (at(i) == idx) {
        return true;
      }
    }
    return false;
  }

  ////////////////////////////////////////////////////////////
  bool has(T* idx) const noexcept {
    for (int i = 0; i < count(); ++i) {
      if (&(at(i)) == idx) {
        return true;
      }
    }
    return false;
  }

  ////////////////////////////////////////////////////////////
  T& at(size_t idx) const {
    MewAssert(has(idx));
    return data()[idx];
  }
  ////////////////////////////////////////////////////////////
  T& at(size_t idx, size_t offset) const {
    MewAssert(has(idx) && idx-(offset/sizeof(T)) >= 0);
    T* _d = &(data()[idx]);
    _d = (T*)((byte*)_d - offset);
    return *(T*)(_d);
  }

  ////////////////////////////////////////////////////////////
  size_t get_real_idx(int idx) const noexcept {
    MewUserAssert(count() != 0, "stack is empty");
    if (idx == 0) {
      return 0;
    }
    size_t real_idx;
    real_idx = mod(idx, count());
    real_idx = (count() + real_idx) % count();
    return real_idx;
  }

  ////////////////////////////////////////////////////////////
  size_t rget_real_idx_AT(int idx) const noexcept {
    if (idx == 0) {
      return 0;
    }
    int real_idx;
    int ssize = size();
    real_idx = mod(idx, ssize);
    real_idx = (ssize + real_idx) % ssize;
    real_idx = real_idx % ssize;
    return real_idx;
  }

  ////////////////////////////////////////////////////////////
  T& at(int idx) const {
    return at(get_real_idx(idx));
  }

  ////////////////////////////////////////////////////////////
  [[deprecated("unsafe")]]
  T& at(int idx, size_t offset) const {
    return at(get_real_idx(idx), offset);
  }

  ////////////////////////////////////////////////////////////
  [[deprecated("unsafe")]]
  byte* rat(int offset) const {
    MewUserAssert(count() != 0, "stack is empty");
    size_t real_idx = rget_real_idx_AT(offset);
    MewUserAssert(real_idx < count(), "index out of range");
    T* _d = &(data()[real_idx]);
    _d = (T*)((byte*)_d - offset);
    return (byte*)(_d);
  }

  ////////////////////////////////////////////////////////////
  _GLIBCXX20_CONSTEXPR
  T& operator[](int idx) {
    return at(idx);
  }

  ////////////////////////////////////////////////////////////
  _GLIBCXX20_CONSTEXPR
  const T& operator[](int idx) const {
    return at(idx);
  }

  ////////////////////////////////////////////////////////////
  size_t push(T&& value) {
    T* pointer = _M_allocator.alloc();
    copy_to(pointer, value);
    return _M_allocator.count()-1;
  }

  ////////////////////////////////////////////////////////////
  size_t push(const T& value) {
    T* pointer = _M_allocator.alloc();
    copy_to(pointer, value);
    return _M_allocator.count()-1;
  }

  ////////////////////////////////////////////////////////////
  size_t push(T* ptr) {
    T* pointer = _M_allocator.alloc();
    copy_to(pointer, *ptr);
    return _M_allocator.count()-1;
  }

  ////////////////////////////////////////////////////////////
  [[deprecated("unsafe")]]
  size_t push(const T& value, size_t offset) {
    T* pointer = _M_allocator.alloc(offset);
    memcpy(pointer, &value, sizeof(value));
    return offset+1;
  }

  ////////////////////////////////////////////////////////////
  template<typename ...Args>
  size_t emplace(Args&&... args) {
    T* pointer = _M_allocator.alloc();
    new (pointer) T(std::forward<Args>(args)...);
    return _M_allocator.count() - 1;
  }

  ////////////////////////////////////////////////////////////
  size_t insert(const T& value, size_t idx) {
    T* pointer = _M_allocator.alloc(idx);
    memcpy(pointer, &value, sizeof(value));
    return idx+1;
  }
  
  ////////////////////////////////////////////////////////////
  size_t insert(T&& value, size_t idx) {
    T* pointer = _M_allocator.alloc(idx);
    memcpy(pointer, &value, sizeof(value));
    return idx+1;
  }

  ////////////////////////////////////////////////////////////
  stack<T>* copy() {
    stack<T>* ptr = new stack<T>();
    ptr->_M_allocator.copy(_M_allocator); 
    return ptr;
  }

  ////////////////////////////////////////////////////////////
  T* copy_data() {
    return rcopy(_M_allocator.begin(), _M_allocator.size());
  }
  
  ////////////////////////////////////////////////////////////
  T& top() {
    return at(-1);
  }
  ////////////////////////////////////////////////////////////
  T& top(int offset) {
    return at(-1, offset);
  }

  ////////////////////////////////////////////////////////////
  T pop() {
    T t = top();
    _M_allocator.pop();
    return t;
  }
  
  ////////////////////////////////////////////////////////////
  template<typename K>
  K npop() {
    K t;
    MewUserAssert(_M_allocator.size() >= sizeof(K), "not enough data to pop");
    memcpy(&t, _M_allocator.end() - sizeof(K), sizeof(K));
    _M_allocator.pop(sizeof(K));
    return t;
  }

  ////////////////////////////////////////////////////////////
  void erase() {
    _M_allocator.pop();
  }

  ////////////////////////////////////////////////////////////
  void erase(size_t start, size_t size = 1) {
    _M_allocator.erase(start, size);
  }
  
  ////////////////////////////////////////////////////////////
  void erase(int idx) {
    erase(get_real_idx(idx), 1);
  }

  ////////////////////////////////////////////////////////////
  void erase(T& value) {
    size_t idx = indexOf(value);
    if (idx != (size_t)(-1)) {
      erase(idx, 1);
    }
  }

  ////////////////////////////////////////////////////////////
  bool empty() const noexcept {
    return size() == 0;
  }

  ////////////////////////////////////////////////////////////
  void clear() {
    _M_allocator.clear();
  }

  ////////////////////////////////////////////////////////////
  void fill(T value) {
    _M_allocator.fill(value);
  }

  ////////////////////////////////////////////////////////////
  void clear(T value) {
    _M_allocator.clear(value);
  }

  ////////////////////////////////////////////////////////////
  size_t indexOf(const T& value) const {
    for (int i = 0; i < size(); ++i) {
      T& cur_el = _M_allocator.begin()[i];
      if (value == cur_el) {
        return i;
      }
    }
    return (size_t)(-1);
  }

  ////////////////////////////////////////////////////////////
  size_t indexOf(T&& value) const {
    for (int i = 0; i < size(); ++i) {
      T& cur_el = _M_allocator.begin()[i];
      if (value == cur_el) {
        return i;
      }
    }
    return (size_t)(-1);
  }

  ////////////////////////////////////////////////////////////
  void shift() {
    _M_allocator.shift();
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
  friend bool operator==(stack<T, _Alloc>& st, std::initializer_list<T> list) {
    auto a = memcmp(st.begin(), list.begin(), st.size());
    return (st.count() == list.size()) && 
      (0 == memcmp(st.begin(), list.begin(), st.size()));
  }

  ////////////////////////////////////////////////////////////
  friend bool operator!=(stack<T, _Alloc>& st, std::initializer_list<T> list) {
    return (st.count() != list.size()) || 
      (0 != memcmp(st.begin(), list.begin(), st.size()));
  }

  typedef void(*each_fn)(T&, size_t);
  typedef void(*printer)(T);
  
  ////////////////////////////////////////////////////////////
  void each(each_fn it_fn) {
    for (int i = 0; i < count(); ++i) {
      it_fn(at(i), i);
    }
  }
  
  ////////////////////////////////////////////////////////////
  void print(printer p) {
    for (int i = 0; i < count(); ++i) {
      p((*this)[i]);
    }
  }
};

}

#endif