cmake_minimum_required(VERSION 4.0.2)

# Project metadata
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C)

# Compiler settings
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Raylib paths
set(RAYLIB_DIR "$ENV{HOME}/_dev/raylib")
set(RAYLIB_SRC_DIR "${RAYLIB_DIR}/src")

# SMILE static library
add_library(smile STATIC
    src/StateMachine/StateMachine.c
    src/ParticleSystem/ParticleSystem.c
)

# Public and private includes
target_include_directories(smile PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(smile PRIVATE "${RAYLIB_SRC_DIR}")

# Test: StateMachine
add_executable(TestStateMachine tests/StateMachine/test_state_machine.c)
target_link_libraries(TestStateMachine PRIVATE smile "${RAYLIB_SRC_DIR}/libraylib.a")
target_include_directories(TestStateMachine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${RAYLIB_SRC_DIR}"
)

# Test: ParticleSystem
add_executable(TestParticleSystem tests/ParticleSystem/test_particle_system.c)
target_link_libraries(TestParticleSystem PRIVATE smile "${RAYLIB_SRC_DIR}/libraylib.a")
target_include_directories(TestParticleSystem PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${RAYLIB_SRC_DIR}"
)
