# Smile Tools 🛠️

Smile ships a set of CLI tools alongside the library to automate repetitive
development tasks so you can stay focused on your game logic.

---

## ⚙️ Installing

To make them accessible from anywhere, run the `--install` command after building:

```bash
# Mac/Linux:
sudo cmake --install smile/build

# Windows (run from an admin terminal):
cmake --install smile/build
```

After installation, you're able to call the tool directly by its name: `<ToolName> <Args>`

If you skip installation, run them from your project's root: `smile/build/<ToolName> <Args>`

---

## 🧰 Tools

| Tool                    | Description                                                                    |
|-------------------------|--------------------------------------------------------------------------------|
| [GenScene](GenScene.md) | Generates boilerplate scene source and header files for use with SceneManager. |
