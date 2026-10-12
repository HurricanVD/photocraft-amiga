# PhotoCraft-Amiga Backlog (canonical)

- Datum: 2026-10-12. Geltungsbereich: ausschliesslich der isolierte `ports/amigaos3/`-Fork, kein Update des originalen Rust-Workspaces.
- Produktziel: vollstaendig autarke AmigaOS-3.2-Anwendung. Kein Host-Rechenserver, kein Cloud- oder Remote-Backend. Ein nativer ReAction-/RTG-M2-Pfad zuerst, MiniGL optional.
- Quelle des Status: Einzelstories, akzeptierte ADR-0001/0002/0003 und [PF-all-open-ready-review 2026-10-12](reviews/PF-all-open-ready-review-2026-10-12.md).
- WIP-Limit: maximal zwei `in_progress` gleichzeitig (unveraendert).
- `ready` = Umfang/DoR/Architektur/IP fuer die Umsetzung vorbereitet, **nicht** implementiert. Geplante Tests sind `not_run`, der individuelle Implementierungsstart verlangt separate Implementation-Gates.

## Prioritaeten

- `P0`: bestaetigte absturz-/datenkorrumpionskritische Ursachen; keine ungeprueften Neuzuweisungen.
- `P1`: kritischer Weg zum lokalen Open/Render/Edit/Undo/PNG-Export und RAM/OOM-Sicherheit sowie Upstream-Semantik.
- `P2`: GUI-Komfort und Layered-Persistenz-Spikes, die fuer erste Offline-PNG-Editierung nicht zwingend sind.
- `P3`: nicht notwendige GPU-Runtime und eingefrorener historischer Parent.

## Review-Priorisierung vom 2026-10-12 (PR #18 beruecksichtigt)

Die aus dem Code-Review erkannten Raster-Risiken sind **keine neuen Stories**: `pc_raster_write_region` ist bei spaeterem Allocator-Ausfall nicht transaktional und `pc_find` sucht linear; beides gehoert bereits zu **PF-TD-001 (P1)**. Es ist ein Risiko bzw. unzureichender Fehlerpfad, **kein** durch Fault-Injection bestaetigter Datenverlustfall; deshalb ohne neue Evidenz nicht P0.

- **P1 / Reihenfolge A – Core-Sicherheit:** PF-TD-001 (Region-Transaktionen, COW/Allocator-Injektion, Tile-Index/Benchmark), gefolgt von PF-SP-008 (Undo-Transaktionen) und PF-SP-007 (Compositor-Restparitaet). PF-PR-001 stuetzt fortlaufend die Rust/C99-Semantik.
- **P1 / Reihenfolge B – Native Integration:** PF-SP-010/011 sind auf PR #18 bereits als begrenzter Preview-Subset integriert; offen bleiben volle Story-Gates, Refresh/RTG-Fehlerpfade und dokumentierte reproduzierbare Target-Abnahme. PF-SP-012 (IP/Provenienz) vor jeder weiteren Codeuebernahme; danach PF-SP-013/014/015 fuer echte Bedienung.
- **P1 / Reihenfolge C – Nutzbarer Offline-MVP:** PF-SP-017 (PNG/IFF-Import) -> PF-SP-018 (PNG-Export) und PF-SP-019 (Brush, gekoppelt mit PF-SP-008) -> PF-SP-020 (End-to-End-Target-Abnahme). Die vorhandenen Storys enthalten diese Umsetzung bereits; keine Dubletten.
- **P2 / neue abgegrenzte Follow-ups:** PF-TD-003 (Ressourcenbilanz: Leak bislang nicht bestaetigt), PF-TD-004 (Status-/Evidence-Synchronisierung nach Preview-Merge). Die bisherigen PF-SP-009/016/021 bleiben P2.
- **P3:** PF-SP-001 (optionales MiniGL) und PF-SP-002 (historischer blockierter Parent).

Die Einordnung als `integrated_preview_subset` ist ein **Implementierungsbefund**, kein neuer Lifecycle-Status: `ready` und nicht abgeschlossene Gesamtakzeptanz bleiben bestehen. Der Owner-Waiver zum Low-Fast-RAM-Preview-Test ist kein bestandenes OOM-Gate. Priorisierung autorisiert weder Implementierung noch Release und aendert kein WIP-Limit.

## Bereit

