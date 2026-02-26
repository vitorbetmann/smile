# SceneManager — Getting Started 🤖

The `SceneManager` module provides a simple, flexible system for defining and
controlling game flow through independent scenes. Each scene defines its own
behavior using enter, update, draw, and exit callback functions.

Module contract:

- Call `smStart()` before all SceneManager APIs except `smIsRunning()`.
- Scene names passed as `name` must be non-null and non-empty.

For detailed documentation see: [SceneManager – API](SceneManagerAPI.md)

### 🚨 Warning! This module is not thread-safe!
---

## 📋 Table of Contents

- [Visual Example](#-visual-example)
- [Module Header](#-module-header)
- [SceneManager Lifecycle](#-scenemanager-lifecycle)
- [Quick Reference Table](#-quick-reference-table)
- [Workflow Example](#-workflow-example)

---

## 👀 Visual Example

<p align="center">
  <img src="../../docs/_Internal/__Assets/SceneManager/SceneManagerDemo.gif" width="90%" alt="Scene Manager example"/>
</p>

---

## 😊 Module Header

The module’s header is `SceneManager.h`. Its full Smile path is:
`smile/include/SceneManager.h`

✅ Example

```c
#include <SceneManager.h>
```

---

## 🔄 SceneManager Lifecycle

1️⃣ Begin with `smStart()`. This initializes the internal system, allowing you
to register, switch, and manage scenes. No other function except `smIsRunning()`
will work until this step is complete.

2️⃣ Define each scene in its own header/source files to represent a
self-contained scene (e.g., a main menu, level, or pause screen). Each scene
must define at least one, and up to four, lifecycle callbacks (enter, update,
draw, and/or exit).

3️⃣ Use `smCreateScene()` to register uniquely named scenes with their callbacks
into memory. You can create as many scenes as you like.

4️⃣ Use `smSetScene()` to transition into a new scene, optionally passing data
to it.

5️⃣ `smUpdate()` and `smDraw()` should be called every frame (typically inside
your game loop) to run the current scene’s logic and rendering.

6️⃣ Always call `smStop()` when SceneManager is no longer needed. This ensures
all registered scenes are properly cleaned up, preventing memory leaks and
dangling pointers.

---

## 🔍 Quick Reference Table

### Data Types

— Function Pointers

| Signature                       | Description                                                                               |
|---------------------------------|-------------------------------------------------------------------------------------------|
| `void (*smEnterFn)(void *args)` | Runs once when entering a scene, often to load assets or initialize data.                 |     
| `void (*smUpdateFn)(float dt)`  | Runs every frame to update game logic, using `dt` as the delta time since the last frame. |
| `void (*smDrawFn)(void)`        | Runs every frame to render visuals.                                                       |
| `void (*smExitFn)(void)`        | Runs once when exiting a scene, often used for freeing memory and unloading resources.    |

<br>

### Functions

| Signature                                                                                                | Description                                                                                                     |
|----------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------|
| `bool smStart(void)`                                                                                     | Initializes SceneManager and prepares it for use.                                                               |
| `bool smIsRunning(void)`                                                                                 | Checks whether SceneManager has been initialized.                                                               |
| `bool smCreateScene(const char *name, smEnterFn enter, smUpdateFn update, smDrawFn draw, smExitFn exit)` | Registers a new named scene with its lifecycle callbacks.                                                       |
| `bool smSceneExists(const char *name)`                                                                   | Checks if a scene with the given name exists.                                                                   |
| `bool smSetScene(const char *name, void *args)`                                                          | Calls the current scene's `exit` function, then sets a new active scene by name and calls its `enter` function. |
| `const char *smGetCurrentSceneName(void)`                                                                | Returns the name of the current active scene.                                                                   |
| `bool smDeleteScene(const char *name)`                                                                   | Deletes a non-active a scene by name.                                                                           |
| `int smGetSceneCount(void)`                                                                              | Returns the total number of registered scenes.                                                                  |
| `bool smUpdate(float dt)`                                                                                | Calls the update function of the active scene.                                                                  |
| `float smGetDt(void)`                                                                                    | Returns the delta time (in seconds) since the last frame.                                                       |
| `bool smDraw(void)`                                                                                      | Calls the draw function of the active scene.                                                                    |
| `bool smStop(void)`                                                                                      | Calls the current scene's `exit` function, then stops SceneManager and frees all registered scenes.             |

---

## 🧪️ Workflow Example

In your main file, you can start like this:

```c
#include <SceneManager.h>
#include "Menu.h"
#include "LevelOne.h"
#include "LevelTwo.h"

int main(void) {
    // Other SceneManager functions will not work if smStart is not called.
    smStart();
    
    // Create your scenes. Callback functions declared in respective header files.
    smCreateScene("menu", nullptr, menuUpdate, menuDraw, menuExit);
    smCreateScene("level 1", levelOneEnter, levelOneUpdate, levelOneDraw, levelOneExit);
    smCreateScene("level 2", levelTwoEnter, levelTwoUpdate, levelTwoDraw, nullptr);
    
    /* We start in the main menu, which, in this example, requires no arguments,
     * so we pass in nullptr.
     */
    smSetScene("menu", nullptr); 
    
    // Below is a typical game loop using Smile
    while (smIsRunning()) {
        float dt = smGetDt();
        smUpdate(dt);
        smDraw();
    }
}
```

Make sure you call `smStop()` before exiting your program, otherwise you risk
memory leaks!

Calling `smStop()` also calls the current scene's exit function and sets
`smIsRunning()` to false, breaking the main game loop. Good places to call
`smStop()` include your main menu or pause screen:

An example of a `Menu.c` file can be seen below:

```c
#include <SceneManager.h>
#include "Menu.h"

// menu has no Enter function in this example

void menuUpdate(float dt) {
    // Handle update
    ...
    
    if (PlayButtonPressed()) {
        /* level 1's enter function takes no arguments in this example, so we
         * pass in nullptr as args to smSetScene()
         */
        smSetScene("level 1", nullptr);
    }
    
    if (QuitButtonPressed()) {
        smStop();
    }
}

void menuDraw(void) {
    // Handle rendering
}

void menuExit(void) {
    // Handle cleanup
}
```

You can pass custom data between scenes through the `args` parameter of
`smSetScene()`. Typically, this involves defining a custom struct to hold any
data you want to share, then passing a pointer to it as `args`.

`smSetScene()` will first call the exit function of the current scene,
followed by the enter function of the next scene.

Below is a typical scene header file using `LevelTwo.h` as an example:

```c
#ifndef LEVEL_TWO_H
#define LEVEL_TWO_H

/* Below is the data that will be passed to smSetScene if its enter function
 * requires any arguments
 */

typedef struct {
    int score;
    float position;
} PlayerData;

void levelTwoEnter(void *args);         // Called when entering level 2
void levelTwoUpdate(float dt);          // Called each frame to update level 2
void levelTwoDraw(void);                // Called each frame to draw level 2
                                        // level 2 has no Exit function in this example

#endif
```

You can now use the `PlayerData` struct to pass the necessary args into
`smSetScene()`. In `LevelOne.c` we do:

```c
#include <SceneManager.h>
#include "LevelOne.h"
#include "LevelTwo.h" // Must include to have access to PlayerData

static int currentScore = 0;
static float currentPosition = 0;

void levelOneEnter(void *args) {
    // level 1 takes no args in this example, so we ignore args
    // Handle initialization
}

void levelOneUpdate(float dt) {
    // Handle update

    // Passing player data to next scene    
    if (playerWon) {
        PlayerData playerData = { 
            .score = currentScore,
            .position = currentPosition
        };
        smSetScene("level 2", &playerData);
    }
}

void levelOneDraw(void) {
    // Handle rendering
}

void levelOneExit(void) {
    // Handle cleanup
}
```

In the next scene's `enter` function, cast the `void *` argument back to
your custom struct type to access the data. This enables flexible communication
and scene initialization based on dynamic data while avoiding reliance on global
variables.

So, in `LevelTwo.c`:

```c
#include <SceneManager.h>
#include "LevelTwo.h"

static PlayerData myPlayerData;

// in this example, level 2 is always entered with non-null args
void levelTwoEnter(void *args) {     
    PlayerData *myArgs = args;   
    myPlayerData.score = myArgs->score;
    myPlayerData.position = myArgs->position;
}

void levelTwoUpdate(float dt) {
    // Update whatever
    
    if (gameOver) {
        // Stop SceneManager when the game ends
        smStop();
    }
                                  
}

void levelTwoDraw(void) {
    // Handle rendering
}

// level 2 has no Exit function in this example
```

---

## ✏️ Last Modified

| Last modified | Author (username) | Description                                                                |
|---------------|-------------------|----------------------------------------------------------------------------|
| Feb 25, 2026  | vitorbetmann      | Clarified callback/args example consistency and updated workflow snippets. |
