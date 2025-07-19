# Minimum required CMake version
cmake_minimum_required(VERSION 3.25)

# Set project name and metadata
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C)

# Compiler configuration
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Raylib precompiled static lib and headers in external/
set(RAYLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib")
set(RAYLIB_LIB "${RAYLIB_DIR}/libraylib.a")
set(RAYLIB_INCLUDE "${RAYLIB_DIR}/include")

# Build the SMILE static library
add_library(smile STATIC
    src/StateMachine/StateMachine.c
    src/ParticleSystem/ParticleSystem.c
)

# Include raylib headers for Smile
target_include_directories(smile PRIVATE "${RAYLIB_INCLUDE}")

# Link raylib static library for Smile
target_link_libraries(smile PRIVATE "${RAYLIB_LIB}")


# Set public and private include paths
target_include_directories(smile PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(smile PRIVATE "${RAYLIB_SRC_DIR}")

# Option to enable/disable release mode
option(SMILE_RELEASE "Compile SMILE in release mode" OFF)

# Option to enable/disable warnings
option(SMILE_WARNINGS "Enable runtime warning messages from SMILE" ON)

# Inform user
if(SMILE_RELEASE)
    message(STATUS "SMILE: Compiling in RELEASE mode")
    add_compile_definitions(SMILE_RELEASE)
else()
    message(STATUS "SMILE: Compiling in DEBUG mode")
endif()

if(SMILE_WARNINGS AND NOT SMILE_RELEASE)
    message(STATUS "SMILE: Warnings ENABLED")
    add_compile_definitions(SMILE_WARNINGS)
else()
    message(STATUS "SMILE: Warnings DISABLED")
endif()

# Option to enable test builds
option(SMILE_TESTS "Build test executables" OFF)
if(SMILE_TESTS)
    message(STATUS "SMILE: Compiling TEST files")

    # Add and link StateMachine test
    add_executable(TestStateMachine tests/StateMachine/TestStateMachine.c)
    target_include_directories(TestStateMachine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    # Add and link ParticleSystem test
    add_executable(TestParticleSystem tests/ParticleSystem/TestParticleSystem.c)
    target_link_libraries(TestParticleSystem PRIVATE smile "${RAYLIB_LIB}")
    target_include_directories(TestParticleSystem PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        "${RAYLIB_INCLUDE}"
    )
endif()
