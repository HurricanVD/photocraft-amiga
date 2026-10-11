# PF-TN-024 — PF-SP-020 verification plan

- Status: `planned / not_run`; Datum 2026-10-12.
- Story: [PF-SP-020](../stories/PF-SP-020.md)
- Module: ports/amigaos3/ Makefile, app entry, native UI/core facade, packaging manifest; existing approved core and UI modules.
- Positive/negative tests: HUNK binary/Compiler/SHA/target CPU/Fast-RAM aufgenommen und Screenshot + Output-PNG pixelvalidiert. Start/close/reopen, fehlende NDK-Gadgets/RTG, OOM, Fehler nach Quell-PNG ohne Leak. Ausdruecklicher Nachweis, dass nur die AmigaOS-Umgebung aktiv und kein Host/Netz erforderlich ist.
- Target evidence after implementation: exact Git SHA, GCC/NDK/compiler, HUNK header, vamos O0/O2, original Rust differential when applicable, manual WinUAE evidence for interactive UI.
- `result: not_run`. This planning document never counts as a test execution or release authorization.
