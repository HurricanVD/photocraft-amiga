# PF-SP-016 — Native Kontextmenues und begrenzte ReAction-Dialoge adaptieren

- Story-ID: `PF-SP-016`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `draft` (keine Implementierungsfreigabe)
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

- `refinement_triage: needs_architecture`; `atomization_report.decision: keep_single_story` (vorlaeufig, formaler Scope-/IP-/Architekturreview offen).
- Aenderungsachse: Native Kontextmenues und begrenzte ReAction-Dialoge adaptieren.
- `rights_status: pending_PF-SP-012`.
- `arch_review.initial: pending`.
- Vor `ready`: separate `arch_review.pre_ready_final: pass` und `dor_check: pass` mit echten Belegen erforderlich; WIP-Limit bleibt unveraendert.

## 9. Review-/Implementierungsgates

- `arch_review.pre_ready_final: pending`
- `dor_check: pending`
- `implementation_gate_report: missing/pending`
- `review_report: pending`
- `test_evidence: not_run`
- `done: not_authorized`
