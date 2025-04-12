project(mew_stack_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

add_executable(${PROJECT_NAME} main.cpp)