# PF-SP-004 — Change summary (2026-10-11)

- Feature: `pc_document_shift_layer` implementiert Rust-kompatible bottom-first Layer-Bewegung inklusive Grenzfehler, stabile IDs; `opacity` und `fill_opacity` sind unabhängige endliche f32-Metadaten, werden beim Clone erhalten.
- PF-BG-001: `pc_document_append_raster` behaelt den exklusiven Pointer-Claim auch ueber Dokumente; eigenstaendige COW-Klone.
- In Scope: portable C99 Document-Prototyp, Host-Sanitizer, Original-Rust-Oracle, m68k/vamos; kein Compositor, Group, Mask, PSD, native GUI, Produktions-ABI oder Release.
- PR #3 merged `e3b1c183ce1f1b3dece8a72728a8c811222bed77`; Code-Head `40ec4a17d023954a6e2752b4f676d77ba35a2143` mit [CI #38159331127](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159331127) und [Host #38159331128](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159331128) `success`.
- Architektur: ADR-0001/0002/0003 unveraendert, `ip_provenance_impact: reference_only`, kein Third-Party-Shipping.
- Story und Test archiviert: [Story](../stories/done/PF-SP-004.md), [Test](../tests/done/PF-TN-005.md).
