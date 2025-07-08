# SMILE Engine: Modular Tools for 2D Game Development in C 😊

## What is SMILE?

SMILE stands for **State Machine is Lowkey Elegant**.

It's a beginner-friendly collection of tools designed to simplify 2D game development in C that builds on top of raylib for rendering, input, and other low-level tasks.

It's ideal for quick prototyping and game jams.

## Included Modules

SMILE is designed with modularity in mind — you only include the modules you need for your project. Current modules include:

- **StateMachine**: A simple and efficient state machine system for managing game states and transitions.
- **ParticleSystem**: A flexible particle system for creating visual effects like explosions and smoke.
- _(More modules coming soon!)_

---

## Building SMILE

To build SMILE, clone the repository and use CMake:

```zsh
git clone https://github.com/vitorbetmann/smile_engine.git
cd smile_engine
cmake -S . -B build
# Then build using your preferred builder, for example:
ninja -C build     # or
cmake --build build
```

This will generate a static library (`libsmile.a`) inside the `build` directory that you can link against in your projects. Note that `libsmile.a` includes the StateMachine and ParticleSystem modules by default.

---

## 🎮 Using the State Machine Module in Your Game

To compile your game with the state machine module, use:

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

## Documentation

Each module has its own documentation to help you get started quickly:

- [StateMachine Module Docs](./docs/statemachine.md)
- [ParticleSystem Module Docs](./docs/particlesystem.md)

Check these docs for detailed usage examples, API references, and integration tips.

---

Making games is hard enough. Try to have fun, and remember to SMILE! 😊
