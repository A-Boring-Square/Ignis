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

namespace Ignis {

inline std::unordered_map<std::string, void*> function_registry;

inline void register_function(const char* name, void* ptr) {
    function_registry[name] = ptr;
}

inline void* get_function(const char* name) {
    auto it = function_registry.find(name);
    if (it != function_registry.end()) return it->second;
    return nullptr;
}

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
    return LoadLibraryA(path);
#else
    return dlopen(path, RTLD_NOW);
#endif
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


#define IGNIS_EXPORT(ret, name, ...) \
    extern "C" ret IGNIS_CALL name(__VA_ARGS__); \
    struct __ignis_reg_##name { \
        __ignis_reg_##name() { Ignis::register_function(#name, (void*)&name); } \
    }; \
    static __ignis_reg_##name __ignis_reg_instance_##name; \
    extern "C" ret IGNIS_CALL name(__VA_ARGS__)

} // namespace Ignis
