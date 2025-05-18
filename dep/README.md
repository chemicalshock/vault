# Dependency Folder (`dep/`)

This folder is reserved for external or third-party dependencies that are used by the project, such as testing frameworks, libraries, or shared modules.

## 🔗 How to Add a Dependency

Instead of copying or duplicating files, use a **symbolic link** (symlink) to link to the actual dependency folder elsewhere on your system.

### ✅ Example

To link the `shocktest` framework into `dep/`:

```bash
ln -s /full/path/to/shocktest dep/shocktest
```

> This creates a soft link `dep/shocktest` pointing to your actual ShockTest location.

## 💡 Why Use Symlinks?

- Keeps your project clean
- Allows sharing dependencies across multiple projects
- Avoids file duplication or version mismatch
- Works well with custom build systems or Makefiles

## 📁 Typical Structure After Linking

```
project_root/
├── dep/
│   └── shocktest/   -> symlink to external ShockTest folder
├── src/
├── makefile
```

## 📝 Notes

- You can create symlinks for other tools like `fmt`, `gtest`, or your own shared modules.
- Avoid committing external dependency code directly to your repo unless vendoring intentionally.
