# PF-SP-016 — Native Kontextmenues und begrenzte ReAction-Dialoge adaptieren

- Story-ID: `PF-SP-016`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `ready` (scoped static pre-ready review 2026-10-12)
- Prioritaet: `P2`
- Entry-Profil: `standard`
- Datum: 2026-10-12
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-020` (geplant, `not_run`)
- Dunkelkammer Quell-Pin: `26295506ba363b1bd52bfca1852b0230ffb2bad9` (`main`, 2026-08-20)

## 1. Problem / Nutzerwert

PhotoCraft benoetigt fuer die ersten nativen Layeroperationen kontextsensitive Aktionen und Parametereingaben. Dunkelkammer besitzt einen separaten Intuition-Popup und ein Slider-/Integer-Dialogmuster; beide sind schneller adaptierbar als eine Neuentwicklung.

## 2. In Scope

- Domainfreie Popup-Helfer aus src/ui/dk_ctx_menu.c/.h und Text-Messhelfer aus dk_ui_draw.c/.h fuer PhotoCraft-Namen/Command-IDs adaptieren.
- Einen begrenzten, abbrechbaren ReAction-Slider-/Zahlen-Dialog aus src/ui/dk_ui_dialogs.c fuer beispielsweise Deckkraft an den PF-SP-015-Command-Adapter anbinden.
- Klick-/Hold-Popup, Popup-Lebenszyklus, Screen-Clipping, Fokus-/Abbruchregeln, ungültige Parameter und explizite Commit-Grenzen nach PhotoCraft-Kontrakt kapseln.
- Popup-Vorlagen aus virtualcommander separat provenienzpruefen (PF-SP-012), keine fremden GUI-Assets kopieren.

## 3. Nicht-Ziele

- Keine Uebernahme des gesamten Dunkelkammer-Dialogsystems oder seiner Effekt-/History-Operationen.
- Keine proprietaeren Menue-Assets oder fremde ungeklaerte Lizenzanteile.
- Keine neue Render-/Raster-Engine.

## 4. User Story

Als PhotoCraft-Amiga-Nutzer moechte ich native Kontextaktionen und einfache Dialoge verwenden, ohne dass Abbrechen mein Bild veraendert.

## 5. Akzeptanzkriterien

1. RMB-Popup trifft den richtigen Layer, bleibt innerhalb des Screens und liefert eindeutige Command-IDs; Escape und Klick ausserhalb brechen folgenlos ab.
2. Slider und Zahlenfeld synchronisieren sich, pruefen 0..100 und schreiben erst bei OK; Cancel laesst Dokument und History unveraendert.
3. Fehlende Klassen, Out-of-Memory und Fensterverluste beenden die UI ohne Leaks oder haengende Modalfenster.
4. Host-Parameter-/Abbruchtests, m68k-Build und manueller WinUAE-Test fuer Kontext-/Dialog-Lifecycle sind dokumentiert.

## 6. Architektur, Abhaengigkeiten und Grenzen

- Ziel: `ports/amigaos3/` (C99-Ausnahme), AmigaOS 3.2, 68040+, RTG/P96, 32 MiB Fast-RAM-Baseline.
- Abhaengigkeiten: PF-SP-012 (Lizenz/Provenienz), PF-SP-011 (Shell), PF-SP-013 (Ebenenliste), PF-SP-015 (Command-Bridge), PF-SP-008 fuer History-wirksame Updates.
- PhotoCraft Rust bleibt semantische Referenz; 256x256-Sparse-Tiles, Gruppen, Masken und Commands gehoeren dem PhotoCraft-Kern. Kein Ersatz durch Dunkelkammer-Dokument-/Rastermodell.
- Provenienz: Dunkelkammer src/ui/dk_ctx_menu.c/.h, dk_ui_draw.c/.h, dk_ui_dialogs.c @ source pin.
- Explizite Eigentuemeraussage zur Wiederverwendung liegt vor; bei direkter Veroeffentlichung kopierter proprietaerer Quelldateien ist der dateibezogene Lizenz- und Notice-Nachweis aus PF-SP-012 vor der Codeuebernahme erforderlich.
- Der Code-Import ist kein Scope dieses Dokumentations-PRs. Kein MiniGL als harte Abhaengigkeit; keine Produkt-/ABI-/Release-Freigabe.

## 7. Tests und Evidenz

- PF-TN-020: Popup-Event-FSM, Hit-Test, Cancel-vs-Commit, Invalid/OOM, WinUAE-Dialog.
- Host-/ASan/UBSan-Tests fuer portablen Teil, echte m68k-HUNK/vamos-O0/O2-Smokes fuer anwendbaren Teil; GUI/Presenter erfordert zusaetzlich manuellen WinUAE-AmigaOS-3.2-Nachweis.
- Prozessgesteuerte automatisierte WinUAE-UI-/Broker-/Capture-Skripte bleiben `disabled_by_process` und werden nicht ausgefuehrt.
- Alle neuen Tests und Runtime-Checks: `not_run`; keine PASS-Evidenz aus Dunkelkammer wird als PhotoCraft-Port-PASS ausgegeben.

## 8. Refinement / atomization_report

- `refinement_triage`: `pass`
- `atomization_report.decision: keep_single_story` (vorlaeufig, formaler Scope-/IP-/Architekturreview offen).
- Aenderungsachse: Native Kontextmenues und begrenzte ReAction-Dialoge adaptieren.
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

- Separate, bounded delivery: Popup/dialog widgets with Cancel-no-mutate and OK-once, correct focus/OOM lifetime, preserve MIT ancestor notice.
- Architecture and DoR: [final pre-ready report](../reviews/PF-all-open-ready-review-2026-10-12.md); native self-contained C99 PhotoCraft AmigaOS 3.2, no host/server, accepted ADR-0001/0002/0003.
- License: Dunkelkammer owner code is MIT (merge PR #9), VirtualCommander/vxplatform notices and third-party rights retained. The [source inventory](../../ports/amigaos3/docs/dunkelkammer-ui-source-inventory.md) is an *input to*, not a substitute for, the specific import diff review.
- No implemented changes, no runtime/GUI tests, `implementation_gate_report: pending` and WIP limit unchanged. Any dependency gap blocks the later implementation gate, not a fabricated PASS.
