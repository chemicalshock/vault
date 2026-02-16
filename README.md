# `vault<T>` – Lightweight Dynamic Slot Container

The `vault<T>` container is a high-performance, minimal-overhead replacement for std::vector-style storage,
designed to support stack-like semantics, pointer-stable elements, and fast rollback functionality.

## 🔧 Features

- Manual memory control with raw allocation
- Fast `emplace()` to store objects and retrieve stable `T*` pointers
- Safe `checkpoint()` and `restore()` to manage temporary storage
- Lock/seal mechanism to restrict or freeze access
- No standard library dependencies other than basic types

## 🚀 Usage Example

```cpp
vault<MyType> v;
v.emplace(1, 2, 3);
v.checkpoint();

auto* p = v.emplace(99);
v.restore();  // removes p safely
```

## 📚 Key Methods

| Method             | Description |
|--------------------|-------------|
| `emplace(args...)` | Constructs and stores an object, returns `T*` |
| `get(index)`       | Returns `T*` at index (throws on error) |
| `checkout(index)`  | Alias for `get()` for API clarity |
| `checkin(ptr)`     | Currently a no-op; placeholder for future reuse logic |
| `checkpoint()`     | Pushes a rollback point onto internal stack |
| `restore()`        | Destroys elements added after the last checkpoint |
| `clear()`          | Clears all elements and resets state |
| `seal()`           | Prevents further growth or mutation |
| `lock()`           | Marks critical section (optional use) |

## 🔐 Safety

This container avoids `std::realloc()` for non-trivial types and moves objects using placement `new`.
It guarantees stable addresses for all inserted objects as long as they are not restored or cleared.

## 📁 Integration

Place `vault.hpp` inside your `src/inc` or shared `dep/` directory and include it with:

```cpp
#include "vault.hpp"
```

## 🧪 Unit Tests

Test coverage is provided in `test_vault.cpp` using the `shocktest` framework. Run:

```bash
make ut
```

to compile and execute tests.

## 📄 License

See the docs LICENSE file for usage terms.
