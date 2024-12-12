/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_STACK_LIB_SO2U
#define MEW_STACK_LIB_SO2U
#include "mewlib.h"
#include "mewmath.hpp"
#include "mewstack.hpp"

namespace mew {

template<typename Key, typename Val, size_t alloc_size = 1U>
class map {
private:
  mew::stack<size_t> _M_keys;
  mew::stack<Val> _M_values;
public:
  ////////////////////////////////////////////////////////////
  map() {}

  ////////////////////////////////////////////////////////////
  bool has(Key& idx) const noexcept {
    return _M_keys.has(mew::get_hash(idx));
  }

  ////////////////////////////////////////////////////////////
  T& at(Key& idx) {
    MewAssert(has(idx));
    size_t hash = mew::get_hash(idx);
    size_t index = _M_keys.indexOf(hash);
    MewAssert(index != (size_t)(-1));
    return _M_values.at(index);
  }

  ////////////////////////////////////////////////////////////
  size_t size() const noexcept {
    return _M_keys.size();
  }

  ////////////////////////////////////////////////////////////
  void erase(Key& idx) {
    MewAssert(has(idx));
    size_t hash = mew::get_hash(idx);
    size_t index = _M_keys.indexOf(hash);
    MewAssert(index != (size_t)(-1));
    _M_value.erase(index);
    _M_keys.erase(hash);
  }

  ////////////////////////////////////////////////////////////
  bool empty() const noexcept {
    return _M_keys.empty();
  }

  ////////////////////////////////////////////////////////////
  void clear() {
    _M_keys.clear();
    _M_values.clear();
  }

  ////////////////////////////////////////////////////////////
  void insert(Key& k, Val& v) {
    size_t hash = mew::get_hash(idx);
    size_t index = _M_keys.indexOf(hash);
    if (index == (size_t)(-1)) {
      index = _M_keys.push(hash);
    }
    if (_M_values.has(index)) {
      memcpy(&_M_values.at(index), &v, sizeof(v));
    } else {
      _M_values.push(v);
    }
  }
};

}

#endif