/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#define MEW_ARENA_LIB_SO2U
#ifndef MEW_ARENA_LIB_SO2U
#include "mewlib.h"
#include <stdlib.h>
#include "mewmath.hpp"

namespace mew {
  class Arena;

  class Pointer {
  friend class Arena;
  private:
    Arena* parent;
    size_t size;
    byte data[];
    size_t links = 1;

    Pointer() {}
    Pointer(size_t size, byte data)
    : data(data), size(size) {}
    Pointer(size_t size)
    : data(0), size(size) {}
    template<typename T>
    Pointer(T data)
    : data(data), size(sizeof(T)) {}
  public:
    template<typename T>
    T unpack() {
      MewUserAssert(size < sizeof(T), "cannot unpack for <T> value, too big");
      return (T)*data;
    }

    template<typename T>
    T& operator=(T&& value) {
      MewUserAssert(size < sizeof(T), "cannot unpack for <T> value, too big");
      memcpy(&data, &value, sizeof(T));
      return *this;
    }
  };

  class Arena {
  private:
    mew::stack<Pointer> datas;
  public:
    Arena() {}

    Pointer& alloc(size_t size) {
      Pointer _ptr(size);
      _ptr.parent = this;
      datas.push(_ptr);
      return datas.top();
    }

    template<typename T>
    Pointer& alloc() {
      return alloc(sizeof(T));
    }


  };

}

#endif