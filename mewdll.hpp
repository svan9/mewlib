#ifndef MEW_DLL_LIB_SO2U
#define MEW_DLL_LIB_SO2U

#include "mewall.h"
#include <filesystem>
#ifdef _WIN32
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif
namespace mew {

#ifdef _WIN32
typedef HANDLE    _dll_handle;
typedef HINSTANCE _dll_hinstance;
typedef FARPROC   _dll_farproc;
#else 
typedef void* _dll_handle;
typedef void* _dll_hinstance;
typedef int(*_dll_farproc)(void);
#endif
  _dll_hinstance LoadDll(std::filesystem::path& path) {
    #ifdef _WIN32
    return ::LoadLibraryA(path.string().c_str());
    #else
    return ::dlopen(path.string().c_str(), RTLD_LAZY);
    #endif
  }

  _dll_hinstance LoadDll(const char* name) {
    std::filesystem::path __path(name);
    if (!__path.is_absolute()) {
      __path = std::filesystem::absolute(__path.lexically_normal());
    }
    return LoadDll(__path);
  }

  _dll_farproc GetFunction(_dll_hinstance handle, const char* name) {
    # ifdef _WIN32
      return ::GetProcAddress(handle, name);
    #else
      return ::dlsym(handle, name);
    #endif
  }

  void UnloadDll(_dll_hinstance handle) {
    #ifdef _WIN32
    ::FreeLibrary(handle);
    #else
    ::dlclose(handle);
    #endif
  }
}

#endif