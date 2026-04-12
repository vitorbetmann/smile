# Smile Tools 🛠️

Smile ships a set of CLI tools alongside the library to automate repetitive
development tasks — things like generating boilerplate files — so you can stay
focused on your game logic.

---

## ⚙️ Getting Started

To call Smile tools from your project root, install them first:

```zsh
cmake --install smile/build          # Windows
sudo cmake --install smile/build     # Mac / Linux
```

If you'd rather skip the installation step, you can run any tool directly by path:

```zsh
smile/build/<ToolName> <args>
```

---

## 🧰 Tools

| Tool                    | Description                                                                    |
|-------------------------|--------------------------------------------------------------------------------|
| [GenScene](GenScene.md) | Generates boilerplate scene source and header files for use with SceneManager. |
