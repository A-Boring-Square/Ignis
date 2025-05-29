package Igneis

import "core:c"
import "core:sys/windows"

when ODIN_OS == .Windows {
    when ODIN_DEBUG == true {
        when ODIN_ARCH == .amd64 {
            foreign import Ignis "../../build/x64/Debug/ignis.lib"
        }
        when ODIN_ARCH == .i386 {
            foreign import Ignis "../../build/x86/Debug/ignis.lib"
        }
        when ODIN_ARCH == .arm64 {
            foreign import Ignis "../../build/arm64/Debug/ignis.lib"
        }
    }
    else {
        when ODIN_ARCH == .amd64 {
            foreign import Ignis "../../build/x64/Debug/ignis.lib"
        }
        when ODIN_ARCH == .i386 {
            foreign import Ignis "../../build/x86/Debug/ignis.lib"
        }
        when ODIN_ARCH == .arm64 {
            foreign import Ignis "../../build/arm64/Debug/ignis.lib"
        }
    }
}
else {
    when ODIN_DEBUG == true {
        when ODIN_ARCH == .amd64 {
            foreign import Ignis "../../build/x64/Debug/libignis.a"
        }
        when ODIN_ARCH == .i386 {
            foreign import Ignis "../../build/x86/Debug/libignis.a"
        }
        when ODIN_ARCH == .arm64 {
            foreign import Ignis "../../build/arm64/Debug/libignis.a"
        }
    }
    else {
        when ODIN_ARCH == .amd64 {
            foreign import Ignis "../../build/x64/Debug/libignis.a"
        }
        when ODIN_ARCH == .i386 {
            foreign import Ignis "../../build/x86/Debug/libignis.a"
        }
        when ODIN_ARCH == .arm64 {
            foreign import Ignis "../../build/arm64/Debug/libignis.a"
        }
    }
}

Ignis_Version :: struct {
   major: c.int,
   minor: c.int,
   patch: c.int
}

when ODIN_OS == .Windows {
    Ignis_LibHandle :: windows.HINSTANCE
}
else {
    Ignis_LibHandle :: rawptr
}

foreign Ignis {
    ignis_init               :: proc() ---
    ignis_register_function  :: proc(name: cstring, ptr: rawptr) ---
    ignis_get_function       :: proc(name: cstring) -> rawptr ---
    ignis_get_version_string :: proc() -> cstring ---
    ignis_get_version        :: proc() -> Ignis_Version ---
    ignis_load_mod           :: proc(path: cstring) -> Ignis_LibHandle ---
    ignis_get_symbol         :: proc(lib: Ignis_LibHandle, name: cstring) -> rawptr ---
    ignis_close_mod          :: proc(lib: Ignis_LibHandle) ---
}



IGNIS_CALL_CONVENTION :: "system"