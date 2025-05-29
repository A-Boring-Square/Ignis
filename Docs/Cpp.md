Ignis Usage Guide: C++ Version
==============================

1\. Initialize Ignis
--------------------
```cpp
#include "ignis.hpp"
    
int main() {
    Ignis::init();
    // ...
}
```

2\. Export functions in module
------------------------------
```cpp
namespace Ignis {
    
IGNIS_EXPORT(void, hello_world, (void)) {
    std::cout << "Hello from C++ module!\n";
}
    
IGNIS_EXPORT(int, add, (int a, int b)) {
    return a + b;
}
    
} // namespace Ignis
```

_Note:_ Wrap parameters in parentheses.

3\. Load modules and call functions
-----------------------------------
```cpp
Ignis::LibHandle mod = Ignis::load_mod("plugin.dll");

if (!mod) {
    std::cerr << "Failed to load module\n";
    return 1;
}
    
using HelloFn = void (*)();
auto hello = reinterpret_cast(Ignis::get_function("hello_world"));
if (hello) hello();
    
Ignis::close_mod(mod);
```