<!-- VD canonical story/test record. Source port record kept intact at ports/amigaos3/docs/tests/PF-TN-004.md. -->
# PF-TN-004 — Typed raster and flat-layer host / m68k parity

- Date: 2026-10-09
- Story: [PF-SP-002](../stories/PF-SP-002.md) (`in_progress`)
- Evidence: [GitHub Actions #37919197110](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37919197110)
- Code SHA: `7860de9972e6d2951a99b5c0b4dd2043331bf3b9`
- Overall scoped test result: **pass** (not a full port acceptance)
- Profiles: Linux C99 host GCC / ASan+UBSan; PhotoCraft original Rust crates; GCC/Bebbo 13.3 m68k HUNK; pinned `vamos`.
- Compiler image/tag/version: `stefanreinauer/amiga-gcc:gcc-v13.3-20260622`, strict `13.3.0` version/target guard. See run logs for resolved digest; product/default compiler is not decided.
- Central WinUAE automation: `disabled_by_process`, not used.

## Executed gates

| Test | Expected | Result |
|---|---|---|
| C99 host core/raster/document tests | 256×256 sparse COW, cross-boundary raw region, typed default, bottom-first flat layer order, snapshot lifetime | **PASS** |
| ASan + UBSan host tests | no observed memory/UB violation | **PASS** |
| Original PhotoCraft Rust crate tests | geom/color/raster current reference | **52 PASS** |
| Rust original `Surface::write_interleaved`/`to_interleaved` vs C99 typed `PcRaster` | byte-identical raw data for RGBA8, RGBA16, RGBA32F, GRAYA8 and CMYKA8, including cross-tile region and COW writes | **PASS** |
| m68k HUNK build | six test binaries per compiler optimization level, O0 and O2 | **12 HUNK files PASS** |
| m68k/vamos runtime | legacy core, tile conversion, new typed raster, flat document, two Rust oracles at O0/O2 | **12 executions PASS** |
| m68k typed C99 to original Rust oracle | O0 and O2 encoded-format differential | **PASS, byte-identical** |
| Real WinUAE/QuarkTex NG | native MiniGL SDK/driver path | not_run; PF-SP-001 |
| PiStorm3D, ReAction GUI, PSD/ICC/Compositor | real product integration | not_implemented / not_run |

## Actual CI log markers

```text
PASS: GCC13 O0 m68k/vamos vs original PhotoCraft Rust
PASS: GCC13 O2 m68k/vamos vs original PhotoCraft Rust
PASS: PhotoCraft encoded U8/U16/F32 raster regions/COW
PASS: PhotoCraft flat raster-layer document ownership/order/COW
PASS: GCC13 O0 typed U8/U16/F32 raster vs PhotoCraft Rust
PASS: GCC13 O2 typed U8/U16/F32 raster vs PhotoCraft Rust
```

## Limitations and open issues

- C99 typed API uses *encoded sample bytes*, native-endian U16/F32; does not implement numeric sample conversion, ICC profile transforms or PhotoCraft colour conversions.
- Flat raster-layer ownership is a scaffolding only: no groups, vector masks, selections, adjustments, PSD, compositing or history; no separate Rust `doc::Document` parity yet.
- A failed multi-tile bulk write can leave prior pixels modified; an atomic transaction design is pending ADR-0002 review and fault-injection tests.
- Prototype still uses O(number of tiles) linear lookup, not production-scale balanced map/hash indexing.
- Per-surface typed-format test handles a small fixture, not out-of-memory stress or massive images; some tiles can be 1 MiB for RGBA32F.
- ADR-0002 and ADR-0003 remain `proposed`. They do not constitute authorization of a shipping ABI, compiler version, or port completion.
- No MiniGL/QuarkTex or AmigaOS interactive testing was conducted in this run.

This technical milestone advances PF-SP-002 but does not close the Story.
