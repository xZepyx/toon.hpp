# Documentation

`toon.hpp` is a lightweight **cpp header** for reading, writing, and editing **TOON (Token-Oriented Object Notation)** files.

It provides a dynamic value type similar to nholmann::json library and allows applications to treat `.toon` files as editable configuration data.

---

# Installation

`toon.hpp` is header-only. You only need to include the header in your project.

Project structure example:

```
your_project/
├─ include/
│  └─ toon/
│     └─ toon.hpp
└─ src/
```

Include it in your code:

```cpp
#include "toon/toon.hpp"
```

No linking or build configuration is required.

---

# Basic Usage

Create and modify data:

```cpp
#include "toon/toon.hpp"
#include <iostream>

int main() {

    toon::value config;

    config["window"]["width"] = 1280;
    config["window"]["height"] = 720;

    config["user"]["name"] = "zepyx";
    config["user"]["admin"] = true;

    std::cout << toon::dump(config);
}
```

Output:

```
window:
  width: 1280
  height: 720
user:
  name: zepyx
  admin: true
```

---

# Reading Files

Use `parse_file()` to read a `.toon` file.

```cpp
auto config = toon::parse_file("config.toon");
```

Example file:

```
window:
  width: 1280
  height: 720
```

Access values:

```cpp
int width = config["window"]["width"];
```

---

# Writing Files

Use `write_file()` to write or overwrite files.

```cpp
toon::write_file("config.toon", config);
```

This automatically serializes the `toon::value` into TOON format.

---

# Creating Values

`toon::value` can store several types.

Supported types:

* null
* boolean
* number
* string
* array
* object

Examples:

```cpp
toon::value v1 = true;
toon::value v2 = 42;
toon::value v3 = 3.14;
toon::value v4 = "hello";
```

---

# Objects

Objects behave like key-value maps.

```cpp
toon::value obj;

obj["name"] = "Alice";
obj["age"] = 21;
```

Result:

```
name: Alice
age: 21
```

---

# Arrays

Arrays store ordered values.

```cpp
toon::value list;

list.push_back("apple");
list.push_back("banana");
list.push_back("orange");
```

Result:

```
- apple
- banana
- orange
```

Access array elements:

```cpp
auto first = list[0];
```

---

# Parsing Text

You can parse TOON text directly.

```cpp
auto data = toon::parse(R"(

user:
  name: Alice
  admin: true

)");
```

---

# Serializing Data

Convert a value to TOON text using `dump()`.

```cpp
std::string text = toon::dump(data);
```

Example output:

```
user:
  name: Alice
  admin: true
```

---

# Editing Values

Values can be modified like nested objects.

```cpp
auto cfg = toon::parse_file("config.toon");

cfg["window"]["width"] = 1920;
cfg["window"]["height"] = 1080;

toon::write_file("config.toon", cfg);
```

---

# Utility Functions

## `read_file`

Reads a file into a string.

```cpp
std::string text = toon::read_file("file.toon");
```

## `write_file`

Writes text or a `toon::value` to a file.

```cpp
toon::write_file("file.toon", value);
```

---

# Error Handling

Parsing errors throw `std::runtime_error`.

Example:

```cpp
try {
    auto data = toon::parse_file("config.toon");
}
catch (std::runtime_error& e) {
    std::cout << e.what();
}
```

---

# Example Configuration

Example `.toon` configuration file:

```
app:
  name: ExampleApp
  version: 1.0

window:
  width: 1280
  height: 720
  fullscreen: false

user:
  name: zepyx
  admin: true
```

---

# Design Goals

`toon.hpp` focuses on:

* header-only simplicity
* minimal dependencies
* readable configuration files
* easy editing of structured data
* lightweight parsing

---

# License

Add your chosen license (MIT recommended) in the project repository.

---

# Repository

Example repository structure:

```
toon.hpp/
├─ include/
│  └─ toon/
│     └─ toon.hpp
├─ examples/
│  └─ basic.cpp
├─ README.md
└─ LICENSE
```

---
