# PF-TD-004 — Integrationsstatus und Testevidenz nach Preview-Merge konsolidieren

- Story-ID: `PF-TD-004`
- Story-Art: `TD`
- Typ: `tech_debt/process`
- Status: `draft` (Neuanlage 2026-10-12; kein Implementation-Gate)
- Prioritaet: `P2`
- Backlog: `docs/backlog.md`
- Bezug: PF-TD-002 (abgeschlossener historischer Statusabgleich), PF-SP-007/010/011 und PR #18

## Problem / Beobachtung
Der Merge von PR #18 nach `main` liefert einen nativen PcDocument->CPU-Normal-Compositor->M2->ReAction-Preview mit dokumentierter manueller WinUAE-Beobachtung. Der kanonische Backlog und mehrere Einzelstories beschreiben dennoch pauschal `no implementation` / `tests not_run`. Die vormals abgeschlossene PF-TD-002 bleibt archiviert; diese neue Story betrifft eine **spaetere, eigenstaendige Statusabweichung**, nicht die Wiedereroeffnung der damaligen Arbeit.

## Eine Aenderungsachse
Genaue Gegenueberstellung der bereits integrierten **Teilimplementierung** mit dem noch ausstehenden Gesamtumfang und seinen jeweiligen Test-Gates. Keine Feature-Implementierung, keine retrospektiven PASS-Erfindungen.

## Scope
- Snapshot des gemergten Preview-Codes und seiner CI-/WinUAE-Evidenz mit Commit-SHA, PR, Zieltest-IDs und Umfang erfassen.
- PF-SP-007/010/011 sowie betroffene READMEs/Story- und Testdateien aktualisieren: `integrated_preview_subset` vs. offene volle Storyakzeptanz.
- Historische `not_run`-Hinweise nicht loeschen, sondern explizit auf die ungetestete **Restanforderung** beziehen; `observed_pass`, CI-Erfolg und `not_run` getrennt ausweisen.
- Prozessregel/Checkliste fuer kuenftige Merge-PRs festhalten: Backlog-/Story-/Teststatus gemeinsam pruefen; `ready` bleibt unveraendert bis formalem Lifecycle-Gate.

## Akzeptanzkriterien
1. Kein kanonisches Dokument behauptet, es gebe ueberhaupt keinen nativen Preview.
2. Kein Dokument bezeichnet ungetestete Full-Story-Gates allein wegen PR #18 als bestanden oder `done`.
3. Pfad-/Story-/Test-Matrix mit Commit und Evidenzquelle ist reviewbar; alte Aussagen sind als historische Snapshots gekennzeichnet.
4. Unveraenderte ADR-, IP-, Toolchain-, Offline- und WinUAE-Prozessgrenzen sind belegt.

## Gates
- `refinement_triage: pending`; `arch_review.initial: pending`; `dor_check: pending`.
- `implementation_gate_report: pending`; `test_evidence: not_run` (Dokumenten-Audit); `review_report: pending`; `done: not_authorized`.
- Prioritaet P2: verhindert Fehlsteuerung, ist aber kein neuer Runtime-Blocker.
