/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_XML_LIB_SO2U
#define MEW_XML_LIB_SO2U
#include "mewlib.h"
#include "mewstack.hpp"
#include "mewutils.hpp"
#include <unordered_map>

namespace mew::xml {
  #ifdef __CXX20
    template<typename T>
    concept ElementAttributeSupportedTypes = std::is_same<T, int>::value &&
      std::is_same<T, float>::value &&
      std::is_same<T, double>::value &&
      std::is_same<T, const char*>::value;
  #endif
  class Element {
  public:
    struct Attribute {
      enum struct Type {
        STR, INT, DBL, FLT
      } type;
      union {
        const char* str_value;
        int int_value;
        double dbl_value;
        float flt_value;
      };
    };
    typedef const char* attr_key;
    typedef mew::stack<Element> children_t;
    typedef std::unordered_map<attr_key, Attribute> attrs_t;
  private:
    const char* m_tag;
    children_t m_children;
    attrs_t m_attrs;
  public:
    Element() { }

    const char* tag() const noexcept {
      return m_tag;
    }

    attrs_t& attrs() {
      return m_attrs;
    }

    children_t& children() {
      return m_children;
    }

    void find(const char* tag, mew::stack<Element*>& elements) {      
      for(int i = 0; i < m_children.size(); ++i) {
        Element& el = m_children[i];
        if (mew::strcmp(el.tag(), tag)) {
          elements.push(&el);
        }
        el.find(tag, elements);
      }
    }

    mew::stack<Element*> find(const char* tag) {
      mew::stack<Element*> elements;
      for(int i = 0; i < m_children.size(); ++i) {
        Element& el = m_children[i];
        if (mew::strcmp(el.tag(), tag)) {
          elements.push(&el);
        }
        el.find(tag, elements);
      }
      return elements;
    }

    void attr(attr_key name, Attribute&& value) noexcept {
      auto finded = m_attrs.find(name);
      if (finded == m_attrs.end()) {
        m_attrs.insert({name, value});
      } else {
        finded->second = value;
      }
    }

    void attr(attr_key name, int value) {
      attr(name, {
        .type = Attribute::Type::INT,
        .int_value = value
      });
    }
    void attr(attr_key name, float value) {
      attr(name, {
        .type = Attribute::Type::FLT,
        .flt_value = value
      });
    }
    void attr(attr_key name, double value) {
      attr(name, {
        .type = Attribute::Type::DBL,
        .dbl_value = value
      });
    }
    void attr(attr_key name, const char* value) {
      attr(name, {
        .type = Attribute::Type::STR,
        .str_value = value
      });
    }
    char* attr(const char* text) {
      using namespace mew::utils;
      char* begin = (char*)text;
      char* end = begin + strlen(text);
      while (begin != end) {
        size_t attr_size = 0;
        while (*begin != '=' || *begin != '\0') {
          ++begin; ++attr_size;
          if (*begin == '=') ++begin; 
        }
        if (*begin == '\0') ++begin;
        char* attr_name = scopy(begin, --attr_size);
        begin = shift_word(begin);
        if (begin == nullptr) {
          return nullptr;
        }
        char* str_value = str_to_str(begin);
        if (str_value != nullptr) {
          attr(attr_name, str_value);
          begin = shift_word(begin);
          continue;
        }
        bool success;
        int int_value = str_to_int(begin, success);
        if (success) {
          attr(attr_name, int_value);
          begin = shift_word(begin);
          continue;
        }
        float flt_value = str_to_float(begin, success);
        if (success) {
          attr(attr_name, flt_value);
          begin = shift_word(begin);
          continue;
        }
        double dbl_value = str_to_double(begin, success);
        if (success) {
          attr(attr_name, dbl_value);
          begin = shift_word(begin);
          continue;
        }
      }
      return begin;
    }

    Attribute& get_attr(attr_key name) {
      auto finded = m_attrs.find(name);
      MewUserAssert(finded != m_attrs.end(), "cannot find attribute");
      return finded->second;
    }

    Attribute* get_attr_s(attr_key name) {
      auto finded = m_attrs.find(name);
      if(finded != m_attrs.end()) {
        return nullptr;
      }
      return &finded->second;
    }

    bool has_attr(attr_key name) const noexcept {
      return m_attrs.find(name) != m_attrs.end();
    }

    #ifdef __CXX20
    template<ElementAttributeSupportedTypes T>
    #else
    template<typename T>
    #endif
    T attr(attr_key name) {
    #ifndef __CXX20
      static_assert(
        std::is_same<T, int>::value &&
        std::is_same<T, float>::value &&
        std::is_same<T, double>::value &&
        std::is_same<T, const char*>::value,
         "unsupported user type of attribute value");
    #endif
      auto attr = get_attr(name);
      switch (attr.type) {
        case Attribute::Type::STR: 
          MewUserAssert((std::is_same<T, const char*>::value), "mismatch types");
          return attr.str_value;
        case Attribute::Type::INT: 
          MewUserAssert((std::is_same<T, int>::value), "mismatch types");
          return attr.int_value;
        case Attribute::Type::DBL: 
          MewUserAssert((std::is_same<T, double>::value), "mismatch types");
          return attr.dbl_value;
        case Attribute::Type::FLT: 
          MewUserAssert((std::is_same<T, float>::value), "mismatch types");
          return attr.flt_value;
        default: MewUserAssert(false, "unsupported user type of attribute value");
      }
    }
    
    void parse(const char* text) {
      using namespace mew::utils;
      this->children().clear();
      char* begin = (char*)text;
      char* end = begin + strlen(text);
      while (begin != end) {
        if (*begin == '<') {
          size_t tag_size = 0;
          char* tag_end = begin;
          while (*tag_end != '>' || *tag_end != '\1') {
            ++tag_end; ++tag_size;
          }
          char* words = str_separate(begin+1, tag_size-1);
          Element el;
          el.m_tag = words;
          for (;words != nullptr;) {
            words = shift_word(words);
            words = el.attr(words);
          }
          m_children.push(el);
          
        }
      }
    }
  };
}

namespace mew::Tests {
  void test_xml() {
    using namespace mew::xml;
    const char* text = "<Element>";
  }
}

#endif