cmake_minimum_required(VERSION 4.0.2)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Raylib setup ---
set(RAYLIB_DIR "$ENV{HOME}/_dev/raylib")
set(RAYLIB_SRC_DIR "${RAYLIB_DIR}/src")

# --- Static library target ---
add_library(smile STATIC
    StateMachine/StateMachine.c
    ParticleSystem/ParticleSystem.c
)

# Public headers for the library
target_include_directories(smile PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(smile PRIVATE "${RAYLIB_SRC_DIR}")

# --- Test executable target ---
# --- Test executables ---
add_executable(TestSmile tests/TestSmile.c)
target_link_libraries(TestSmile PRIVATE smile "${RAYLIB_SRC_DIR}/libraylib.a")
target_include_directories(TestSmile PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include "${RAYLIB_SRC_DIR}")

add_executable(TestParticleSystem tests/TestParticleSystem.c)
target_link_libraries(TestParticleSystem PRIVATE smile "${RAYLIB_SRC_DIR}/libraylib.a")
target_include_directories(TestParticleSystem PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include "${RAYLIB_SRC_DIR}")
