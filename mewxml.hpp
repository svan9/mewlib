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
#include <vector>
#include <string>
#include <variant>

namespace mew::xml {

  class Element {
  public:
    using Attribute = std::variant<std::monostate, const char *, int, double, float>;
    typedef const char* attr_key;
    typedef std::vector<Element> children_t;
    struct AttributePair {
      attr_key name;
      Attribute value;

      AttributePair(attr_key name, Attribute value): name(name), value(value) {} 
    };
    // typedef std::unordered_map<attr_key, Attribute> attrs_t;
    typedef std::vector<AttributePair> attrs_t;
  private:
    const char* m_tag;
    children_t m_children;
    attrs_t m_attrs;
    const char* m_text_value = nullptr;
  public:
    Element(): m_tag("dom") { }
    // text element
    Element(const char* value): m_text_value(value), m_tag("TEXT") { }

    const char* text() const {
      return m_text_value;
    }

    const char* tag() const {
      return m_tag;
    }

    void tag(const char* tag) {
      m_tag = tag;
    }

    void attr(attr_key key, Attribute value) {
      AttributePair pair(key, value);
      m_attrs.push_back(pair);
    }

    children_t& Children() {
      return m_children;
    }

    Attribute attr(attr_key key) const {
      for (auto& pair: m_attrs) {
        if (mew::strcmp(pair.name, key)) {
          return pair.value;
        }
      }
      return nullptr;
    }

    void addChild(const Element& child) {
      m_children.push_back(child);
    }

    Element& child() {
      m_children.emplace_back();
      return m_children.back();
    }
    
    template<typename... Args>
    Element& child(Args &&... args) {
      m_children.emplace_back(args...);
      return m_children.back();
    }

    void child(Element& child) {
      m_children.push_back(child);
    }

    void child(Element* child) {
      if (child) {
        m_children.push_back(*child);
      }
    }

    std::string toString() {
      std::string str;
      str += '  <';
      if (m_text_value) {
        str += m_tag;
        str += ">";
        str += m_text_value;
        str += "</";
        str += m_tag;
        str += ">\n";
        return str;
      }
      if (m_children.empty()) {
        str += "/";
      }
      str += m_tag;
      for (auto& pair: m_attrs) {
        str += ' ';
        str += pair.name;
        str += '=';
        std::visit([&str](auto&& arg) {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, const char*>) {
            str += '"';
            str += arg ? arg : "";
            str += '"';
          } else
          if constexpr (std::is_same_v<T, std::monostate>) {
            return;
          } else {
            str += std::to_string(arg);
          }
        }, pair.value);
      }
      str += ">\n";
      if (!m_children.empty()) {
        for (auto& child: m_children) {
          str += child.toString();
        }
        str += "</";
        str += m_tag;
        str += ">\n";
      }
      return str;
    }
    
  };

  class XMLParser {
  private:
    enum struct TokenType {
      OpenTag,
      CloseTag,
      Slash,
      Text,
      String,
      Equal,
      EndOfFile
    };

    struct Token {
      TokenType type;
      const char* value = nullptr;

      Token() {}
      Token(TokenType t, const char* v) : type(t), value(v) {}
      Token(TokenType t) : type(t) {}
    };
    
    mew::stack<Token> m_tokens;

  public:
    XMLParser() { }
    XMLParser(const char* text) { 
      parse(text);  
    }

    void parse(const char* text) {
      mew::utils::TokenRow tr(text);
      const char* word;
      while ((word = *tr++) != nullptr) {
        if (word[0] == '\0') continue;
        if (word[0] == '<') {
          m_tokens.push(Token(TokenType::OpenTag));
        } else 
        if (word[0] == '>') {
          m_tokens.push(Token(TokenType::CloseTag));
        } else
        if (word[0] == '/') {
          m_tokens.push(Token(TokenType::Slash));
        } else
        if (word[0] == '\"' || word[0] == '\'') {
          m_tokens.push(Token(TokenType::String, mew::utils::str_to_str(word)));
        } else
        if (word[0] == '=') {
          m_tokens.push(Token(TokenType::Equal));
        } else {
          m_tokens.push(Token(TokenType::Text, scopy(word)));
        }
      }
    }

    void parseAttribute(int& i, Element* e) {
      for (; i < m_tokens.count() && m_tokens[i].type != TokenType::CloseTag; ++i) {
        Token &attr_token = m_tokens[i];
        if (attr_token.type == TokenType::Equal) {
          const char *attr_key = m_tokens[i - 1].value;
          Element::Attribute attr_value;
          if (m_tokens[i + 1].type == TokenType::String) {
            const char *str_attr_value = m_tokens[i + 1].value;
            attr_value = str_attr_value;
          }
          else {
            auto number_value = mew::utils::str_to_number(m_tokens[i + 1].value);
            if (std::holds_alternative<int>(number_value)) {
              attr_value = std::get<int>(number_value);
            }
            else if (std::holds_alternative<double>(number_value)) {
              attr_value = std::get<double>(number_value);
            }
            else if (std::holds_alternative<float>(number_value)) {
              attr_value = std::get<float>(number_value);
            }
          }
          ++i;
          e->attr(scopy(attr_key), attr_value);
        }
      }
    }

    Element* getDom() {
      Element* root = new Element();
      Element* current = root;
      mew::stack<Element*> opens;
      opens.push(root);

      for (int i = 0; i < m_tokens.count(); ++i) {
        Token& token = m_tokens.at(i);
        switch (token.type) {
          case TokenType::OpenTag: { 
            if (m_tokens.at(i + 1).type == TokenType::Slash) { // </'element ...>
              ++i; // Skip the slash
              Token& next = m_tokens.at(++i);
              // if is tag & not in open
              bool is_self_closing = (next.type == TokenType::Text) && 
                (!opens.empty() && opens.top()->tag() != next.value);
              if (is_self_closing) { // self close element
                current->tag(scopy(next.value));
                parseAttribute(i, current);
              } else { // close element
                opens.erase(current);
              }
            } else { // <'element ...> open element
              if (opens.has(current)) { // if current is open
                current = &current->child();
              } else { // if current is closed
                current = new Element();
              }
              opens.push(current);
              Token& next = m_tokens.at(++i);
              current->tag(scopy(next.value));
              parseAttribute(i, current);
            }
          } break;
          case TokenType::Text:
            current->child(scopy(token.value));
            break;
          default: break;
        }
      }
      return root;
    }
  };

  
  Element* parse(const char* text) {
    using namespace mew::utils;
    XMLParser parser(text);
    return parser.getDom();
  }
  
 
}

namespace mew::Tests {
  void test_xml() {
    using namespace mew::xml;
    const char* text = "<Element attr1='hellow'>";
  }
}

#endif