# PF-TN-005 — Flat Document shift and layer opacity parity

- Story: [PF-SP-002](../stories/PF-SP-002.md), **in_progress**.
- Started: 2026-10-10.
- Decision basis: [accepted ADR-0002](../adr/ADR-0002-photocraft-core-and-memory-contract.md) and [ADR-0003](../adr/ADR-0003-amigaos-toolchain-and-verification.md).
- Scope: experimental flat-raster `PcDocument` metadata. This is **not** the product `doc::Document`, an approved C ABI or a native GUI.
- Scoped verification result: **PASS** for the implementation commit `8926cb9046b3c8a69cdd0dbe3e4f49de196a0995` on [CI #38033507546](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38033507546). The broader multi-platform/root CI result remains separate from these completed jobs.

## Verification matrix

| Gate | Command / fixture | Expected |
|---|---|---|
| C99 document host fixtures | `make -C ports/amigaos3 host-test` | **PASS** — shifts, stable IDs, ownership, bounds, NaN/INF and clone isolation |
| Host sanitizer | `make -C ports/amigaos3 host-core-sanitize` | **PASS** — no ASan/UBSan findings |
| Original Rust `doc` test suite | `cargo test -p photocraft-doc --locked --lib` | **PASS** — 51 tests, 0 failures |
| Flat-document Rust-vs-C oracle | Rust `tests/rust_oracle ... -- document` against C `tests/document_oracle.c` | **PASS** — byte-for-byte identical order/shift/metadata output |
| GCC/Bebbo 13.3 m68k | `gcc13-build-typed`, `gcc13-check-typed-hunks` | **PASS** — six genuine HUNK binaries, including extended `pc_document_test` at O0/O2 |
| `vamos` runtime | Pinned machine68k/amitools + `vamos -S -C 20 -m 8192 -s 128` on extended document binary | **PASS** — document test under O0 and O2 |

## Out-of-scope and acceptance limits

This test covers no group insertion, masks, blending, compositing, ICC,
PSD save/load, command routing, undo, WinUAE/MiniGL/QuarkTex NG or PiStorm3D.
No product compiler, shipping rights or full-port acceptance is implied.
`PF-SP-002` stays `in_progress`; full original Rust document semantics
require further implementation and verification.

## Executed evidence and engineering correction

- Source commit tested: `8926cb9046b3c8a69cdd0dbe3e4f49de196a0995`.
- [GitHub CI #38033507546](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38033507546), scoped jobs:
  `PF-SP-002 flat Document C99 vs original Rust` **success** and
  `PF-SP-002 GCC13 m68k Document O0/O2 vamos` **success**.
- Unmodified PhotoCraft Rust `doc` unit tests: **51 passing**.
- C99 and ASan+UBSan host tests: **PASS**.
- Original Rust `Document::shift` / `Layer` opacity vs independent
  C99 differential output: **byte-identical** on the enumerated vectors.
- GCC/Bebbo `13.3.0` opt-in test profile; tagged image
  `stefanreinauer/amiga-gcc:gcc-v13.3-20260622`, resolved image digest
  `sha256:f9d09422a89f317a2f59d5db46227ad9bd8d753d5c9fe41553fcfdc6ecf60ce8`.
- Six HUNK binaries recognized by `check-hunk.py`; extended document
  fixture ran through pinned `vamos` at both `-O0` and `-O2`, PASS.
- An earlier [CI #38033254880](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38033254880)
  detected genuine `-msoft-float` linker failures
  (`__lesf2`, `__gesf2`, `__nesf2`) in the initial f32-comparison
  implementation. Corrected the cause by checking finite `f32`
  encoded IEEE-754 binary32 patterns with `memcpy`/integer masks;
  native test assertions also compare bit patterns. The follow-up
  GCC13 O0/O2 HUNK/vamos lane passed.
- No WinUAE/QuarkTex/PiStorm3D run performed. No native product ABI or
  shipping compiler selected. This is a **scoped PF-SP-002 slice only**.

Future edits still require fresh execution on their final commit before
claiming a complete PR or merge status.
