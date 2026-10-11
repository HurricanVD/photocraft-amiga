# PF-SP-015 — Native Toolbar und Menues an PhotoCraft-Commands binden

- Story-ID: `PF-SP-015`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `draft` (keine Implementierungsfreigabe)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-12
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-019` (geplant, `not_run`)
- Dunkelkammer Quell-Pin: `26295506ba363b1bd52bfca1852b0230ffb2bad9` (`main`, 2026-08-20)

## 1. Problem / Nutzerwert

Dunkelkammer besitzt ReAction-Toolbar, native Menues, Status/Progress-UI und Command-Routing. Eine einfache Uebernahme von dk_ui.c wuerde aber Dunkelkammer-Dokument- und History-Code mitziehen und PhotoCraft-Upstream-Semantik umgehen.

## 2. In Scope

- Schmalen PC-UI-Command-Adapter definieren: Command-ID, Parameter, enabled/capabilities, success/error, changed-region; IDs orientieren sich an der originalen PhotoCraft-Rust-Command-Registry.
- Native Toolbar/Menu/Shortcuts mit minimalen Aktionen anbinden: Tool-Selection als UI-State, Layer-Select, Sichtbarkeit und ggf. ein nachweislich implementierter Command.
- UI-Eventloop aus PF-SP-011 benutzt einen zentralen Dispatch; GUI fragt UI-/Core-Faehigkeiten vor Aktivierung ab, nicht implementierte Commands sind deaktiviert.
- Aus Dunkelkammer nur ReAction-Gadget-Aufbau, Toolbar-Hooks und lifecyclebezogene Patterns uebernehmen; Verflechtungen mit DkSession/DkCommand/DkHistory ausdruecklich entfernen.

## 3. Nicht-Ziele

- Kein Import der Dunkelkammer-Engine, Commands, 16-Layer-Annahmen oder kompletter GUI-Monolith.
- Kein eigenes Photoshop-Kommandoregister ohne Bezug zu PhotoCraft.
- Keine neuen Pixel-Algorithmen oder Reimplementierung der History in der UI.

## 4. User Story

Als PhotoCraft-Amiga-Nutzer moechte ich Menues, Shortcuts und Werkzeugleiste konsistent bedienen, damit alle sichtbaren Aktionen denselben PhotoCraft-Modellzustand verwenden.

## 5. Akzeptanzkriterien

1. Ein und dieselbe Funktion wird aus Menue, Toolbar und Shortcut genau einmal ueber denselben Command-/UI-State-Pfad ausgefuehrt.
2. Nicht vorhandene Befehle werden als disabled dargestellt; Fehler zeigen eine Meldung ohne Teilmutation oder doppelte History-Eintraege.
3. Mindestens Auswahl/Sichtbarkeit und ein freigegebener Modell-Command sind mit klaren Parametern und aktualisiertem Panel/Viewport integriert.
4. Host-Mapping-/Enabled-Tests, m68k-Smoke, echte WinUAE-Menue-/Shortcut-Akzeptanz und Nachweis ohne direkten DkCore-Zugriff dokumentiert.

## 6. Architektur, Abhaengigkeiten und Grenzen

- Ziel: `ports/amigaos3/` (C99-Ausnahme), AmigaOS 3.2, 68040+, RTG/P96, 32 MiB Fast-RAM-Baseline.
- Abhaengigkeiten: PF-SP-011 (Shell), PF-SP-012 (Freigabe), PF-SP-013 (Ebenenliste); PF-SP-008 (Command-/History-Vertrag) vor mutierenden History-relevanten Aktionen.
- PhotoCraft Rust bleibt semantische Referenz; 256x256-Sparse-Tiles, Gruppen, Masken und Commands gehoeren dem PhotoCraft-Kern. Kein Ersatz durch Dunkelkammer-Dokument-/Rastermodell.
- Provenienz: Dunkelkammer src/ui/dk_ui.c + dk_ui_internal.h + dk_ui_render.c als punktuelle Referenz/Extraktionsquelle; NICHT dk_ui.c en bloc.
- Explizite Eigentuemeraussage zur Wiederverwendung liegt vor; bei direkter Veroeffentlichung kopierter proprietaerer Quelldateien ist der dateibezogene Lizenz- und Notice-Nachweis aus PF-SP-012 vor der Codeuebernahme erforderlich.
- Der Code-Import ist kein Scope dieses Dokumentations-PRs. Kein MiniGL als harte Abhaengigkeit; keine Produkt-/ABI-/Release-Freigabe.

## 7. Tests und Evidenz

- PF-TN-019: UI/Menu/Shortcut-Pfadgleichheit, disabled-State, Command-Fehler, invalid input, WinUAE-ReAction.
- Host-/ASan/UBSan-Tests fuer portablen Teil, echte m68k-HUNK/vamos-O0/O2-Smokes fuer anwendbaren Teil; GUI/Presenter erfordert zusaetzlich manuellen WinUAE-AmigaOS-3.2-Nachweis.
- Prozessgesteuerte automatisierte WinUAE-UI-/Broker-/Capture-Skripte bleiben `disabled_by_process` und werden nicht ausgefuehrt.
- Alle neuen Tests und Runtime-Checks: `not_run`; keine PASS-Evidenz aus Dunkelkammer wird als PhotoCraft-Port-PASS ausgegeben.

## 8. Refinement / atomization_report

- `refinement_triage: needs_architecture`; `atomization_report.decision: keep_single_story` (vorlaeufig, formaler Scope-/IP-/Architekturreview offen).
- Aenderungsachse: Native Toolbar und Menues an PhotoCraft-Commands binden.
- `rights_status: owner_MIT_confirmed; PF-SP-012_notices_pending`.
- `arch_review.initial: pending`.
- Vor `ready`: separate `arch_review.pre_ready_final: pass` und `dor_check: pass` mit echten Belegen erforderlich; WIP-Limit bleibt unveraendert.

## 9. Review-/Implementierungsgates

- `arch_review.pre_ready_final: pending`
- `dor_check: pending`
- `implementation_gate_report: missing/pending`
- `review_report: pending`
- `test_evidence: not_run`
- `done: not_authorized`

## MIT-Nachweis 2026-10-12

- Die Lizenz der Dunkelkammer-Eigenanteile ist seit [Merge PR #9](https://github.com/HurricanVD/dunkelkammer/pull/9) am Commit `70897e91c94214421b0595b3e858446d6b3586ea` **MIT** (Copyright 2026 Virtual Dimension). UI-Quellenstand: `26295506ba363b1bd52bfca1852b0230ffb2bad9`.
- Kopien lizenzierter Eigenanteile sind unter den MIT-Bedingungen moeglich. Vor Quellimport: Herkunft je Datei, MIT-Copyright-/Permission-Notice, VirtualCommander-/vxplatform-Anteile und unlizenzierte Fremdassets kontrollieren; [Quellmatrix](../../ports/amigaos3/docs/dunkelkammer-ui-source-inventory.md).
- Keine kopierten Quellen oder Tests in diesem Dokumentations-PR. Story weiterhin `draft`, Umsetzungsgates und WinUAE-Evidenz `not_run`.
