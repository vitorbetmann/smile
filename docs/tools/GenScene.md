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

| Flag                     | Description                                                                                         |
|--------------------------|-----------------------------------------------------------------------------------------------------|
| `-h, --help`             | Prints usage information and exits. Only works as first flag.                                       |
| `-as, --add-sections`    | Adds Smile-style section headers to the generated files for code organization.                      |
| `-ne, --no-enter`        | Omits the enter callback from the generated files.                                                  |
| `-nu, --no-update`       | Omits the update callback from the generated files.                                                 |
| `-nd, --no-draw`         | Omits the draw callback from the generated files.                                                   |
| `-nx, --no-exit`         | Omits the exit callback from the generated files.                                                   |
| `-si, --source-in <dir>` | Outputs the .c file to `<dir>` (default: src/). Prompts to create `<dir>` if it does not exist.     |
| `-hi, --header-in <dir>` | Outputs the .h file to `<dir>` (default: include/). Prompts to create `<dir>` if it does not exist. |

- Note: The scene must have at least 1 callback
- Note: `<SceneName>` must start with a letter or underscore, contain only letters, digits, underscores, or spaces (
  spaces capitalize the next character and trailing/leading whitespace is ignored), and must not exceed 64 characters.
  To pass a name with spaces, wrap it in quotes: `GenScene "Level 1"` → `Level1.c` / `Level1.h`
- Note: `<dir>` is resolved relative to the current working directory, may not contain '..' segments
- Note: Neither path (i.e., `<dir>/<SceneName>.c` or `<dir>/<SceneName>.h`) must exceed 256 characters

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

Generate a scene with spaces in the name, without enter and exit callbacks, outputting to custom
directories:

```
GenScene "Main Menu" --no-enter --no-exit --source-in src/scenes --header-in include/scenes
```

Result — `include/scenes/MainMenu.h`:

```c
#ifndef MAINMENU_H
#define MAINMENU_H


void MainMenuUpdate(float dt);

void MainMenuDraw(void);


#endif
```

Result — `src/scenes/MainMenu.c`:

```c
#include <SceneManager.h>

#include "MainMenu.h"

void MainMenuUpdate(float dt)
{
    // TODO
}

void MainMenuDraw(void)
{
    // TODO
}
```

<br>

Generate a scene with Smile-style section headers and no draw callback:

```
GenScene HUD --add-sections --no-draw
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