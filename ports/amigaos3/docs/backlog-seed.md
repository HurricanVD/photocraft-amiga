# Pre-bootstrap backlog seed (no canonical IDs)

The `PF` prefix is now **reserved** in the private VD process registry, so formal `PF-*` story/test IDs may be allocated when these draft items are refined. No canonical IDs are allocated in this seed; completion of the process bootstrap and its reviews is still pending.

1. Review the port seam and select the m68k-core strategy (Rust target feasibility vs C implementation checked against original PhotoCraft reference).
2. PF-SP-001 / PF-TN-001: portable Hosttests durchgeführt und MiniGL-Smoke erweitert. m68k-/WinUAE-Zielnachweis weiterhin blocked; nach Verfügbarkeit fortsetzen.
3. Manually test the MiniGL standalone sample in WinUAE/QuarkTex NG and record settings, library builds, screenshots and logs.
4. Implement and test the optional RTG fallback using an explicit port-owned interface, keeping PhotoCraft engine semantics separate from the display.
5. Prototype an off-screen MiniGL bitmap context for ReAction without painting on gadgets.
6. Port first nontrivial PhotoCraft CPU component, proving byte/behavior parity on synthetic cases and exact limits.
7. Only after core parity: add ReAction shell, document/session/command bridges and PSD interoperability.

8. PF-SP-002 / PF-TN-002: host-first Kernport (geom/color-Metadaten/RGBA8 sparse COW) with passing host fixture tests; Rust binary oracle and m68k remain pending.

9. PF-SP-003 / PF-TN-003: **technical O0/O2 GCC13.3 m68k/vamos tests and Rust parity PASS** (CI #37915607402); formal VD closure review pending. Separate from vxplatform GCC16 default and MiniGL.
