#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#ifdef _WIN32
  #include <windows.h>
  using LibHandle = HMODULE;
  #define IGNIS_CALL __stdcall
#else
  #include <dlfcn.h>
  using LibHandle = void*;
  #define IGNIS_CALL
#endif

// --- Version Macros ---
#define IGNIS_VERSION_MAJOR 1
#define IGNIS_VERSION_MINOR 0
#define IGNIS_VERSION_PATCH 0

#define IGNIS_STR_HELPER(x) #x
#define IGNIS_STR(x) IGNIS_STR_HELPER(x)

#define IGNIS_VERSION_STRING \
    IGNIS_STR(IGNIS_VERSION_MAJOR) "." IGNIS_STR(IGNIS_VERSION_MINOR) "." IGNIS_STR(IGNIS_VERSION_PATCH)

namespace Ignis {

struct Version {
    int major;
    int minor;
    int patch;
};

inline Version get_version() {
    return { IGNIS_VERSION_MAJOR, IGNIS_VERSION_MINOR, IGNIS_VERSION_PATCH };
}

inline const char* get_version_string() {
    return IGNIS_VERSION_STRING;
}

// --- Function registry ---
inline std::unordered_map<std::string, void*> function_registry;

inline void register_function(const char* name, void* ptr) {
    function_registry[name] = ptr;
}

inline void* get_function(const char* name) {
    auto it = function_registry.find(name);
    return (it != function_registry.end()) ? it->second : nullptr;
}

// --- Dynamic loading ---
inline LibHandle exe_handle = nullptr;

inline void init() {
    function_registry.clear();
#ifdef _WIN32
    exe_handle = GetModuleHandle(nullptr);
#else
    exe_handle = dlopen(nullptr, RTLD_NOW);
#endif
}

inline LibHandle load_mod(const char* path) {
#ifdef _WIN32
    LibHandle lib = LoadLibraryA(path);
#else
    LibHandle lib = dlopen(path, RTLD_NOW);
#endif
    if (!lib) return nullptr;

    using VersionFunc = Version(*)();
    VersionFunc mod_ver_func = (VersionFunc)get_symbol(lib, "ignis_module_version");
    if (!mod_ver_func) {
        close_mod(lib);
        return nullptr;
    }

    Version mod_ver = mod_ver_func();
    if (mod_ver.major != IGNIS_VERSION_MAJOR) {
        close_mod(lib);
        return nullptr;
    }

    return lib;
}

inline void* get_symbol(LibHandle lib, const char* name) {
#ifdef _WIN32
    return (void*)GetProcAddress(lib, name);
#else
    return dlsym(lib, name);
#endif
}

inline void close_mod(LibHandle lib) {
#ifdef _WIN32
    if (lib) FreeLibrary(lib);
#else
    if (lib) dlclose(lib);
#endif
}

// --- Auto-export version function once ---
#ifndef IGNIS_VERSION_FUNCTION_DEFINED
#define IGNIS_VERSION_FUNCTION_DEFINED
extern "C" Version IGNIS_CALL ignis_module_version() {
    return get_version();
}
#endif

// --- Export Macro ---
#define IGNIS_EXPORT(ret, name, ...) \
    extern "C" ret IGNIS_CALL name(__VA_ARGS__); \
    struct __ignis_reg_##name { \
        __ignis_reg_##name() { Ignis::register_function(#name, (void*)&name); } \
    }; \
    static __ignis_reg_##name __ignis_reg_instance_##name; \
    extern "C" ret IGNIS_CALL name(__VA_ARGS__)

} // namespace Ignis
