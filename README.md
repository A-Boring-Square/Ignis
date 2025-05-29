Ignis - Single-header C/C++ Modding Framework
=============================================

Ignis is a lightweight, **single-header**, cross-platform C/C++ modding framework designed to enable easy integration of mods with game executables. The game exports functions dynamically, and mods can call these game functions at runtime using a portable symbol registry.

* * *

Features
--------

*   **Header-only:** No separate compilation needed
*   **C and C++ support:** Pure C API included
*   Minimal dependencies
*   Works on Windows and Linux
*   Dynamic symbol export from the executable
*   Mod `.dll` / `.so` loading and symbol resolution
*   Automatic function registration and retrieval
*   Supports custom calling conventions

* * *

How It Works
------------

In both C and C++, the game registers exported functions using the `IGNIS_EXPORT` macro. Mods do the same to expose functions back to the game.

At runtime, the game loads mod shared libraries (`.so` / `.dll`), resolves exported symbols using `ignis_get_symbol`, and calls mod functions. Ignis uses platform-specific APIs like `dlopen` / `LoadLibrary` for loading and `dlsym` / `GetProcAddress` for symbol lookup.

The function registry allows both game and mod code to dynamically retrieve and call functions using string names.

* * *

Usage
-----

### In your C or C++ game:
```cpp
#include "ignis.hpp" // For C
#include "ignis.hpp" // For C++
    
IGNIS_EXPORT(void, game_log, const char* msg) {
    printf("Game: %s\n", msg);
}
```

### In a C or C++ mod:
```cpp
#include "ignis.hpp" // For C
#include "ignis.hpp" // For C++
    
IGNIS_EXPORT(void, mod_log, const char* msg) {
    printf("Mod: %s\n", msg);
}
```