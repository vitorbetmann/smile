# ——————————————————————————————————————————————————————————————————————————————
# Default: Release build and tests OFF.
# Developer mode: tests default ON; Debug default for single-config generators.
# ——————————————————————————————————————————————————————————————————————————————

cmake_minimum_required(VERSION 3.30)
project(Smile VERSION 1.0 LANGUAGES C)

# ——————————————————————————————————————————————————————————————————————————————
# COMPILER CONFIGURATION
# ——————————————————————————————————————————————————————————————————————————————

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for tools like clangd or IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Disable compiler-specific extensions (GNU, MSVC, etc.) for strict ISO compliance
set(CMAKE_C_EXTENSIONS OFF)


# ——————————————————————————————————————————————————————————————————————————————
# DEVELOPER MODE
# ——————————————————————————————————————————————————————————————————————————————

option(SMILE_DEV "Enable Smile developer mode (debug build with tests and internal checks)" OFF)
if (SMILE_DEV)
    set(SMILE_TESTS_DEFAULT ON)
else ()
    set(SMILE_TESTS_DEFAULT OFF)
endif ()
option(SMILE_TESTS "Build test executables" ${SMILE_TESTS_DEFAULT})

# Apply default build type only for single-config generators and only when user did not set it.
if (SMILE_DEV)
    # Multi-config generators (e.g., Xcode/Visual Studio) ignore CMAKE_BUILD_TYPE.
    if (NOT CMAKE_CONFIGURATION_TYPES AND (NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL ""))
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    endif ()
else ()
    # Multi-config generators (e.g., Xcode/Visual Studio) ignore CMAKE_BUILD_TYPE.
    if (NOT CMAKE_CONFIGURATION_TYPES AND (NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL ""))
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    endif ()
endif ()


# ——————————————————————————————————————————————————————————————————————————————
# BUILD THE "smile" STATIC LIBRARY
# ——————————————————————————————————————————————————————————————————————————————

set(SRC_FILES
        src/Log/Log.c
        src/SceneManager/SceneManager.c
        src/internal/Test/TestInternal.c
        src/internal/_Common/CommonInternal.c
)

add_library(smile STATIC ${SRC_FILES})
if (SMILE_DEV)
    target_compile_definitions(smile PRIVATE SMILE_DEV)
endif ()

# Expose POSIX/BSD extensions (e.g. mkdtemp) in strict -std=c23 mode.
if (NOT WIN32)
    target_compile_definitions(smile PRIVATE _DEFAULT_SOURCE _DARWIN_C_SOURCE)
endif ()


# ——————————————————————————————————————————————————————————————————————————————
# INCLUDE DIRECTORIES (HEADERS)
# ——————————————————————————————————————————————————————————————————————————————

# Expose only public API headers to consumers; keep internal include paths private.
target_include_directories(smile
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Log
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SceneManager
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/Test
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/_Common
)


# ——————————————————————————————————————————————————————————————————————————————
# TOOLS
# ——————————————————————————————————————————————————————————————————————————————

add_executable(GenScene tools/GenScene/GenScene.c)
target_link_libraries(GenScene PRIVATE smile)
target_include_directories(GenScene PRIVATE
        $<TARGET_PROPERTY:smile,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:smile,INCLUDE_DIRECTORIES>
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/GenScene
)
install(TARGETS GenScene RUNTIME DESTINATION bin)


# ——————————————————————————————————————————————————————————————————————————————
# BUILD CONFIGURATION OPTIONS
# ——————————————————————————————————————————————————————————————————————————————

option(SMILE_WARN "Enable runtime warning logs from Smile" ON)
option(SMILE_INFO "Enable runtime info logs from Smile" ON)


# ——————————————————————————————————————————————————————————————————————————————
# CONDITIONAL COMPILE DEFINITIONS
# ——————————————————————————————————————————————————————————————————————————————

if (SMILE_WARN)
    target_compile_definitions(smile PRIVATE SMILE_WARN)
endif ()

if (SMILE_INFO)
    target_compile_definitions(smile PRIVATE SMILE_INFO)
endif ()

# ——————————————————————————————————————————————————————————————————————————————
# TEST BUILD OPTION
# ——————————————————————————————————————————————————————————————————————————————

if (SMILE_TESTS)
    enable_testing()

    function(add_smile_test target_name source_file)
        add_executable(${target_name} ${source_file})
        target_link_libraries(${target_name} PRIVATE smile)
        target_include_directories(${target_name} PRIVATE
                # Reuse smile's include path layout so tests compile under the same header model.
                $<TARGET_PROPERTY:smile,INTERFACE_INCLUDE_DIRECTORIES>
                $<TARGET_PROPERTY:smile,INCLUDE_DIRECTORIES>
        )
        add_test(NAME ${target_name} COMMAND ${target_name})
    endfunction()

    # PUBLIC API TESTS
    add_smile_test(SceneManagerAPITest tests/SceneManagerAPITest.c)
    add_smile_test(LogAPITest tests/LogAPITest.c)

    # TOOL TESTS
    add_executable(GenSceneToolTest tests/GenSceneToolTest.c tools/GenScene/GenScene.c)
    target_compile_definitions(GenSceneToolTest PRIVATE GS_TESTING)
    target_link_libraries(GenSceneToolTest PRIVATE smile)
    target_include_directories(GenSceneToolTest PRIVATE
            $<TARGET_PROPERTY:smile,INTERFACE_INCLUDE_DIRECTORIES>
            $<TARGET_PROPERTY:smile,INCLUDE_DIRECTORIES>
            ${CMAKE_CURRENT_SOURCE_DIR}/tools/GenScene
    )
    add_test(NAME GenSceneToolTest COMMAND GenSceneToolTest)
endif ()


# ——————————————————————————————————————————————————————————————————————————————
# MESSAGES
# ——————————————————————————————————————————————————————————————————————————————

if (CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Smile — Generator configs: ${CMAKE_CONFIGURATION_TYPES}  (select at build time: --config <Debug|Release|...>)")
else ()
    message(STATUS "Smile — Build type: ${CMAKE_BUILD_TYPE}  (override: -DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo|MinSizeRel>)")
endif ()

message(STATUS "Smile — Warning logs: ${SMILE_WARN}  (override: -DSMILE_WARN=ON|OFF)")
message(STATUS "Smile — Info logs: ${SMILE_INFO}  (override: -DSMILE_INFO=ON|OFF)")
message(STATUS "Smile — Build Tests: ${SMILE_TESTS}  (override: -DSMILE_TESTS=ON|OFF)")
