/**
 * @file
 * @brief Declarations of public data types and functions for the
 *        SceneManager module.
 *
 * @see docs/SceneManager/README.md
 *
 * @author Vitor Betmann
 */


#ifndef SMILE_SCENE_MANAGER_H
#define SMILE_SCENE_MANAGER_H


// —————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Function pointer type for scene entry callbacks.
 *
 * @param args Optional arguments passed when entering the scene.
 *
 * @author Vitor Betmann
 */
typedef void (*smEnterFn)(void *args);

/**
 * @brief Function pointer type for scene update callbacks.
 *
 * @param dt Delta time in seconds since the last update.
 *
 * @author Vitor Betmann
 */
typedef void (*smUpdateFn)(float dt);

/**
 * @brief Function pointer type for scene draw callbacks.
 *
 * @author Vitor Betmann
 */
typedef void (*smDrawFn)(void);

/**
 * @brief Function pointer type for scene exit callbacks.
 *
 * @author Vitor Betmann
 */
typedef void (*smExitFn)(void);

// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

// Start Related

/**
 * @brief Initializes SceneManager and prepares it for use.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: SceneManager is already running.
 * @note Logging: warning when called while already running.
 *
 * @see smStop
 * @see smIsRunning
 *
 * @author Vitor Betmann
 */
int smStart(void);

/**
 * @brief Checks whether SceneManager has been initialized.
 *
 * @return Returns true when SceneManager is running, false otherwise.
 *
 * @see smStart
 * @see smStop
 *
 * @author Vitor Betmann
 */
bool smIsRunning(void);

// Scene Functions

/**
 * @brief Creates a new scene with the specified name and callback functions.
 *
 * @param name Unique name identifying the scene.
 * @param enter Callback executed when entering the scene.
 * @param update Callback executed each frame during update.
 * @param draw Callback executed each frame during rendering.
 * @param exit Callback executed when exiting the scene.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: SceneManager is not running; `name` is null or empty;
 *       all callbacks are null; or a scene with `name` already exists.
 * @note Ownership: `name` is copied internally; caller retains ownership of
 *       the input string.
 *
 * @see smSceneExists
 * @see smDeleteScene
 * @see smSetScene
 *
 * @author Vitor Betmann
 */
int smCreateScene(const char *name, smEnterFn enter, smUpdateFn update,
                  smDrawFn draw, smExitFn exit);

/**
 * @brief Checks whether a scene with the given name exists.
 *
 * @param name Name of the scene to check.
 *
 * @return Returns true when the scene exists, false otherwise.
 *
 * @note Returns false if: SceneManager is not running or `name` is null or
 *       empty.
 *
 * @see smCreateScene
 * @see smDeleteScene
 *
 * @author Vitor Betmann
 */
bool smSceneExists(const char *name);

/**
 * @brief Sets the current active scene by name and triggers its enter function.
 *
 * @param name Name of the scene to switch to.
 * @param args Optional arguments passed to the scene's enter function.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: SceneManager is not running; `name` is null or empty; or
 *       the target scene does not exist.
 * @note Side effects: if an active scene has an exit callback, it is called
 *       before switching; then target scene enter callback is called.
 * @note Ownership: `args` is borrowed for the duration of the enter callback.
 *
 * @see smGetCurrentSceneName
 * @see smCreateScene
 *
 * @author Vitor Betmann
 */
int smSetScene(const char *name, void *args);

/**
 * @brief Retrieves the name of the currently active scene.
 *
 * @return Returns a pointer to the current scene name, or `nullptr` if no
 *         scene is active or SceneManager is not running.
 *
 * @note Ownership: the returned string is owned by SceneManager. The user must not
 *       attempt to modify or free it and should make a copy before doing so.
 *       The pointer remains valid until the scene is deleted or SceneManager
 *       is stopped.
 *
 * @see smSetScene
 * @see smDeleteScene
 * @see smStop
 *
 * @author Vitor Betmann
 */
const char *smGetCurrentSceneName(void);

/**
 * @brief Deletes a scene by name from SceneManager.
 *
 * @param name Name of the scene to delete.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: SceneManager is not running; `name` is null or empty; the
 *       scene does not exist; or `name` is the currently active scene.
 *
 * @see smCreateScene
 * @see smSceneExists
 * @see smGetCurrentSceneName
 *
 * @author Vitor Betmann
 */
int smDeleteScene(const char *name);

/**
 * @brief Retrieves the total number of registered scenes.
 *
 * @return Returns the number of registered scenes on success, or a negative
 *         error code on failure.
 *
 * @see smCreateScene
 * @see smDeleteScene
 *
 * @author Vitor Betmann
 */
int smGetSceneCount(void);

// Lifecycle Related

/**
 * @brief Updates the currently active scene.
 *
 * @param dt Delta time in seconds since the last update.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: SceneManager is not running; no scene is active; or the
 *       active scene has no update callback.
 * @note Logging: warning when active scene has no update callback.
 *
 * @see smGetDt
 * @see smDraw
 *
 * @author Vitor Betmann
 */
int smUpdate(float dt);

/**
 * @brief Calculates the delta time, in seconds, since last invoked.
 *
 * @return Returns the elapsed time in seconds since the previous call to
 *         `smGetDt()`, or a negative error code (as `float`) on failure.
 *
 * @note Delta time is measured using a high-resolution monotonic clock. On the
 *       first call, it returns a duration equivalent to one frame at the
 *       configured target FPS (currently 60 by default).
 * @note Fails if: SceneManager is not running or time acquisition fails.
 *
 * @see smUpdate
 *
 * @author Vitor Betmann
 */
float smGetDt(void);

/**
 * @brief Executes the draw function of the currently active scene.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: SceneManager is not running; no scene is active; or the
 *       active scene has no draw callback.
 * @note Logging: warning when active scene has no draw callback.
 *
 * @see smUpdate
 *
 * @author Vitor Betmann
 */
int smDraw(void);

// Stop Related

/**
 * @brief Stops SceneManager and frees all allocated scenes.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: SceneManager is not running, or internal cleanup invariants
 *       fail.
 * @note Side effects: current scene exit callback is called before cleanup.
 *       All internal data is reset after stop; restart with `smStart()`.
 *
 * @see smStart
 * @see smIsRunning
 *
 * @author Vitor Betmann
 */
int smStop(void);


#endif
