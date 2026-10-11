# PF-SP-013 — BOOPSI-Ebenenliste aus Dunkelkammer fuer PhotoCraft adaptieren

- Story-ID: `PF-SP-013`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `ready` (scoped static pre-ready review 2026-10-12)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-12
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-017` (geplant, `not_run`)
- Dunkelkammer Quell-Pin: `26295506ba363b1bd52bfca1852b0230ffb2bad9` (`main`, 2026-08-20)

## 1. Problem / Nutzerwert

PhotoCraft hat einen getesteten Raster-/Gruppenbaum, aber kein bedienbares natives Ebenenpanel. Dunkelkammers domainfreie BOOPSI-Listenklasse besitzt bereits Auswahl, Augenicon, Hit-Test, Drag-Reorder und partielles Repaint; ihr aktueller Adapter erwartet jedoch Dunkelkammer-Layer und feste Grenzen.

## 2. In Scope

- Das kernunabhaengige Listen-Gadget als getrennte UI-Komponente portieren, mit expliziten Copyright/Provenienz-Notices; Quellbasis src/ui/dk_ui_layer_list.c/.h und bedarfsgerecht dk_ui_draw.c/.h.
- Einen neuen PhotoCraft-Adapter (nicht dk_ui_layer_panel.c unveraendert) fuer stabile PcDocument-Layer-IDs, bottom-first Modell- und top-first Display-Reihenfolge bereitstellen.
- Verschachtelte Gruppen und Layer im sichtbaren Baum eindeutig darstellen, zunaechst notfalls alle Gruppen expandiert; nur erlaubte sibling-only-Reorders anbieten, kein implizites Reparenting.
- Feste Dunkelkammer-Grenzen (16 Layer im Modell, 64 Rows im Gadget) nicht uebernehmen: sichtbare Zeilen/Scrollposition speicherbegrenzt verwalten und groessere Testbaeume sicher darstellen.

## 3. Nicht-Ziele

- Kein Ersatz von PcDocument durch DkDocument oder 128x128-Tiles.
- Keine neue Layer-Engine oder Cross-Group-Reparenting-Semantik.
- Keine neuen Photoshop-Layerfeatures oder Vollbild-UI.

## 4. User Story

Als PhotoCraft-Nutzer moechte ich meine Ebenen und Gruppen in einer nativen Amiga-Liste sehen und auswaehlen koennen, damit Dokumente auf dem Amiga bedienbar werden.

## 5. Akzeptanzkriterien

1. Flat-Layer-Reihenfolge, Gruppenverschachtelung, Sichtbarkeit und stabile IDs werden ohne Indexvertauschung dargestellt; Gruppe und Layer sind erkennbar.
2. Augenicon, Auswahl und sibling-only Drag-Reorder erzeugen jeweils genau eine korrekt zugeordnete Aktion; ungueltige Dropziele bleiben ohne Modellmutation.
3. 64+ angezeigte Eintraege sowie leere, tiefe und grosse Hierarchien bleiben scroll-/anzeigbar innerhalb des dokumentierten RAM-Budgets; kein stilles 16-/64-Layer-Limit.
4. Host-seitige Mapping-/Bounds-Tests, m68k-Build/HUNK/vamos und manuelle ReAction/WinUAE-Akzeptanz (inkl. Klasse freigeben nach Fenster-Dispose) sind dokumentiert.

## 6. Architektur, Abhaengigkeiten und Grenzen

- Ziel: `ports/amigaos3/` (C99-Ausnahme), AmigaOS 3.2, 68040+, RTG/P96, 32 MiB Fast-RAM-Baseline.
- Abhaengigkeiten: PF-SP-011 (Shell), PF-SP-012 (oeffentliche Codefreigabe), PF-SP-004/005/006 (done); mutierende Aktionen erst mit PF-SP-015 und ggf. PF-SP-008, bis dahin read-only.
- PhotoCraft Rust bleibt semantische Referenz; 256x256-Sparse-Tiles, Gruppen, Masken und Commands gehoeren dem PhotoCraft-Kern. Kein Ersatz durch Dunkelkammer-Dokument-/Rastermodell.
- Provenienz: Dunkelkammer src/ui/dk_ui_layer_list.c/.h, dk_ui_layer_panel.c/.h und dk_ui_draw.c/.h @ source pin; UI-Code vom Modell entkoppeln.
- Explizite Eigentuemeraussage zur Wiederverwendung liegt vor; bei direkter Veroeffentlichung kopierter proprietaerer Quelldateien ist der dateibezogene Lizenz- und Notice-Nachweis aus PF-SP-012 vor der Codeuebernahme erforderlich.
- Der Code-Import ist kein Scope dieses Dokumentations-PRs. Kein MiniGL als harte Abhaengigkeit; keine Produkt-/ABI-/Release-Freigabe.

## 7. Tests und Evidenz

- PF-TN-017: Model-/Display-Mapping, 64+-Zeilen, Group-Drag-Bounds, repeated window lifecycle und manuelle WinUAE-Bedienung.
- Host-/ASan/UBSan-Tests fuer portablen Teil, echte m68k-HUNK/vamos-O0/O2-Smokes fuer anwendbaren Teil; GUI/Presenter erfordert zusaetzlich manuellen WinUAE-AmigaOS-3.2-Nachweis.
- Prozessgesteuerte automatisierte WinUAE-UI-/Broker-/Capture-Skripte bleiben `disabled_by_process` und werden nicht ausgefuehrt.
- Alle neuen Tests und Runtime-Checks: `not_run`; keine PASS-Evidenz aus Dunkelkammer wird als PhotoCraft-Port-PASS ausgegeben.

## 8. Refinement / atomization_report

- `refinement_triage`: `pass`
- `atomization_report.decision: keep_single_story` (vorlaeufig, formaler Scope-/IP-/Architekturreview offen).
- Aenderungsachse: BOOPSI-Ebenenliste aus Dunkelkammer fuer PhotoCraft adaptieren.
- `rights_status: owner_MIT_confirmed; PF-SP-012_notices_pending`.
- `arch_review.initial`: `pass`
- Vor `ready`: separate `arch_review.pre_ready_final: pass` und `dor_check: pass` mit echten Belegen erforderlich; WIP-Limit bleibt unveraendert.

## 9. Review-/Implementierungsgates

- `arch_review.pre_ready_final`: `pass`
- `dor_check`: `pass`
- `implementation_gate_report`: `pending`
- `review_report: pending`
- `test_evidence: not_run`
- `done: not_authorized`

## MIT-Nachweis 2026-10-12

- Die Lizenz der Dunkelkammer-Eigenanteile ist seit [Merge PR #9](https://github.com/HurricanVD/dunkelkammer/pull/9) am Commit `70897e91c94214421b0595b3e858446d6b3586ea` **MIT** (Copyright 2026 Virtual Dimension). UI-Quellenstand: `26295506ba363b1bd52bfca1852b0230ffb2bad9`.
- Kopien lizenzierter Eigenanteile sind unter den MIT-Bedingungen moeglich. Vor Quellimport: Herkunft je Datei, MIT-Copyright-/Permission-Notice, VirtualCommander-/vxplatform-Anteile und unlizenzierte Fremdassets kontrollieren; [Quellmatrix](../../ports/amigaos3/docs/dunkelkammer-ui-source-inventory.md).
- Keine kopierten Quellen oder Tests in diesem Dokumentations-PR. Story weiterhin `draft`, Umsetzungsgates und WinUAE-Evidenz `not_run`.

## Pre-ready delta 2026-10-12

- Separate, bounded delivery: ReAction BOOPSI group/layer-tree view, stable PhotoCraft IDs, >64 rows, model-display order, safe drag.
- Architecture and DoR: [final pre-ready report](../reviews/PF-all-open-ready-review-2026-10-12.md); native self-contained C99 PhotoCraft AmigaOS 3.2, no host/server, accepted ADR-0001/0002/0003.
- License: Dunkelkammer owner code is MIT (merge PR #9), VirtualCommander/vxplatform notices and third-party rights retained. The [source inventory](../../ports/amigaos3/docs/dunkelkammer-ui-source-inventory.md) is an *input to*, not a substitute for, the specific import diff review.
- No implemented changes, no runtime/GUI tests, `implementation_gate_report: pending` and WIP limit unchanged. Any dependency gap blocks the later implementation gate, not a fabricated PASS.
