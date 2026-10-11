<!-- VD canonical story/test record. Source port record kept intact at ports/amigaos3/docs/tests/PF-TN-002.md. -->
# PF-TN-002 — PhotoCraft core host fixtures

Date: 2026-10-09
Story: PF-SP-002
Scope: independent C99 PhotoCraft geometry, format metadata, RGBA8 sparse COW
Original host test environment: Linux GCC/cc; additional m68k/vamos tests completed later in PF-TN-003.

## Executed checks

- `make host-core-test`: GCC C99 with `-O2 -Wall -Wextra -Werror -pedantic`,
  result `PASS: PhotoCraft host core parity fixtures (geom/color/raster RGBA8)`.
- `make host-core-sanitize`: `-fsanitize=address,undefined`,
  same passing test message; no sanitizer violation observed.
- Test cases cover rectangle semantics, i32-extreme width/translation,
  negative tile coordinates (including -257), PixelFormat metadata,
  sparse default tiles, snapshot isolation after mutation, prune and a
  coordinate matrix crossing tile boundaries.

## Evidence level

Host-side tests: **pass**.
Rust executable-to-C differential tests: **pass**, performed on GitHub Actions Ubuntu runner with real Rust toolchain.
AmigaOS cross-compile/vamos: **not_run**.
WinUAE and PiStorm3D: **not_run**.
Historischer Testzeitpunkt: PF-SP-002 stand am 2026-10-09 auf `in_progress`. **Aktueller kanonischer Status seit 2026-10-11: `blocked`** (`split_required`); PF-TN-002 belegt ausschließlich die erste Raster-/Core-Teilimplementierung.

Reference Rust sources were read via the connected GitHub repository in
`HurricanVD/photocraft-amiga` on the same `amigaos32` branch.
The new C files are independently authored and contain no imported private
Dunkelkammer implementation.

## Live original-Rust differential verification (new workflow)

A new independent `rust-original-parity` job runs the original, UNMODIFIED
`photocraft-geom`, `photocraft-color` and `photocraft-raster` library
tests through `cargo test --locked`. It then builds a standalone Rust
program with path dependencies on those original crates and compares its
machine-readable result vectors with an independent C99 executable using
`diff -u` (the job fails if any byte differs). No C code enters the Rust
Cargo workspace.

- C oracle: `tests/core_oracle.c`
- Rust oracle: `tests/rust_oracle/src/main.rs`
- GitHub Actions: `.github/workflows/amigaos3-host.yml`

This was the initial workflow setup note. Its runtime result was subsequently confirmed PASS by actual GitHub Actions #37909489232 and repeated in #37915607402. See the dated verification sections below; do not equate that limited subset with full app parity.

## Verified Rust original vs C99 CI run (2026-10-09)

- Workflow: [AmigaOS 3.2 Host Core #37909489232](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37909489232)
- Commit: `bc063cf156b4931d09b58a52584215ccd3f4d72b`
- Job: `Rust originals vs C99 port` — **success**.
- Original source: original, unchanged `photocraft-geom`, `photocraft-color`, `photocraft-raster` crates built through `cargo test --locked --lib`.
- Rust results: `photocraft-color` 21 passed; `photocraft-geom` 14 passed; `photocraft-raster` 17 passed. Total **52 passed, 0 failed**.
- Independent Rust oracle: `tests/rust_oracle/src/main.rs` uses original crate APIs.
- Independent C oracle: `tests/core_oracle.c` uses the new `pc_core` API.
- Exact comparison: `diff -u oracle-rust.txt oracle-c.txt` produced no differences, final log:
  `PASS: original PhotoCraft Rust crates match C99 port oracle`.
- C99 + AddressSanitizer + UndefinedBehaviorSanitizer job: **success** on the same commit.

**Meaning:** executable, byte-for-byte parity for this *enumerated fixture set*.
**Not proven:** complete Surface API, all pixel depths and modes, document/layer model, compositing, cross-compiler build, WinUAE/QuarkTex runtime, or complete upstream PhotoCraft functionality.

## 2026-10-09 evidence reconciliation

The initially missing m68k smoke **now passes for the RGBA8/geometry subset only**, via archived [PF-TN-003](done/PF-TN-003.md) (GCC13.3 and vamos, -O0/-O2, Rust oracle differential). The full format-generic raster, layer/document, editing/compose and MiniGL/GUI tests are not implemented or not yet target-verified. PF-SP-002 war bei dieser Testausfuehrung `in_progress`, ist nach retrospektiver Atomisierung seit 2026-10-11 `blocked`; die breiteren Ziele sind getrennte Child-Stories.

## 2026-10-09 extension

Broader typed U8/U16/F32/region/COW and flat-document fixture evidence is tracked in [PF-TN-004](PF-TN-004.md). The earlier RGBA8-specific entries above are historical. Host and m68k comparisons for the new experimental typed raster passed in CI #37919197110; full document/PSD/compositor parity remains unverified.
