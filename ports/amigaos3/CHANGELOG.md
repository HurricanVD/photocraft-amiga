# PhotoCraft AmigaOS port changelog

This is a **port-local** engineering changelog. Upstream PhotoCraft's version
and top-level changelog remain unchanged.

## 2026-10-09 — CPU-core test evidence / PF-SP-003

- Added an opt-in GCC/Bebbo 13.3.0 target-build lane patterned after
  vxplatform's GCC13 provider profile, with explicit compiler/root validation.
- Verified six m68k Amiga HUNK binaries at O0 and O2 using pinned vamos;
  C99 core/oracle fixtures match the original Rust photoCraft crates.
- Diagnosed an excessive-memory regression fixture and bounded the live
  256×256 RGBA8 tiles without removing any of 169 coordinate combinations.
- Host C99/sanitizer and original Rust-versus-C differential tests pass.
- Scope is infrastructure/core fixture testing; no editor, GPU backend, GUI,
  full document model or distribution artifact is released.

- PF-SP-003 technically completed and archived (GCC13/vamos/Rust comparison only); process startup chronology and bootstrap differences explicitly recorded in final review. No new shipping capability.

## 2026-10-09 — Typed PhotoCraft raster and flat-layer experiment (PF-SP-002)

- Added experimental format-dependent 256×256 sparse COW tile storage (U8/U16/F32 raw encoded pixels), region IO, default-pixel and prune support.
- Added minimal flat raster-layer ownership, bottom-first order, visibility and document COW snapshot; not yet a full PhotoCraft document model.
- Passed C99 host/sanitizer and original Rust interleaved differential on CI, plus twelve GCC/Bebbo 13.3 HUNK/vamos checks at O0/O2 ([run #37919197110](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37919197110)).
- New ADR-0002/ADR-0003 remain *proposed*; no public ABI or production compiler approved.

## 2026-10-09 — Architecture approvals

- Project owner accepted ADR-0002 (PhotoCraft tile/core/memory strategy) and ADR-0003 (separated toolchain and runtime validation profiles).
- GCC13.3 remains an opt-in validated target test lane, not a designated native product compiler; full API, ReAction, MiniGL and release decisions remain open.
- No PhotoCraft Rust upstream changes; fork-specific architecture and tests remain isolated under `ports/amigaos3`.
