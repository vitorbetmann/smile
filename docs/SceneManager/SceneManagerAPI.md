# SceneManager — API 🤖

The `SceneManager` module manages game flow by organizing behavior into
independent scenes. Each scene defines its own enter, update, draw, and exit
callbacks.

For workflow examples see: [SceneManager – Getting Started](README.md)

### 🚨 Warning! This module is not thread-safe!

---

## 📋 Table of Contents

- [Module Header](#-module-header)
- [Data Types](#-data-types)
    - [Function Pointers](#-function-pointers)
- [Functions](#-functions)
    - [Start Related](#-start-related)
    - [Scene Functions](#-scene-functions)
    - [Lifecycle Functions](#-lifecycle-functions)
    - [Stop Related](#-stop-related)

---

## 😊 Module Header

The module’s header is `SceneManager.h`. Its full Smile path is:
`smile/include/SceneManager.h`

✅ Example

```c
#include <SceneManager.h>
```

----

## 📦 Data Types

### — Function Pointers

| `void (*smEnterFn)(void *args)` |
|---------------------------------|

Function pointer type for scene enter callbacks.

- Parameters:
    - `args` — Optional arguments passed when entering the scene.

- Notes:
    - The enter function should handle its own resource management, such as
      allocating memory, loading assets, or performing initialization steps.
    - If any operation fails during setup (for example, memory allocation or
      file loading) the user must handle the failure within the function.

✅ Example

```c
typedef struct {
    float xPosition;
} PlayerData;

PlayerData *myPlayerData;

void mySceneEnter(void *args)
{
    myPlayerData = malloc(sizeof(PlayerData));
    if (!myPlayerData)
    {
        // Handle malloc fail
        return;
    }

    if (args)
    {
        myPlayerData->xPosition = ((PlayerData *)args)->xPosition;
    } else
    {
        myPlayerData->xPosition = 0.0f;
    }
}
```

<br>

| `void (*smUpdateFn)(float dt)` |
|--------------------------------|

Function pointer type for scene update callbacks.

- Parameters:
    - `dt` — Delta time in seconds since the last update.

✅ Example

```c
void mySceneUpdate(float dt)
{
    if (!myPlayerData)
    {
        return;
    }

    myPlayerData->xPosition += 5.0f * dt;
}
```

<br>

| `void (*smDrawFn)(void)` |
|--------------------------|

Function pointer type for scene draw callbacks.

✅ Example

```c
#include <raylib.h> // This example uses raylib for rendering 

void mySceneDraw(void)
{
    BeginDrawing();
    ClearBackground(BLACK);

    if (!myPlayerData)
    {
        EndDrawing();
        return;
    }

    // Player is a red circle moving right.    
    int radius = 10;
    int yPosition = 200;
    DrawCircle(myPlayerData->xPosition, yPosition, radius, RED);
    
    EndDrawing();
}
```

<br>

| `void (*smExitFn)(void)` |
|--------------------------|

Function pointer type for scene exit callbacks.

✅ Example

```c
void mySceneExit(void) {
    free(myPlayerData);
    myPlayerData = nullptr;
}
```

<br>

---

## 🛠️ Functions

### — Start Related

| `int smStart(void)` |
|---------------------|

Initializes SceneManager and prepares it for use.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if SceneManager is already running.
    - Logging: warning when called while already running.

✅ Example

```c
#include <SceneManager.h>

int main(void) {
    smStart();
    ...
}
```

<br>

| `bool smIsRunning(void)` |
|--------------------------|

Checks whether SceneManager has been initialized.

- Returns: True if SceneManager has been started, false otherwise.

✅ Example

```c
while (smIsRunning()) {
    // Game loop
}
```

<br>

### — Scene Functions

| `int smCreateScene(const char *name, smEnterFn enter, smUpdateFn update, smDrawFn draw, smExitFn exit)` |
|---------------------------------------------------------------------------------------------------------|

Creates a new scene with the specified name and callback functions.

- Parameters:
    - `name` — Unique name identifying the scene.
    - `enter` — Callback executed when entering the scene.
    - `update` — Callback executed each frame during update.
    - `draw` — Callback executed each frame during rendering.
    - `exit` — Callback executed when exiting the scene.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if: SceneManager is not running; `name` is null or empty; all
      callbacks are null; or a scene with `name` already exists.
    - Ownership: `name` is copied internally; caller retains ownership of the
      input string.

✅ Example

```c
// ----- In Menu.h -----
#ifndef MENU_H
#define MENU_H

// No enter function in this example
void menuUpdate(float dt);           
void menuDraw(void);                 
void menuExit(void);                 

#endif


// ----- Menu.c -----
#include <SceneManager.h>
#include "Menu.h"

void menuUpdate(float dt)
{
    // Handle update
    
    if (PlayButtonPressed())
    {
        smSetScene("level 1", nullptr);
    }
}

void menuDraw(void)
{
    // Handle rendering
}

void menuExit(void)
{
    // Handle cleanup
}

// ----- In main.c or in another scene. -----
#include <SceneManager.h>

int main(void)
{
    smStart();
    smCreateScene("menu", nullptr, menuUpdate, menuDraw, menuExit);
    ...
}
```

<br>

| `bool smSceneExists(const char *name)` |
|----------------------------------------|

Checks whether a scene with the given name exists.

- Parameters:
    - `name` — Name of the scene to check.

- Returns: True if the scene exists, false otherwise.

- Notes:
    - Returns `false` if: SceneManager is not running or `name` is null or
      empty.

✅ Example

```c
if (!smSceneExists("level 1"))
{
    smCreateScene("level 1", enter, update, draw, exit);
}
```

<br>

| `int smSetScene(const char *name, void *args)` |
|------------------------------------------------|

Sets the current active scene by name and triggers its enter function.

- Parameters:
    - `name` — Name of the scene to switch to.
    - `args` — Optional pointer to arguments passed to the scene's enter
      function.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if: SceneManager is not running; `name` is null or empty; or the
      target scene does not exist.
    - Side effects: if an active scene has an exit callback, it is called
      before switching; then target scene enter callback is called.
    - Ownership: `args` is borrowed for the duration of the enter callback.
    - The `args` pointer may be null if no data is required.

✅ Example

```c
// Switch to "level 1" with no arguments.
smSetScene("level 1", nullptr);

...

// Passing player data to next scene.
typedef struct {
    int currentScore;
    int highScore;
} PlayerScore;

PlayerScore score = { 10, 50 };
smSetScene("level 2", &score);
```

<br>

| `const char *smGetCurrentSceneName(void)` |
|-------------------------------------------|

Retrieves the name of the currently active scene.

- Returns: A pointer to the name of the current scene, or `nullptr` if no scene
  is
  active or SceneManager is not running.

- Notes:
    - The returned string is owned by SceneManager. The user must not attempt to
      modify or free it and should make a copy of the string if they wish to do
      so.
    - The pointer remains valid until the scene is deleted or SceneManager
      is stopped.

✅ Example

```c
const char *currScene = smGetCurrentSceneName();
if (currScene)
{
    ...
}
```

<br>

| `int smDeleteScene(const char *name)` |
|---------------------------------------|

Deletes a scene by name from SceneManager.

- Parameters:
    - `name` — Name of the scene to delete.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if: SceneManager is not running; `name` is null or empty; the scene
      does not exist; or `name` is the currently active scene.

✅ Example

```c
smDeleteScene("level 1");
```

<br>

| `int smGetSceneCount(void)` |
|-----------------------------|

Retrieves the total number of registered scenes.

- Returns: The number of registered scenes on success, or a negative result
  code on failure.

✅ Example

```c
int mySceneCount = smGetSceneCount();
```

<br>

### — Lifecycle Functions

| `int smUpdate(float dt)` |
|--------------------------|

Updates the currently active scene.

- Parameters:
    - `dt` — Delta time in seconds since the last update.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if: SceneManager is not running; no scene is active; or the
      active scene has no update callback.
    - Logging: warning when active scene has no update callback.

✅ Example

```c
while (smIsRunning())
{    
    float dt = smGetDt(); 
    smUpdate(dt);
    ...
}
```

<br>

| `float smGetDt(void)` |
|-----------------------|

Calculates and returns the delta time since the last frame update.

- Returns: The time elapsed in seconds since the previous call to
  `smGetDt()`, or a negative result code cast to `float` on failure.

- Notes:
    - Delta time is measured using a high-resolution monotonic clock. On the
      first call, it returns a duration equivalent to one frame at the
      configured target FPS (currently 60 by default).
    - Fails if: SceneManager is not running or `clock_gettime` fails.

✅ Example

```c
while (smIsRunning())
{    
    float dt = smGetDt(); 
    smUpdate(dt);
    ...
}
```

<br>

| `int smDraw(void)` |
|--------------------|

Executes the draw function of the currently active scene.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if: SceneManager is not running; no scene is active; or the
      active scene has no draw callback.
    - Logging: warning when active scene has no draw callback.

✅ Example

```c
while (smIsRunning())
{    
    smUpdate(smGetDt());
    smDraw();
}
```

<br>

### — Stop Related

| `int smStop(void)` |
|--------------------|

Stops SceneManager and frees all allocated memory associated with created
scenes.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if: SceneManager is not running.
    - May fail with `SM_RESULT_FREE_ALL_SCENES_FAILED` if cleanup invariants
      are violated.
    - The exit function of the current scene is called before cleanup.
    - After stopping, all internal data is reset. SceneManager must be
      restarted with smStart().

✅ Example

```c
#include <SceneManager.h>
#include "menu.h"

void menuUpdate(float dt)
{
    // Handle update
    ...
    
    if (QuitButtonPressed())
    {
        smStop();
    }
}

void menuDraw(void)
{
    // Handle rendering
}
```

---

## ✏️ Last Modified

| Last modified | Author (username) | Description                                                     |
|---------------|-------------------|-----------------------------------------------------------------|
| Feb 26, 2026  | vitorbetmann      | Updated API docs for int result codes and added result mapping. |
