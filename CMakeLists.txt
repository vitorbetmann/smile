cmake_minimum_required(VERSION 4.0.2)

# Extract project name from the current source directory
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C)

# Set C standard and require it
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile_commands.json for tools like clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Raylib setup ---
# Set your local raylib directory (adjust as needed)
set(RAYLIB_DIR "$ENV{HOME}/_dev/raylib")
set(RAYLIB_SRC_DIR "${RAYLIB_DIR}/src")

# --- Create static library target for SMILE engine ---
add_library(smile STATIC
    StateMachine/StateMachine.c
    ParticleSystem/ParticleSystem.c
)

# Public headers for users of the smile library
target_include_directories(smile PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Private include directory for raylib source (for internal compilation)
target_include_directories(smile PRIVATE "${RAYLIB_SRC_DIR}")

# --- Test executables for StateMachine and ParticleSystem modules ---

# StateMachine test
add_executable(TestStateMachine tests/StateMachine/Test_SM.c)
target_link_libraries(TestStateMachine PRIVATE smile "${RAYLIB_SRC_DIR}/libraylib.a")
target_include_directories(TestStateMachine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${RAYLIB_SRC_DIR}"
)

# ParticleSystem test
add_executable(TestParticleSystem tests/TestParticleSystem.c)
target_link_libraries(TestParticleSystem PRIVATE smile "${RAYLIB_SRC_DIR}/libraylib.a")
target_include_directories(TestParticleSystem PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${RAYLIB_SRC_DIR}"
)
