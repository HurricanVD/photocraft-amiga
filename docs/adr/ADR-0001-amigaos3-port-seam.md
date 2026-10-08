# ADR-0001: Native AmigaOS 3.2 port seam

- Status: **proposed**
- Date: 2026-10-08
- Scope: `ports/amigaos3/` in `HurricanVD/photocraft-amiga`
- Decision type: far_reaching (OS target, language boundary, GUI/renderer and future toolchain)
- Approval / runtime evidence: pending

## Context

PhotoCraft is an upstream Rust 2024 application, with egui/eframe and wgpu/WGSL GPU layers. AmigaOS 3.2/68k does not have an established drop-in target for that complete stack. The fork must remain an identifiable **PhotoCraft port**, rather than replacing the data model with an existing Amiga editor.

The owner also maintains `HurricanVD/dunkelkammer`, with native ReAction, RTG M2 present, I/O and a proven 68k build/test methodology. That repository is currently proprietary; its implementation is not automatically redistributable in this public fork.

## Proposed decision

1. Preserve upstream PhotoCraft code and architecture as the behavioral oracle and primary source tree.
2. Keep AmigaOS-specific code isolated under `ports/amigaos3/`; C99 is permitted there for platform APIs and independent proof-of-concept tests. No modification to the Cargo workspace is required for initial probes.
3. Prove an actual Rust/m68k `std`-capable subset before promising that Rust PhotoCraft crates run natively. If not viable, port selected CPU algorithms/data-model semantics to C with reference fixtures and parity tests.
4. Use PhotoCraft's native 256x256 tiling and document model as the target contract; legacy Dunkelkammer ARGB32/128x128 models are staging adapters, not replacements.
5. Keep CPU reference compositing authoritative; add a MiniGL **viewport** backend for QuarkTex NG and compatible PiStorm3D runtime implementations. Keep a software/RTG fallback.
6. Treat ReAction event/UI primitives and M2 RTG lessons from Dunkelkammer as reusable platform knowledge. Any direct proprietary code copy into this public fork requires documented copyright-holder permission and appropriate public licensing.
7. WinUAE with QuarkTex NG is the first manual graphics test environment; separately test PiStorm3D on actual supported hardware. No unverified performance claims.
8. Use vd-amiga-dev-process v0.2.1 as a port-local proposed overlay; do not consume canonical PF story IDs until prefix reservation is recorded.

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

## Verification criteria before accepting this ADR

Host staging tests; actual m68k HUNK compilation; compatible MiniGL SDK/runtime evidence; correct RTG off-screen bitmap presentation without gadget corruption; documented approval for any proprietary code reuse. No acceptance or runtime success is implied by this draft.
