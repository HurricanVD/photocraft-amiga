# PF-TN-007 — Gray8 LayerMask ownership and COW differential

- Date: 2026-10-10
- Story: PF-SP-002 (in_progress).
- Stacked branch: `feature/pf-sp-002-layer-masks`, based on [group PR #4](https://github.com/HurricanVD/photocraft-amiga/pull/4).
- Scoped result: **PASS**, verified on source commit `8389d3a56a8c458595c03b6e264710de1f0f61b4` in [CI #38061132027](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38061132027). Whole multi-platform CI conclusion must be checked independently.
- Scope: original Rust LayerMask with **fixed density=1, feather=0** and
  a sparse Gray8/U8 default surface, `enabled` and `linked` flags.
  Not a compositor, generic grayscale format or shipping C ABI.

## Required test matrix

| Test | Expected evidence |
|---|---|
| Host C99 | Reveal-all default 255, hide-all default 0, negative coordinates and sparse tile edge, enabled=false reveals 255 |
| Format/ownership guard | Reject RGB/U8, null/invalid layer, duplicate IDs/pointers and second mask; no mutation or double-free |
| Nested groups | Masks attach by globally unique ID to nested raster and group nodes |
| Clone COW | Mask surfaces and metadata independent between document snapshots; original survives copy mutation and vice versa |
| Detach | Mask ownership returned; no stale mask metadata, clone remains valid |
| ASan/UBSan | No observed leaks or undefined behavior on tests |
| Original Rust vs C99 oracle | Byte-for-byte parity with `LayerMask::reveal_all`, `value`, enabled/link state and clone mutation on enumerated Gray8 samples |
| AmigaOS m68k | GCC/Bebbo 13.3 opt-in O0/O2 HUNK and pinned vamos run of extended `pc_document_test` |
| Full CI | Root Linux/Windows/macOS, documentation, packaging, PF static preflight |

## Verification commands

`make -C ports/amigaos3 host-test`

`make -C ports/amigaos3 host-core-sanitize`

`cargo run --quiet --manifest-path ports/amigaos3/tests/rust_oracle/Cargo.toml -- mask`

`ports/amigaos3/build/mask_oracle`

Compare the last two outputs byte-for-byte in the root CI
`PF-SP-002 flat Document C99 vs original Rust` job. The m68k job
rebuilds and runs the extended document test at both optimization levels.

## Non-goals and open requirements

Density/feather math, per-layer effective pixel compositing, color-managed
mask conversions, mask transforms, masks in PSD import/export, history/undo,
zero-copy surface APIs, production allocator/ABI and MiniGL/WinUAE hardware
are **not implemented**. These require their own story/review/test evidence.

## Executed evidence — 2026-10-10

- Tested implementation source: `8389d3a56a8c458595c03b6e264710de1f0f61b4`.
- [GitHub Actions #38061132027](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38061132027)
  scoped job `PF-SP-002 flat Document C99 vs original Rust`:
  **completed/success**. Portable host C99, ASan+UBSan, 51 unchanged
  original `photocraft-doc` unit tests and the byte-for-byte `-- mask`
  original Rust/C99 oracle passed.
- Observed log markers: `PASS: PhotoCraft Gray8 LayerMask ownership/defaults/flags/deep COW`
  and `PASS: original Rust LayerMask Gray8 value/COW vs C99`.
- Same run, scoped job `PF-SP-002 GCC13 m68k Document O0/O2 vamos`:
  **completed/success**. Six real m68k HUNK binaries passed the format
  verifier; extended mask-aware `pc_document_test` passed pinned
  `vamos` at both `-O0` and `-O2`.
- Compiler opt-in test profile: Bebbo GCC `13.3.0`, image
  `stefanreinauer/amiga-gcc:gcc-v13.3-20260622`,
  digest `sha256:f9d09422a89f317a2f59d5db46227ad9bd8d753d5c9fe41553fcfdc6ecf60ce8`.
- First CI attempt [#38061023830](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38061023830)
  exposed a shell orchestration error: mask artifact lines were inserted
  between group-oracle commands. The workflow was corrected in
  `8389d3a`; it did not represent a failing mask algorithm.
- The **overall** PR CI includes additional root Rust/platform/corpus
  jobs and must finish on the frozen PR head before any merge. No native
  MiniGL/WinUAE/QuarkTex NG or shipping-ABI acceptance is implied.
