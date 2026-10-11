# PF-TD-002 — Change summary (2026-10-11)

- Story: [PF-TD-002](../stories/PF-TD-002.md)
- Branch: `docs/pf-td-002-readme-status`
- Review Stage: `review`, **not** `done`
- Tracked in [PR #10](https://github.com/HurricanVD/photocraft-amiga/pull/10)
- DoR und Entry-Gate: [pre-ready review](PF-TD-002-pre-ready-review-2026-10-11.md), [implementation gate](PF-TD-002-implementation-gate-2026-10-11.md)
- Change type: eigene Dokumentation, keine AmigaOS-API oder Produktcodeaenderung

## Aenderungen
- Root-`README.md`: Der AmigaOS-Port liegt inzwischen auf `main`, `amigaos32` bleibt separater Entwicklungsbranch. C99-Modelle und m68k-Headless-Tests werden klar von einem tatsaechlich lauffaehigen Editor unterschieden.
- `ports/amigaos3/README.md`: historischer Entwicklungsstand von 2026-10-09 explizit als solcher gekennzeichnet, kanonischer `PF-SP-002: blocked/split_required` und `PF-SP-001: blocked` verlinkt; Tests und Laufzeitgrenzen klar abgegrenzt.
- Kanonische Story, Backlog und Testprotokoll werden auf den aktuellen Dokumentationsarbeitsstand synchronisiert.

## Ausgefuehrte Tests und Rest
- Acht connector-basierte Status-/Backlog-/Links-Assertions: `PASS`.
- [PF Bootstrap Docs Preflight #38126908022](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38126908022): `success`.
- [Documentation/mdBook #38126908029](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38126908029): `success`.
- Vollstaendiger General-CI-Run #38126908016 bei Erstfassung noch nicht abgeschlossen. Kein `review_report: approve` und kein behaupteter `done`-Status.
- Alle Details: [PF-TN-015](../tests/PF-TN-015.md).
- Kein Changelog/-Release-/Abhaengigkeits- oder OS-API-Effekt.
