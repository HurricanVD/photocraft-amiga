# PF-TN-014 — PF-SP-011 planned acceptance

- Story: [PF-SP-011](../stories/PF-SP-011.md)
- Date: 2026-10-12
- Status: `planned / not_run`. No implementation, build, runtime or manual native test PASS asserted.
- Scope: ReAction window/class opening, closing, repeated lifecycle, resize/refresh, Workbench/Shell and stack/OOM; manual WinUAE.

## Proposed verification

1. Fixture-positive and negative/boundary cases and repeatability.
2. Host C99 smoke/ASan/UBSan and unchanged original PhotoCraft Rust oracle for OS-free semantics; m68k/GCC13 -O0 and -O2 HUNK/vamos for target-portable code.
3. If GUI/RTG, direct manual AmigaOS 3.2 + WinUAE window/gadget visual and interaction test, exact binary SHA, hardware/RTG settings, screenshots and logs.
4. Invalid input, allocator fault, cleanup, rollback and RAM peak; no floating-point/byte-order assumptions outside documented contract.
5. No automated WinUAE UI/broker/capture flows while `disabled_by_process`.

- `test_evidence: not_run`
- `runtime_evidence: none`
- `release_authorization: no`

## Implementierungs-Nachweis 2026-10-12 (PR #15; eingeschraenkt)

- GCC/Bebbo 13.3 HUNK-Link fuer `pc_amiga_shell.c` + M2 Presenter auf Revision `9b6f4c79c62ae28c8de07a0675f63cc236cdf777`: CI [GCC13.3 m68k HUNK + vamos vs Rust #38182781986](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38182781986) PASS; der Workflow prueft das erzeugte HUNK-Format.
- Bestehende Headless-C99, GCC/vamos und Rust-Differentialtests PASS; sie starten **nicht** die ReAction-Oberflaeche. Die GUI hat im Shell-MVP nur synthetische PhotoCraft-RGBA8-Pixel.
- Der erste GCC13-Compilefehler (fehlende `RA_OpenWindow`/`RA_CloseWindow`/`RA_HandleInput` Makrodeklarationen) wurde mit `reaction/reaction_macros.h` korrigiert.
- Offen: visueller manueller WinUAE-GUI-Lauf mit Workbench/Shell, Resize, Close/Repeated Open, RTG-Staging und Speicher-Fallback. `manual_target: not_run`; keine fertige Editor-App oder PF-SP-011-Done-Freigabe.
