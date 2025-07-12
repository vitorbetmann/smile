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

### Function Signatures

```c
void SM_Init(void);
```

Initializes the state machine. Must be called before using any other SMILE functions.

---

```c
bool SM_IsInitialized(void);
```

Returns `true` if the state machine is initialized, otherwise `false`.

---

```c
void SM_RegisterState(const char *name, EnterFn enterFn, UpdateFn updateFn, DrawFn drawFn, ExitFn exitFn);
```

Registers a new state with the system.

- `name`: Unique string name for the state.
- `enterFn`: Called when the state is entered.
- `updateFn`: Called every frame during the game loop.
- `drawFn`: Called every frame for rendering.
- `exitFn`: Called when leaving the state.

**Note:** All function pointers may be `NULL`.

---

```c
void SM_ChangeStateTo(const char *name, void *args);
```

Transitions to the state identified by `name`.

- `name`: Name of the state to switch to.
- `args`: Optional data passed to the next state's `enter` function.

If `name` is the current state, `exit` and `enter` are still invoked.

---

```c
void SM_Update(float dt);
```

Calls the `update` function of the current active state.

- `dt`: Delta time (in seconds) since the last frame.

---

```c
void SM_Draw(void);
```

Calls the `draw` function of the current active state.

---

```c
void SM_Shutdown(void);
```

Cleans up all allocated memory and resets the state machine.

**Warning:** Do not access any `State` pointers after shutdown.

---

```c
const char *SM_GetCurrStateName(void);
```

Returns the name of the current active state, or `NULL` if none.

---

## ⚠️ Notes

- State names must be unique.
- Do not call SMILE functions before `SM_Init()` or after `SM_Shutdown()`.
- All lifecycle functions are optional (`NULL` is allowed).
- Arguments passed to `SM_ChangeStateTo` must remain valid until used.
