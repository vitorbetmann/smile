# Welcome to Smile 😊

## An open-source, modular 2D game engine in C

Smile stands for `Simple Modularity Is Lowkey Elegant`, and it's a
lightweight collection of C modules that simplifies 2D game development.

Ideal for `game jams` and `rapid prototyping`, Smile provides clean
abstractions that keep your code lean, organized, and fully under your control.

---

<br>

## 🎮 Smile Demo

![Smile Demo](docs/_Internal/__Assets/_SmileREADME/SmileDemo.gif)

---

<br>

## Why Smile?

Supported on Windows, Mac, and Linux, Smile is a modular collection of C
libraries that streamline common 2D game-development tasks such as `managing
states`, `simulating particles`, and `saving or loading data`. It’s
built for developers who want simplicity without sacrificing control.

You can mix and match only the modules you need (for example, using StateMachine
without ParticleSystem) to keep your project lightweight and focused. And while
Smile doesn’t handle rendering, input, or audio directly, it integrates
seamlessly with libraries like [raylib](https://www.raylib.com).

Smile's community is also active and willing to help! Have questions about
low-level programming, Game Development, Programming Patterns, or anything else?
We can help you out at the [Discord Server](https://discord.gg/EdS6PwMffH).

Learning together! That’s what Smile is all about.

---

<br>

## 🚀 Building Your Game

### Prerequisites

Before building Smile, make sure you have the following installed:

- `CMake` 3.25 or higher
- A build tool such as `Make` or `Ninja`
- A C compiler with C23 support such as `Clang` or `GCC`

### Cloning and Building

From your game directory, run:

```zsh
git clone https://github.com/vitorbetmann/smile.git

# Mac / Linux
cmake -S smile -B smile/build
cmake --build smile/build # or ninja -C smile/build

# Windows
cmake -S .\smile\ -B .\smile\build\
cmake --build .\smile\build\ # or ninja -C .\smile\build
```

Smile builds as libsmile.a, a static library containing all Smile modules
that you can link directly into your game.

A typical project structure might look like this:

```plaintext
/my_game_project/
├── main.c                # Your game's entry point
└── smile/                # Smile cloned here
```

#### Note

By default, Smile compiles with runtime `warning` and `info` logs enabled.
Below is an example of how they would appear in your terminal:

![Example of Smile's Logs](docs/_Internal/__Assets/LogExample.png)

If you want to disable them, pass the following flags when configuring your
build with CMake:

 ```zsh
 cmake -S ./smile -B smile/build -DSMILE_LOG_WARNING=OFF -DSMILE_LOG_INFO=OFF
 ```

This will disable all Smile `warning` and `info` logging output at build
time. `Error` logs cannot be disabled.

### Compiling

There are two common ways to compile your game using Smile:

---

#### Option 1 — With Clang (from your source directory)

```zsh
clang main.c -Ismile/include -Lsmile/build -lsmile -o my_game
```

If needed, replace `main.c` with your entry source file and/or `my_game` with
your desired output name.

#### Option 2 — With CMake

Your project directory should look like this:

```plaintext
/my_game_project/
├── CMakeLists.txt        # Your game's CMake file
├── main.c                # Your game's entry point
└── smile/                # Smile cloned or copied here
```

In your CMakeLists.txt, add:

```cmake
# Add Smile as a subdirectory
add_subdirectory(smile)

# Include Smile headers
target_include_directories(my_game PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/smile/include
)

# Link against the Smile static library
target_link_libraries(my_game PRIVATE smile)
```

If needed, replace `my_game` with the name of your project's executable, and you
can compile your game normally using cmake.

---

<br>

## ⌨️ Actually Coding

Okay, now that you have cloned and built Smile, what next?

Below is an example of how to use the StateMachine module. It follows a set of
conventions shared across modules, making it easy to learn new ones:

```c
#include "StateMachine.h"
#include "Menu.h"            // Define your states in other files.
#include "LevelOne.h"

int main(void)
{
     /* Most modules have a Start function. The first 2 letter preceding 'Start'
      * serve to identify from which module that function belongs (sm for
      * StateMachine, sl for SaveLoad, lg for Log...)  
      */
    smStart();
    
    // Pass in the states' functions into smCreateState.
    smCreateState("menu", menuEnter, menuUpdate, menuDraw, menuExit);
    smCreateState("level 1", levelOneEnter, levelOneUpdate, levelOneDraw, levelOneExit);
    
    smSetState("menu", nullptr); // Choose where you want to start
    
    while (smIsRunning())      // Run your game until you Stop StateMachine
    { 
        float dt = smGetDt();    // Calculate the delta time since the last frame 
        smUpdate(dt);            // Update game logic 
        smDraw();                // Render to the screen
    }
}
```

Without all the comments, this is how short your main.c file can be:

```c
#include <StateMachine.h>
#include "menu.h"
#include "levelOne.h"

int main(void)
{
    smStart();
    
    smCreateState("menu", menuEnter, menuUpdate, menuDraw, menuExit);
    smCreateState("level 1", levelOneEnter, levelOneUpdate, levelOneDraw, levelOneExit);
    
    smSetState("menu", nullptr); 
    
    while (smIsRunning())
    {
        smUpdate(smGetDt());
        smDraw();
    }
}
```

Then in your states' header files you could have something like:

```c
#ifndef MENU_H
#define MENU_H

void menuEnter(const void *args);
void menuUpdate(float dt);
void menuDraw(void);
void menuExit(void);

#endif
```

And in the source files:

```c
#include "menu.h"
#include "StateMachine.h"

void menuEnter(void *args)
{
    // Handle initialization
}

void menuUpdate(float dt)
{
    // Handle inputs and updates

    // Changing states is easy after they're created:
    if (PlayButtonPressed())
    {
        smSetState("level 1", nullptr);
    }
    
    // So is quitting the game:
    else if (QuitButtonPressed())
    {
        smStop(); /* This calls this state's exit function and sets smIsRunning
                   * to false, breaking the main game loop.
                   * Most modules have a Stop function as well. Therefore, the
                   * workflow of Start → Use → Stop is common, making it easy to
                   * pick up and learn other modules.
                   */
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
```

It's as simple as that! All the rest is handled by StateMachine.

And this is the overall philosophy of Smile. It handles the boilerplate in the
background so you can focus on letting your creativity out!

If you're interested, feel free to explore each module for detailed guides and
examples:

| Module                                                        | Description                               |
|---------------------------------------------------------------|-------------------------------------------|
| [Log](docs/Log)                                               | Debug code and handle fatal errors easily |
| [ParticleSystem](docs/ParticleSystem) (🚧 under construction) | Simulate smoke, dust, and fire            |
| [SaveLoad](docs/SaveLoad) (🚧 under construction)             | Quickly save and load your game           |
| [StateMachine](docs/StateMachine)                             | Manage states and transitions cleanly.    |

---

<br>

## 🤝 Contributing

> _"I built Smile to be the open source project I wish I had in my early
programming days._
>
> _1. Low barrier of entry_  
> _2. Well documented_  
> _3. Learning-focused"_
>
> — Vitor Betmann, creator of Smile

Whether you're a first-year university student, a self-taught coder, or a
professional branching into new areas, Smile welcomes contributors who are
curious, motivated, and eager to learn.

Smile is meant to grow slowly and thoughtfully, with help from a community that
values learning, clarity, experimentation, and shared growth.

If that got you excited, there are many ways to contribute (plus, you'll get
full credit in the code and Git history!):

- Suggesting improvements
- Adding features
- Writing/editing documentation
- Making games and reporting bugs
- Asking questions and helping others in
  the [Discord Server](https://discord.gg/EdS6PwMffH).

To learn more, check out the [Contributing Guide](docs/_Internal/_Contributing).

---

<br>

## 🪪 License

Smile is released under the MIT License. See the [LICENSE](./LICENSE) file for
details.

---

<br>

| Last modified | Author        | Description                 |
|---------------|---------------|-----------------------------|
| Oct 24, 2025  | Vitor Betmann | Documented Windows support. |