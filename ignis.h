#pragma once
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

#ifdef __cplusplus
extern "C" {
#endif

#define IGNIS_VERSION_MAJOR 1
#define IGNIS_VERSION_MINOR 0
#define IGNIS_VERSION_PATCH 0

#define IGNIS_STR_HELPER(x) #x
#define IGNIS_STR(x) IGNIS_STR_HELPER(x)

#define IGNIS_VERSION_STRING \
    IGNIS_STR(IGNIS_VERSION_MAJOR) "." IGNIS_STR(IGNIS_VERSION_MINOR) "." IGNIS_STR(IGNIS_VERSION_PATCH)

typedef struct {
    int major;
    int minor;
    int patch;
} Ignis_Version;

const char* ignis_get_version_string(void);
Ignis_Version ignis_get_version(void);

void ignis_init(void);
void ignis_register_function(const char* name, void* ptr);
void* ignis_get_function(const char* name);

Ignis_LibHandle ignis_load_mod(const char* path);
void* ignis_get_symbol(Ignis_LibHandle lib, const char* name);
void ignis_close_mod(Ignis_LibHandle lib);

typedef Ignis_Version (*Ignis_ModuleVersionFunc)(void);


#define IGNIS_EXPORT(ret, name, ...) \
    ret IGNIS_CALL name(__VA_ARGS__); \
    static void* _ignis_##name##_ptr = (ignis_register_function(#name, (void*)&name), (void*)1); \
    ret IGNIS_CALL name(__VA_ARGS__)

#ifdef IGNIS_IMPLEMENTATION

// --- Registry ---
#define IGNIS_MAX_FUNCTIONS 256
static void* ignis_function_ptrs[IGNIS_MAX_FUNCTIONS];
static const char* ignis_function_names[IGNIS_MAX_FUNCTIONS];
static int ignis_function_count = 0;

void ignis_register_function(const char* name, void* ptr) {
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

void* ignis_get_function(const char* name) {
    for (int i = 0; i < ignis_function_count; ++i) {
        if (strcmp(ignis_function_names[i], name) == 0) {
            return ignis_function_ptrs[i];
        }
    }
    return 0;
}

// --- Version Info ---
const char* ignis_get_version_string(void) {
    return IGNIS_VERSION_STRING;
}

Ignis_Version ignis_get_version(void) {
    Ignis_Version v = { IGNIS_VERSION_MAJOR, IGNIS_VERSION_MINOR, IGNIS_VERSION_PATCH };
    return v;
}

// --- Library loader ---
static Ignis_LibHandle ignis_exe_handle = 0;

void ignis_init(void) {
    ignis_function_count = 0;
#ifdef _WIN32
    ignis_exe_handle = GetModuleHandleA(0);
#else
    ignis_exe_handle = dlopen(0, RTLD_NOW);
#endif
}

Ignis_LibHandle ignis_load_mod(const char* path) {
    Ignis_LibHandle lib = 0;
#ifdef _WIN32
    lib = LoadLibraryA(path);
#else
    lib = dlopen(path, RTLD_NOW);
#endif
    if (!lib) return 0;

    Ignis_ModuleVersionFunc get_ver = (Ignis_ModuleVersionFunc)ignis_get_symbol(lib, "ignis_module_version");
    if (!get_ver) {
        ignis_close_mod(lib);
        return 0;
    }

    Ignis_Version mod_ver = get_ver();
    if (mod_ver.major != IGNIS_VERSION_MAJOR) {
        ignis_close_mod(lib);
        return 0;
    }

    return lib;
}

void* ignis_get_symbol(Ignis_LibHandle lib, const char* name) {
#ifdef _WIN32
    return (void*)GetProcAddress(lib, name);
#else
    return dlsym(lib, name);
#endif
}

void ignis_close_mod(Ignis_LibHandle lib) {
#ifdef _WIN32
    if (lib) FreeLibrary(lib);
#else
    if (lib) dlclose(lib);
#endif
}

#endif // IGNIS_IMPLEMENTATION

#ifdef __cplusplus
}
#endif
