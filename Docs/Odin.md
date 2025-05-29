Ignis Usage Guide: Odin Version
============================================

1\. Initialize Ignis
--------------------
```cpp
ignis_init()
```
    
2\. Export functions
--------------------
```cpp
@export
hello_world :: proc IGNIS_CALL_CONVENTION () {
    fmt.printf("Hello from Odin module!\n");
}

@export
add :: proc IGNIS_CALL_CONVENTION (a: c.int, b: c.int) -> c.int {
    return a + b;
}
```

_Note:_ Use Odin's export to expose functions

3\. Load modules and call functions
-----------------------------------
```cpp
mod := ignis_load_mod("plugin.dll");
if mod == 0 {
    panic("Failed to load module\n");
        return;
    }
    
hello := cast(proc()) ignis_get_function("hello_world");
if hello != 0 {
    hello();
}

ignis_close_mod(mod);
```