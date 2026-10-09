# ADR-0001: Native AmigaOS 3.2 port seam

- ADR-ID: `ADR-0001`
- Status: **accepted**
- Created: 2026-10-08
- Last updated: 2026-10-09 (verification metadata only; accepted architectural decision unchanged)
- Lifecycle action: `accept`
- Scope: `ports/amigaos3/` in `HurricanVD/photocraft-amiga`
- Decision depth: `far_reaching` (OS target, language boundary, GUI/renderer, build strategy)
- Decision authority: PhotoCraft AmigaOS fork owner (explicit user approval)
- Approval: `approved` — direct chat instruction "registriere pf und akzeptiere den adr" on 2026-10-08
- Architecture review: `approve_with_followups` — [review](../../ports/amigaos3/docs/reviews/ADR-0001-architecture-review-2026-10-08.md)
- Relevant standards: VD Amiga process 0.2.1; Amiga architecture baseline; Amiga best practices; IP/third-party policy; upstream PhotoCraft AGENTS/architecture
- Reviewed capabilities: ReAction/window.class and Intuition windowing; Picasso96/RTG; MiniGL shared library (runtime behavior unverified); additional OS capability decisions per implementation story
- NDK/API evidence: `unverified` for MiniGL bitmap context and display lifecycle; test/SDK-backed gate remains open
- OS deviation log: `ports/amigaos3/docs/os-deviation-log.md`
- vxlibs baseline: `ready_components_available`; usage decision `spike_required` for any future relevant shared helpers
- Implementation/runtime evidence (2026-10-09): **partial** — scoped portable RGBA8/geometry C core validated with GCC13.3 HUNK/vamos O0/O2 against original Rust fixtures ([PF-TN-003](../../ports/amigaos3/docs/tests/done/PF-TN-003.md), [CI #37915607402](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37915607402)); PhotoCraft editor/GUI/MiniGL/WinUAE/PSD engine **not validated**. Architectural sign-off is not runtime or release sign-off.
- Supersedes: none
- Superseded by: none
- Lifecycle source: `HurricanVD/vd-amiga-dev-process/docs/process/adr-lifecycle.md`

## Status history

| Date | Status | Action | Authority / evidence |
|---|---|---|---|
| 2026-10-08 | proposed | propose | Initial fork architecture and scoped C99/AmigaOS plan |
| 2026-10-08 | accepted | accept | Explicit project owner approval in conversation; static architecture review with follow-ups documented below |



## Context

PhotoCraft is an upstream Rust 2024 application, with egui/eframe and wgpu/WGSL GPU layers. AmigaOS 3.2/68k does not have an established drop-in target for that complete stack. The fork must remain an identifiable **PhotoCraft port**, rather than replacing the data model with an existing Amiga editor.

The owner also maintains `HurricanVD/dunkelkammer`, with native ReAction, RTG M2 present, I/O and a proven 68k build/test methodology. That repository is currently proprietary; its implementation is not automatically redistributable in this public fork.

## Decision

1. Preserve upstream PhotoCraft code and architecture as the behavioral oracle and primary source tree.
2. Keep AmigaOS-specific code isolated under `ports/amigaos3/`; C99 is permitted there for platform APIs and independent proof-of-concept tests. No modification to the Cargo workspace is required for initial probes.
3. Prove an actual Rust/m68k `std`-capable subset before promising that Rust PhotoCraft crates run natively. If not viable, port selected CPU algorithms/data-model semantics to C with reference fixtures and parity tests.
4. Use PhotoCraft's native 256x256 tiling and document model as the target contract; legacy Dunkelkammer ARGB32/128x128 models are staging adapters, not replacements.
5. Keep CPU reference compositing authoritative; add a MiniGL **viewport** backend for QuarkTex NG and compatible PiStorm3D runtime implementations. Keep a software/RTG fallback.
6. Treat ReAction event/UI primitives and M2 RTG lessons from Dunkelkammer as reusable platform knowledge. Any direct proprietary code copy into this public fork requires documented copyright-holder permission and appropriate public licensing.
7. WinUAE with QuarkTex NG is the first manual graphics test environment; separately test PiStorm3D on actual supported hardware. No unverified performance claims.
8. Use vd-amiga-dev-process v0.2.1 as the port-local overlay. The `PF` prefix is `reserved` in the shared registry; assign canonical `PF-*` IDs as formal stories are promoted from bootstrap seeds. Prefix activation remains a separate post-bootstrap gate.

## Consequences and tradeoffs

+ Upstream core stays reviewable and syncable.
+ Clear C/AmigaOS platform boundary with isolated build/test targets.
+ Optional graphics runtime permits degradation without MiniGL.
- Genuine engine port feasibility remains open.
- C parity-port of algorithms may be significant work and requires exhaustive testing.
- RGBA8 staging conversion and GPU upload can cost time; benchmark against M2 cached RTG blits.
- ReAction canvas off-screen ownership and MiniGL bitmap compatibility require target evidence.

## Alternatives rejected for now

- Rename/fork Dunkelkammer as PhotoCraft: would not preserve PhotoCraft behavior or architecture.
- Port `wgpu` wholesale to MiniGL: not an equivalent shader/compute API.
- Draw MiniGL directly across the ReAction main window: risks damaging side-panel gadgets; prefer an off-screen bitmap spike.
- Copy proprietary Dunkelkammer files directly to the public fork: deferred until explicit approval and licence statement.

## Implementation verification gates (still open)

Architecture acceptance **approves the direction and boundaries only**. It does not mean the program compiles on AmigaOS or works under WinUAE/PiStorm3D. Before marking a first implementation story done or declaring a working editor, obtain evidence for:

- Host staging and deterministic reference/parity tests.
- Actual m68k HUNK compilation and applicable vamos tests.
- Compatible MiniGL SDK and installed runtime; GL initialization, texture upload and errors on WinUAE/QuarkTex NG.
- Correct off-screen RTG bitmap presentation and ReAction window/gadget ownership, plus fallback handling.
- A/B comparison with Dunkelkammer M2 RTG path and later PiStorm3D hardware checks.
- Specific rights/redistribution approval, licence record and required notices **before any proprietary Dunkelkammer code is copied into the public fork**.

Implementation/NDK checks and final pre-ready reviews remain separate. The accepted architectural scope must not be silently expanded; material changes require a new superseding ADR.

## Review status

- Initial architecture review: `approve_with_followups` (2026-10-08); see [review report](../../ports/amigaos3/docs/reviews/ADR-0001-architecture-review-2026-10-08.md).
- User acceptance: `approved` (2026-10-08; direct chat instruction).
- Final pre-ready review: pending, to be performed for the first implementation story.
- Implementation review: scoped GCC13.3/vamos CPU-core test spike PF-SP-003 technically passed and archived (PF-TN-003; 2026-10-09); main PhotoCraft implementation review still pending (PF-SP-001/002).
- Documentation sync: scope and port metadata updated with this acceptance; original upstream Rust workspace remains unchanged.

