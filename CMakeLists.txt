cmake_minimum_required(VERSION 3.16)
project(mewlib-test LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-fconcepts)
add_compile_options(-fpermissive)

include_directories("./")
add_subdirectory(tests)

aux_source_directory("./" SOURCE)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "./")
target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})