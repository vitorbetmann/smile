# ------------------------------------------------------------------------------
# Smile compiles in Release mode by default, with tests disabled.
# Enable developer mode with -DSMILE_DEVELOPER=ON to build with tests and debug settings.
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.25)
project(Smile VERSION 1.0 LANGUAGES C)

# ------------------------------------------------------------------------------
# COMPILER CONFIGURATION
# ------------------------------------------------------------------------------

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for tools like clangd or IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Disable compiler-specific extensions (GNU, MSVC, etc.) for strict ISO compliance
set(CMAKE_C_EXTENSIONS OFF)


# ------------------------------------------------------------------------------
# DEVELOPER MODE
# ------------------------------------------------------------------------------

option(SMILE_DEVELOPER "Enable Smile developer mode (Debug build with tests)" OFF)

# Default build type and tests
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()
if (NOT DEFINED SMILE_BUILD_TESTS)
    set(SMILE_BUILD_TESTS OFF CACHE BOOL "Build test executables" FORCE)
endif ()

# Developer mode overrides defaults
if (SMILE_DEVELOPER)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    set(SMILE_BUILD_TESTS ON CACHE BOOL "Build test executables" FORCE)
endif ()

message(STATUS "Smile — Build type: ${CMAKE_BUILD_TYPE}")


# ------------------------------------------------------------------------------
# RAYLIB CONFIGURATION
# ------------------------------------------------------------------------------

# Path to the bundled Raylib static library and headers (in external/raylib/)
set(RAYLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib")
set(RAYLIB_LIB "${RAYLIB_DIR}/libraylib.a")     # precompiled static lib
set(RAYLIB_INCLUDE "${RAYLIB_DIR}/include")     # include headers


# ------------------------------------------------------------------------------
# BUILD THE "smile" STATIC LIBRARY
# ------------------------------------------------------------------------------

# Automatically gather all C source files in src/*/*.c and src/_Internal/*/*.c
file(GLOB SRC_PUBLIC "src/*/*.c")
file(GLOB SRC_INTERNAL "src/_Internal/*/*.c")

set(SRC_FILES
        ${SRC_PUBLIC}
        ${SRC_INTERNAL}
)

add_library(smile STATIC ${SRC_FILES})

# Add raylib headers so Smile can include them internally
target_include_directories(smile PRIVATE "${RAYLIB_INCLUDE}")

# Link the precompiled raylib static library into Smile
target_link_libraries(smile PRIVATE "${RAYLIB_LIB}")


# ------------------------------------------------------------------------------
# INCLUDE DIRECTORIES (HEADERS)
# ------------------------------------------------------------------------------

# Allow includes relative to the project root
# Example: #include <include/Log.h>, #include <src/_Internal/StateMachine/InternalState.h>, etc.
target_include_directories(smile
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}                # enables root-relative includes
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# Automatically include all directories in src that contain headers
file(GLOB_RECURSE SRC_HEADERS CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/_Internal/*/*.h
)

set(SRC_INCLUDE_DIRS "")
foreach (header ${SRC_HEADERS})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND SRC_INCLUDE_DIRS ${dir})
endforeach ()

# Remove duplicates
list(REMOVE_DUPLICATES SRC_INCLUDE_DIRS)

# Add them as PRIVATE include paths
target_include_directories(smile PRIVATE ${SRC_INCLUDE_DIRS})


# ------------------------------------------------------------------------------
# BUILD CONFIGURATION OPTIONS
# ------------------------------------------------------------------------------

option(SMILE_LOG_WARNING "Enable runtime warning logs from Smile" ON)
option(SMILE_LOG_INFO "Enable runtime info logs from Smile" ON)

# ------------------------------------------------------------------------------
# CONDITIONAL COMPILE DEFINITIONS
# ------------------------------------------------------------------------------

# Logging settings
if (SMILE_LOG_WARNING)
    add_compile_definitions(SMILE_LOG_WARNING)
    message(STATUS "Smile — Warning logs ENABLED")
else ()
    message(STATUS "Smile — Warning logs DISABLED")
endif ()

if (SMILE_LOG_INFO)
    add_compile_definitions(SMILE_LOG_INFO)
    message(STATUS "Smile — Info logs ENABLED")
else ()
    message(STATUS "Smile — Info logs DISABLED")
endif ()


# ------------------------------------------------------------------------------
# TEST BUILD OPTION
# ------------------------------------------------------------------------------

option(SMILE_BUILD_TESTS "Build test executables" OFF)

if (SMILE_BUILD_TESTS)
    if (SMILE_DEVELOPER OR CMAKE_VERBOSE_MAKEFILE)
        message(STATUS "Smile: Compiling TEST files")
    endif ()

    function(add_smile_test name)
        file(GLOB TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/tests/${name}/*.c")
        if (TEST_SRC)
            set(test_target "${name}Test")
            add_executable(${test_target} ${TEST_SRC})
            target_link_libraries(${test_target} PRIVATE smile)
            # Inherit include dirs from the main library for consistency
            target_include_directories(${test_target} PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    $<TARGET_PROPERTY:smile,INTERFACE_INCLUDE_DIRECTORIES>
                    $<TARGET_PROPERTY:smile,INCLUDE_DIRECTORIES>
            )
        else ()
            message(WARNING "No test source files found for ${name}")
        endif ()
    endfunction()

    # --------------------------
    # PUBLIC TESTS
    # --------------------------
    add_smile_test(SaveLoad)
    add_smile_test(StateMachine)

    # --------------------------
    # INTERNAL TESTS
    # --------------------------
    # TODO add_smile_test(CommonInternalTest)
    # TODO add_smile_test(LogInternalTest)
    # TODO add_smile_test(TestInternalTest)
endif ()