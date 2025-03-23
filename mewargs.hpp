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
  // #include <libloaderapi.h>
#endif

namespace mew {

  ///////////////////////////////////////////////////////
  bool str_is_path(const char* path) {
    std::ifstream test(path);
    return !(!test);
  }

  inline bool is_exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
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
      if (mew::strcmp(arg, (char*)exec_name, strlen(exec_name))) {
        return begin;
      }
    }
    return nullptr;
  }

  template<size_t N>
  void SkipToExec(mew::stack<char*, N>& _line) {
    const char* exec_name = executable_name(); 
    for (int i = 0; i < _line.size(); ++i) {
      char* arg = _line[i];
      if (mew::strcmp(arg, exec_name)) {
        _line.shift();
        break;
      }
    }
  }

  HANDLE run_cmd_async(char* items) {
    STARTUPINFO siStartInfo;
    ZeroMemory(&siStartInfo, sizeof(siStartInfo));
    siStartInfo.cb = sizeof(STARTUPINFO);
    // TODO: check for errors in GetStdHandle
    siStartInfo.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
    siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    siStartInfo.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION piProcInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    BOOL bSuccess = CreateProcessA(NULL, items, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo);
    CloseHandle(piProcInfo.hThread);
    return piProcInfo.hProcess;
  }

  size_t run_cmd_sync(char* cmd) {
    HANDLE hProcess = run_cmd_async(cmd);
    WaitForSingleObject(hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(hProcess, &exitCode);
    CloseHandle(hProcess);

    return exitCode;
  }
  std::string run_cmd_get_output(const char* cmd) {
    std::string result;
    char buffer[128];
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) {
      throw std::runtime_error("popen() failed!");
    }
    try {
      while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
      }
    } catch (...) {
      _pclose(pipe);
      throw;
    }
    _pclose(pipe);
    return result;
  }


  class args {
  private:
    mew::stack<char*, 8U> _M_args;
  public:
    ////////////////////////////////////////////////////////////
    args(int argc, char** argv): _M_args(argv, argc) {
    }
    
    ////////////////////////////////////////////////////////////
    void normalize()  {
      SkipToExec(_M_args);
      // _M_args.print((mew::stack<char*, 8U>::printer)puts);
    }

    ////////////////////////////////////////////////////////////
    bool has(const char* element) const noexcept {
      for (int i = 0; i < _M_args.size(); ++i) {
        char* e = _M_args.at(i);
        if (mew::strcmp(e, element)) {
          return true;
        }
      }
      return false;
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
        if (mew::memcmp(e, start, length)) {
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
        if (mew::memcmp(e, start, length)) {
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
    
    ////////////////////////////////////////////////////////////
    void print() {
      printf("{ |%llu|", _M_args.size());
      for (int i = 0; i < _M_args.size(); ++i) {
        printf("[%s]", _M_args[i]);
      }
      printf(" }\n");
    }
  };

}

#endif