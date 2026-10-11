# PF-TN-015 — Kanonischer README-/Story-Status-Abgleich (verifiziert)

- Story: [PF-TD-002](../../stories/done/PF-TD-002.md)
- Status: `pass` (2026-10-11; kanonischer Status-/Link-/Backlog-Audit, PF-/Docs-Checks und General CI auf finalem PR-Head success, archiviert)
- Gate: reine Dokumentationsregression, nicht ein Runtime-Test.

## Testfaelle

1. Gegen den tatsaechlich gemergten `main`-Stand pruefen: `ports/amigaos3/` ist vorhanden; die Root-README nennt nicht nur den Branch `amigaos32`.
2. Gegen `docs/backlog.md` und `docs/stories/PF-SP-002.md` pruefen: historischer Parent `blocked` seit 2026-10-11, `split_required`, keine versehentliche `in_progress`-Promotion durch port-lokale Altakte.
3. Pruefen, dass weiterhin kein funktionsfaehiger AmigaOS-PhotoCraft-Editor, freigegebener Produktcompiler, freier MiniGL-SDK-Redistributionsanspruch oder erfolgreich ausgefuehrter WinUAE-Test behauptet wird.
4. Linkziele der neu geaenderten README-/Port-Stellen und ADR-/Story-Links nachvollziehen.
5. Bestehende Dokumentations-/Backlog-Pruefung in GitHub Actions nachweisbar ausfuehren. Falls Checks nicht starten koennen: Ursache/Blocker statt PASS dokumentieren.

## Test-/Review-Protokoll

- Build-/Branch-SHA: `1fe904c7f11f53ff3fb748e3bb35b57ef260fe46` fuer urspruenglichen `in_progress`-Check; letzter dokumentierter Full-CI-Head vor dieser Review-Korrektur `179dbda0e679d5c11d7c40b18720b854e44bc2f4`.
- Dokumentationsdiff: Root-/Port-README und kanonische PF-TD-002-Story-/Backlog-Akte; kein Source-/Binary-Produktcode.
- Link-/Status-Check: Urspruengliche acht Assertions PASS (auf `in_progress`-Stand); finale `review`-Backlog-Pruefung gesondert unten.
- CI-Run/Exitcode: [#38127048797](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38127048797) `success` auf altem PR-Head; spaeterer neuer Review-Korrektur-Head separat nachzuweisen.
- Dokumentationsevidenz: PF Preflight #38127048802 und Documentation #38127048787 `success`.
- Review: `approve` (siehe finalen Story-Review PF-TD-002)

Die Story wurde ueber das dokumentierte Implementation-Gate umgesetzt und nach erfolgreichen PF-/Docs-/CI-Pruefungen sowie Finalreview als `done` archiviert; Release bleibt unfreigegeben.

## Ausgefuehrte Pruefung 2026-10-11

- Source-Commit: `1fe904c7f11f53ff3fb748e3bb35b57ef260fe46` (Dokumentations-Code-Diff)
- Erfasster Istzustand: Root-README verweist auf `main`, `amigaos32` ist Entwicklungsbranch, der nicht lauffaehige Port wird klar benannt.
- **Historische** Connector-Pruefung auf Source-Commit `1fe904c7`: Acht von acht Assertions PASS, darunter die damalige einmalige `in_progress`-Backlog-Zeile. Dies ist **nicht** die Verifikation der spaeteren `review`-Transition.
- Existenz der referenzierten lokalen Dateien `ports/amigaos3/README.md`, `docs/backlog.md`, `docs/stories/PF-SP-001.md`, `docs/stories/PF-SP-002.md` und `docs/stories/PF-TD-002.md` ueber GitHub-Dateifetch verifiziert.
- [PF Bootstrap Docs Preflight #38126908022](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38126908022) — `success`, Public PF documentation gate.
- [Documentation #38126908029](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38126908029) — `success`, mdBook build.
- Vollstaendiger CI-Lauf [#38127048797](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38127048797) auf dem frueheren Head `179dbda` erfolgreich abgeschlossen. Kein vorweggenommener Erfolg auf dem durch diese Korrektur entstehenden SHA.
- Keine AmigaOS-GUI-/MiniGL-Laufzeittests erforderlich, weil keine Ausfuehrungslogik geaendert wurde.
- Finaler getrennter fachlicher Review und Merge von PR #10 dokumentiert. Keine selbst erteilte GitHub-APPROVE-Submission und keine Release-Freigabe.

## Finaler Testabschluss — 2026-10-11

- Finaler PR-Head `4af4441828f431c163a4d27013b378fb57e14f5f`, [Merge #10](https://github.com/HurricanVD/photocraft-amiga/pull/10) unter `7621c7fe814d73aec58ec7561e7ef612e23e185e`.
- [GitHub CI #38141046023](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38141046023): `completed/success`.
- [Documentation #38141046048](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38141046048): `completed/success`.
- [PF Bootstrap Docs Preflight #38141046033](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38141046033): `completed/success`.
- [AmigaOS 3.2 Host Core #38141046042](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38141046042): `completed/success` auf dem integrierten PR-Head.
- Nach dem Konfliktmerge wurde dokumentiert verifiziert: `PF-BG-001` und `PF-TD-002` standen jeweils genau einmal in der Sektion `Aktive PF-Stories` als `review`; Root-/Port-README verweisen korrekt auf `main`, der historische Parent PF-SP-002 ist weiterhin `blocked`. Mit der Archivierung stehen beide ausschließlich in `backlog-done.md`.
- Manueller Anwendungstest: `n/a` (reine Dokumentation), automatisierte Checks als Regression. Source-/Produktcode, NDK, MiniGL und Release waren nicht Teil dieser Story.
