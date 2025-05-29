#!/bin/sh

# Default args
PLATFORM="${1:-x64}"
CONFIG="${2:-Release}"

# Select compiler
if command -v clang >/dev/null 2>&1; then
    CC=clang
    AR=llvm-ar
else
    CC=gcc
    AR=ar
fi

# Set flags
CFLAGS="-Wall -Wextra -std=c11"
LDFLAGS=""
OUTDIR="build/$PLATFORM/$CONFIG"

case "$CONFIG" in
    Debug)
        CFLAGS="$CFLAGS -g -O0"
        ;;
    Release)
        CFLAGS="$CFLAGS -O2"
        ;;
    *)
        echo "Unsupported config: $CONFIG"
        exit 1
        ;;
esac

# Platform-specific flags
case "$PLATFORM" in
    x86)
        CFLAGS="$CFLAGS -m32"
        ;;
    x64)
        CFLAGS="$CFLAGS -m64"
        ;;
    arm32)
        CC=arm-linux-gnueabihf-gcc
        AR=arm-linux-gnueabihf-ar
        ;;
    arm64)
        CC=aarch64-linux-gnu-gcc
        AR=aarch64-linux-gnu-ar
        ;;
    *)
        echo "Unsupported platform: $PLATFORM"
        exit 1
        ;;
esac

# Check if cross-compiler exists
if ! command -v "$CC" >/dev/null 2>&1; then
    echo "Compiler $CC not found."
    exit 1
fi

# Create output dir
mkdir -p "$OUTDIR"

# Build
OBJ="$OUTDIR/static_lib.o"
LIB="$OUTDIR/libignis.a"

$CC -c $CFLAGS static_lib.c -o "$OBJ" || { echo "Compilation failed."; exit 1; }
$AR rcs "$LIB" "$OBJ" || { echo "Library creation failed."; exit 1; }

echo "Build successful: $LIB"
