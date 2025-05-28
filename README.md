Ignis - Single-header C++ Modding Framework
===========================================

Ignis is a lightweight, **single-header** cross-platform C++ modding framework designed to enable easy integration of mods with game executables. The game exports functions dynamically, and mods can call these game functions at runtime.

* * *

Features
--------

*   Single-header, no separate compilation needed
*   Minimal dependencies
*   Works on Windows and Linux
*   Dynamic symbol export from executable
*   Mod DLL/shared library loading and symbol resolution
*   Automatic function registration and retrieval
*   Supports custom calling conventions

* * *

How It Works
------------

The game executable registers exported functions via `IGNIS_EXPORT` macros.  
Mods register their own exported functions similarly.  
At runtime, the game loads mod shared libraries (`.so`/`.dll`), resolves symbols using Ignis, and calls mod functions.  
Ignis uses platform-specific dynamic loading (`dlopen`/`LoadLibrary`) and symbol lookup (`dlsym`/`GetProcAddress`).

* * *

Usage
-----

### In your game:

    IGNIS_EXPORT(void, game_log, const char* msg) {
        printf("Game: %s\n", msg);
    }

### In a mod:

    IGNIS_EXPORT(void, mod_log, const char* msg) {
        printf("Mod: %s\n", msg);
    }

* * *
