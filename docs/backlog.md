# Backlog

ID-Konvention: `PF-(PO|BG|SP|LL|PR|TD)-NNN` (Formatbeispiel, **keine zugeteilte Story**: `PF-PO-001`).
Die Nummer `NNN` ist pro Prefix und Story-Art fortlaufend dreistellig.

## Aktuelle Phase

- Phase: Erste technische AmigaOS-Portphase (C99-Kern, Rust-Parität, m68k-HUNK, danach Grafik-Spike)
- Ziel: Nachweisbare Portgrundlagen ohne Behauptung eines vollständigen AmigaOS-Editors

## Prioritaetslogik (Audit 2026-10-11)

- **P0:** potenziell absturz- oder speicherkorruptionskritischer Bugfix.
- **P1:** Speicher-/Core-Fundament, laufende Layer-PR-Kette und minimaler CPU-/RTG-Pfad.
- **P2:** darauf aufbauende oder optional blockierte Arbeiten und Dokumentationspflege.
- **P3:** spaete Persistenzarbeit oder eingefrorener historischer Parent.
- Begründung je Story und Gate-Grenzen: [Prioritaets-/Ready-Audit](reviews/PF-backlog-priority-ready-audit-2026-10-11.md).

## Bereit

| ID | Titel | Typ | Prioritaet | Status | Notiz |
|---|---|---|---|---|---|



## Offen

| ID | Titel | Typ | Prioritaet | Status | Notiz |
|---|---|---|---|---|---|
| PF-TD-001 | Tile-Lookup, RAM/OOM-Rollback | TD | P1 | draft | 32MiB-/Allocator-Fault-/Performance-Gate; OOM-Region-Transaktionen und Tile-Index bereits hier erfasst |
| PF-SP-007 | CPU-Compositor-Paritaetsspike | SP | P1 | draft | Nach Gruppen-/Masken-Basis, keine Implementation |
| PF-SP-010 | Optionaler RTG-Software-Viewport | SP | P1 | draft | PF-TN-013 geplant; CPU-/Presenter-Seam, unabhängig von MiniGL; nach PF-SP-007 |
| PF-SP-008 | Command-/Undo-Redo-Prototyp | SP | P2 | draft | Eigenstaendiges Modell-/History-Gate |
| PF-SP-011 | Minimale ReAction-Fenster- und Event-Shell | SP | P2 | draft | PF-TN-014 geplant; nach PF-SP-010, kein voller Editor |
| PF-SP-009 | Serialisierungs-Roundtrip-Spike | SP | P3 | draft | Format- und IP-Vertrag offen |

## Blockiert

| ID | Titel | Typ | Prioritaet | Status | Blocker |
|---|---|---|---|---|---|
| PF-SP-001 | MiniGL / QuarkTex NG | SP | P2 | blocked | MiniGL/QuarkTex/WinUAE target gate ausstehend |
| PF-SP-002 | PhotoCraft Core-Port (historischer Parent) | SP | P3 | blocked | split_required; siehe atomization_report, keine weitere Implementierung |

## Aktive PF-Stories

| ID | Titel | Art | Priorität | Status |
|---|---|---|---|---|
| PF-SP-006 | Gray8-LayerMask-Sparse-COW | SP | P1 | in_progress |

Statusquelle: [PF-SP-002-Atomisierung](reviews/PF-SP-002-atomization-report-2026-10-11.md) und [Prioritaets-/Ready-Audit 2026-10-11](reviews/PF-backlog-priority-ready-audit-2026-10-11.md). `PF-BG-001` und `PF-TD-002` haben dokumentierte DoR/Pre-ready-Nachweise. PF-SP-004 steht im `review`; PF-SP-005 und PF-SP-006 sind nach eigenstaendigen Child-DoR-/Implementation-Gates `in_progress` fuer ihren begrenzten aktuellen Stack-Scope. Die alten Parent-Commits bleiben historische WIP. `ready` ist **keine** Freigabe fuer `in_progress`, Merge oder Release. PF-SP-002 bleibt blocked/frozen; WIP-Limit VD v0.2.1 beachten.

Hinweis zu späteren Lifecycle-Änderungen: `draft|refining` gehören unter **Offen**, `ready` unter **Bereit**, `blocked|rejected` unter **Blockiert**, `in_progress|review` in die **Aktive PF-Stories**-Tabelle; abgeschlossene IDs wandern ausschließlich nach `docs/backlog-done.md` und `docs/stories/done/`. Die PF-Validierung verlangt vor jeder `ready`-Promotion `refinement_triage: pass`, `arch_review.pre_ready_final: pass` und `dor_check: pass`; vor `in_progress` zusätzlich `implementation_gate_report: pass`, vor `done` außerdem `review_report: approve`. Diese Marker dürfen nur nach real dokumentierten Gates gesetzt werden.
