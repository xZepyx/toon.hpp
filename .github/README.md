# toon-cpp

**toon-cpp** is a lightweight **header-only C++ library** for reading, writing, and editing **TOON (Token-Oriented Object Notation)** files.

[TOON](https://github.com/toon-format/toon) is a structured configuration format designed to be **compact, readable, and efficient**. `toon-cpp` allows C++ programs to parse `.toon` files into dynamic objects, modify them, and write them back to disk.

---

# Features

* Header-only library
* Parse TOON text and files
* Write and overwrite `.toon` files
* Dynamic value system (objects, arrays, strings, numbers, booleans)
* Nested object editing
* Minimal dependencies (C++ standard library only)

---

# Installation

Clone the repository:

```bash
git clone https://github.com/yourname/toon-cpp
```

Add the header to your project:

```id="avw7oq"
#include "toon/toon.hpp"
```

No linking or build configuration is required.

---

# Example

```cpp id="g1yyr6"
#include "toon/toon.hpp"
#include <iostream>

int main() {

    toon::value config;

    config["window"]["width"] = 1280;
    config["window"]["height"] = 720;

    config["user"]["name"] = "zepyx";
    config["user"]["admin"] = true;

    std::cout << toon::dump(config);

    toon::write_file("config.toon", config);
}
```

Output:

```id="d0n6h8"
window:
  width: 1280
  height: 720
user:
  name: zepyx
  admin: true
```

---

# Reading Files

```cpp id="1uiay9"
auto config = toon::parse_file("config.toon");

int width = config["window"]["width"];
```

Example `config.toon`:

```id="7s1q1e"
window:
  width: 1280
  height: 720
```

---

# Writing Files

```cpp id="u8z3ww"
toon::write_file("config.toon", config);
```

This serializes the structure and overwrites the file.

---

# Editing Data

```cpp id="fj6rza"
auto cfg = toon::parse_file("config.toon");

cfg["window"]["width"] = 1920;
cfg["window"]["height"] = 1080;

toon::write_file("config.toon", cfg);
```

---

# Arrays

```cpp id="e8t45q"
toon::value fruits;

fruits.push_back("apple");
fruits.push_back("banana");
fruits.push_back("orange");
```

Serialized output:

```id="5jzt1f"
- apple
- banana
- orange
```

---

# Parsing Text

```cpp id="g0ymtw"
auto data = toon::parse(R"(

user:
  name: Alice
  admin: true

)");
```

---

# Documentation

Documentation: [docs/root.md](../docs/root.md)

Todo: [docs/todo.md](../docs/todo.md)

---

# License

© 2025 xZepyx — Licensed under the MIT License.

---

# Status

Early development version.

APIs may change as the parser and format support expand.

# Contributing

Feel free to contribute. It's a open-source project duh?

---
