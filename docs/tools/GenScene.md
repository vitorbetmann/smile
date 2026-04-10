# GenScene — CLI Tool 🏗️

`GenScene` is a code-generation tool that scaffolds boilerplate scene source and
header files for use with Smile's SceneManager module.

---

## 📋 Table of Contents

- [Synopsis](#-usage)
- [Options](#-options)
- [Examples](#-examples)

---

## 🧑‍💻 Usage

```
GenScene <SceneName> [options]
```

- `SceneName` — Name of the scene to generate. Used as the base name for the
  output files (e.g. `Menu` produces `Menu.c` and `Menu.h`).

---

## ⚙️ Options

| Flag                     | Description                                                                                                                                                    |
|--------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `-ne, --no-enter`        | Omits the enter callback from the generated files.                                                                                                             |
| `-nu, --no-update`       | Omits the update callback from the generated files.                                                                                                            |
| `-nd, --no-draw`         | Omits the draw callback from the generated files.                                                                                                              |
| `-nx, --no-exit`         | Omits the exit callback from the generated files.                                                                                                              |
| `-si, --source-in <dir>` | Outputs the `.c` file to `dir`. Defaults to `src/`. Path must be relative, may not contain `..` segments, and must not exceed 256 characters. Prompts to create `dir` if it does not exist.     |
| `-hi, --header-in <dir>` | Outputs the `.h` file to `dir`. Defaults to `include/`. Path must be relative, may not contain `..` segments, and must not exceed 256 characters. Prompts to create `dir` if it does not exist. |
| `-s, --sections`         | Adds Smile-style section headers to the generated files for code organization.                                                                                 |
| `-h, --help`             | Prints usage information and exits.                                                                                                                            |

---

## 💡 Examples

Generate a scene with all callbacks and default output paths:

```
GenScene Menu
```

Result — `include/Menu.h`:

```c
#ifndef MENU_H
#define MENU_H


void MenuEnter(void *args);

void MenuUpdate(float dt);

void MenuDraw(void);

void MenuExit(void);


#endif
```

Result — `src/Menu.c`:

```c
#include <SceneManager.h>
#include "Menu.h"

void MenuEnter(void *args)
{
    // TODO
}

void MenuUpdate(float dt)
{
    // TODO
}

void MenuDraw(void)
{
    // TODO
}

void MenuExit(void)
{
    // TODO
}
```

<br>

Generate a scene without enter and exit callbacks, outputting to custom
directories:

```
GenScene Level1 --no-enter --no-exit --source-in src/scenes --header-in include/scenes
```

Result — `include/scenes/Level1.h`:

```c
#ifndef LEVEL1_H
#define LEVEL1_H


void Level1Update(float dt);

void Level1Draw(void);


#endif
```

Result — `src/scenes/Level1.c`:

```c
#include <SceneManager.h>
#include "Level1.h"

void Level1Update(float dt)
{
    // TODO
}

void Level1Draw(void)
{
    // TODO
}
```

<br>

Generate a scene with Smile-style section headers and no draw callback:

```
GenScene HUD --sections --no-draw
```

Result — `include/HUD.h`:

```c
#ifndef HUD_H
#define HUD_H


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void HUDEnter(void *args);

void HUDUpdate(float dt);

void HUDExit(void);


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————


#endif
```

Result — `src/HUD.c`:

```c
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <SceneManager.h>
#include "HUD.h"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void HUDEnter(void *args)
{
    // TODO
}

void HUDUpdate(float dt)
{
    // TODO
}

void HUDExit(void)
{
    // TODO
}
```