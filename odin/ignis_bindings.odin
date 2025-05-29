package Igneis

when ODIN_OS == .Windows {
    when ODIN_DEBUG == true {
        when ODIN_ARCH == .amd64 {
            foreign import lib "../build/x64/Debug/ignis.lib"
        }
        when ODIN_ARCH == .i386 {
            foreign import lib "../build/x86/Debug/ignis.lib"
        }
        when ODIN_ARCH == .arm64 {
            foreign import lib "../build/arm64/Debug/ignis.lib"
        }
    }
    else {
        when ODIN_ARCH == .amd64 {
            foreign import lib "../build/x64/Debug/ignis.lib"
        }
        when ODIN_ARCH == .i386 {
            foreign import lib "../build/x86/Debug/ignis.lib"
        }
        when ODIN_ARCH == .arm64 {
            foreign import lib "../build/arm64/Debug/ignis.lib"
        }
    }
}
else {
    when ODIN_DEBUG == true {
        when ODIN_ARCH == .amd64 {
            foreign import lib "../build/x64/Debug/libignis.a"
        }
        when ODIN_ARCH == .i386 {
            foreign import lib "../build/x86/Debug/libignis.a"
        }
        when ODIN_ARCH == .arm64 {
            foreign import lib "../build/arm64/Debug/libignis.a"
        }
    }
    else {
        when ODIN_ARCH == .amd64 {
            foreign import lib "../build/x64/Debug/libignis.a"
        }
        when ODIN_ARCH == .i386 {
            foreign import lib "../build/x86/Debug/libignis.a"
        }
        when ODIN_ARCH == .arm64 {
            foreign import lib "../build/arm64/Debug/libignis.a"
        }
    }
}

foreign lib {
    ignis_init              :: proc() ---
    ignis_register_function :: proc(name: cstring, ptr: rawptr) ---
    ignis_get_function      :: proc(name: cstring) -> rawptr ---
}

