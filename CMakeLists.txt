cmake_minimum_required(VERSION 3.16)

# Находим все файлы в директории
file(GLOB MEWLIB_SOURCES "*")

# Исключаем заголовки и другие файлы
list(FILTER MEWLIB_SOURCES EXCLUDE REGEX "\\.h$|\\.hpp$|\\.git.*|main\\.cpp")

# Создаем shared library (DLL)
add_library(mewlib SHARED ${MEWLIB_SOURCES})
set_target_properties(mewlib PROPERTIES LINKER_LANGUAGE CXX)

# Статическая линковка std для DLL
target_link_libraries(mewlib PUBLIC -static-libgcc -static-libstdc++ -pthread)

# Включаем директорию
target_include_directories(mewlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Копируем DLL в bin после сборки
add_custom_command(TARGET mewlib POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mewlib> ${CMAKE_BINARY_DIR}/bin/)

install(TARGETS mewlib)