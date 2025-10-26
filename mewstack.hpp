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
  typedef stack<T, _Alloc> self;
  _Alloc _M_allocator;
  stack(_Alloc allocator): _M_allocator(allocator) { }
public:
  stack(): _M_allocator(1) { } // Initialize _M_capacity
  stack(const stack<T, _Alloc>& ref): _M_allocator(ref._M_allocator) { }
  stack(stack<T, _Alloc>&& ref): _M_allocator(ref._M_allocator) { }
  stack<T, _Alloc>& operator=(const stack<T, _Alloc>& ref) { 
    if (this != &ref) {
      _M_allocator = ref._M_allocator; // Use assignment operator of _Alloc
    }
    return *this;
  }
  stack<T, _Alloc>& operator=(stack<T, _Alloc>&& ref) { 
    if (this != &ref) {
      _M_allocator = std::move(ref._M_allocator); // Use move assignment operator of _Alloc
    }
    return *this;
  }
  
  ////////////////////////////////////////////////////////////
  u64 size() const noexcept {
    return _M_allocator.size();
  }

  ////////////////////////////////////////////////////////////
  u64 count() const noexcept {
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
  stack(T* _array, u64 _length): _M_allocator(1U) {
    _M_allocator.copy(_array, _length);
  }

  ////////////////////////////////////////////////////////////
  stack(stack<T>& ref): _M_allocator(ref._M_allocator) { }
  
  ////////////////////////////////////////////////////////////
  stack(u64 size): _M_allocator(size) { }

  ////////////////////////////////////////////////////////////
  T* data() const noexcept {
    return _M_allocator.begin();
  }

  ////////////////////////////////////////////////////////////
  void resize(u64 new_size, bool ignore_size = false) {
    if (ignore_size) {
      _M_allocator.reserve(new_size);
    } else {
      _M_allocator.resize(new_size);
    }
  }
  
  ////////////////////////////////////////////////////////////
  void reserve(u64 size) {
    _M_allocator.reserve(size);
  }

  ////////////////////////////////////////////////////////////
  bool has(int idx) const noexcept {
    u64 real_idx = get_real_idx(idx);
    return real_idx < count();
  }

  ////////////////////////////////////////////////////////////
  bool has(u64 idx) const noexcept {
    return idx < count();
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
  T& at(u64 idx) const {
    MewAssert(has(idx));
    return data()[idx];
  }
  ////////////////////////////////////////////////////////////
  T& at(u64 idx, u64 offset) const {
    MewAssert(has(idx) && idx-(offset/sizeof(T)) >= 0);
    T* _d = &(data()[idx]);
    _d = (T*)((byte*)_d - offset);
    return *(T*)(_d);
  }

  ////////////////////////////////////////////////////////////
  u64 get_real_idx(int idx) const noexcept {
    MewUserAssert(count() != 0, "stack is empty");
    if (idx == 0) {
      return 0;
    }
    u64 real_idx;
    real_idx = mod(idx, count());
    real_idx = (count() + real_idx) % count();
    return real_idx;
  }

  ////////////////////////////////////////////////////////////
  u64 rget_real_idx_AT(int idx) const noexcept {
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
  T& at(int idx, u64 offset) const {
    return at(get_real_idx(idx-offset));
  }

  ////////////////////////////////////////////////////////////
  [[deprecated("unsafe")]]
  byte* rat(int offset) const {
    MewUserAssert(count() != 0, "stack is empty");
    u64 real_idx = rget_real_idx_AT(offset);
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
  u64 push(T&& value) {
    T* pointer = _M_allocator.alloc();
    copy_to(pointer, value);
    return _M_allocator.count()-1;
  }
  
  ////////////////////////////////////////////////////////////
  template<typename K>
  u64 push(K& value) {
    MewUserAssert(sizeof(T) < sizeof(K), "cant push less type size");
    K* pointer = _M_allocator.template alloc<K>();
    copy_to(pointer, value);
    return _M_allocator.count()-1;
  }

  ////////////////////////////////////////////////////////////
  template<typename K>
  u64 push(K&& value) {
    MewUserAssert(sizeof(T) < sizeof(K), "cant push less type size");
    K* pointer = _M_allocator.template alloc<K>();
    copy_to(pointer, value);
    return _M_allocator.count()-1;
  }
  
  ////////////////////////////////////////////////////////////
  template<u64 N>
  u64 push_array(T(&value)[N]) {
    T* pointer = _M_allocator.template alloc_array<T>(N);
    copy_to(pointer, value);
    return _M_allocator.count()-1;
  }
  ////////////////////////////////////////////////////////////
  u64 push_array(T* value, u64 _count) {
    T* pointer = _M_allocator.template alloc_array<T>(_count);
    copy_to(pointer, value);
    return _M_allocator.count()-1;
  }

  ////////////////////////////////////////////////////////////
  u64 push(const T& value) {
    T* pointer = _M_allocator.alloc();
    copy_to(pointer, value);
    return _M_allocator.count()-1;
  }

  // ////////////////////////////////////////////////////////////
  // u64 push(T* ptr) {
  //   T* pointer = _M_allocator.alloc();
  //   copy_to(pointer, *ptr);
  //   return _M_allocator.count()-1;
  // }

  ////////////////////////////////////////////////////////////
  [[deprecated("unsafe")]]
  u64 push(const T& value, u64 offset) {
    T* pointer = _M_allocator.alloc(offset);
    memcpy(pointer, &value, sizeof(value));
    return offset+1;
  }

  ////////////////////////////////////////////////////////////
  template<typename ...Args>
  u64 emplace(Args&&... args) {
    T* pointer = _M_allocator.alloc();
    new (pointer) T(std::forward<Args>(args)...);
    return _M_allocator.count() - 1;
  }

  ////////////////////////////////////////////////////////////
  u64 insert(const T& value, u64 idx) {
    T* pointer = _M_allocator.alloc(idx);
    memcpy(pointer, &value, sizeof(value));
    return idx+1;
  }
  
  ////////////////////////////////////////////////////////////
  u64 insert(T&& value, u64 idx) {
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
  stack<T, _Alloc>& copy(T* _array, u64 _length, u64 offset = 0) {
    this->_M_allocator.copy(_array, _length, offset = 0);
    return *this;
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
  void vpop() {
    _M_allocator.pop();
  }

  ////////////////////////////////////////////////////////////
  void asc_pop(u64 count = 1) {
    _M_allocator.reduceSize(count);
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
  void erase(u64 start, u64 size = 1) {
    _M_allocator.erase(start, size);
  }
  
  ////////////////////////////////////////////////////////////
  void eraseAfter(u64 start) {
    _M_allocator.eraseAfter(start);
  }
  
  ////////////////////////////////////////////////////////////
  void erase(int idx) {
    erase(get_real_idx(idx), 1);
  }

  ////////////////////////////////////////////////////////////
  void erase(T& value) {
    u64 idx = indexOf(value);
    if (idx != (u64)(-1)) {
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
  u64 indexOf(const T& value) const {
    for (int i = 0; i < size(); ++i) {
      T& cur_el = _M_allocator.begin()[i];
      if (value == cur_el) {
        return i;
      }
    }
    return (u64)(-1);
  }

  ////////////////////////////////////////////////////////////
  u64 indexOf(T&& value) const {
    for (int i = 0; i < size(); ++i) {
      T& cur_el = _M_allocator.begin()[i];
      if (value == cur_el) {
        return i;
      }
    }
    return (u64)(-1);
  }

  ////////////////////////////////////////////////////////////
  void shift() {
    _M_allocator.shift();
  }

  ////////////////////////////////////////////////////////////
  void pushIfNotExists(T&& value) {
    if (indexOf(value) != (u64)(-1)) {
      push(value);
    }
  }

  ////////////////////////////////////////////////////////////
  void pushIfNotExists(const T& value) {
    if (indexOf(value) != (u64)(-1)) {
      push(value);
    }
  }
  
  ////////////////////////////////////////////////////////////
  self shiftAfter(u64 start) {
    auto allocator = _M_allocator.shiftAfter(start);
    return self(allocator);
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

  typedef void(*each_fn)(T&, u64);
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


template<typename T>
void writeArray(std::ofstream& file, T* arr, u64 size) {
  writeBytes(file, size);
  writeBytes(file, arr, size);
}

template<typename T>
u64 readArray(std::ifstream& file, T* arr = nullptr) {
  u64 size = mew::readUInt64(file);
  if (arr) {
    delete arr;
  }
  arr = new T[size];
  for (u64 i = 0; i < size; ++i) {
    file >> arr[i];
  }
  return size;
}

inline void writeString(std::ofstream& file, const char* str) {
  writeArray(file, str, strlen(str)+1);
}

inline u64 readString(std::ifstream& file, char* str) {
  return readArray(file, str);
}

template<typename T, typename _Alloc>
void writeStack(std::ofstream& file, stack<T, _Alloc>& _stack) {
  writeBytes(file, _stack.count());
  writeBytes(file, _stack.begin(), _stack.size());
}

template<typename T, typename _Alloc>
void readStack(std::ifstream& file, stack<T, _Alloc>& _stack) {
  u64 size = mew::readUInt64(file);
  for (u64 i = 0; i < size; ++i) {
    T val;
    file >> val;
    _stack.push(val);
  }
}

template<typename T, typename _Alloc>
stack<T, _Alloc> readStack(std::ifstream& file) {
  stack<T, _Alloc> _stack;
  u64 size = mew::readUInt64(file);
  for (u64 i = 0; i < size; ++i) {
    T val;
    file >> val;
    _stack.push(val);
  }
}


template<typename T>
using stackWriter_t = void(*)(std::ofstream&, T&);
template<typename T>
using stackReader_t = T(*)(std::ifstream&);

template<typename T, typename _Alloc>
stack<T, _Alloc> readStack(stack<const char*> pathes, stackReader_t<T> reader) {
  stack<T, _Alloc> _stack;
  for (u64 i = 0; i < _stack.count(); ++i) {
    std::ifstream& file(std::ios::in | std::ios::binary);
    file.seekg(std::ios::beg);
    MewAssert(file.is_open());
    file >> std::noskipws;
    _stack.push(reader(file));
    file.close();
  }
  return _stack;
}


template<typename T, typename _Alloc>
void writeStack(std::ofstream& file, stack<T, _Alloc>& _stack, stackWriter_t<T> writer) {
  writeBytes(file, _stack.count());
  for (u64 i = 0; i < _stack.count(); ++i) {
    writer(_stack.at(i));
  }
}

template<typename T, typename _Alloc>
void readStack(std::ifstream& file, stack<T, _Alloc>& _stack, stackReader_t<T> reader) {
  u64 size = mew::readUInt64(file);
  for (u64 i = 0; i < size; ++i) {
    _stack.push(reader(file));
  }
}

}

#endif