/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_ARGS_LIB_SO2U
#define MEW_ARGS_LIB_SO2U
#include "mewlib.h"
#include "mewstack.hpp"
#include "mewstring.hpp"
#include <iostream>
#include <fstream>
#ifdef _WIN32
  #include <windows.h>
  #include <libloaderapi.h>
#endif

namespace mew {

  ///////////////////////////////////////////////////////
  bool str_is_path(const char* path) {
    std::ifstream test(path);
    return !(!test);
  }
  
  const char* executable_name() {
  #if defined(PLATFORM_POSIX) || defined(__linux__) //check defines for your setup
    std::string sp;
    std::ifstream("/proc/self/comm") >> sp;
    return sp.c_str();

  #elif defined(_WIN32)
    char* buf = new char[MAX_PATH];
    GetModuleFileNameA(NULL, buf, MAX_PATH);
    return buf;

  #else
    MewUserAssert(false, "unrecognized platform");
  #endif
  }

  char** SkipToExec(int argc, char** args) {
    const char* exec_name = executable_name(); 
    char** begin = args;
    char** end = args+(argc*sizeof(*args));
    while (begin != end) {
      char* arg = *(begin++);
      if (strcmp(arg, (char*)exec_name, strlen(exec_name)) == 1) {
        return begin;
      }
    }
    return nullptr;
  }

  template<size_t N>
  void SkipToExec(mew::stack<char*, N>& _line) {
    const char* exec_name = executable_name(); 
    char** begin = _line.data();
    char** end = begin+(_line.size());
    while (begin != end) {
      _line++;
      char* arg = *(begin++);
      if (strcmp(arg, (char*)exec_name, strlen(exec_name)) == 1) {
        break;
      }
    }
  }

  class args {
  private:
    mew::stack<char*, 8U> _M_args;
  public:
    ////////////////////////////////////////////////////////////
    args(int argc, char** argv): _M_args(argv, argc) {}

    ////////////////////////////////////////////////////////////
    void normalize()  {
      SkipToExec(_M_args.size(), _M_args.data());
    }

    ////////////////////////////////////////////////////////////
    bool has(const char* element) const noexcept {
      return -1 != _M_args.indexOf((char*)element);
    }

    ////////////////////////////////////////////////////////////
    char* getNext(const char* element) {
      size_t idx = _M_args.indexOf((char*)element);
      MewAssert(idx != -1);
      return _M_args.at(idx+1);
    }

    ////////////////////////////////////////////////////////////
    char* getNextPath(size_t idx = 0) {
      int counter = 0;
      for (int i = 0; i < _M_args.size(); ++i) {
        char* e = _M_args.at(i);
        if (str_is_path(e)) {
          if (counter++ >= idx) {
            return e;
          };
        }
      }
      return nullptr;
    }

    ////////////////////////////////////////////////////////////
    char* getOneStartsWith(const char* start) {
      size_t length = strlen(start);
      for (int i = 0; i < _M_args.size(); ++i) {
        char* e = _M_args.at(i);
        if (0 == memcmp(e, start, length)) {
          return e+length;
        }
      }
      return nullptr;
    }

    ////////////////////////////////////////////////////////////
    mew::stack<char*, 8U> getStartsWith(const char* start) {
      size_t length = strlen(start);
      mew::stack<char*, 8U> all;
      for (int i = 0; i < _M_args.size(); ++i) {
        char* e = _M_args.at(i);
        if (0 == memcmp(e, start, length)) {
          all.push(e+length);
        }
      }
      return all;
    }

    ////////////////////////////////////////////////////////////
    size_t size() const noexcept {
      return _M_args.size();
    }

    ////////////////////////////////////////////////////////////
    bool has_needs(size_t needs_count) const noexcept {
      return _M_args.size() >= needs_count;
    }
  };

}

#endif