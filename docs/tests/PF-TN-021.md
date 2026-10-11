# PF-TN-021 — PF-SP-017 verification plan

- Status: `planned / not_run`; Datum 2026-10-12.
- Story: [PF-SP-017](../stories/PF-SP-017.md)
- Module: ports/amigaos3/src: eigene pc_io_import und pc_document-Adapter; Dunkelkammer dk_png/dk_iff/dk_datatype nur mit MIT+Third-Party-Notice.
- Positive/negative tests: Kleines RGB/RGBA-PNG, IFF-Sample, Alpha-/Stride-/Endianness-Oracles und Null-Bilder. Truncated/Malformed/huge dimensions, Dateifehler und deterministischer Allocator-Fault; kein Leak, altes Dokument bleibt gueltig. Host C99/ASan, HUNK/vamos und GUI-Pfad per manuellem WinUAE/ASL-Roundtrip.
- Target evidence after implementation: exact Git SHA, GCC/NDK/compiler, HUNK header, vamos O0/O2, original Rust differential when applicable, manual WinUAE evidence for interactive UI.
- `result: not_run`. This planning document never counts as a test execution or release authorization.
