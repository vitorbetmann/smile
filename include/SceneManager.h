/**
 * @file
 * @brief Public declarations of data types and functions for scenes
 *        module.
 *
 * Defines the public API for creating, switching, updating, drawing, and
 * deleting game scenes. Each scene encapsulates its own behavior using callback
 * functions for entry, update, drawing, and exit events.
 *
 * @see SceneManager.c
 * @see SceneManagerInternal.h
 * @see SceneManagerMessages.h
 *
 * @author Vitor Betmann
 * @date 2026-02-17
 * @version 1.1.0
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
 * @brief Initializes scenes and prepares it for use.
 *
 * @return True if scenes started successfully, false otherwise.
 *
 * @note Calling this function when scenes is already started
 *       will log a warning and return false.
 *
 * @author Vitor Betmann
 */
bool smStart(void);

/**
 * @brief Checks whether scenes has been initialized.
 *
 * @return True if scenes has been started, false otherwise.
 *
 * @author Vitor Betmann
 */
bool smIsRunning(void);

// scene Functions

/**
 * @brief Creates a new scene with the specified name and callback functions.
 *
 * @param name Unique name identifying the scene.
 * @param enter Callback executed when entering the scene.
 * @param update Callback executed each frame during update.
 * @param draw Callback executed each frame during rendering.
 * @param exit Callback executed when exiting the scene.
 *
 * @return True if the scene was created successfully, false otherwise.
 *
 * @note All function pointers are optional, but at least one must be provided.
 *       Attempting to create a scene that already exists will fail.
 *
 * @author Vitor Betmann
 */
bool smCreateScene(const char *name, smEnterFn enter, smUpdateFn update,
                   smDrawFn draw, smExitFn exit);

/**
 * @brief Checks whether a scene with the given name exists.
 *
 * @param name Name of the scene to check.
 *
 * @return True if the scene exists, false otherwise.
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
 * @return True if the scene was successfully changed, false otherwise.
 *
 * @note If a scene is active, its exit function is called before switching.
 *
 * @author Vitor Betmann
 */
bool smSetScene(const char *name, void *args);

/**
 * @brief Retrieves the name of the currently active scene.
 *
 * @return A pointer to the name of the current scene, or nullptr if none is active.
 *
 * @note The returned string is owned by scenes. The user must not
 *       attempt to modify or free it and should make a copy before doing so.
 *       The pointer remains valid until the scene is deleted or the scene
 *       machine is stopped.
 *
 * @author Vitor Betmann
 */
const char *smGetCurrentSceneName(void);

/**
 * @brief Deletes a scene by name from scenes.
 *
 * @param name Name of the scene to delete.
 *
 * @return True if the scene was successfully deleted, false otherwise.
 *
 * @note Attempting to delete the currently active scene will fail.
 *
 * @author Vitor Betmann
 */
bool smDeleteScene(const char *name);

/**
 * @brief Retrieves the total number of registered scenes.
 *
 * @return The number of registered scenes, or -1 if scenes is not started.
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
 * @return True if the update function was called successfully, false otherwise.
 *
 * @note If the current scene has no update function, a warning is logged.
 *
 * @author Vitor Betmann
 */
bool smUpdate(float dt);

/**
 * @brief Calculates the delta time, in seconds, since last invoked.
 *
 * @return The time elapsed in seconds since the previous call to `smGetDt()`,
 *         or -1.0f if scenes has not been started.
 *
 * @note Delta time is measured using a high-resolution monotonic clock. On the
 *       first call, it returns a duration equivalent to one frame at 60 FPS.
 *
 * @author Vitor Betmann
 */
float smGetDt(void);

/**
 * @brief Executes the draw function of the currently active scene.
 *
 * @return True if the draw function was called successfully, false otherwise.
 *
 * @note If the current scene has no draw function, a warning is logged.
 *
 * @author Vitor Betmann
 */
bool smDraw(void);

// Stop Related

/**
 * @brief Stops scenes and frees all allocated scenes.
 *
 * @return True if scenes was successfully stopped, false otherwise.
 *
 * @note The exit function of the current scene is called before cleanup.
 *       After stopping, all internal data is reset. scenes must be
 *       restarted with smStart().
 *
 * @author Vitor Betmann
 */
bool smStop(void);


#endif // #ifndef SMILE_SCENE_MANAGER_H
