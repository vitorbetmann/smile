/**
 * @file
 * @brief Developer-mode test hooks for SceneManager.
 *
 * These declarations exist so SceneManager can expose deterministic hook points
 * for tests without depending on headers under `tests/`.
 *
 * @author Vitor Betmann
 */

#ifndef SMILE_SCENE_MANAGER_TEST_HOOKS_H
#define SMILE_SCENE_MANAGER_TEST_HOOKS_H

#include <stdbool.h>
#include <time.h>

/**
 * @brief Stores counters and mock scene data used during test execution.
 */
typedef struct {
    int enterCount;
    int exitCount;
} MockData;

/**
 * @brief Represents a simple mock argument structure passed to scene functions.
 */
typedef struct {
    bool flag;
} MockArgs;

/**
 * @brief Hook type for enter callbacks without explicit args.
 */
typedef void (*smTestEnterFn)(MockData *data);

/**
 * @brief Hook type for enter callbacks with explicit args.
 */
typedef void (*smTestEnterWithArgsFn)(MockData *data, MockArgs *args);

/**
 * @brief Hook type for exit callbacks.
 */
typedef void (*smTestExitFn)(MockData *data);

extern smTestEnterFn smTestEnter;
extern smTestEnterWithArgsFn smTestEnterWithArgs;
extern smTestExitFn smTestExit;
extern MockArgs *smMockArgs;
extern MockData *smMockData;
extern struct timespec smMockCurrTime;


#endif // #ifndef SMILE_SCENE_MANAGER_TEST_HOOKS_H
