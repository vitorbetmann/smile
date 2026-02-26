# SceneManagerInternal — API 🎬

`SceneManagerInternal` contains internal result codes, data structures, and
helper functions used by the SceneManager module implementation.

For public API see: [SceneManager – API](../SceneManager/SceneManagerAPI.md)


### 🚨 Warning! This module is not thread-safe!

---

## 📋 Table of Contents

- [Module Header](#module-header)
- [Data Types](#-data-types)
    - [Enums](#-enums)
    - [Structs](#-structs)
- [Functions](#-functions)
    - [Lookup Related](#-lookup-related)

## 😊Module Header

The module’s header is `SceneManagerInternal.h`. Its full Smile path is:
`src/SceneManager/SceneManagerInternal.h`

✅ Example

```c
#include "SceneManagerInternal.h"
```

## 📦 Data Types

### — Enums

| `smResult` |
|------------|

SceneManager-specific result codes used when failures are not covered
by [cmResult](../_Common/CommonInternalAPI.md#-enums).

- SceneManager-specific failures cover the following range: `-100..-199`.

| Item                                    | Value  | Summary                                                     |
|-----------------------------------------|--------|-------------------------------------------------------------|
| `SM_RESULT_SCENE_ALREADY_EXISTS`        | `-100` | A scene with the same name already exists.                  |
| `SM_RESULT_SCENE_NOT_FOUND`             | `-101` | Requested scene was not found.                              |
| `SM_RESULT_NO_VALID_FUNCTIONS`          | `-102` | Scene creation received no valid lifecycle callbacks.       |
| `SM_RESULT_CANNOT_DELETE_CURRENT_SCENE` | `-103` | Attempted to delete the currently active scene.             |
| `SM_RESULT_NO_CURRENT_SCENE`            | `-104` | Operation requires an active scene, but none is set.        |
| `SM_RESULT_NO_UPDATE_FUNCTION`          | `-105` | Active scene has no update callback.                        |
| `SM_RESULT_NO_DRAW_FUNCTION`            | `-106` | Active scene has no draw callback.                          |
| `SM_RESULT_FREE_ALL_SCENES_FAILED`      | `-107` | Internal cleanup invariant failed while freeing all scenes. |

<br>

### — Structs

| `smInternalScene` |
|-------------------|

Represents a scene and its lifecycle callbacks.

| Field    | Type         | Summary                                   |
|----------|--------------|-------------------------------------------|
| `name`   | `char *`     | Scene name (owned by SceneManager).       |
| `enter`  | `smEnterFn`  | Optional callback executed when entering. |
| `update` | `smUpdateFn` | Optional callback executed during update. |
| `draw`   | `smDrawFn`   | Optional callback executed during draw.   |
| `exit`   | `smExitFn`   | Optional callback executed when exiting.  |

<br>

| `smInternalSceneMap` |
|----------------------|

Hash map entry that associates a scene name with its internal scene state.

| Field   | Type                | Summary                           |
|---------|---------------------|-----------------------------------|
| `name`  | `char *`            | Hash key / scene name.            |
| `state` | `smInternalScene *` | Pointer to scene state.           |
| `hh`    | `UT_hash_handle`    | uthash handle for map operations. |

<br>

| `smInternalTracker` |
|---------------------|

Tracks current SceneManager runtime state.

| Field        | Type                      | Summary                                              |
|--------------|---------------------------|------------------------------------------------------|
| `sceneMap`   | `smInternalSceneMap *`    | Hash map of registered scenes.                       |
| `currScene`  | `const smInternalScene *` | Current active scene (or `nullptr`).                 |
| `sceneCount` | `int`                     | Number of currently registered scenes.               |
| `fps`        | `int`                     | Target FPS (used by delta-time first-call fallback). |
| `lastTime`   | `struct timespec`         | Last timestamp used by delta-time computation.       |

---

## 🛠️ Functions

### — Lookup Related

| `const smInternalScene *smInternalGetScene(const char *name)` |
|---------------------------------------------------------------|

Retrieves a scene pointer by name.

- Parameters:
    - `name` — Name of the scene to look up.
- Returns:
    - Pointer to matching scene.
    - `nullptr` if not found.

✅ Example

```c
const smInternalScene *scene = smInternalGetScene("menu");
if (!scene)
{
    // Scene not found
}
```

<br>

| `smInternalSceneMap *smInternalGetEntry(const char *name)` |
|------------------------------------------------------------|

Retrieves a scene-map entry pointer by name.

- Parameters:
    - `name` — Name of the scene-map entry to look up.
- Returns:
    - Pointer to matching map entry.
    - `nullptr` if not found.

✅ Example

```c
smInternalSceneMap *entry = smInternalGetEntry("menu");
if (entry)
{
    // Entry found
}
```

---

## ✏️ Last Modified

| Last modified | Author (Discord)             | Description                                            |
|---------------|------------------------------|--------------------------------------------------------|
| Feb 26, 2026  | Vitor Betmann (vitorbetmann) | Created SceneManagerInternal API docs with `smResult`. |
