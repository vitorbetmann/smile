# SMILE State Machine: API 🤖

The SMILE engine provides a modular, lightweight, and flexible state machine system for managing game states. Each state represents a phase of the game (e.g., menus, levels, splash screens), and supports optional `enter`, `update`, `draw`, and `exit` functions.

---

## 🌀 Lifecycle Overview

1. **Initialization**

   - Call `SM_Init()` once at the start of your program.

2. **State Registration**

   - Use `SM_RegisterState()` to define and register your states.

3. **State Switching**

   - Use `SM_ChangeStateTo()` to transition to a different state by name.

4. **Main Loop**

   - Call `SM_Update(dt)` and `SM_Draw()` each frame.

5. **Shutdown**
   - Call `SM_Shutdown()` before exiting your program.

---

## 📚 Function Reference

### `bool SM_Init(void);`

**Initializes the state machine.**  
Allocates internal structures and prepares the system to register and run states.

> **Note:** This function is safe to call multiple times. If the machine is already initialized, it returns `false` without error.

**Returns:**  
`true` if initialized successfully, `false` if already initialized or if memory allocation failed.

---

### `bool SM_IsInitialized(void);`

**Checks whether the state machine has been initialized.**

**Returns:**  
`true` if initialized, `false` otherwise.

---

### `bool SM_RegisterState(const char *name, void (*enterFn)(void *), void (*updateFn)(float), void (*drawFn)(void), void (*exitFn)(void));`

**Registers a new named state with optional lifecycle callbacks.**  
Each state must have a unique name. At least one lifecycle function must be non-`NULL`.

- `name`: The name of the state (must be non-`NULL` and non-empty).
- `enterFn`: Called when entering this state (can be `NULL`).
- `updateFn`: Called every update tick while this state is active (can be `NULL`).
- `drawFn`: Called every frame while this state is active (can be `NULL`).
- `exitFn`: Called when exiting this state (can be `NULL`).

**Returns:**  
`true` if registration succeeds, `false` otherwise.

---

### `bool SM_IsStateRegistered(char *name);`

**Checks whether a state with the given name is registered.**

- `name`: The name of the state to check.

**Returns:**  
`true` if a state with the given name exists, `false` otherwise.

---

### `bool SM_ChangeStateTo(const char *name, void *args);`

**Switches to a different state by name, optionally passing arguments.**  
Exits the current state (if any) and enters the new one. Will also re-enter the same state if the requested name matches the current state's name.

- `name`: The name of the state to switch to.
- `args`: Optional arguments to pass to the new state's `enter` function.

**Returns:**  
`true` if the state change succeeded, `false` otherwise.

---

### `bool SM_Update(float dt);`

**Calls the update function of the current active state.**  
Does nothing if the state machine is not initialized or if no update function is defined.

- `dt`: Delta time since last update.

**Returns:**  
`true` if update was successful, `false` otherwise.

---

### `bool SM_Draw(void);`

**Calls the draw function of the current active state.**  
Does nothing if the state machine is not initialized or if no draw function is defined.

**Returns:**  
`true` if draw was successful, `false` otherwise.

---

### `bool SM_Shutdown(void);`

**Shuts down the state machine and frees all internal memory.**  
Calls the `exit` function of the current state (if defined) before cleanup. After shutdown, all registered states are discarded and the tracker is reset.

**Returns:**  
`true` if shutdown succeeded, `false` if the machine was not initialized.

---

### `const char *SM_GetCurrStateName(void);`

**Gets the name of the current active state.**

**Returns:**  
The name of the current state, or `NULL` if no state is active or the machine is uninitialized.

---

## ⚠️ Notes

- State names must be unique.
- Do not call SMILE functions before `SM_Init()` or after `SM_Shutdown()`.
- All lifecycle functions are optional (`NULL` is allowed).
- Arguments passed to `SM_ChangeStateTo` must remain valid until used.
