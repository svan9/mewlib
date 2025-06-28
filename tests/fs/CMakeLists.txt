project(mew_fs_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # -Ofast

add_executable(${PROJECT_NAME} main.cpp)
if(WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()