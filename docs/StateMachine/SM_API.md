# SMILE State Machine API

The SMILE engine provides a modular, lightweight, and flexible state machine system for managing game states. Each state represents a mode of the game (e.g., menus, levels, splash screens), with support for custom enter, update, draw, and exit functions.

---

## 🌀 Lifecycle Overview

1. **Initialization**
   - Call `SM_Init()` once at the start of your program.
2. **State Registration**
   - Use `NewState()` to define all your states.
3. **State Switching**
   - Use `SM_ChangeState()` to switch between registered states.
4. **Game Loop**
   - Call `SM_Update(dt)` and `SM_Draw()` every frame.
5. **Shutdown**
   - Call `SM_Shutdown()` before exiting your program.

---

# SMILE State Machine API

The SMILE engine provides a modular, lightweight, and flexible state machine system for managing game states. Each state represents a mode of the game (e.g., menus, levels, splash screens), with support for custom enter, update, draw, and exit functions.

---

## 🌀 Lifecycle Overview

1. **Initialization**
   - Call `SM_Init()` once at the start of your program.
2. **State Registration**
   - Use `NewState()` to define all your states.
3. **State Switching**
   - Use `SM_ChangeState()` to switch between registered states.
4. **Game Loop**
   - Call `SM_Update(dt)` and `SM_Draw()` every frame.
5. **Shutdown**
   - Call `SM_Shutdown()` before exiting your program.

---

## 📚 Function Reference

### `void SM_Init(ConfigArgs *configArgs)`

### Purpose

Initializes the state machine system. If `configArgs` is `NULL`, a linked list is used for state storage; otherwise, a preallocated array is created for fixed capacity.

### Parameters

- `ConfigArgs *configArgs`: Optional configuration arguments for preallocating state storage. Pass `NULL` to use dynamic linked list.

### Warnings

- Must be called once before using any other SMILE functions.
- Calling multiple times without `SM_Shutdown` may cause memory leaks.

---

### `const State *NewState(const char *name, EnterFn enterFn, UpdateFn updateFn, DrawFn drawFn, ExitFn exitFn)`

### Purpose

Creates and registers a new state with the given name and lifecycle callbacks.

### Parameters

- `const char *name`: Unique name of the state.
- `EnterFn enterFn`: Function pointer called when entering the state.
- `UpdateFn updateFn`: Function pointer called each frame to update state logic.
- `DrawFn drawFn`: Function pointer called each frame to draw state visuals.
- `ExitFn exitFn`: Function pointer called when exiting the state.

### Returns

- Pointer to the newly created `State` structure, or `NULL` on failure.

### Warnings

- State names must be unique.
- Call after `SM_Init()`.

---

### `void SM_ChangeState(const State *state, void *args)`

### Purpose

Transitions from the current state to the specified new state.

### Parameters

- `const State *state`: Pointer to the state to switch to.
- `void *args`: Optional arguments passed to the new state's enter function.

### Warnings

- If called with the same state as current, the exit and enter functions will still be invoked.
- The `args` pointer must remain valid during the enter function execution.

---

### `void SM_Update(float dt)`

### Purpose

Calls the update function of the current active state.

### Parameters

- `float dt`: Delta time in seconds since last frame.

### Warnings

- Does nothing if no current state is active.

---

### `void SM_Draw(void)`

### Purpose

Calls the draw function of the current active state.

### Warnings

- Does nothing if no current state is active.

---

### `void SM_Shutdown(void)`

### Purpose

Frees all allocated memory and resets the state machine to an uninitialized state.

### Warnings

- After calling, all pointers to states or state IDs become invalid.
- Must be called before program exit to avoid memory leaks.

---

### `const State *SM_GetCurrState(void)`

### Purpose

Returns a pointer to the currently active state.

### Returns

- Pointer to current `State`, or `NULL` if no active state.

---

### `const StateID *SM_GetCurrStateID(void)`

### Purpose

Returns the ID of the currently active state.

### Returns

- Pointer to current state's `StateID`, or `NULL` if no active state.

---

### `const State *SM_GetStateByIDName(const char *name)`

### Purpose

Finds and returns a state by its registered name.

### Parameters

- `const char *name`: Name of the state to find.

### Returns

- Pointer to the `State` if found, or `NULL` if not found.

---

### `const State *SM_GetStateByIDNum(unsigned int num)`

### Purpose

Finds and returns a state by its numeric ID.

### Parameters

- `unsigned int num`: Numeric ID of the state to find.

### Returns

- Pointer to the `State` if found, or `NULL` if not found.

---

### `const StateID *SM_GetStateID(const State *state)`

### Purpose

Returns the ID structure associated with the given state.

### Parameters

- `const State *state`: Pointer to the state.

### Returns

- Pointer to the `StateID` of the state, or `NULL` if invalid.

---

## ⚠️ Notes

- You must not dereference or retain any `State` or `StateID` pointers after `SM_Shutdown()`.
- All lifecycle functions (`enter`, `update`, `draw`, `exit`) are optional — they may be `NULL`.
