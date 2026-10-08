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

Install a matching QuarkTex NG host DLL and `LIBS:minigl.library` on WinUAE (plus enabled **Allow native code**, RTG/P96, FPU host 80-bit, and JIT if applicable); see [QuarkTex NG](https://github.com/Sdursun/QuarkTexNG). Run `build/pc_minigl_smoke` inside AmigaOS. It paints one 256x256 RGBA checker texture in an application-owned MiniGL window and reports GL errors. This only checks the graphics API, **not PhotoCraft CPU compositing**.

For physical hardware, test separately against [PiStorm3D](https://github.com/SteffenHaeuser/MiniGL_Library_68k), whose documented GPU backend requires a Pi 4/CM4-class PiStorm setup.

See [the MiniGL spike](docs/miniGL-spike.md) and [the architecture draft](../../docs/adr/ADR-0001-amigaos3-port-seam.md).

## Current non-goals

Do not claim that the Rust engine builds for m68k, that an editor UI exists, that `mglCreateContextFromBitMap` works with an arbitrary bitmap, or that QuarkTex acceleration is faster than the existing RTG M2 fallback. These are open, testable hypotheses.
