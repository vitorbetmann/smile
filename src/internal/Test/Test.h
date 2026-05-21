#pragma once

// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Identifies system functions for failure simulation.
 *
 * Used with tsDisable() to specify which function to intercept.
 */
typedef enum
{
    MALLOC,
    CALLOC,
    REALLOC,
    FOPEN,
    MKDIR,
} tsSysFn;

// Functions ———————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Log a "[PASS]" message for a successful test case.
 *
 * @param fnName Name of the test function that passed.
 */
void tsPass(const char *fnName);

/**
 * @brief Force a system function to fail at the nth call.
 *
 * After the failure fires, the function resumes normal behavior.
 *
 * @param fnName Which function to intercept (MALLOC, CALLOC, REALLOC, FOPEN, MKDIR).
 * @param at     Call count at which failure fires; must be ≥ 1.
 * @return true on success, false if fnName is invalid or at is 0.
 */
bool tsDisable(tsSysFn fnName, unsigned int at);

/**
 * @brief Wrapper around malloc() with optional failure simulation.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to allocated memory, or nullptr if failure is simulated.
 */
void *tsMalloc(size_t size);

/**
 * @brief Wrapper around calloc() with optional failure simulation.
 *
 * @param numItems Number of elements to allocate.
 * @param size     Size of each element in bytes.
 * @return Pointer to zero-initialized memory, or nullptr if failure is simulated.
 */
void *tsCalloc(size_t numItems, size_t size);

/**
 * @brief Wrapper around realloc() with optional failure simulation.
 *
 * @param dest Pointer to the memory block to resize.
 * @param size New size in bytes.
 * @return Pointer to reallocated memory, or nullptr if failure is simulated.
 */
void *tsRealloc(void *dest, size_t size);

/**
 * @brief Wrapper around fopen() with optional failure simulation.
 *
 * @param path File path to open.
 * @param mode Mode string passed to fopen().
 * @return FILE pointer, or nullptr if failure is simulated.
 */
FILE *tsFopen(const char *path, const char *mode);

/**
 * @brief Wrapper around mkdir() with optional failure simulation.
 *
 * @param path Directory path to create.
 * @return 0 on success, -1 on failure (real or simulated).
 */
int tsMkdir(const char *path);

/**
 * @brief Portable wrapper around mkdtemp().
 *
 * Replaces the trailing "XXXXXX" in tmpl with a unique suffix and creates the directory.
 * On POSIX, delegates to mkdtemp(); on Windows, uses _mktemp + _mkdir.
 *
 * @param tmpl Template string ending in "XXXXXX", modified in-place.
 * @return Pointer to tmpl on success, nullptr on failure.
 */
char *tsMkdtemp(char *tmpl);

/**
 * @brief Reset all failure simulation state to its defaults.
 *
 * Call at the start of any test that uses tsDisable() to guarantee a clean slate.
 */
void tsReset(void);

// Variables ———————————————————————————————————————————————————————————————————————————————————————

/** @brief Mock delta-time value (0.016 s ≈ 60 fps) for use in tests. */
extern const float TS_MOCK_DT;