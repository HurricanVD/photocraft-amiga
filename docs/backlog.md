# Backlog

ID-Konvention: `PF-(PO|BG|SP|LL|PR|TD)-NNN` (Formatbeispiel, **keine zugeteilte Story**: `PF-PO-001`).
Die Nummer `NNN` ist pro Prefix und Story-Art fortlaufend dreistellig.

## Aktuelle Phase

- Phase: Erste technische AmigaOS-Portphase (C99-Kern, Rust-Parität, m68k-HUNK, danach Grafik-Spike)
- Ziel: Nachweisbare Portgrundlagen ohne Behauptung eines vollständigen AmigaOS-Editors

## Bereit

| ID | Titel | Typ | Prioritaet | Status | Notiz |
|---|---|---|---|---|---|

## Offen

| ID | Titel | Typ | Prioritaet | Status | Notiz |
|---|---|---|---|---|---|
| PF-SP-004 | Flat-Layer-Reihenfolge und Deckkraft | SP | P1 | refining | PF-TN-005 / PR #3; vorlaeufige Implementierung, DoR offen |
| PF-SP-005 | Verschachtelte Rastergruppen und Deep-COW | SP | P1 | refining | PF-TN-006 / PR #4; haengt von PF-SP-004 ab |
| PF-SP-006 | Gray8-LayerMask-Sparse-COW | SP | P1 | refining | PF-TN-007 / PR #5; haengt von PF-SP-005 ab |
| PF-SP-007 | CPU-Compositor-Paritaetsspike | SP | P1 | draft | Nach Gruppen-/Masken-Basis, keine Implementation |
| PF-SP-008 | Command-/Undo-Redo-Prototyp | SP | P2 | draft | Eigenstaendiges Modell-/History-Gate |
| PF-SP-009 | Serialisierungs-Roundtrip-Spike | SP | P2 | draft | Format- und IP-Vertrag offen |
| PF-TD-001 | Tile-Lookup, RAM/OOM-Rollback | TD | P1 | draft | 32MiB-/Allocator-Fault-/Performance-Gate |

## Blockiert

| ID | Titel | Typ | Prioritaet | Status | Blocker |
|---|---|---|---|---|---|
| PF-SP-002 | PhotoCraft Core-Port (historischer Parent) | SP | P1 | blocked | split_required; siehe atomization_report, keine weitere Implementierung |

## Aktive PF-Stories

| ID | Titel | Art | Priorität | Status |
|---|---|---|---|---|
| PF-SP-001 | MiniGL / QuarkTex NG | SP | P1 | blocked |

Statusquelle: [PF-SP-002-Atomisierung](reviews/PF-SP-002-atomization-report-2026-10-11.md). Child-Stories sind `refining|draft`, nicht `ready|in_progress|done`. Keine weitere Implementierung auf Parent; WIP-Limit nach VD v0.2.1 beachten.
