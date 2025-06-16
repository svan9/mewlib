/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/

#ifndef MEW_FEATURE_LIB_SO2U
#define MEW_FEATURE_LIB_SO2U
#include "mewlib.h"
#include <chrono>
#include <thread>

#include <iostream>

namespace mew {
  namespace fs = std::filesystem;
  typedef void(*watch_file_fn)(const char* path);

  void watch_file(const char* file_path, watch_file_fn handler) { 
    handler(file_path);
    // todo impl with mew::fs;
  }

};

#endif