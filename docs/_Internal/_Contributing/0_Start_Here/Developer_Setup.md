# Developer Setup ⌨️

## Thank you for your interest in contributing to Smile!

This series of documents in the `_Contributing` directory will guide you through
everything you need to know to work effectively within the project.

The document you’re reading introduces Smile’s Developer Mode and helps you set
up your environment to start contributing.

In addition, the other files each focus on a specific aspect of Smile’s
development workflow:

- [1_Understanding_Smile](/docs/_Internal/_Contributing/1_Understanding_Smile)
    - [Structure](/docs/_Internal/_Contributing/1_Understanding_Smile/Structure.md)
- [2_Coding_Guidelines](/docs/_Internal/_Contributing/2_Coding_Guidelines)
    - [Code_Organization](/docs/_Internal/_Contributing/2_Coding_Guidelines/Code_Organization.md)
    - [Code_Style](/docs/_Internal/_Contributing/2_Coding_Guidelines/Code_Style.md)
- [3_Documentation_Guidelines] (🚧 Under Development)
- [4_Testing_Guidelines] (🚧 Under Development)
- [5_Pull_Requesting] (🚧 Under Development)
- [6_Issued_And_Suggestions] (🚧 Under Development)

---

<br>

## Table of Contents

- [Differences Between DEV and USER Modes](#-differences-between-dev-and-user-modes)
- [Building Smile In Developer Mode](#-building-smile-in-developer-mode)
- [Up Next](#up-next)

---

<br>

## ⚖️ Differences Between DEV And USER Modes

By default, Smile builds in User Mode, optimized for performance and minimal
build size. It excludes development tools and test files, focusing on delivering
the final library or application.

Developer Mode, on the other hand, is intended for contributors. It enables
extra build steps, includes internal tests, and provides additional output
during configuration to help with debugging and development.

---

<br>

## 🚀 Building Smile in Developer Mode

### Prerequisites

Before building Smile, make sure you have the following installed:

- **CMake** 3.25 or higher
- A build tool such as **Make** or **Ninja**
- A C compiler such as **Clang** or **GCC**

### Cloning and Building

If you have not yet cloned Smile, run:

```zsh
git clone https://github.com/vitorbetmann/smile.git
cd smile
````

Now you can compile it in Developer Mode:

```zsh
cmake -S . -B build -DSMILE_DEVELOPER=ON
cmake --build build
```

After running the commands above, you should see the following on your terminal:

```plaintext
-- Smile — Build type: Debug
-- Smile — Warning logs: ON
-- Smile — Info logs: ON
-- Smile — Tests: ON
```

This confirms Smile is built in developer mode.

**Note:**  
By default, Smile compiles with runtime **warning** and **info** logs enabled.
Below is an example of how they would appear in your terminal:

![Example of Smile's Logs](/docs/_Internal/__Assets/_SmileREADME/LogExample.png)

If you want to disable them, pass the following flags when configuring your
build with CMake:

 ```zsh
 cmake -S ./smile -B smile/build -DSMILE_DEVELOPER=ON -DSMILE_LOG_WARNING=OFF -DSMILE_LOG_INFO=OFF
 ```

This will disable all Smile **warning** and **info** logging output at build
time. Errors cannot be disabled.

---

<br>

## ➡️ Up Next

Understand the [Structure](/docs/_Internal/_Contributing/1_Understanding_Smile/Structure.md) of Smile.

---

<br>

## ✏️ Last Modified

| Last modified | Author (username)             | Description       |
|---------------|------------------------------|-------------------|
| Feb 10, 2026  | vitorbetmann | Update links from relative to absolute path; Update Last Modified section to include GitHub username over Discord's; Add emoji to Up Next; |
