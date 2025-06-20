# State Machine is Lowkey Elegant (SMILE 😊)

## The SMILE engine is a beginner-friendly state machine-based game engine for making 2D games

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

This will generate a static library called `libsmile.a` inside the `build` folder

---

## 🚀 Using SMILE in your game

To compile your game with SMILE, use:

**With Clang:**

```zsh
clang main.c -Ismile_engine/include -Lsmile_engine/build -lsmile -o my_game
```

Replace `my_game` with the desired output name. If needed, also replace `main.c` with your entry source file.

**With CMake**

In your `CMakeLists.txt` add:

```cmake
# Link it to the SMILE library
target_link_libraries(my_game PRIVATE smile)

# Include the SMILE headers
target_include_directories(my_game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/smile_engine/include
)
```

Don't forget to replace `my_game` with the name of your project's executable.

---

## 🧪 Example usage:

```C
#include "smile.h"

extern State exampleState;

int main(void) {
  // ------------------------------
  // Initialize everything you need
  // ------------------------------

  float dt;
  bool isGameRunning = true;
  SmileInit();
  SmileChangeState(&exampleState, NULL);

  while (isGameRunning) {
    // --------------
    // Get delta time
    // --------------

    SmileUpdate(dt);
    SmileDraw();
  }

  SmileShutdown();
  return 0;
}
```
