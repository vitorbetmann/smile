cmake_minimum_required(VERSION 4.0.2)
project(smile_engine VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create the static library
add_library(smile STATIC
    src/smile.c
)

# Public headers go here
target_include_directories(smile PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Add test executable
add_executable(test_smile
    tests/test_smile.c
)

target_link_libraries(test_smile PRIVATE smile)
target_include_directories(test_smile PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
