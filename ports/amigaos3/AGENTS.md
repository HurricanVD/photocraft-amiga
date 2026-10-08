# PhotoCraft AmigaOS 3 port rules

This directory is a fork-specific, isolated native AmigaOS 3.2 port. It is an explicit scoped exception to the root AGENTS.md "Rust only" rule: C99 is allowed ONLY within ports/amigaos3 for AmigaOS APIs, graphics backends, and independent port test harnesses. The main Rust crates, desktop shell, web app, and cargo workspace remain upstream-compatible and subject to the root rules.

- Keep PhotoCraft's original Rust document model, pixel formats, command semantics, and CPU compositor the authoritative reference. Do not silently substitute Dunkelkammer's DkDocument or its 128x128 tiles for PhotoCraft's 256x256 tiles.
- Separate platform services, rendering, UI, and portable algorithms. No direct OS calls in portable code. Follow the VD Amiga development process when implementing product features.
- Do not copy proprietary Dunkelkammer sources or assets into this PUBLIC repository without an explicit redistribution/relicensing decision by their copyright holder and a provenance entry.
- Do not vendor MiniGL SDK, Windows DLLs, fonts, AmigaOS NDK or ROMs. Keep all local toolchain paths configurable.
- Prefer deterministic host-side C tests, m68k HUNK/vamos checks, then manual WinUAE+QuarkTex NG and PiStorm3D evidence. Never present unrun checks as passing.
- Do not invoke automated WinUAE UI/broker/capture scripts from vd-amiga-dev-process while the process marks that path disabled_by_process.
- Treat all minigl.library-dependent code as optional and guard its runtime availability. Preserve a software/RTG presentation path.
- Every real ported PhotoCraft algorithm should have parity tests against the unmodified Rust reference before being called complete.
- Document all deviations from root rules and upstream behavior in this port's docs; new durable decisions begin as proposed ADRs.
