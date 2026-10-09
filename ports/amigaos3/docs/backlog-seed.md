# Pre-bootstrap backlog seed and tracked PF spikes

The `PF` prefix is now **reserved** in the private VD process registry, so formal `PF-*` story/test IDs may be allocated when these draft items are refined. The tracked spikes PF-SP-001..003 and reports PF-TN-001..003 now use reserved canonical IDs in the port-local overlay; completion of the full central process bootstrap and registry activation is still pending.

1. Review the port seam and select the m68k-core strategy (Rust target feasibility vs C implementation checked against original PhotoCraft reference).
2. PF-SP-001 / PF-TN-001: portable Hosttests durchgeführt und MiniGL-Smoke erweitert. m68k-/WinUAE-Zielnachweis weiterhin blocked; nach Verfügbarkeit fortsetzen.
3. Manually test the MiniGL standalone sample in WinUAE/QuarkTex NG and record settings, library builds, screenshots and logs.
4. Implement and test the optional RTG fallback using an explicit port-owned interface, keeping PhotoCraft engine semantics separate from the display.
5. Prototype an off-screen MiniGL bitmap context for ReAction without painting on gadgets.
6. Port first nontrivial PhotoCraft CPU component, proving byte/behavior parity on synthetic cases and exact limits.
7. Only after core parity: add ReAction shell, document/session/command bridges and PSD interoperability.

8. PF-SP-002 / PF-TN-002: host-first Kernport (geom/color-Metadaten/RGBA8 sparse COW) with passing host/Rust oracle and m68k/vamos fixture tests; full format-generic raster/document port remains in progress.

9. PF-SP-003 / PF-TN-003: **done / archived** (port-local `stories/done/`, `tests/done/`). GCC13.3 m68k/vamos and Rust parity pass at O0/O2 (CI #37915607402); final review and closure documented, vxplatform GCC16 default and MiniGL unaffected.

10. PF-SP-002: experimental typed encoded U8/U16/F32 raster regions plus flat raster-layer document prototype; new Rust differential + host/m68k/vamos gates, pending verification; ADR-0002/0003 remain proposed.

11. PF-SP-002 / PF-TN-004: typed U8/U16/F32 raw encoded raster and flat raster-only layer ownership prototype, 12 valid HUNK/vamos tests and Rust-oracle fixtures PASSED (CI #37919197110). Full PhotoCraft document engine and production ABI are still in progress; ADR-0002/0003 proposed.
