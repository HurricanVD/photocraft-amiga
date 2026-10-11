# PF-TN-013 — PF-SP-010 planned acceptance

- Story: [PF-SP-010](../stories/PF-SP-010.md)
- Date: 2026-10-12
- Status: `planned / not_run`. No implementation, build, runtime or manual native test PASS asserted.
- Scope: Synthetic RGBA8 CPU band -> ARGB32 RTG M2, dirty clipping, OOM, bitmap lifetime; manual WinUAE.

## Proposed verification

1. Fixture-positive and negative/boundary cases and repeatability.
2. Host C99 smoke/ASan/UBSan and unchanged original PhotoCraft Rust oracle for OS-free semantics; m68k/GCC13 -O0 and -O2 HUNK/vamos for target-portable code.
3. If GUI/RTG, direct manual AmigaOS 3.2 + WinUAE window/gadget visual and interaction test, exact binary SHA, hardware/RTG settings, screenshots and logs.
4. Invalid input, allocator fault, cleanup, rollback and RAM peak; no floating-point/byte-order assumptions outside documented contract.
5. No automated WinUAE UI/broker/capture flows while `disabled_by_process`.

- `test_evidence: not_run`
- `runtime_evidence: none`
- `release_authorization: no`

## Implementierungs-Nachweis 2026-10-12 (PR #14; eingeschraenkt)

- C99 Host RGBA8 -> numerisches ARGB32: PASS auf CI [AmigaOS 3.2 Host Core #38182485235](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38182485235), inkludiert Host- und ASan/UBSan-Gates fuer den Konverter.
- GCC/Bebbo 13.3 RTG-M2-Module `pc_present_convert.c` / `pc_present_m2.c`: Cross-Compile PASS (gleicher CI-Workflow), SHA `664c4932f537f1c8c24a1ebef7245e2a0f6fed59`. **Compile-only**: noch kein PhotoCraft-HUNK-Fenster in dieser Story.
- MIT/Provenienz: `NOTICE-DUNKELKAMMER-MIT.txt`, Original `HurricanVD/dunkelkammer@26295506`, MIT-Merge `70897e91`; keine Fremdassets oder DkDocument.
- Fehlend: echter WinUAE/P96/CGX-Grafiklauf, Pixelvergleich auf RTG, Failover ohne RTG und reale 32 MiB-Peakmessung. `manual_target: not_run`; Story nicht `done`.
