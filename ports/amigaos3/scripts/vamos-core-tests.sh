#!/bin/sh
# PF-SP-003: execute HUNK tests in vamos and diff target output with Rust.
set -eu
: "${VAMOS:=vamos}"
: "${M68K_BUILD_DIR:=build/m68k-gcc13}"
: "${RUST_ORACLE:?Set RUST_ORACLE to the original Rust output text}"
test -f "$RUST_ORACLE" || { echo "FAIL: Rust oracle file missing" >&2; exit 2; }
command -v "$VAMOS" >/dev/null 2>&1 || {
  echo "BLOCKED: vamos executable not found" >&2; exit 2;
}
for opt in O0 O2; do
  dir="$M68K_BUILD_DIR/$opt"
  mkdir -p "$dir/logs"
  for test_name in pc_core_test pc_tile_convert_test; do
    log="$dir/logs/$test_name.log"
    echo "vamos: $opt $test_name"
    "$VAMOS" -S -C 20 -m 8192 -s 128 "$dir/$test_name" > "$log" 2>&1 || {
      cat "$log"; echo "FAIL: vamos $opt $test_name" >&2; exit 1;
    }
    cat "$log"
    case "$test_name" in
      pc_core_test)
        grep -q '^PASS: PhotoCraft host core parity fixtures (geom/color/raster RGBA8)$' "$log" ;;
      pc_tile_convert_test)
        grep -q '^PASS: portable ARGB/RGBA staging checks$' "$log" ;;
    esac || { echo "FAIL: expected PASS line missing" >&2; exit 1; }
  done
  log="$dir/logs/core_oracle.log"
  "$VAMOS" -S -C 20 -m 8192 -s 128 "$dir/core_oracle" > "$log" 2>&1 || {
    cat "$log"; echo "FAIL: vamos $opt oracle" >&2; exit 1;
  }
  diff -u "$RUST_ORACLE" "$log" || {
    echo "FAIL: m68k C oracle differs from original PhotoCraft Rust ($opt)" >&2
    exit 1
  }
  echo "PASS: GCC13 $opt m68k/vamos vs original PhotoCraft Rust"
done
