# Initial PhotoCraft AmigaOS port scaffold

Date: 2026-10-08
Branch: `amigaos32`
Source: `storytold/photocraft` via public `HurricanVD/photocraft-amiga` fork
Process baseline proposed: `HurricanVD/vd-amiga-dev-process` 0.2.1

This is **manual pre-bootstrap scaffolding**, not a completed canonical VD process bootstrap. Existing root PhotoCraft documentation and Rust AGENTS rules are maintained. C99 exceptions are strictly isolated in `ports/amigaos3`.

Prefix: `PF` proposed, **not reserved**. No canonical story/test IDs assigned. Activation: follow_up_required.

Implemented in this scaffold: testable C99 RGBA staging utility, its host test suite, standalone MiniGL smoke source and make targets, architecture proposal, IP provenance register and test plan.

Evidence: source written, builds NOT executed on a m68k toolchain; WinUAE and PiStorm3D tests NOT run. The `host-test` target must be executed before claiming portable test pass.

Next gates: reserve PF prefix in process registry; review proposed ADR; run host tests; verify m68k compile with actual SDK; manually exercise QuarkTex NG.
