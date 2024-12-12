/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_SMAP_LIB_SO2U
#define MEW_SMAP_LIB_SO2U
#include "mewlib.h"
#include "mewmath.hpp"
#include "mewstack.hpp"

namespace mew {

template<typename Key, typename Val, size_t alloc_size = 1U>
class simple_map {
public:
  struct pair {
    Key k;
    Val v;
  };
private:
  mew::stack<pair, alloc_size> _M_elements;
public:
  ////////////////////////////////////////////////////////////
  simple_map() {}

  ////////////////////////////////////////////////////////////
  bool has(Key& idx, size_t* out_idx = nullptr) noexcept {
    for (int i = 0; i < _M_elements.size(); ++i) {
      pair* element = &_M_elements.at(i);
      if (0 == memcmp(&element->k, &idx, sizeof(Key))) {
        if (out_idx != nullptr) {
          *out_idx = i;
        }
        return true;
      }
    }
    *out_idx = -1;
    return false;
  }

  ////////////////////////////////////////////////////////////
  bool has(Key& idx, pair** out_pair = nullptr) noexcept {
    for (size_t i = 0; i < _M_elements.size(); ++i) {
      pair* element = &_M_elements.at(i);
      if (0 == memcmp(&element->k, &idx, sizeof(Key))) {
        if (out_pair != nullptr) {
          *out_pair = element;
        }
        return true;
      }
    }
    *out_pair = nullptr;
    return false;
  }

  ////////////////////////////////////////////////////////////
  Val& at(Key& idx) {
    pair* p = (pair*)malloc(sizeof(pair));
    MewAssert(has(idx, &p));
    return *p;
  }
  ////////////////////////////////////////////////////////////
  Val& at(const Key& idx) {
    pair* p = (pair*)malloc(sizeof(pair));
    MewAssert(has(idx, &p));
    return *p;
  }
  ////////////////////////////////////////////////////////////
  Val& at(Key&& idx) {
    pair* p = (pair*)malloc(sizeof(pair));
    MewAssert(has(idx, &p));
    return p->v;
  }

  ////////////////////////////////////////////////////////////
  size_t size() const noexcept {
    return _M_elements.size();
  }

  ////////////////////////////////////////////////////////////
  void erase(Key& idx) {
    size_t real_idx;
    MewAssert(has(idx, &real_idx));
    _M_elements.erase(real_idx);
  }

  ////////////////////////////////////////////////////////////
  bool empty() const noexcept {
    return _M_elements.empty();
  }

  ////////////////////////////////////////////////////////////
  void clear() {
    _M_elements.clear();
  }

  ////////////////////////////////////////////////////////////
  void insert(pair& p) {
    _M_elements.push(p);
  }

  ////////////////////////////////////////////////////////////
  void insert(pair&& p) {
    _M_elements.push(p);
  }
  ////////////////////////////////////////////////////////////
  void insert(const pair& p) {
    _M_elements.push(p);
  }

};

}

#endif