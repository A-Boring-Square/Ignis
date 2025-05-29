Ignis Usage Guide: Zig Version
==============================

1\. Initialize Ignis
--------------------
```cpp
ignis.init();
```
2\. Export functions
--------------------
```cpp
const std = @import("std");

export fn hello_world() void callconv(CallConv) {
        // Your code here
}
    
export fn add(a: i32, b: i32) i32 callconv(CallConv) {
        return a + b;
}
``` 

_Note: Use `export` and `callconv(CallConv)` for plugin exports._

3\. Load modules and call functions
-----------------------------------
```cpp
const mod = ignis.loadMod("plugin.dll");
if (mod == null) {
    std.debug.print("Failed to load module\n", .{});
    return;
}
    
const hello_fn = @ptrCast(fn() void, ignis.getFunction("hello_world"));
if (hello_fn != null) {
    hello_fn();
}
    
ignis.closeMod(mod);
```