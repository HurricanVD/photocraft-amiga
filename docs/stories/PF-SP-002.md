<!-- VD canonical story/test record. Source port record kept intact at ports/amigaos3/docs/stories/PF-SP-002.md. -->
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

The original unchanged geom/color/raster crates passed 52 Rust library tests in GitHub Actions, followed by a byte-identical comparison of the independent Rust API oracle against C99 for explicit geometry/format/RGBA8 sparse-COW vectors. See PF-TN-002 and [CI run](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37909489232). Keep the story **in_progress** because broad format/document contracts and their native application/runtime are not established; the RGBA8 68k subset was subsequently demonstrated in PF-SP-003.

## New native-slice evidence (2026-10-09)

[PF-TN-003](../tests/done/PF-TN-003.md) and [CI #37915607402](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37915607402) verify the already-implemented RGBA8 C99 subset on an emulated m68k CPU, including byte-identical Rust oracle fixtures. This resolves the previously missing *subset* target smoke only; it does not close PF-SP-002's broader document/image processing implementation.

## 2026-10-09 typed raster and flat-layer exploration (NOT production ABI)

The experimental `PcRaster` under `include/pc_raster.h`/`src/pc_raster.c`
supports raw encoded pixel sizes for U8, U16 and F32, RGB/Gray/CMYK/other
PhotoCraft channel layouts, default pixels, sparse 256x256 COW tiles,
and bounded rectangular raw `read_region`/`write_region`. It deliberately
does **not** claim complete format conversion, ICC, floating-point rendering,
atomic multi-tile edits on OOM or production search/map performance.

The limited flat raster-layer `PcDocument` prototype under
`pc_document.h/.c` preserves bottom-first ordering, unique IDs, names,
visibility and clone ownership. It is **not** the PhotoCraft
`doc::Document` (groups, masks, effects, history, PSD not present).

This exploratory work is permitted under the accepted ADR-0001, and its
semantic/verification boundaries now follow **accepted** [ADR-0002](../adr/ADR-0002-photocraft-core-and-memory-contract.md)
and [ADR-0003](../adr/ADR-0003-amigaos-toolchain-and-verification.md)
(approved by the project owner on 2026-10-09). No stable public C ABI or product compiler is approved.
Typed interleaved-byte conformance is exercised against the real Rust
`Surface::write_interleaved`/`to_interleaved` API. New host, sanitizer,
GCC13 m68k and vamos tests are additional evidence *only once actually green*.

## Verified typed-format experiment and first layer scaffold (2026-10-09)

Implemented `pc_raster.h/.c` for sparse encoded interleaved bytes across U8/U16/F32 and PhotoCraft channel-layout metadata. Added region read/write, COW snapshots, defaults/pruning, and a **flat raster-only** document ownership/order prototype in `pc_document.h/.c`. This is explicitly an implementation **spike**, not a final accepted public ABI. It does not port `Document`'s group/mask/history/compositor semantics.

**Evidence:** [PF-TN-004](../tests/PF-TN-004.md), [CI #37919197110](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37919197110) — C99/ASan+UBSan PASS, Rust-original interleaved-byte differential PASS, 12 m68k HUNK and vamos executions at O0/O2 PASS. The typed/m68k Rust oracle is byte-identical on enumerated fixtures. No native PhotoCraft GUI or product compiler selection follows.

Architecture proposals: [ADR-0002](../adr/ADR-0002-photocraft-core-and-memory-contract.md) and [ADR-0003](../adr/ADR-0003-amigaos-toolchain-and-verification.md), both `accepted` for the architectural scope on 2026-10-09. PF-SP-002 remains `in_progress` for the full PhotoCraft document/image editing core. Implementation/target tests and product-compiler selection are separate gates.
