# Developer Setup ⌨️

## Thank you for your interest in contributing to Smile!

This series of documents in the `_Contributing` directory will guide you through
everything you need to know to work effectively within the project.

The document you’re reading introduces Smile’s Developer Mode and helps you set
up your environment to start contributing.

In addition, the other files each focus on a specific aspect of Smile’s
development workflow:

- [1_The_Structure_Of_Smile](1_The_Structure_Of_Smile.md)
- [2.1_Coding_Guidelines_Code_Organization](2.1_Coding_Guidelines_Code_Organization.md)
- [2.1_Coding_Guidelines_General_Guidelines](2.2_Coding_Guidelines_General_Conventions.md)
- [3_Documentation_Guidelines](3_Documentation_Guidelines.md)
- [4_Testing_Guidelines](4_Testing_Guidelines.md)
- [5_Pull_Requesting](5_Pull_Requesting.md)
- [6_Bug_Reporting_And_Suggesting_Features](6_Bug_Reporting_And_Suggesting_Features.md)

---

<br>

## Table of Contents

- [Differences Between DEV and USER Modes](#-differences-between-dev-and-user-modes)
- [Building Smile In Developer Mode](#-building-smile-in-developer-mode)
- [Up Next](#up-next)
-

---

<br>

## ≠ Differences Between DEV And USER Modes

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
```

Now you can compile it in Developer Mode:

```zsh
cmake -S . -B build -DSMILE_DEVELOPER=ON
cmake --build build # or ninja -C build
```

After running the commands above, you should see the following on your terminal:

```plaintext
-- Smile — Build type: Debug
-- Smile — Warning logs ENABLED
-- Smile — Info logs ENABLED
-- Smile: Compiling TEST files
```

This confirms Smile is built in developer mode.

**Note:**  
By default, Smile compiles with runtime **warning** and **info** logs enabled.
Below is an example of how they would appear in your terminal:

![Example of Smile's Logs](../__Assets/LogExample.png)

If you want to disable them, pass the following flags when configuring your
build with CMake:

 ```zsh
 cmake -S ./smile -B smile/build -DSMILE_DEVELOPER=ON -DSMILE_LOG_WARNING=OFF -DSMILE_LOG_INFO=OFF
 ```

This will disable all Smile **warning** and **info** logging output at build
time. Errors cannot be disabled.

---

<br>

## Up Next

Learn how Smile's files and directories are organized
in [1_The_Structure_Of_Smile](1_The_Structure_Of_Smile.md)

---

<br>

| Last modified | Author        | Description       |
|---------------|---------------|-------------------|
| Oct 21, 2025  | Vitor Betmann | Created document. |