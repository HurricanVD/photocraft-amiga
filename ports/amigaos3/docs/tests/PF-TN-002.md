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
