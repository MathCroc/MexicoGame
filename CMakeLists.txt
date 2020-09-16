cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(COMMON_COMPILE_OPTIONS -std=c++17 -Wall -Werror -O3 -march=native -pthread)

add_executable(mexico-game main.cpp)
target_compile_options(mexico-game PRIVATE ${COMMON_COMPILE_OPTIONS})
target_include_directories(mexico-game
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ./include
        ./src
        ./players)
