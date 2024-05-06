# Custom-Game-Engine

## Overview
This repository provides a flexible data type, `Datum`, capable of dynamically adapting to various data types during runtime. Whether you're working with integers, floating-point numbers, strings, vectors, matrices, or RTTI.

## Features
- **Dynamic Typing**: `Datum` is designed to handle different data types seamlessly during runtime.
- **Versatile Usage**: From simple integers to complex matrices, `Datum` supports a wide range of data types commonly used in programming.
- **Easy Integration**: Integrate `Datum` into your projects effortlessly and enjoy its dynamic capabilities.

## Supported Data Types
- **int**: Integer data type.
- **float**: Floating-point number data type.
- **std::string**: Standard string data type.
- **glm::vec4**: Vector with four components data type from the GLM library.
- **glm::mat4**: 4x4 Matrix data type from the GLM library.
- **RTTI**: Run-Time Type Information data type for dynamic type identification.

## Getting Started
To start using `Datum` in your project, simply clone this repository and include the necessary files in your codebase. Refer to the provided examples and documentation for detailed usage instructions.

## Usage Example
```cpp
#include "datum.h"

int main() {
    // Create a Datum object
    Datum data;

    // Assign different data types
    data.set<int>(42);
    data.set<float>(3.14f);
    data.set<std::string>("Hello, Datum!");
    // ...

    // Retrieve and use the data
    int intValue = data.get<int>();
    float floatValue = data.get<float>();
    std::string stringValue = data.get<std::string>();
    // ...

    return 0;
}
```


---
