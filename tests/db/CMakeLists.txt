project(mew_socket_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # -Ofast

add_executable("${PROJECT_NAME}_server" server.cpp)
add_executable("${PROJECT_NAME}_client" client.cpp)
if(WIN32)
		target_link_libraries("${PROJECT_NAME}_server" PRIVATE ws2_32)
		target_link_libraries("${PROJECT_NAME}_client" PRIVATE ws2_32)
else()
		target_link_libraries("${PROJECT_NAME}_server" PRIVATE pthread)
		target_link_libraries("${PROJECT_NAME}_client" PRIVATE pthread)
endif()