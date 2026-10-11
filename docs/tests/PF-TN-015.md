# PF-TN-015 — Kanonischer README-/Story-Status-Abgleich (verifiziert)

- Story: [PF-TD-002](../stories/PF-TD-002.md)
- Status: `partial_verified`; Execution: `pass` fuer dokumentierte Status-/Link-/Docs-/PF-Checks am 2026-10-11; allgemeine CI noch offen
- Gate: reine Dokumentationsregression, nicht ein Runtime-Test.

## Testfaelle

1. Gegen den tatsaechlich gemergten `main`-Stand pruefen: `ports/amigaos3/` ist vorhanden; die Root-README nennt nicht nur den Branch `amigaos32`.
2. Gegen `docs/backlog.md` und `docs/stories/PF-SP-002.md` pruefen: historischer Parent `blocked` seit 2026-10-11, `split_required`, keine versehentliche `in_progress`-Promotion durch port-lokale Altakte.
3. Pruefen, dass weiterhin kein funktionsfaehiger AmigaOS-PhotoCraft-Editor, freigegebener Produktcompiler, freier MiniGL-SDK-Redistributionsanspruch oder erfolgreich ausgefuehrter WinUAE-Test behauptet wird.
4. Linkziele der neu geaenderten README-/Port-Stellen und ADR-/Story-Links nachvollziehen.
5. Bestehende Dokumentations-/Backlog-Pruefung in GitHub Actions nachweisbar ausfuehren. Falls Checks nicht starten koennen: Ursache/Blocker statt PASS dokumentieren.

## Test-/Review-Protokoll (auszufuellen)

- Build-/Branch-SHA:
- Dokumentationsdiff:
- Link-/Status-Check:
- CI-Run/Exitcode: `not_run`
- Dokumentationsevidenz: `not_run`
- Review: `pending`

Die Story ist nach gegenwaertigem DoR-Review `ready`, aber eine Umsetzung oder ein erfolgreicher Testlauf ist **nicht** behauptet.

## Ausgefuehrte Pruefung 2026-10-11

- Source-Commit: `1fe904c7f11f53ff3fb748e3bb35b57ef260fe46` (Dokumentations-Code-Diff)
- Erfasster Istzustand: Root-README verweist auf `main`, `amigaos32` ist Entwicklungsbranch, der nicht lauffaehige Port wird klar benannt.
- Connector-basierter Lesetest **PASS**: Root-README `main` und kanonische Links vorhanden; Port-README enthaelt historische Scope-Warnung; kanonische PF-SP-002 ist `blocked`, PF-SP-001 ist `blocked`; PF-TD-002 im Backlog steht genau **einmal** unter `in_progress`. Acht von acht Assertions PASS.
- Existenz der referenzierten lokalen Dateien `ports/amigaos3/README.md`, `docs/backlog.md`, `docs/stories/PF-SP-001.md`, `docs/stories/PF-SP-002.md` und `docs/stories/PF-TD-002.md` ueber GitHub-Dateifetch verifiziert.
- [PF Bootstrap Docs Preflight #38126908022](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38126908022) — `success`, Public PF documentation gate.
- [Documentation #38126908029](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38126908029) — `success`, mdBook build.
- Allgemeiner CI-Lauf [#38126908016](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38126908016) zur Zeit dieser Evidenzerfassung noch `in_progress`; kein vorweggenommenes PASS.
- Keine AmigaOS-GUI-/MiniGL-Laufzeittests erforderlich, weil keine Ausfuehrungslogik geaendert wurde.
- Unabhaengiger finaler Review und Merge: `pending`.
