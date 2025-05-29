const std = @import("std");

const c = @cImport("#include \"../ignis.h\"");

const CallConv = switch (std.builtin.os.tag) {
    .windows => .Stdcall,
    else => .C,
};

const LibHandle = if (std.builtin.os.tag == .windows) std.os.windows.HMODULE else ?*anyopaque;


pub fn init() void {
    c.ignis_init();
}

pub fn registerFunction(name: [:0]const u8, ptr: ?*anyopaque) void {
    c.ignis_register_function(name.ptr, ptr);
}

pub fn getFunction(name: [:0]const u8) ?*anyopaque {
    return c.ignis_get_function(name.ptr);
}

pub fn loadMod(path: [:0]const u8) LibHandle {
    return c.ignis_load_mod(path.ptr);
}

pub fn getSymbol(lib: LibHandle, name: [:0]const u8) ?*anyopaque {
    return c.ignis_get_symbol(lib, name.ptr);
}

pub fn closeMod(lib: LibHandle) void {
    c.ignis_close_mod(lib);
}

