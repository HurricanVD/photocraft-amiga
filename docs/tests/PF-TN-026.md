# PF-TN-026 — PF-PR-001 verification plan

- Status: `planned / not_run`; Datum 2026-10-12.
- Story: [PF-PR-001](../stories/PF-PR-001.md)
- Module: .github/workflows/ + ports/amigaos3/tests oracles; keine Upstream-Rust-Crates aendern.
- Positive/negative tests: Synthetic upstream-change fixture reports changed files, impacted port IDs, unknown API fields and outdated oracles. Workflow mit PR-only App-Tokens/least privilege und Notifikation testen; no-secrets logs. Die Upstream-Quelle bleibt original; kein m68k-std oder Remote-Engine erforderlich.
- Target evidence after implementation: exact Git SHA, GCC/NDK/compiler, HUNK header, vamos O0/O2, original Rust differential when applicable, manual WinUAE evidence for interactive UI.
- `result: not_run`. This planning document never counts as a test execution or release authorization.
