# PF-TN-023 — PF-SP-019 verification plan

- Status: `planned / not_run`; Datum 2026-10-12.
- Story: [PF-SP-019](../stories/PF-SP-019.md)
- Module: ports/amigaos3/src pc_paint_commands + UI pointer adapter, PhotoCraft Rust paint as semantic oracle.
- Positive/negative tests: Golden RGBA8 stroke/pixel vectors gegen originalen Rust-Algorithmus wo mathematisch gleich; Abweichungen exakt markieren. Eine Undo/Redo-Transaktion pro Stroke; Pointer-Abbruch und 256er Grenzen. C99+ASan, HUNK/vamos and manual WinUAE brush/edit/undo test.
- Target evidence after implementation: exact Git SHA, GCC/NDK/compiler, HUNK header, vamos O0/O2, original Rust differential when applicable, manual WinUAE evidence for interactive UI.
- `result: not_run`. This planning document never counts as a test execution or release authorization.
