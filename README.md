# Smile 😊 — A modular 2D game engine in C

**Smile** stands for **Simple Modularity Is Lowkey Elegant** — a lightweight,
modular collection of C libraries that simplifies 2D game development.

Ideal for **game jams** and **rapid prototyping**, Smile provides clean
abstractions that keep your code lean, organized, and fully under your control.

---

<br>

## 🎮 Made with Smile

---

<br>

## Why Smile?

Smile is a modular collection of C libraries that streamline common 2D
game-development tasks such as managing states, simulating particles, and
saving or loading data.

It’s built for developers who want simplicity without sacrificing control.
You can mix and match only the modules you need (for example, using the
StateMachine without the ParticleSystem) to keep your project lightweight
and focused.

While Smile doesn’t handle rendering, input, or audio directly, it integrates
seamlessly with libraries like [raylib](https://www.raylib.com).

Here’s a summary of the current modules:

- **Log** — Create custom logs for debugging and control how fatal errors are
  handled.
- **ParticleSystem** — Add explosions, smoke, and other effects with a simple
  yet flexible system built on raylib.
- **SaveLoad** — Save and load your game with one-shot functions or detailed
  session control.
- **StateMachine** — Manage your game states and transitions cleanly and
  efficiently.
- **More modules coming soon!**

---

<br>

## 🚀 Building Your Game

### Prerequisites

Before building Smile, make sure you have the following installed:

- **CMake** 3.25 or higher
- A build tool such as **Make** or **Ninja**
- A C compiler such as **Clang** or **GCC**

### Cloning and Building

From your game directory, run:

```zsh
git clone https://github.com/vitorbetmann/smile.git
cmake -S ./smile -B smile/build
cmake --build smile/build
```

Smile builds as libsmile.a, a static library containing all Smile modules
that you can link directly into your game.

A typical project structure might look like this:

```plaintext
/my_game_project/
├── main.c                # Your game's entry point
└── smile/                # Smile cloned here
```

### Compiling

There are two common ways to compile your game using Smile:

---

#### **Option 1 — With Clang (from your source directory)**

```zsh
clang main.c -Ismile/include -Lsmile/build -lsmile -o my_game
```

If needed, replace `main.c` with your entry source file and/or `my_game` with
your
desired output name.

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

If needed, replace `my_game` with the name of your project's executable.

---

<br>

## ⌨️ Actually Coding

Okay, now that you have cloned and built Smile, what next?

Below is a small demo of how to use the StateMachine module:



---

<br>

## 🤝 Contributing

> _"I built Smile to be the open source project I wish I had in my early
programming days._
>
> _1. Low barrier of entry_  
> _2. Well documented_  
> _3. Pressure-free and learning-focused environment"_
>
> — Vitor Betmann, creator of Smile

Whether you're a first-year university student, a self-taught coder, or a
professional branching into new areas, Smile welcomes contributors who are
curious, motivated, and eager to learn.

Smile is meant to grow slowly and thoughtfully, with help from a community that
values learning, clarity, experimentation, and shared growth.

### —_What do you need to get started?_

There are many ways to contribute:

- Fixing bugs or suggesting improvements
- Adding small features or new modules
- Writing or editing documentation
- Asking questions, sharing insights, or helping others learn

To learn more, check
out the [Contributing Guide](docs/_Internal/_Contributing).

---

<br>

## 🪪 License

Smile is released under the MIT License. See the [LICENSE](./LICENSE) file for
details.

---

<br>

## 📜 Documentation

Explore each module’s documentation for detailed guides and examples:

- [Log](docs/Log)
- [ParticleSystem](docs/ParticleSystem)
- [SaveLoad](docs/SaveLoad)
- [StateMachine](docs/StateMachine)

---

<br>

Authored by: Vitor Betmann