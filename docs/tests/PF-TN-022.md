# PF-TN-022 — PF-SP-018 verification plan

- Status: `planned / not_run`; Datum 2026-10-12.
- Story: [PF-SP-018](../stories/PF-SP-018.md)
- Module: ports/amigaos3/src pc_io_export, pc_compositor, Dunkelkammer-PNG-Encoder hinter schmalem Adapter.
- Positive/negative tests: Original-Rust-CPU-Referenz gegen PNG-dekodierte Pixel fuer kleine Multilayer-Fixtures. Disk-full, Invalid path, OOM, kurze Schreiboperation, Cleanup temporärer Datei, kein unbemerkter Verlust vorhandener Datei. Host encoder/raster tests, genuine HUNK/vamos and manual WinUAE open/export/reimport.
- Target evidence after implementation: exact Git SHA, GCC/NDK/compiler, HUNK header, vamos O0/O2, original Rust differential when applicable, manual WinUAE evidence for interactive UI.
- `result: not_run`. This planning document never counts as a test execution or release authorization.
