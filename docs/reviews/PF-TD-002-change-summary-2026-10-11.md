# PF-TD-002 — Change summary (2026-10-11)

> Historischer Change Summary aus der Review-Phase. Die alten Status- und CI-Pending-Aussagen bleiben als damaliger Stand erhalten; der [Finalreview](PF-TD-002-final-review-2026-10-11.md) dokumentiert den späteren Abschluss.

- Story: [PF-TD-002](../stories/done/PF-TD-002.md)
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
- Alle Details: [PF-TN-015](../tests/done/PF-TN-015.md).
- Kein Changelog/-Release-/Abhaengigkeits- oder OS-API-Effekt.


## Finaler Nachtrag 2026-10-11

- PR #10 ist unter Merge `7621c7fe814d73aec58ec7561e7ef612e23e185e` auf `main` integriert.
- Der finale PR-Head hat [CI #38141046023](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38141046023), [Docs #38141046048](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38141046048) und [PF Preflight #38141046033](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38141046033) erfolgreich abgeschlossen.
- Die Story und `PF-TN-015` sind unter `done/` archiviert; [Finalreview](PF-TD-002-final-review-2026-10-11.md) empfiehlt `approve` fuer den reinen Dokumentationsscope, nicht fuer Release.
