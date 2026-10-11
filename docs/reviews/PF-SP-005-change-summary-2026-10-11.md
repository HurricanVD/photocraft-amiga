# PF-SP-005 — Change summary (2026-10-11)

- Experimenteller `PcDocument`-Group-Tree: Root/Child bottom-first, global eindeutige IDs, Gruppenebenen bis 100, Sibling-Shift `pc_document_shift_layer`, Root-vs-Total-Layer-Counts und Deep-COW/Destroy mit unabhaengig besessenen PcRaster-Wrappern.
- Portseitiger PF-BG-001-Claim gilt fuer auch verschachtelte Owning-Appends und geklonte Nodes, nicht nur innerhalb eines Dokuments. Keine Maske oder Reparent-Funktion in diesem Scope.
- C99, Sanitizer, Original-Rust Group-Oracle und GCC13-HUNK/vamos O0/O2 mit echtem Compilerprofil bestanden auf `b2f6290f9fa81d8f0ccd5c269972ea341d399cfa`; [CI #38159390811](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159390811), [Host #38159390900](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159390900).
- [PR #4](https://github.com/HurricanVD/photocraft-amiga/pull/4) gemergt als `bbe1e019541c912b9d1abec3e129bd33d571c542`.
- Architektur: [Portbeschreibung](../../ports/amigaos3/docs/architecture.md) aktualisiert, ADR-0001..0003 weiterhin akzeptiert, ABI unfixiert, Rust Original `reference_only`, keine Drittassets.
- Story-/Testakte: [Story](../stories/done/PF-SP-005.md), [Test](../tests/done/PF-TN-006.md).
