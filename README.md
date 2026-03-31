# Welcome to Smile 😊

Smile (`Simple Modularity Is Lowkey Elegant`) is an open-source, dependency-free,
modular collection of C libraries that reduces boilerplate for common 2D game-development
tasks such as `managing scenes`, `simulating particles`, and `saving/loading`.

Currently supported on `Mac` and `Windows` (Linux untested but POSIX-compliant), Smile is built
for people who value development speed without sacrificing control by having modules follow a
consistent `Start → Use → Stop` pattern and encapsulating memory management, making it ideal for
game jams and quick prototyping.

You can also mix and match only the modules you need (for example, using SceneManager
without ParticleSystem) to keep your project lightweight and focused. And while Smile
doesn't handle rendering, input, or audio directly, it integrates seamlessly with
libraries like [raylib](https://www.raylib.com).

## 🎮 Smile Demo

<p align="center">
  <img src="/docs/_Internal/__Assets/SceneManager/SceneManagerDemo.gif" width="45%"/>
  <img src="/docs/_Internal/__Assets/ParticleSystem/ParticleSystemDemo.gif" width="45%"/>
</p>

<p align="center">
  <img src="/docs/_Internal/__Assets/SaveLoad/SaveLoadDemo.gif" width="45%"/>
  <img src="/docs/_Internal/__Assets/Log/LogDemo.gif" width="45%"/>
</p>

## 🚀 Building Your Game

### — Prerequisites

Before building Smile, make sure you have the following installed:

- `CMake` 3.30 or higher
- A build tool such as `Make` or `Ninja`
- A C compiler with C23 support such as `Clang` or `GCC`

---

### — Cloning and Building

From your game directory, run:

```zsh
git clone https://github.com/vitorbetmann/smile.git

cmake -S smile -B smile/build
cmake --build smile/build
```

Smile builds as a static library (libsmile.a on Mac/Linux, smile.lib on
Windows) containing all Smile modules that you can link directly into your game.

A typical project structure might look like this:

```plaintext
/my_game_project/
├── main.c                # Your game's entry point
└── smile/                # Smile cloned here
```

#### Note

By default, Smile compiles with runtime `warning` and `info` logs enabled.
Below is an example of how they would appear in your terminal:

![Example of Smile's Logs](/docs/_Internal/__Assets/_SmileREADME/LogExample.png)

If you want to disable them, pass the following flags when configuring your
build with CMake:

 ```zsh
 cmake -S ./smile -B smile/build -DSMILE_WARN=OFF -DSMILE_INFO=OFF
 ```

This will disable all Smile `warning` and `info` logging output at build
time. `Error` logs cannot be disabled.

---

### — Compiling

There are two common ways to compile your game using Smile:

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

## ⌨️ Actually Coding

Okay, now that you have cloned and built Smile, what next?

Below is an example of how to use the SceneManager module. It follows a set of
conventions shared across modules, making it easy to learn new ones:

```c
#include "SceneManager.h"
#include "Menu.h"            // Define your scenes in other files.
#include "LevelOne.h"

int main(void)
{
     /* Most modules have a Start function. The first 2 letter preceding 'Start'
      * serve to identify from which module that function belongs (sm for
      * SceneManager, sl for SaveLoad, lg for Log...)  
      */
    smStart();
    
    // Pass in the scenes' functions into smCreateScene.
    smCreateScene("menu", menuEnter, menuUpdate, menuDraw, menuExit);
    smCreateScene("level 1", levelOneEnter, levelOneUpdate, levelOneDraw, levelOneExit);
    
    smSetScene("menu", nullptr); // Choose where you want to start
    
    while (smIsRunning())      // Run your game until you Stop SceneManager
    { 
        float dt = smGetDt();    // Calculate the delta time since the last frame 
        smUpdate(dt);            // Update game logic 
        smDraw();                // Render to the screen
    }
}
```

Without all the comments, this is how short your main.c file can be:

```c
#include <SceneManager.h>
#include "menu.h"
#include "levelOne.h"

int main(void)
{
    smStart();
    
    smCreateScene("menu", menuEnter, menuUpdate, menuDraw, menuExit);
    smCreateScene("level 1", levelOneEnter, levelOneUpdate, levelOneDraw, levelOneExit);
    
    smSetScene("menu", nullptr); 
    
    while (smIsRunning())
    {
        smUpdate(smGetDt());
        smDraw();
    }
}
```

Then in your scenes' header files you could have something like:

```c
#ifndef MENU_H
#define MENU_H

void menuEnter(void *args);
void menuUpdate(float dt);
void menuDraw(void);
void menuExit(void);

#endif
```

And in the source files:

```c
#include "menu.h"
#include "SceneManager.h"

void menuEnter(void *args)
{
    // Handle initialization
}

void menuUpdate(float dt)
{
    // Handle inputs and updates

    // Changing scenes is easy after they're created:
    if (PlayButtonPressed())
    {
        smSetScene("level 1", nullptr);
    }
    
    // So is quitting the game:
    else if (QuitButtonPressed())
    {
        smStop(); /* This calls this scene's exit function and sets smIsRunning
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

It's as simple as that! All the rest is handled by SceneManager.

And this is the overall philosophy of Smile. It handles the boilerplate in the
background so you can focus on letting your creativity out!

If you're interested, feel free to explore each module for detailed guides and
examples:

| Module                                                        | Description                               |
|---------------------------------------------------------------|-------------------------------------------|
| [Log](/docs/Log)                                              | Debug code and handle fatal errors easily |
| [ParticleSystem](/docs/ParticleSystem) (🚧 Under Development) | Simulate smoke, dust, fire, and more      |
| [SaveLoad](/docs/SaveLoad) (🚧 Under Development)             | Quickly save and load your game           |
| [SceneManager](/docs/SceneManager)                            | Manage scenes and transitions cleanly     |

## 🤝 Contributing

> _"I built Smile to be the open source project I wish I had in my early
programming days._
>
> _1. Low barrier of entry_  
> _2. Well documented_  
> _3. Learning-focused"_
>
> — Vitor Betmann, creator of Smile

Smile is an open learning project. Everyone is welcome to use Smile to learn,
suggest improvements, and help it evolve.

That said, Smile is also a personal sandbox project I’m building to understand
how game frameworks work from the ground up. I reserve some systems to design
and architect myself first as part of that learning journey and once those
foundations are ready, contributions that improve, expand, or refine them are
always welcome.

If that got you excited, there are many ways to contribute (plus, you'll get
full credit in the code and Git history!):

- Suggesting improvements/optimizations
- Adding features
- Writing/editing documentation
- Making games and reporting bugs

To learn more, check out
the [Contributing Guide](/docs/_Internal/_Contributing).

## 🪪 License

Smile is released under the MIT License. See the [LICENSE](./LICENSE) file for
details.