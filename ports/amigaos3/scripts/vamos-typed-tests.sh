#!/bin/sh
# PF-SP-002: additional compiler/runtime tests and original-Rust typed API parity.
set -eu
: "${VAMOS:=vamos}"
: "${M68K_BUILD_DIR:=build/m68k-gcc13}"
: "${RUST_ORACLE_TYPED:?Set RUST_ORACLE_TYPED to original PhotoCraft Rust output}"
test -f "$RUST_ORACLE_TYPED" || { echo "FAIL: missing original Rust typed oracle" >&2; exit 2; }
command -v "$VAMOS" >/dev/null 2>&1 || { echo "BLOCKED: vamos unavailable" >&2; exit 2; }
for opt in O0 O2; do
  dir="$M68K_BUILD_DIR/$opt"
  mkdir -p "$dir/logs"
  for test_name in pc_raster_test pc_document_test; do
    log="$dir/logs/$test_name.log"
    "$VAMOS" -S -C 20 -m 8192 -s 128 "$dir/$test_name" > "$log" 2>&1 || {
      cat "$log"; echo "FAIL: $opt $test_name" >&2; exit 1;
    }
    cat "$log"
    case "$test_name" in
      pc_raster_test)
        grep -q '^PASS: PhotoCraft encoded U8/U16/F32 raster regions/COW$' "$log";;
      pc_document_test)
        grep -q '^PASS: PhotoCraft flat raster-layer document ownership/order/COW/opacity/shift$' "$log";;
    esac || { echo "FAIL: missing $opt $test_name pass marker" >&2; exit 1; }
  done
  log="$dir/logs/typed_oracle.log"
  "$VAMOS" -S -C 20 -m 8192 -s 128 "$dir/typed_oracle" > "$log" 2>&1 || {
    cat "$log"; echo "FAIL: $opt typed oracle" >&2; exit 1;
  }
  diff -u "$RUST_ORACLE_TYPED" "$log" || {
    echo "FAIL: $opt typed bytes differ from original PhotoCraft Rust" >&2
    exit 1
  }
  echo "PASS: GCC13 $opt typed U8/U16/F32 raster vs PhotoCraft Rust"
done
