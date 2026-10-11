# PF-TN-015 — Kanonischer README-/Story-Status-Abgleich (geplant)

- Story: [PF-TD-002](../stories/PF-TD-002.md)
- Status: `planned`; Execution: `not_run` (2026-10-11)
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