| ID | Titel | Typ | Prioritaet | Status | Notiz |
|---|---|---|---|---|---|
| PF-PR-001 | Upstream-Delta-Tracking und Rust/C99-Oracle | PR | P1 | ready | PF-TN-026 geplant |
| PF-SP-007 | CPU-Compositor-Paritaetsspike | SP | P1 | ready | Nach Gruppen-/Masken-Basis, keine Implementation; DoR 2026-10-12, tests not_run |
| PF-SP-008 | Command-/Undo-Redo-Prototyp | SP | P1 | ready | Eigenstaendiges Modell-/History-Gate; DoR 2026-10-12, tests not_run |
| PF-SP-010 | Optionaler RTG-Software-Viewport | SP | P1 | ready | PR #18 M2-Preview-Subset integriert; volle Presenter-/RTG-/Fehlerpfad-Abnahme bleibt offen |
| PF-SP-011 | Minimale ReAction-Fenster- und Event-Shell | SP | P1 | ready | PR #18 native Shell/Workbench-Preview beobachtet; volle Lifecycle-/Target-Abnahme offen; PF-TD-003 ist nur Attribution |
| PF-SP-012 | Dunkelkammer-UI-Reuse: Lizenz, Provenienz, Modulgrenze | SP | P1 | ready | PF-TN-016 geplant; vor direktem oeffentlichen Source-Import; DoR 2026-10-12, tests not_run |
| PF-SP-013 | BOOPSI-Ebenenliste fuer PhotoCraft | SP | P1 | ready | PF-TN-017 geplant; Gruppen, Scroll, stabile IDs; DoR 2026-10-12, tests not_run |
| PF-SP-014 | Canvas-Navigation und Pointer-Queue | SP | P1 | ready | PF-TN-018 geplant; nach PF-SP-010/011; DoR 2026-10-12, tests not_run |
| PF-SP-015 | Toolbar/Menue-Command-Adapter | SP | P1 | ready | PF-TN-019 geplant; nach PF-SP-008/011/013; DoR 2026-10-12, tests not_run |
| PF-SP-017 | PNG-/IFF-Bildimport in natives PcDocument | SP | P1 | ready | PF-TN-021 geplant |
| PF-SP-018 | RGBA8-CPU-Komposit als PNG exportieren | SP | P1 | ready | PF-TN-022 geplant |
| PF-SP-019 | Brush-Tool mit einem History-Commit | SP | P1 | ready | PF-TN-023 geplant |
| PF-SP-020 | Autarker AmigaOS-3.2-MVP: Open/Edit/Undo/Export | SP | P1 | ready | PF-TN-024 geplant |
| PF-TD-001 | Tile-Lookup, RAM/OOM-Rollback | TD | P1 | ready | **P1-A / zuerst**: Region-Transaktionen und indexed Tile-Lookup bereits im Scope; Fault-Injection, COW-Paritaet und RAM-Budget noch nicht abgenommen |
| PF-SP-009 | Serialisierungs-Roundtrip-Spike | SP | P2 | ready | Format- und IP-Vertrag offen; DoR 2026-10-12, tests not_run |
| PF-SP-016 | Native Kontextmenues und ReAction-Dialoge | SP | P2 | ready | PF-TN-020 geplant; nach PF-SP-011/013/015; DoR 2026-10-12, tests not_run |
| PF-SP-021 | Layered-Projektformat-Spike | SP | P2 | ready | PF-TN-025 geplant |

## Offen

| ID | Titel | Typ | Prioritaet | Status | Notiz |
|---|---|---|---|---|---|
| PF-TD-003 | ReAction/RTG-Ressourcenbilanz reproduzierbar pruefen | TD | P2 | draft | Eigenstaendige Analyse/Attribution; PF-SP-011 deckt Lifecycle-Abnahme bereits ab; Leak nicht bewiesen |
| PF-TD-004 | Integrationsstatus und Testevidenz nach Preview-Merge konsolidieren | TD | P2 | draft | Neues Delta nach archivierter PF-TD-002; PF-SP-007/010/011 Preview-Subset gegen Full-Gates abgleichen |

## Blockiert

| ID | Titel | Typ | Prioritaet | Status | Blocker |
|---|---|---|---|---|---|
| PF-SP-001 | MiniGL / QuarkTex NG | SP | P3 | blocked | MiniGL/QuarkTex/WinUAE target gate ausstehend |
| PF-SP-002 | PhotoCraft Core-Port (historischer Parent) | SP | P3 | blocked | split_required; siehe atomization_report, keine weitere Implementierung |

## Aktive PF-Stories

| ID | Titel | Art | Priorität | Status |
|---|---|---|---|---|

## Umsetzungskette und Testnachweise

1. PF-SP-012 (MIT-Notices) und PF-TD-001 (RAM/OOM) schaffen die Baseline; PF-SP-010 (RTG-Presenter) und PF-SP-011 (native ReAction-Shell) koennen ein synthetisches Testbild zeigen, ohne PF-SP-007 schon fertig zu haben.
2. PF-SP-007 CPU-Kompositor, PF-SP-013 Ebenenliste, PF-SP-014 Pointer/Viewport, PF-SP-015 Command-Bridge und PF-SP-008 Undo verbinden das PhotoCraft-Dokumentmodell mit der nativen UI.
3. PF-SP-017 PNG/IFF-Import, PF-SP-018 PNG-Export, PF-SP-019 erster Brush; danach PF-SP-020 lokaler End-to-End-Open/Edit/Undo/Export-Test unter AmigaOS 3.2.
4. PF-PR-001 erkennt Upstream-Rust-Deltas und testet Semantik/Oracles bei jedem Aktualisierungsversuch. Code-Uebernahme bleibt reviewpflichtig.
5. PF-SP-016 Menues/Dialogkomfort, PF-SP-009 und PF-SP-021 geschichtete Persistenz-Spikes nach erster PNG-MVP-Funktion. Ein stabiles Produktdateiformat benoetigt gesonderte ADR-/Freigabeentscheidung.
6. PF-SP-001 MiniGL und PF-SP-002 historischer `split_required`-Parent bleiben `blocked`; kein `ready` ohne je eigene Blockerloesung. Bereits `done` archivierte PF-SP-003/004/005/006, PF-BG-001, PF-TD-002 verbleiben ausschliesslich in [backlog-done.md](backlog-done.md).

## Architektur und IP

- [Native Port-Architektur](../ports/amigaos3/docs/architecture.md); [Dunkelkammer-MIT-Quellinventar](../ports/amigaos3/docs/dunkelkammer-ui-source-inventory.md); [PhotoCraft Rust-Referenz](architecture.md).
- Dunkelkammer-Eigencode ist MIT nach [Merge PR #9](https://github.com/HurricanVD/dunkelkammer/pull/9), Copy nur mit Copyright-/Permission-Notices, Third-Party-Lizenzen und konkretem Source-SHA.
- `ready` macht weder Native-GUI-Tests noch produktives GCC-Toolchain-Profil oder Shipping-Format zu PASS. WinUAE GUI nur manuell nach realem Build, automatisierter WinUAE Broker weiterhin `disabled_by_process`.
