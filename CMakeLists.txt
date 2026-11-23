cmake_minimum_required(VERSION 3.16)
project(lab1 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(main
    src/main.c
    src/player.c
    src/map.c
    src/utils.c
)

target_include_directories(main PRIVATE src)

target_compile_definitions(main PRIVATE DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/")

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
endif()