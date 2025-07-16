# Minimum required CMake version
cmake_minimum_required(VERSION 3.25)

# Set project name and metadata
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C)

# Compiler configuration
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Raylib source and install paths
set(RAYLIB_DIR "$ENV{HOME}/_dev/raylib")
set(RAYLIB_SRC_DIR "${RAYLIB_DIR}/src")

# Build the SMILE static library
add_library(smile STATIC
    src/StateMachine/StateMachine.c
    src/ParticleSystem/ParticleSystem.c
)

# Set public and private include paths
target_include_directories(smile PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(smile PRIVATE "${RAYLIB_SRC_DIR}")

# Option to enable test builds
option(BUILD_TESTS "Build test executables" OFF)
if(BUILD_TESTS)
    # Add and link StateMachine test
    add_executable(TestStateMachine tests/StateMachine/TestStateMachine.c)
    target_link_libraries(TestStateMachine PRIVATE smile "${RAYLIB_SRC_DIR}/libraylib.a")
    target_include_directories(TestStateMachine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        "${RAYLIB_SRC_DIR}"
    )

    # Add and link ParticleSystem test
    add_executable(TestParticleSystem tests/ParticleSystem/TestParticleSystem.c)
    target_link_libraries(TestParticleSystem PRIVATE smile "${RAYLIB_SRC_DIR}/libraylib.a")
    target_include_directories(TestParticleSystem PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        "${RAYLIB_SRC_DIR}"
    )
endif()
