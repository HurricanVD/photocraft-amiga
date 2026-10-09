# PF-SP-002 — Host-first PhotoCraft raster/core port

- Story ID: PF-SP-002
- Type: SP (technical spike)
- Status: in_progress (host slice implemented, target and Rust-runtime oracle evidence pending)
- Priority: P1
- Date: 2026-10-09
- Architecture: accepted ADR-0001
- Test evidence: PF-TN-002

## Purpose

Start from the *original* PhotoCraft source semantics rather than adapting
Dunkelkammer DkDocument. Implement a testable C99 subset with no AmigaOS GUI,
MiniGL or SDK dependency.

Reference:
- \`crates/geom/src/lib.rs\`: 256x256 tile size, half-open Rect, signed
  negative coordinate \`div_euclid\`, intersect/union/saturating translation.
- \`crates/color/src/lib.rs\`: pixel format metadata for RGB/Gray/CMYK,
  U8/U16/F32 storage sizes.
- \`crates/raster/src/lib.rs\`: sparse RGBA8 pixels, default pixel, Arc-like
  COW clones, prune and tile count.

## Scope

- C99 \`pc_core.h\` / \`pc_core.c\` with explicit owner and error returns.
- No changes to the Rust crates or cargo workspace.
- Host test fixtures derived from upstream Rust test cases and negative tile
  boundary grids.
- Existing \`host-test\` runs the staging test and first core test.
- Optional ASan/UBSan target for memory and signed arithmetic diagnostics.
- No third-party or proprietary Dunkelkammer code copied.

## Gates not yet satisfied

This code is NOT the complete PhotoCraft document model or its image editor.
Raster support is RGBA8-only, with a linear lookup prototype rather than a
production ordered map. Missing: full format-generic U16/F32/CMYK/Gray
raster support, Fill/Region and selection data, Layer/Group hierarchy,
blend/compositor algorithms, commands/undo, persistence, reference Rust
binary-to-C conformance run, and AmigaOS target compilation.

The host tests are parity *fixtures* derived from source assertions. Because
Rust cargo is not installed in the execution environment, tests have NOT been
run against an actual compiled Rust reference binary. Do not mark the full
port or story done until the corresponding independent oracles and target
gates are satisfied.
