# SceneManager — API 🤖

The `SceneManager` module provides a simple and flexible system for defining
and controlling game flow through independent scenes. Each scene can specify its
own behavior using enter, update, draw, and exit callback functions.

### 🚨 Warning! This module is not thread-safe!

## 📋 Table of Contents

- [Module Header](#module-header)
- [Data Types](#-data-types)
    - [Function Pointers](#-function-pointers)
- [Functions](#-functions)
    - [Start Related](#-start-related)
    - [scene Functions](#-scene-functions)
    - [Lifecycle Functions](#-lifecycle-functions)
    - [Stop Related](#-stop-related)

## 😊Module Header

The module’s header is `SceneManager.h`. Its full Smile path is:
`smile/include/SceneManager.h`

✅ Example

```c
#include <SceneManager.h>
```

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
      -If any operation fails during setup (for example, memory allocation or
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

    myPlayerData->xPosition = ((PlayerData *)args)->xPosition;
}
```

| `void (*smUpdateFn)(float dt)` |
|--------------------------------|

Function pointer type for scene update callbacks.

- Parameters:
    - `dt` — Delta time in seconds since the last update.

✅ Example

```c
void mySceneUpdate(float dt)
{
    myPlayerData->xPosition += 5.0f * dt;
}
```

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

    // Player is a red circle moving right.    
    int radius = 10;
    int yPosition = 200;
    DrawCircle(myPlayerData->xPosition, yPosition, radius, RED);
    
    EndDrawing();
}
```

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

## 🛠️ Functions

### — Start Related

| `bool smStart(void)` |
|----------------------|

Initializes SceneManager and prepares it for use.

- Returns: True if SceneManager started successfully, false otherwise.

- Notes:
    - Calling this function when SceneManager is already started will log a
      warning and return false.

✅ Example

```c
#include <SceneManager.h>

int main(void) {
    smStart();
    ...
}
```

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

---

### — scene Functions

| `bool smCreateScene(const char *name, smEnterFn enter, smUpdateFn update, smDrawFn draw, smExitFn exit)` |
|----------------------------------------------------------------------------------------------------------|

Creates a new scene with the specified name and callback functions.

- Parameters:
    - `name` — Unique name identifying the scene.
    - `enter` — Callback executed when entering the scene.
    - `update` — Callback executed each frame during update.
    - `draw` — Callback executed each frame during rendering.
    - `exit` — Callback executed when exiting the scene.

- Returns: True if the scene was created successfully, false otherwise.

- Notes:
    - All function pointers are optional, but at least one must be provided.
    - Attempting to create a scene that already exists will fail.

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

| `bool smSceneExists(const char *name)` |
|----------------------------------------|

Checks whether a scene with the given name exists.

- Parameters:
    - `name` — Name of the scene to check.

- Returns: True if the scene exists, false otherwise.

✅ Example

```c
if (!smSceneExists("level 1"))
{
    smCreateScene("level 1", enter, update, draw, exit);
}
```

| `bool smSetScene(const char *name, void *args)` |
|-------------------------------------------------|

Sets the current active scene by name and triggers its enter function.

- Parameters:
    - `name` — Name of the scene to switch to.
    - `args` — Optional pointer to arguments passed to the scene's enter
      function.

- Returns: True if the scene was successfully changed, false otherwise.

- Notes:
    - If a scene is already active, its exit function is called before
      switching.
    - The `args` pointer may be `NULL` or `nullptr` if no data is required.

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

| `const char *smGetCurrentSceneName(void)` |
|-------------------------------------------|

Retrieves the name of the currently active scene.

- Returns: A pointer to the name of the current scene, or nullptr if none is
  active.

- Notes:
    - The returned string is owned by SceneManager. The user must not attempt to
      modify or free it and should make a copy of the string if they wish to do
      so.
    - The pointer remains valid until the scene is deleted or SceneManager
      is stopped.

✅ Example

```c
const char *currScene = smGetCurrentSceneName();
```

| `bool smDeleteScene(const char *name)` |
|----------------------------------------|

Deletes a scene by name from SceneManager.

- Parameters:
    - `name` — Name of the scene to delete.

- Returns: True if the scene was successfully deleted, false otherwise.

- Notes:
    - Attempting to delete the currently active scene will fail.

✅ Example

```c
smDeleteScene("level 1");
```

| `int smGetSceneCount(void)` |
|-----------------------------|

Retrieves the total number of registered scenes.

- Returns: The number of registered scenes, or `-1` if SceneManager is
  not started.

✅ Example

```c
int mySceneCount = smGetSceneCount();
```

<br>

For more, see [Workflow Example](README.md#-workflow-example) in
the [SceneManager Getting Started](README.md) doc.

---

### — Lifecycle Functions

| `bool smUpdate(float dt)` |
|---------------------------|

Updates the currently active scene.

- Parameters:
    - `dt` — Delta time in seconds since the last update.

- Returns: True if the update function was called successfully, false
  otherwise.

- Notes:
    - If the current scene has no update function, a warning is logged.

✅ Example

```c
while (smIsRunning())
{    
    float dt = smGetDt(); 
    smUpdate(dt);
    ...
}
```

| `float smGetDt(void)` |
|-----------------------|

Calculates and returns the delta time since the last frame update.

- Returns: The time elapsed in seconds since the previous call to
  `smGetDt()`, or `-1.0f` if SceneManager has not been started.

- Notes:
    - Delta time is measured using a high-resolution monotonic clock. On the
      first call, it returns a duration equivalent to one frame at 60 FPS.
      After that, the FPS rate is treated as uncapped.

✅ Example

```c
while (smIsRunning())
{    
    float dt = smGetDt(); 
    smUpdate(dt);
    ...
}
```

| `bool smDraw(void)` |
|---------------------|

Executes the draw function of the currently active scene.

- Returns: True if the draw function was called successfully, false
  otherwise.

- Notes:
    - If the current scene has no draw function, a warning is logged.

✅ Example

```c
while (smIsRunning())
{    
    smUpdate(smGetDt());
    smDraw();
}
```

---

### — Stop Related

| `bool smStop(void)` |
|---------------------|

Stops SceneManager and frees all allocated memory associated with created
scenes.

- Returns: True if SceneManager was successfully stopped, false
  otherwise.

- Notes:
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

## 🔄 Workflow Example

For more, see [Workflow Example](README.md#-workflow-example) in
the [SceneManager Getting Started](README.md) doc.

## ✏️ Last Modified

| Last modified | Author (username) | Description                           |
|---------------|-------------------|---------------------------------------|
| Feb 17, 2026  | vitorbetmann      | Renamed SceneMachine to SceneManager; |