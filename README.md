# SMILE 😊: A modular 2D game engine in C

## What is SMILE?

SMILE stands for **State Machine Is Lowkey Elegant**.

It’s a modular collection of tools designed to simplify 2D game development in C, aimed at providing a lightweight and extensible foundation for developers at all skill levels.

Ideal for rapid prototyping, learning C through small games, or game jam entries where full control without bloat is desired.

---

### Mission

> _"I built SMILE to be the open source project I wish I had in my early programming days:_
>
> _1. Low barrier of entry_  
> _2. Well documented_  
> _3. Pressure-free and learning-focused environment"_
>
> — Betmann, creator of SMILE

Whether you're a first-year university student, a self-taught coder, or a professional branching into new areas, SMILE welcomes contributors who are curious, motivated, and eager to learn.

The engine is meant to grow slowly and thoughtfully, with help from a community that values learning, clarity, experimentation, and shared growth — not just shipping features.

---

## ✨ Features

SMILE is modular, so you can include only what you need.

Current modules:

- **StateMachine**: A clean and efficient way to manage game states and transitions.
- **ParticleSystem**: A simple but flexible system for effects like explosions, smoke, and more.
- _More modules coming soon!_

---

## 🚀 Quick Start

### Prerequisites

Before building SMILE, ensure the following are installed:

- CMake version 3.25 or higher
- A build tool such as Make or Ninja
- A C compiler such as Clang or GCC

### Clone and Build

```zsh
git clone https://github.com/vitorbetmann/smile_engine.git
cd smile_engine
cmake -S . -B build
cmake --build build
```

This builds libsmile.a — a static library that includes the StateMachine and ParticleSystem modules by default.

Your Source Directory should now look something like this:

```plaintext
/my_game_project/
├── main.c                # Your game's entry point
└── smile_engine/         # SMILE cloned or copied here
```

---

## 🎮 Building Your Game with SMILE

There are two common ways to compile your game using SMILE:

**With Clang (from your source directory):**

```zsh
clang main.c -Ismile_engine/include -Lsmile_engine/build -lsmile -o my_game
```

If needed, replace `my_game` with the desired output name and/or `main.c` with your entry source file.

**With CMake:**

Your Source Directory should look similar to this:

```zsh
/my_game_project/
├── CMakeLists.txt        # Your game's CMake file
├── main.c                # Your game's entry point
└── smile_engine/         # SMILE cloned or copied here
```

In your `CMakeLists.txt`, add:

```cmake
# Add SMILE engine as a subdirectory
add_subdirectory(smile_engine)

# Include SMILE headers
target_include_directories(my_game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/smile_engine/include
)

# Link against the SMILE library target
target_link_libraries(my_game PRIVATE smile)
```

Don't forget to replace `my_game` with the name of your project's executable, if applicable.

---

## Contributions Welcome

SMILE is a place to learn, experiment, and grow — especially if you're still early in your programming journey.

### SMILE is the right place for you! No matter if you're:

- A first-year computer science student curious about how game engines work.
- An experienced web developer looking to explore graphics.
- A self-taught programmer looking to get more comfortable with GitHub, unit testing, or contributing to open source projects.
- None of the above — just someone eager to learn something new.

### What do you need to get started?

Just a willingness to learn. That’s it.

SMILE is designed to be approachable, collaborative, and beginner-friendly — while still aiming to grow into something powerful and extensible over time.

There are many ways to contribute:

- Fixing bugs or suggesting improvements
- Adding small features or new modules
- Writing or editing documentation
- Asking questions, sharing insights, or helping others learn

This is a space to build skills, grow confidence, and collaborate on something meaningful.  
Whether you're here to level up your C programming, explore game dev concepts, or just try something new — you're welcome here. 😊

To learn more, check out [how to contribute](./docs/Contributing.md).

---

## License

SMILE is released under the MIT License. See the [LICENSE](./LICENSE) file for details.

---

## Documentation

Explore each module’s documentation for detailed guides and examples:

- [State Machine Getting Started](./docs/StateMachine/SM_GettingStarted.md)
- [Particle System Getting Started](./docs/ParticleSystem/PS_GettingStarted.md)

Dive deeper with the full API references:

- [State Machine API](./docs/StateMachine/SM_API.md)
