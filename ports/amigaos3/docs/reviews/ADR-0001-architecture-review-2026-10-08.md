# ADR-0001 architecture review — PhotoCraft AmigaOS 3.2

- Date: 2026-10-08
- Scope: PhotoCraft fork `amigaos32` architecture decision and VD port-local overlay
- Decision depth: `far_reaching`
- User decision: **approved** by explicit instruction in the current conversation ("registriere pf und akzeptiere den adr"; 2026-10-08)
- Architectural outcome: **approve_with_followups** — acceptable scope/boundary choice, not a claim of working native implementation
- Next review: separate `pre_ready_final` on the first implementation-driving story, including exact NDK evidence and real runtime results

## Inputs reviewed

1. PhotoCraft upstream architecture and isolated fork port (Rust document/COW/256x256 semantics, egui/wgpu separation, existing proposed ADR-0001).
2. VD process v0.2.1: `docs/process/adr-lifecycle.md`, `repo-adoption.md`, `workspace-id-prefixes.md`, `amiga-architecture-baseline.md`, `amiga-best-practices.md`, `amiga-capability-catalog.md`.
3. Dunkelkammer private source (platform/canvas/core APIs, documented M2 RTG strategy and provenance restrictions): **reference only** for this public fork.
4. QuarkTex NG MiniGL reference and PiStorm3D shared `minigl.library` API documentation (published descriptions, no hardware execution).

## Architecture findings

- **Pass / scope:** The public fork leaves Rust crates, the Cargo workspace, and Upstream `main` intact. C99 is isolated to `ports/amigaos3/`. PhotoCraft's document model, commands and 256x256 tile semantics remain the port's authoritative target.
- **Pass / OS-first:** Target is AmigaOS 3.2 and NDK 3.2, native ReAction UI, chosen GCC/Bebbo compiler lane. A software/RTG fallback is planned, avoiding a mandatory MiniGL runtime. The 68040+/32 MiB target is an allowed, documented tightening of the common high-end baseline.
- **Pass with follow-up / capability:** ReAction `window.class`/`layout.gadget`, Intuition/ASL/Workbench, RTG/P96 and MiniGL must be evaluated with concrete NDK/SDK evidence in implementation stories. Avoid binding MiniGL directly across sidebar gadgets; offscreen bitmap ownership and presentation require a real test.
- **Pass with follow-up / vxlibs:** No ready shared helper is currently reused by the new isolated renderer/staging test. Any new shared helper must first check the relevant OS capability and then the `ready` vxlibs set; exact deps and provenance must be pinned when used.
- **Pass with follow-up / IP:** This public fork contains no copied Dunkelkammer proprietary code, no MiniGL binaries/SDK and no private VD process files. Explicit copyright-holder release permission, component licence and attribution are required *before* publishing any copied proprietary Dunkelkammer source.
- **Unverified / toolchain:** No m68k HUNK compilation, real rust/m68k `std` support, WinUAE QuarkTex MiniGL run or PiStorm3D hardware tests have been demonstrated in this ADR acceptance. Host test evidence is separately tracked; graphics/runtime parity is pending.
- **Process follow-up:** `PF` was reserved in VD process registry on 2026-10-08. Canonical full repo bootstrap evidence and promotion to `active` remain outstanding; initial port-local overlay is a documented exception preserving upstream documentation.

## Review result and approval

The proposed architecture is internally consistent with the fork's objectives and the referenced VD target baseline. The owner explicitly approved the far-reaching decision and PF reservation. **Accept ADR-0001 as a technical direction with follow-ups**; do not promote any application implementation story to done solely because this ADR is accepted.

Required gates remain: m68k SDK/compiler build, host and parity tests, ReAction+offscreen MiniGL tests, fallback verification, and separate IP/licensing approval for any future Dunkelkammer imports. Material changes to this accepted ADR require a new superseding ADR.
