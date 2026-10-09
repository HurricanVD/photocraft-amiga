# PhotoCraft for AmigaOS 3.2 (experimental)

This directory is the native AmigaOS 3.2 port workspace for the upstream **PhotoCraft** editor in this fork. This is **not yet a working editor**. The initial code comprises a portable ARGB-to-RGBA staging primitive with tests and a standalone MiniGL graphics smoke program.

## Contract and direction

- **PhotoCraft is the product and algorithm reference.** The original Rust `crates/` and apps remain intact. Preserve PhotoCraft's document semantics, 256x256 tiles, masks, layers, history, CPU reference compositor, and command names when functionality is ported.
- **Dunkelkammer is a source of optional AmigaOS platform adapters**, NOT a replacement image engine. ReAction event/host patterns, RTG present code, Amiga file I/O, and selected UI patterns can be extracted following documented copyright-holder approval. No Dunkelkammer application code has been copied in this bootstrap.
- **MiniGL is a viewport backend, not wgpu.** Use CPU compositing first and upload RGBA8 results as textures. GPU filter parity and exotic blend modes are separate later work.
- **Runtime paths:** QuarkTex NG `minigl.library` in WinUAE; PiStorm3D's compatible shared-library API on supported Pi 4/CM4 PiStorm setups. Verify compatibility separately. Neither backend is a mandatory runtime dependency.
- **Fallback:** later implement RTG `WritePixelArray` plus off-screen cached blitting, based on the documented Dunkelkammer M2 approach.

## What is committed at bootstrap

```text
ports/amigaos3/
  AGENTS.md
  Makefile
  include/pc_tile_convert.h
  src/pc_tile_convert.c
  src/pc_minigl_smoke.c
  tests/pc_tile_convert_test.c
  docs/{architecture,development-process,bootstrap-report,
        project-metadata,ip-provenance-register,
        miniGL-spike,backlog-seed,os-deviation-log}...
```

## Portable test (no AmigaOS SDK required)

```sh
make -C ports/amigaos3 host-test
```

This checks ARGB32-to-RGBA8 channel order (including alpha), padding/stride handling, transparent pixels, and invalid inputs.

## MiniGL cross-build (experimental; not yet cross-tested)

Provide an AmigaOS/m68k GCC with the appropriate libraries, plus the separately obtained official MiniGL shared-library SDK. The default `MINIGL_INCLUDE` and `MINIGL_IMPORT_LIB` are only example SDK layouts; override them for the version actually installed.

```sh
make -C ports/amigaos3 amiga-smoke \
  CC68K=m68k-amigaos-gcc \
  MINIGL_INCLUDE=/path/to/minigl/dev/include \
  MINIGL_IMPORT_LIB=/path/to/libminigl.a
```

Install a matching QuarkTex NG host DLL and `LIBS:minigl.library` on WinUAE (plus enabled **Allow native code**, RTG/P96, FPU host 80-bit, and JIT if applicable); see [QuarkTex NG](https://github.com/Sdursun/QuarkTexNG). Run `build/pc_minigl_smoke` inside AmigaOS. It paints a 256x256 RGBA checker texture in a MiniGL test window, checks sample pixels with glReadPixels and validates a 16x16 glTexSubImage2D update. An API PASS does not establish ReAction integration or PhotoCraft document rendering. This only checks the graphics API, **not PhotoCraft CPU compositing**.

For physical hardware, test separately against [PiStorm3D](https://github.com/SteffenHaeuser/MiniGL_Library_68k), whose documented GPU backend requires a Pi 4/CM4-class PiStorm setup.

See [the MiniGL spike](docs/miniGL-spike.md) and [the accepted architecture decision](../../docs/adr/ADR-0001-amigaos3-port-seam.md).

## Current non-goals

Do not claim that the Rust engine builds for m68k, that an editor UI exists, that `mglCreateContextFromBitMap` works with an arbitrary bitmap, or that QuarkTex acceleration is faster than the existing RTG M2 fallback. These are open, testable hypotheses.

## PF-SP-001

The active technical spike is [PF-SP-001](docs/stories/PF-SP-001.md), with [PF-TN-001](docs/tests/PF-TN-001.md) separating host evidence from blocked native tests. The host-side staging test has been executed; a working native m68k/QuarkTex NG runtime has **not** yet been verified.

## Host-first PhotoCraft CPU core (PF-SP-002)

A first independent C99 port of upstream geometry, PixelFormat metadata and a
sparse 256x256 RGBA8 raster surface lives in `include/pc_core.h` and
`src/pc_core.c`. It supports negative document coordinates, default pixels,
COW surface clones, RGBA8 pixel edits and pruning default-only tiles.

```sh
make -C ports/amigaos3 host-test
make -C ports/amigaos3 host-core-sanitize   # Linux/clang/gcc with ASan+UBSan
```

The test cases are taken from the observable expectations in upstream Rust
`crates/geom/src/lib.rs`, `crates/color/src/lib.rs` and
`crates/raster/src/lib.rs`. **This is a narrow host-test implementation**, not
a complete format-generic Rust `Surface` port or a `Document` implementation.
In particular the initial C surface is RGBA8-only, uses a linear tile lookup,
is single-threaded, and has no persistence, fill-rectangle, masks or blend math.
Rust-native parity execution for the enumerated differential fixtures **passed** in GitHub Actions run 37909489232; broader semantic parity and m68k target builds remain follow-up gates.

### Rust-original parity on GitHub Actions

`rust-original-parity` runs actual upstream PhotoCraft Rust library tests for
`geom`, `color`, and `raster`, and compares a deterministic independent C99
oracle with a Rust oracle backed by the original crates. It is separate from
the Linux C99/sanitizer job. See `tests/core_oracle.c` and
`tests/rust_oracle/`.

This covers ONLY the enumerated sample vectors; full equivalence, full pixel
format support and real AmigaOS runtime still require later testing.

**Verified host Rust baseline (2026-10-09):** 52 original Rust tests passed
and the independent Rust/C99 fixture outputs matched byte-for-byte on
[GitHub Actions run 37909489232](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37909489232).
This is a limited host parity milestone, not an AmigaOS executable or full editor.

## GCC13.3 and vamos tests (PF-SP-003)

The optional m68k compatibility lane is modeled on the **GCC13.3 opt-in**
provider contract in `HurricanVD/vxplatform` ADR-0014. vxplatform's default
remains GCC16.1.1b; this fork does **not** change that default.

```sh
make -C ports/amigaos3 gcc13-build-tests \
  GCC_BEBBO_ROOT=/absolute/path/to/amiga-gcc-13.3
make -C ports/amigaos3 gcc13-check-hunks
make -C ports/amigaos3 gcc13-vamos-tests \
  RUST_ORACLE=build/oracle-rust.txt
```

The Rust oracle text is produced by the `rust-original-parity` Actions job
and is downloaded automatically by the GCC13 job. Local runs need a genuine
Rust original-oracle output file; the C oracle is not a substitute. The
GCC13 lane requires neither the MiniGL SDK nor WinUAE.

The CI compiler image is tag-pinned and its resolved digest is logged.
Compiler target and exact 13.3.0 version are checked fail-closed before
building. Re-run evidence is tracked under
[PF-TN-003](docs/tests/PF-TN-003.md). A running/passing CI job is required
before any m68k test is called successful.
