# PF-TN-002 — PhotoCraft core host fixtures

Date: 2026-10-09
Story: PF-SP-002
Scope: independent C99 PhotoCraft geometry, format metadata, RGBA8 sparse COW
Test environment: Linux GCC/cc host, no m68k target runtime

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
Rust executable-to-C differential tests: **not_run**, cargo not present.
AmigaOS cross-compile/vamos: **not_run**.
WinUAE and PiStorm3D: **not_run**.
Overall PF-SP-002 remains `in_progress`.

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

The differential result remains **pending** until the new workflow run is
confirmed and its logs inspected. Do not infer a passing Rust runtime from
the presence of this CI configuration alone.
