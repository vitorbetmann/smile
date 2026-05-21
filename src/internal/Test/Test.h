#pragma once

// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Identifies system functions for failure simulation. Used with tsDisable().
 */
typedef enum
{
    MALLOC,  /**< malloc()  */
    CALLOC,  /**< calloc()  */
    REALLOC, /**< realloc() */
    FOPEN,   /**< fopen()   */
    MKDIR,   /**< mkdir()   */
} tsSysFn;

// Functions ———————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Logs a [PASS] message for a passing test.
 *
 * @param fnName Name of the test function.
 */
void tsPass(const char *fnName);

/**
 * @brief Forces a system function to fail at the nth call; auto-resets after firing.
 *
 * @param fnName Function to intercept.
 * @param at     Call count at which failure fires; must be ≥ 1.
 * @return true on success, false if fnName is invalid or at is 0.
 */
bool tsDisable(tsSysFn fnName, unsigned int at);

/**
 * @brief Wrapper around malloc() with optional failure simulation.
 *
 * @param size Bytes to allocate.
 * @return Allocated pointer, or nullptr if failure is simulated.
 */
void *tsMalloc(size_t size);

/**
 * @brief Wrapper around calloc() with optional failure simulation.
 *
 * @param numItems Element count.
 * @param size     Element size in bytes.
 * @return Zero-initialized pointer, or nullptr if failure is simulated.
 */
void *tsCalloc(size_t numItems, size_t size);

/**
 * @brief Wrapper around realloc() with optional failure simulation.
 *
 * @param dest Block to resize.
 * @param size New size in bytes.
 * @return Resized pointer, or nullptr if failure is simulated.
 */
void *tsRealloc(void *dest, size_t size);

/**
 * @brief Wrapper around fopen() with optional failure simulation.
 *
 * @param path File path.
 * @param mode fopen mode string.
 * @return FILE pointer, or nullptr if failure is simulated.
 */
FILE *tsFopen(const char *path, const char *mode);

/**
 * @brief Wrapper around mkdir() with optional failure simulation.
 *
 * @param path Directory path.
 * @return 0 on success, -1 on failure (real or simulated).
 */
int tsMkdir(const char *path);

/**
 * @brief Portable mkdtemp(); replaces trailing "XXXXXX" in tmpl and creates the directory.
 *        Not intercepted by tsDisable(MKDIR).
 *
 * @param tmpl Template string ending in "XXXXXX", modified in-place.
 * @return Pointer to tmpl on success, nullptr on failure.
 */
char *tsMkdtemp(char *tmpl);

/**
 * @brief Resets all failure simulation state; call at the start of any test that uses tsDisable().
 */
void tsReset(void);

// Variables ———————————————————————————————————————————————————————————————————————————————————————

/** @brief Mock delta-time for tests (0.016 s ≈ 60 fps). */
static constexpr float TS_MOCK_DT = 0.016f;
