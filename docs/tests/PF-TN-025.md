# PF-TN-025 — PF-SP-021 verification plan

- Status: `planned / not_run`; Datum 2026-10-12.
- Story: [PF-SP-021](../stories/PF-SP-021.md)
- Module: ports/amigaos3/tests + begrenzte pc_layered_io-Experimentalfunktionen, docs/adr/ proposed future product-format decision.
- Positive/negative tests: Rust/C typed layer fixture and negative-format bytes. HUNK/vamos reader/writer bounds and corruption rollback, 32 MiB budget. Kein Claim auf reales .pcraft/PSD-Produktaustauschformat ohne separate Entscheidungen.
- Target evidence after implementation: exact Git SHA, GCC/NDK/compiler, HUNK header, vamos O0/O2, original Rust differential when applicable, manual WinUAE evidence for interactive UI.
- `result: not_run`. This planning document never counts as a test execution or release authorization.
