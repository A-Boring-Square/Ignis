Ignis Usage Guide: C Version
============================

1\. Initialize Ignis
--------------------
```c
#include "ignis.h"
    
int main() {
    ignis_init();
    // ...
}
```
    
    

2\. Export functions in your module
-----------------------------------
```c
IGNIS_EXPORT(void, hello_world, (void)) {
    printf("Hello from C module!\n");
}
    
IGNIS_EXPORT(int, add, (int a, int b)) {
    return a + b;
}
```

_Note:_ Always wrap parameters in parentheses.

3\. Load modules and call functions
-----------------------------------
```c
    Ignis_LibHandle mod = ignis_load_mod("plugin.dll");
    if (!mod) {
        fprintf(stderr, "Failed to load module\n");
        return 1;
    }
    
    typedef void (*HelloFn)(void);
    HelloFn hello = (HelloFn)ignis_get_function("hello_world");
    if (hello) hello();
    
    ignis_close_mod(mod);
```