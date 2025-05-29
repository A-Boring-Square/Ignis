#!/bin/sh
set -e

# Default values
ARCH="${1:-x86_64}"
MODE="${2:-release}"
BUILD_DIR="build/$ARCH/$MODE"
SRC="static_lib.c"
OUT_LIB="$BUILD_DIR/libignis.a"

# Create build directory
mkdir -p "$BUILD_DIR"

# Detect host architecture
HOST_ARCH=$(uname -m)
USE_CROSS=0

# Normalize ARCH and HOST_ARCH
norm_arch() {
    case "$1" in
        x86_64|amd64) echo x86_64 ;;
        i386|i686) echo x86 ;;
        armv7l|arm) echo arm32 ;;
        aarch64) echo arm64 ;;
        *) echo "$1" ;;
    esac
}
ARCH=$(norm_arch "$ARCH")
HOST_ARCH=$(norm_arch "$HOST_ARCH")

# Determine if cross-compilation is needed
[ "$ARCH" != "$HOST_ARCH" ] && USE_CROSS=1

# Set compiler
if [ "$USE_CROSS" -eq 1 ]; then
    case "$ARCH" in
        x86)      CC=i686-linux-gnu-gcc ;;
        x86_64)   CC=x86_64-linux-gnu-gcc ;;
        arm32)    CC=arm-linux-gnueabihf-gcc ;;
        arm64)    CC=aarch64-linux-gnu-gcc ;;
        *) echo "Unknown architecture: $ARCH" && exit 1 ;;
    esac
else
    CC=gcc
fi

# Set flags
CFLAGS="-Wall -Wextra -std=c99 -I. -c $SRC -o $BUILD_DIR/static_lib.o"
LFLAGS="-r -o $OUT_LIB"

[ "$MODE" = "release" ] && CFLAGS="$CFLAGS -O2"
[ "$MODE" = "debug" ] && CFLAGS="$CFLAGS -g -O0"

# Build
echo "Compiling with $CC for $ARCH ($MODE)"
$CC $CFLAGS
ar $LFLAGS "$BUILD_DIR/static_lib.o"

echo "Built $OUT_LIB"
