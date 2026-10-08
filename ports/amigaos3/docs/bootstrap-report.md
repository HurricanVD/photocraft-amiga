# Initial PhotoCraft AmigaOS port scaffold

Date: 2026-10-08
Branch: `amigaos32`
Source: `storytold/photocraft` via public `HurricanVD/photocraft-amiga` fork
Process baseline proposed: `HurricanVD/vd-amiga-dev-process` 0.2.1

This is **manual pre-bootstrap scaffolding**, not a completed canonical VD process bootstrap. Existing root PhotoCraft documentation and Rust AGENTS rules are maintained. C99 exceptions are strictly isolated in `ports/amigaos3`.

Prefix: `PF` **reserved** in the shared registry on 2026-10-08 (VD process commit `07be242b760e81c2fc3ae1b4fbcac6d3031489b0`). Project metadata sets `STORY_ID_PREFIX=PF` and `TEST_ID_PREFIX=PF`. No canonical story/test IDs allocated yet. Prefix activation: `follow_up_required` until canonical bootstrap evidence has been reviewed.

Implemented in this scaffold: testable C99 RGBA staging utility, its host test suite, standalone MiniGL smoke source and make targets, architecture proposal (ADR-0001 accepted for architectural direction on 2026-10-08), IP provenance register and test plan.

Evidence: source written, builds NOT executed on a m68k toolchain; WinUAE and PiStorm3D tests NOT run. The `host-test` target must be executed before claiming portable test pass.

Next gates: complete canonical VD bootstrap review, promote seed items into PF-* stories, run host tests, verify m68k compile with actual SDK, complete implementation-level pre-ready review, and manually exercise QuarkTex NG.
