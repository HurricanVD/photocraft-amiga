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
| PF-SP-011 | Minimale ReAction-Fenster- und Event-Shell | SP | P1 | draft | PF-TN-014 geplant; Dunkelkammer Window-/Event-Lifecycle; Testbild nach PF-SP-010 |
| PF-SP-012 | Dunkelkammer-UI-Reuse: Lizenz, Provenienz, Modulgrenze | SP | P1 | draft | PF-TN-016 geplant; vor direktem oeffentlichen Source-Import |
| PF-SP-013 | BOOPSI-Ebenenliste fuer PhotoCraft | SP | P1 | draft | PF-TN-017 geplant; Gruppen, Scroll, stabile IDs |
| PF-SP-014 | Canvas-Navigation und Pointer-Queue | SP | P1 | draft | PF-TN-018 geplant; nach PF-SP-010/011 |
| PF-SP-015 | Toolbar/Menue-Command-Adapter | SP | P1 | draft | PF-TN-019 geplant; nach PF-SP-008/011/013 |
| PF-SP-008 | Command-/Undo-Redo-Prototyp | SP | P2 | draft | Eigenstaendiges Modell-/History-Gate |
| PF-SP-016 | Native Kontextmenues und ReAction-Dialoge | SP | P2 | draft | PF-TN-020 geplant; nach PF-SP-011/013/015 |
| PF-SP-009 | Serialisierungs-Roundtrip-Spike | SP | P3 | draft | Format- und IP-Vertrag offen |

## Blockiert

| ID | Titel | Typ | Prioritaet | Status | Blocker |
|---|---|---|---|---|---|
| PF-SP-001 | MiniGL / QuarkTex NG | SP | P2 | blocked | MiniGL/QuarkTex/WinUAE target gate ausstehend |
| PF-SP-002 | PhotoCraft Core-Port (historischer Parent) | SP | P3 | blocked | split_required; siehe atomization_report, keine weitere Implementierung |

## Aktive PF-Stories

| ID | Titel | Art | Priorität | Status |
|---|---|---|---|---|

Statusquelle: [PF-SP-002-Atomisierung](reviews/PF-SP-002-atomization-report-2026-10-11.md) und [Prioritaets-/Ready-Audit 2026-10-11](reviews/PF-backlog-priority-ready-audit-2026-10-11.md). `PF-BG-001` und `PF-TD-002` haben dokumentierte DoR/Pre-ready-Nachweise. PF-SP-004/005/006 wurden nach separaten Child-Gates, finalen Tests und den Merges #3/#4/#5 am 2026-10-11 als `done` archiviert; die vor der Atomisierung entstandenen Parent-Commits bleiben nur historische WIP-Evidenz. Die aktuellen, alleinigen Done-Zeilen stehen in [backlog-done.md](backlog-done.md). `ready` ist **keine** Freigabe fuer `in_progress`, Merge oder Release. PF-SP-002 bleibt blocked/frozen; WIP-Limit VD v0.2.1 beachten.

Hinweis zu späteren Lifecycle-Änderungen: `draft|refining` gehören unter **Offen**, `ready` unter **Bereit**, `blocked|rejected` unter **Blockiert**, `in_progress|review` in die **Aktive PF-Stories**-Tabelle; abgeschlossene IDs wandern ausschließlich nach `docs/backlog-done.md` und `docs/stories/done/`. Die PF-Validierung verlangt vor jeder `ready`-Promotion `refinement_triage: pass`, `arch_review.pre_ready_final: pass` und `dor_check: pass`; vor `in_progress` zusätzlich `implementation_gate_report: pass`, vor `done` außerdem `review_report: approve`. Diese Marker dürfen nur nach real dokumentierten Gates gesetzt werden.

## Geplantes native-UI-Reuse-Paket (2026-10-12)

- Ziel: **vollstaendig autarke** PhotoCraft-Anwendung fuer AmigaOS 3.2/m68k; keine Remote-Engine, kein Host-Rechenservice und kein MiniGL-Zwang.
- Vorhandene Stories behalten ihre IDs: [PF-SP-010](stories/PF-SP-010.md) (RTG-Software-Presenter) und [PF-SP-011](stories/PF-SP-011.md) (ReAction-Shell). Sie pruefen die Wiederverwendung statt weitere doppelte Presenter-/Shell-Stories zu erzeugen.
- Neue, getrennt lieferbare Draft-Stories: [PF-SP-012](stories/PF-SP-012.md) (Rechte/Provenienz), [PF-SP-013](stories/PF-SP-013.md) (BOOPSI-Ebenenliste), [PF-SP-014](stories/PF-SP-014.md) (Viewport-Navigation), [PF-SP-015](stories/PF-SP-015.md) (Menues/Commands), [PF-SP-016](stories/PF-SP-016.md) (Kontext/Dialogs).
- Rueckgriff auf `HurricanVD/dunkelkammer` Quellstand `26295506ba363b1bd52bfca1852b0230ffb2bad9`; keine Codekopie, Assets oder Lizenzentscheidung in dieser Planungsintegration. Vor direkter Quelluebernahme ist PF-SP-012 nachweislich abzuschliessen. Native GUI-/Presenter-Abnahme ausschliesslich nach echten Zielsystemtests.
- Technische Kette: synthetischer RTG-Presenter (PF-SP-010) -> Fenster-Testansicht (PF-SP-011) -> Navigation (PF-SP-014) und Layeranzeige (PF-SP-013) -> Command-Anbindung (PF-SP-015) -> Dialog/Popup (PF-SP-016). Fuer echte PhotoCraft-Komposition ist PF-SP-007 erforderlich; PF-SP-008 fuer reversible Befehle. PF-TD-001 bleibt eigenstaendiges RAM/OOM-Gate.
- Prioritaetsanpassung: PF-SP-011 von P2 auf P1, weil eine erste sichtbar lauffaehige native ReAction-Shell Teil des kritischen Lieferpfads ist. Kein `ready`, `in_progress` oder `done` ohne separate Prozessgates; WIP-Limit unveraendert.
