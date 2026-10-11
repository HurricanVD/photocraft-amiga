# PF-SP-006 — Change summary (2026-10-11)

- Portierbarer C99-Scope: Gray8/U8-Masken mit sparse default 255/0, enabled/linked, fester density=1, feather=0; Attach in Root-/Group-/Raster-Knoten, Detach als expliziter Uebergang des exklusiven PcRaster-Claims.
- Der globale PF-BG-001-Claim verhindert Masken-/Raster-Alias auch ueber Dokumente hinweg; Clone erzeugt eigenen Mask-/Raster-Wrapper mit COW-Tile-Teilen, keine zweite Objektownership.
- Tests: gezielte Masken-Funktions-/Invalid-Format-/Negative-Coord-/Cross-Document-/Detach-Reattach-Regression, ASan/UBSan, originales Rust LayerMask-Oracle, GCC13 HUNK/vamos O0/O2.
- [PR #5](https://github.com/HurricanVD/photocraft-amiga/pull/5) gemergt `9fabe3440632feaf6d23a54b4c98e68fec72ff67`; PR-Head `93a542257f0475633d07cd599171147c69d0d95d` [CI #38159442928](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159442928), [AmigaOS Host #38159443000](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159443000) jeweils `success`.
- Dokumentation: [Portarchitektur](../../ports/amigaos3/docs/architecture.md) auf Group/Mask-COW ergaenzt, ADR-0001/0002/0003 weiterhin accepted; `reference_only` Rust-Oracle, keine Fremdassets oder Shipping-Freigabe.
- Kein CPU-Composite, Density/Feather-Algorithmen, ReAction/MiniGL, PSD, stable ABI oder Produktrelease.
- [Archivierte Story](../stories/done/PF-SP-006.md), [Test](../tests/done/PF-TN-007.md).
