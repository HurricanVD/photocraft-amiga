# PF-SP-002 — Host-first PhotoCraft raster/core port

- Story ID: PF-SP-002
- Type: SP (technical spike)
- Status: in_progress (the current C99 RGBA8/geometry subset has host, Rust-oracle and GCC13.3/vamos O0/O2 evidence; full format-generic raster/document port remains open)
- Priority: P1
- Date: 2026-10-09
- Architecture: accepted ADR-0001
- Test evidence: PF-TN-002

## Purpose

Start from the *original* PhotoCraft source semantics rather than adapting
Dunkelkammer DkDocument. Implement a testable C99 subset with no AmigaOS GUI,
MiniGL or SDK dependency.

Reference:
- `crates/geom/src/lib.rs`: 256x256 tile size, half-open Rect, signed
  negative coordinate `div_euclid`, intersect/union/saturating translation.
- `crates/color/src/lib.rs`: pixel format metadata for RGB/Gray/CMYK,
  U8/U16/F32 storage sizes.
- `crates/raster/src/lib.rs`: sparse RGBA8 pixels, default pixel, Arc-like
  COW clones, prune and tile count.

## Scope

- C99 `pc_core.h` / `pc_core.c` with explicit owner and error returns.
- No changes to the Rust crates or cargo workspace.
- Host test fixtures derived from upstream Rust test cases and negative tile
  boundary grids.
- Existing `host-test` runs the staging test and first core test.
- Optional ASan/UBSan target for memory and signed arithmetic diagnostics.
- No third-party or proprietary Dunkelkammer code copied.

## Gates not yet satisfied

This code is NOT the complete PhotoCraft document model or its image editor.
Raster support is RGBA8-only, with a linear lookup prototype rather than a
production ordered map. Missing: full format-generic U16/F32/CMYK/Gray
raster support, Fill/Region and selection data, Layer/Group hierarchy,
blend/compositor algorithms, commands/undo, persistence, reference Rust
binary-to-C conformance for the broader surface API, and complete AmigaOS application build.

The fixtures are based on the original PhotoCraft Rust public APIs. Actual Rust tests (52 passing) and Rust/C99 differential output were executed in GitHub Actions; the narrower C99 kernel was also built with GCC/Bebbo 13.3.0 and run under vamos at O0 and O2. These do not prove the format-generic raster API, native PhotoCraft Document/Engine, WinUAE or graphics backend. PF-SP-002 remains in_progress for these broader requirements.

## Executed Rust-original evidence (2026-10-09)

The original unchanged geom/color/raster crates passed 52 Rust library tests in GitHub Actions, followed by a byte-identical comparison of the independent Rust API oracle against C99 for explicit geometry/format/RGBA8 sparse-COW vectors. See PF-TN-002 and [CI run](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37909489232). Keep the story **in_progress**, since the 68k runtime and broader format/doc contracts are not established.

## New native-slice evidence (2026-10-09)

[PF-TN-003](../tests/PF-TN-003.md) and [CI #37915607402](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37915607402) verify the already-implemented RGBA8 C99 subset on an emulated m68k CPU, including byte-identical Rust oracle fixtures. This resolves the previously missing *subset* target smoke only; it does not close PF-SP-002's broader document/image processing implementation.
