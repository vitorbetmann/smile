# SMILE Engine – StateMachine Test Plan 🧪

This document outlines test cases to ensure the reliability, safety, and correctness of the `StateMachine` module in the SMILE Engine.

The goal is to verify core behavior, validate edge cases, and ensure graceful failure in uninitialized or invalid usage.

---

## ✅ Initialization

### SM_Init()

- [ ] Initializes internal tracker
- [ ] Is idempotent: calling twice logs a warning but does not crash
- [ ] SM_IsInitialized() returns true after init

---

## 📦 State Registration

### SM_RegisterState()

- [ ] Registers a new state and allows lookup by name
- [ ] Duplicate state name logs a warning and is ignored
- [ ] Fails safely if called before `SM_Init()`
- [ ] Fails safely if malloc fails (optional)

---

## 🔁 State Transitions

### SM_ChangeStateTo()

- [ ] Switches to the correct state
- [ ] Calls `exit()` on current state before switching
- [ ] Calls `enter(args)` on new state
- [ ] Fails safely if state name is NULL
- [ ] Fails safely if state is not registered
- [ ] Fails safely if state machine is uninitialized

---

## ⏱ Update & Draw

### SM_Update() / SM_Draw()

- [ ] Routes to current state’s update/draw correctly
- [ ] Gracefully handles null `update` or `draw` callbacks
- [ ] Does nothing if no state is active
- [ ] Fails safely if SM is not initialized

---

## 🧹 Shutdown

### SM_Shutdown()

- [ ] Calls `exit()` on current state if present
- [ ] Frees all registered states and clears memory
- [ ] Frees the internal tracker
- [ ] Sets all relevant pointers to NULL
- [ ] Is safe to call twice in a row (idempotent)

---

## 🚫 Invalid Access After Shutdown

- [ ] Calling any public function after shutdown logs a warning
- [ ] Returns safe fallbacks (e.g., NULL) without crashing

---

## 🧪 Internal / Advanced Tests

- [ ] `SM_SetCurrState(NULL)` clears current state
- [ ] `SM_GetState("SomeName")` returns correct pointer
- [ ] `SM_GetCurrState()` returns active state (when internal access is needed)
- [ ] `SM_GetCurrStateName()` returns correct state name or NULL if none

---

## 🚀 Optional / Stress Tests

- [ ] Registering and freeing 100+ states in succession causes no leaks (Valgrind/ASan)
- [ ] State with only some lifecycle callbacks (e.g. just draw + exit) behaves correctly
- [ ] Transitioning rapidly between states doesn’t leak memory or skip callbacks

---

## 🧰 Test Utilities

- Use `SM_Test_GetTracker()` to verify allocation
- Consider adding `SM_Test_NumStates()` later for map size validation
- Assert behavior using standard `assert()` or macros like:

```c
#define ASSERT_STR_EQ(a, b) assert(strcmp((a), (b)) == 0)
```
