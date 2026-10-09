#!/bin/sh
# PF-SP-003: fail-closed opt-in GCC/Bebbo 13.3.0 toolchain guard.
set -eu
: "${GCC_BEBBO_ROOT:?Set explicit GCC_BEBBO_ROOT (same provider contract as vxplatform)}"
case "$GCC_BEBBO_ROOT" in
  /*) ;;
  *) echo "FAIL: GCC_BEBBO_ROOT must be an absolute path" >&2; exit 2 ;;
esac
test -d "$GCC_BEBBO_ROOT" || { echo "FAIL: GCC13 root missing" >&2; exit 2; }
root=$(cd "$GCC_BEBBO_ROOT" && pwd -P)
cc="$root/bin/m68k-amigaos-gcc"
cxx="$root/bin/m68k-amigaos-g++"
test -x "$cc" && test -x "$cxx" || {
  echo "FAIL: GCC13 GCC and G++ must be in one explicit root" >&2
  exit 2
}
case "$(realpath "$cc")" in "$root"/*) ;; *)
  echo "FAIL: GCC13 compiler escapes the configured root" >&2; exit 2 ;;
esac
for tool in "$cc" "$cxx"; do
  ver=$("$tool" -dumpfullversion -dumpversion)
  case "$ver" in 13.3.0*) ;; *)
    echo "FAIL: $tool reports $ver instead of GCC/Bebbo 13.3.0" >&2; exit 2 ;;
  esac
  target=$("$tool" -dumpmachine)
  case "$target" in m68k-amigaos*) ;; *)
    echo "FAIL: wrong target $target" >&2; exit 2 ;;
  esac
done
libgcc=$("$cc" -print-libgcc-file-name)
test -f "$libgcc" || { echo "FAIL: missing libgcc: $libgcc" >&2; exit 2; }
case "$(realpath "$libgcc")" in "$root"/*) ;; *)
  echo "FAIL: libgcc resolves outside selected GCC13 root" >&2; exit 2 ;;
esac
printf 'PASS: GCC/Bebbo 13.3.0 toolchain root=%s\n' "$root"
"$cc" --version | head -1
printf 'target=%s\nlibgcc=%s\n' "$target" "$libgcc"
