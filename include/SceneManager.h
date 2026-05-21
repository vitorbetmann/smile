#pragma once

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/** @brief Callback executed once when entering a scene; receives optional caller-supplied data. */
typedef void (*smEnterFn)(void *args);

/** @brief Callback executed every frame during the update step; receives delta time in seconds. */
typedef void (*smUpdateFn)(float dt);

/** @brief Callback executed every frame during the draw step. */
typedef void (*smDrawFn)(void);

/** @brief Callback executed once when exiting a scene. */
typedef void (*smExitFn)(void);

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

// Lifecycle

/**
 * @brief Initializes SceneManager and prepares it for use.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int smStart(void);

/**
 * @brief Checks whether SceneManager has been initialized.
 *
 * @return true if running, false otherwise.
 */
bool smIsRunning(void);

/**
 * @brief Updates the currently active scene.
 *
 * @param dt Delta time in seconds since the last update.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int smUpdate(float dt);

/**
 * @brief Executes the draw function of the currently active scene.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int smDraw(void);

/**
 * @brief Stops SceneManager and frees all registered scenes.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int smStop(void);

// Registry

/**
 * @brief Registers a new named scene with its lifecycle callbacks.
 *
 * @param name   Unique name identifying the scene; copied internally.
 * @param enter  Callback executed when entering the scene; may be nullptr.
 * @param update Callback executed each frame during update; may be nullptr.
 * @param draw   Callback executed each frame during rendering; may be nullptr.
 * @param exit   Callback executed when exiting the scene; may be nullptr.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int smAddScene(const char *name, smEnterFn enter, smUpdateFn update, smDrawFn draw, smExitFn exit);

/**
 * @brief Checks whether a scene with the given name exists.
 *
 * @param name Name of the scene to check.
 *
 * @return true if the scene exists, false otherwise.
 */
bool smSceneExists(const char *name);

/**
 * @brief Removes a non-active scene by name.
 *
 * @param name Name of the scene to remove.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int smRemoveScene(const char *name);

// Getters

/**
 * @brief Calculates and returns the delta time since the last frame.
 *
 * @return Elapsed time in seconds since the previous call, or a negative result code cast to float
 *         on failure.
 */
float smGetDt(void);

/**
 * @brief Returns the total number of registered scenes.
 *
 * @return The number of registered scenes on success, or a negative result code on failure.
 */
int smGetSceneCount(void);

/**
 * @brief Returns the name of the currently active scene.
 *
 * @return Pointer to the current scene name (owned by SceneManager), or nullptr if no scene is
 *         active or SceneManager is not running.
 */
const char *smGetCurrentSceneName(void);

// Setters

/**
 * @brief Sets the active scene by name, calling the previous scene's exit and the new scene's enter.
 *
 * @param name Name of the scene to switch to.
 * @param args Optional pointer passed to the scene's enter callback.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int smSetScene(const char *name, void *args);