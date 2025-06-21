# Welcome to the SMILE Engine! 😊

**SMILE** stands for **State Machine is Lowkey Elegant**.

## SMILE is a beginner-friendly game engine for making 2D games in C.

It uses a state machine architecture inspired by CS50's _Introduction to Game Development_ course engine written in Lua.

---

## 🔧 Building

Inside your C project directory, clone the SMILE engine:

```zsh
git clone https://github.com/vitorbetmann/smile_engine.git
cd smile_engine
cmake -S . -B build
# Then build using your preferred builder, for example:
ninja -C build     # or
cmake --build build
```

This will generate a static library called `libsmile.a` inside the `build` folder.

---

## 🎮 Using SMILE in your game

To compile your game with SMILE, use:

**With Clang:**

```zsh
clang main.c -Ismile_engine/include -Lsmile_engine/build -lsmile -o my_game
```

If needed, replace `my_game` with the desired output name and/or `main.c` with your entry source file.

**With CMake:**

In your `CMakeLists.txt`, add:

```cmake
# Include the SMILE headers
target_include_directories(my_game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/smile_engine/include
)

# Link with the SMILE static library
target_link_libraries(my_game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/smile_engine/build/libsmile.a
)
```

Don't forget to replace `my_game` with the name of your project's executable, if applicable.

---

## 🧪 Quick example usage:

```C
#include "smile.h"

extern State example_state;
bool is_running = true;
float dt;

int main(void) {
  sm_change_state(&example_state, NULL);

  while (is_running) {
    // Get dt
    sm_update(dt);
    sm_draw();
  }

  return 0;
}
```
