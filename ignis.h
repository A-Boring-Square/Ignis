#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#ifdef _WIN32
  #include <windows.h>
  typedef HMODULE Ignis_LibHandle;
  #define IGNIS_CALL __stdcall
#else
  #include <dlfcn.h>
  typedef void* Ignis_LibHandle;
  #define IGNIS_CALL
#endif

// --- Registry ---
#define IGNIS_MAX_FUNCTIONS 256

static void* ignis_function_ptrs[IGNIS_MAX_FUNCTIONS];
static const char* ignis_function_names[IGNIS_MAX_FUNCTIONS];
static int ignis_function_count = 0;

static void ignis_register_function(const char* name, void* ptr) {
    for (int i = 0; i < ignis_function_count; ++i) {
        if (strcmp(ignis_function_names[i], name) == 0) {
            ignis_function_ptrs[i] = ptr;
            return;
        }
    }
    if (ignis_function_count < IGNIS_MAX_FUNCTIONS) {
        ignis_function_names[ignis_function_count] = name;
        ignis_function_ptrs[ignis_function_count] = ptr;
        ++ignis_function_count;
    }
}

static void* ignis_get_function(const char* name) {
    for (int i = 0; i < ignis_function_count; ++i) {
        if (strcmp(ignis_function_names[i], name) == 0) {
            return ignis_function_ptrs[i];
        }
    }
    return 0;
}

// --- Library loader ---

static Ignis_LibHandle ignis_exe_handle = 0;

static void ignis_init(void) {
    ignis_function_count = 0;
#ifdef _WIN32
    ignis_exe_handle = GetModuleHandleA(0);
#else
    ignis_exe_handle = dlopen(0, RTLD_NOW);
#endif
}

static Ignis_LibHandle ignis_load_mod(const char* path) {
#ifdef _WIN32
    return LoadLibraryA(path);
#else
    return dlopen(path, RTLD_NOW);
#endif
}

static void* ignis_get_symbol(Ignis_LibHandle lib, const char* name) {
#ifdef _WIN32
    return (void*)GetProcAddress(lib, name);
#else
    return dlsym(lib, name);
#endif
}

static void ignis_close_mod(Ignis_LibHandle lib) {
#ifdef _WIN32
    if (lib) FreeLibrary(lib);
#else
    if (lib) dlclose(lib);
#endif
}

// --- Export helper (C only) ---
#define IGNIS_EXPORT(ret, name, ...) \
    ret IGNIS_CALL name(__VA_ARGS__); \
    static void* _ignis_##name##_ptr = (ignis_register_function(#name, (void*)&name), (void*)1); \
    ret IGNIS_CALL name(__VA_ARGS__)

#ifdef __cplusplus
}
#endif
