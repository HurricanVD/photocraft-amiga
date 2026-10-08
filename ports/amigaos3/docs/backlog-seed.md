# Pre-bootstrap backlog seed (no canonical IDs)

Do NOT issue formal VD story or test IDs until the proposed `PF` prefix is reserved in the private process repository.

1. Review the port seam and select the m68k-core strategy (Rust target feasibility vs C implementation checked against original PhotoCraft reference).
2. Run portable C99 staging test; compile a real Amiga HUNK with the project GCC/Bebbo SDK lane.
3. Manually test the MiniGL standalone sample in WinUAE/QuarkTex NG and record settings, library builds, screenshots and logs.
4. Implement and test the optional RTG fallback using an explicit port-owned interface, keeping PhotoCraft engine semantics separate from the display.
5. Prototype an off-screen MiniGL bitmap context for ReAction without painting on gadgets.
6. Port first nontrivial PhotoCraft CPU component, proving byte/behavior parity on synthetic cases and exact limits.
7. Only after core parity: add ReAction shell, document/session/command bridges and PSD interoperability.
