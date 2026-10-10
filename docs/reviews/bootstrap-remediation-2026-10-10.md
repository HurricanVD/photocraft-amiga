# PF bootstrap – remediation / delta evidence

- Datum: 2026-10-10
- Consumer: `HurricanVD/photocraft-amiga`, branch `fix/pf-bootstrap-abnahme` (baseline `2a06a51`).
- Process baseline: `HurricanVD/vd-amiga-dev-process` `0566a39c28ee73285463f33eb72b210b5d215db4`, VERSION `0.2.1`.
- Review: Nachbesserungsnachweis, **keine unabhängige finale Freigabe**.
- PF registry: `reserved` (keine vorgezogene Aktivierung).

## Abnahmefindings F-01 bis F-03

| ID | Umsetzung | Reale Evidenz |
|---|---|---|
| F-01 | 23 lokale Overlay-Felder befüllt, Produktcompiler/GUI/Runtime nicht erfunden | `docs/development-process.md`; statische Gatechecks |
| F-02 | Fremde `VC`-Beispiele entfernt und PF-Stories nicht umklassifiziert | `docs/backlog.md`, `docs/backlog-done.md`, `docs/bootstrap/first-phase-backlog.md` |
| F-03 | Versiondrift, exhaustive Fixtures, `make check`, echter Bootstrap auf Wegwerf-Snapshot, vollständiger Consumer-Snapshot | [private Actions #38007435077](https://github.com/HurricanVD/vd-amiga-dev-process/actions/runs/38007435077) `success` für Consumer `1167e6f7`, private Validator-PR #18 |

## Review-Nacharbeiten (GitHub-Review am 2026-10-10)

| Reviewfinding | Korrektur | Prüf-/Entscheidungsstatus |
|---|---|---|
| Aktuelle technische Evidenz fehlte in kanonischem Bericht | `docs/bootstrap-report.md` nennt den vollständigen Snapshotlauf #38007435077 mit genauem geprüften SHA; spätere Änderungen benötigen erneuten final-head-Run. | Korrigiert; abschließende CI muss zum endgültigen PR-Head passen. |
| Public Preflight triggerte nicht bei Story-/Test-/ADR-/AGENTS-Änderungen | `.github/workflows/pf-bootstrap-validation.yml` läuft jetzt bei **jedem** Pull Request. | Geänderte Konfiguration, PR-Preflight erneut auszuführen. |
| Keine echte Consumer-Reconciliation | Private CI prüft einen **vollständigen temporären Consumer-Git-Snapshot**. Dabei bleibt der echte Fork unangetastet. | Snapshot `PASS` mit zwei protokollierten Abweichungen, explizite Prozess-Overlay-Akzeptanz steht noch aus. |
| Private CI führte öffentliches Prüfshellskript aus | Die private Prozess-PR führt nun ein **privates, im Prozessrepo gewartetes** Gate `tools/check-photocraft-bootstrap.sh` aus. Consumer-Skripte werden gegen ihre gepinnten VD-Templates auf Byte-/Zeilenebene geprüft; für Drift werden nur Prozess-Templates ausgeführt. | Trust-Boundary behoben, keine privaten Artefakte im öffentlichen Repo. |
| Nach Aktivierung würde reserved-only Gate scheitern | PF-Registryprüfung akzeptiert ausdrücklich `reserved` oder `active` für denselben Consumer; fehlende/falsche Registry bleibt Fehler. | Korrigiert. |
| Fehlender Prozess-Changelog | Prozess-PR aktualisiert `CHANGELOG.md [Unreleased]` mit dem nicht-breaking CI-Nachweis. | Korrigiert; finaler Prozessreview vor Merge. |

## Reproduzierbarer Full-Snapshot-Befund

Der nicht-destruktive POSIX-Bootstraplauf auf einem **vollständigen temporären Git-Checkout** des Consumers schrieb gegenüber dem eingefrorenen SHA `1167e6f7` nur:

1. `M docs/bootstrap-report.md`: der Standard-Toolreport würde den maßgeschneiderten PhotoCraft-Nachweis überschreiben.
2. `?? docs/adr/ADR-0001-bootstrap-baseline.md`: der generische ADR-Seed würde zusätzlich zum vorhandenen akzeptierten Port-`ADR-0001` erzeugt; doppelte ID-Semantik ist zu vermeiden.

**Beide Unterschiede sind dokumentierte, noch nicht unabhängig akzeptierte Prozess-Overlay-Ausnahmen.** Deshalb wird das Tool nie `--force` im bestehenden öffentlichen Checkout ausgeführt. Alle anderen von Git getrackten Pfade blieben unverändert; beide Abweichungen sind im privaten CI-Log erfasst. Das ist **nicht** mit einer Produkt-/Runtime-Freigabe gleichzusetzen.

## Abschließende Gates

- Full consumer SHA und pin process SHA im späteren Gate erneut verifizieren. Der Bericht soll nicht ungeprüft als Nachweis für neuere SHA gelten.
- Unabhängiger Review der zwei bewussten Overlay-Ausnahmen und beider PR-Diffs.
- Danach gesonderter Registry-PR im privaten Prozessrepository `PF reserved -> active` samt Nachweis; bis Merge bleibt PF `reserved`.
- Kein Upstream-PR, kein MiniGL/WinUAE-Ziel-PASS und keine Produktcompiler/ABI-/Release-Freigabe.
