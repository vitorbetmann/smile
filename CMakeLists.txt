# Minimum required CMake version
cmake_minimum_required(VERSION 3.25)

# Set project name and metadata
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJECT_NAME)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C)

# Compiler configuration
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_EXTENSIONS OFF)   # OFF = strict ISO C, ON = allow GNU extensions


# Raylib precompiled static lib and headers in external/
set(RAYLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib")
set(RAYLIB_LIB "${RAYLIB_DIR}/libraylib.a")
set(RAYLIB_INCLUDE "${RAYLIB_DIR}/include")

# Build the SMILE static library
add_library(smile STATIC
        src/StateMachine/StateMachine.c
        src/ParticleSystem/ParticleSystem.c
        src/SaveLoad/SaveLoad.c

        src/_Internal/Log/Log.c
        src/_Internal/Test/TestInternal.c
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
option(SMILE_WARNINGS_ENABLED "Enable runtime warning messages from SMILE" ON)

# Option to enable/disable info logs
option(SMILE_INFO_ENABLED "Enable runtime info messages from SMILE" ON)

# Option to enable/disable debug logs
option(SMILE_DEBUG_ENABLED "Enable debug messages from SMILE" OFF)

# Inform user
if (SMILE_RELEASE)
    message(STATUS "SMILE: Compiling in RELEASE mode")
    add_compile_definitions(SMILE_RELEASE)
else ()
    message(STATUS "SMILE: Compiling in DEBUG mode")
endif ()

if (SMILE_WARNINGS_ENABLED AND NOT SMILE_RELEASE)
    message(STATUS "SMILE: Warnings ENABLED")
    add_compile_definitions(SMILE_WARNINGS_ENABLED)
else ()
    message(STATUS "SMILE: Warnings DISABLED")
endif ()

if (SMILE_INFO_ENABLED AND NOT SMILE_RELEASE)
    message(STATUS "SMILE: Info logs ENABLED")
    add_compile_definitions(SMILE_INFO_ENABLED)
else ()
    message(STATUS "SMILE: Info logs DISABLED")
endif ()

# if(SMILE_DEBUG_ENABLED AND NOT SMILE_RELEASE)
#     message(STATUS "SMILE: Debug logs ENABLED")
#     add_compile_definitions(SMILE_DEBUG_ENABLED)
# else()
#     message(STATUS "SMILE: Debug logs DISABLED")
# endif()

# Option to enable test builds
option(SMILE_TESTS "Build test executables" OFF)
if (SMILE_TESTS)
    message(STATUS "SMILE: Compiling TEST files")

    # --------------------------------------------------
    # PUBLIC
    # --------------------------------------------------

    # StateMachine -------------------------------------
    add_executable(StateMachineTest tests/StateMachine/StateMachineTest.c)
    target_link_libraries(StateMachineTest PRIVATE smile)
    target_include_directories(StateMachineTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    # ParticleSystem -----------------------------------
    # TODO Add and link Particle System test

    # SaveLoad -----------------------------------------
    add_executable(SaveLoadTest tests/SaveLoad/SaveLoadTest.c)
    target_link_libraries(SaveLoadTest PRIVATE smile)
    target_include_directories(SaveLoadTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    # --------------------------------------------------
    # INTERNAL
    # --------------------------------------------------

    # _Common ------------------------------------------
    # TODO #13 Add and link _Common test

    # Log ----------------------------------------------
    # TODO #13 Add and link Log test

    # Test ---------------------------------------------
    # TODO #13 Add and link Test test

endif ()
