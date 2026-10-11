# PF-TD-002 — README-/Story-Status mit kanonischem Backlog abgleichen

- Story-ID: `PF-TD-002`
- Story-Art: `TD`
- Typ: `tech_debt`
- Status: `review` (README-Änderungen und Dokumentations-/PF-Preflight erfolgreich; finaler PR-Review und Merge offen)
- Prioritaet: `P2`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-015` (kanonische Status-/Linkchecks und Docs-/PF-Preflight ausgefuehrt; CI #38127048797 PASS auf altem Teststand, Endreview offen)

## 1. Problem / Nutzerwert

Die Root-`README.md` behauptet, der isolierte AmigaOS-Port liege auf dem Branch `amigaos32`. Die Port-Dateien und der kanonische Backlog sind inzwischen auch auf `main` vorhanden. Port-lokale historische Storykopien enthalten teils alte `in_progress`-Stati; die Hauptquelle fuehrt PF-SP-002 seit 2026-10-11 als `blocked` (`split_required`). Solche Diskrepanzen koennen die Arbeitsplanung und Nutzererwartungen verfaelschen.

## 2. In Scope

- Root-README-Status und Branchbeschreibung auf den **tatsaechlichen** Stand von `main` korrigieren.
- Klar zwischen Upstream-Funktionsumfang und dem **nicht funktionsfaehigen AmigaOS-Port** unterscheiden.
- Historische Port-Dokumente als historische Belege kennzeichnen und auf `docs/backlog.md` sowie kanonische `docs/stories/`-Akte verlinken.
- Links auf Stories/ADRs/Tests, und Statusangaben auf Dokumentationsseiten kontrollieren.

## 3. Nicht-Ziele

- Runtime-/Produkt-Feature-Implementierung oder pauschale Neuformulierung des upstream README.
- Rueckdatierte CI- oder Lifecycle-Genehmigung; keine automatische Uebernahme von `ready`/`done`.
- Aufraeumen des privaten VD-Prozessrepos oder Kopieren dritter Assets.

## 4. User Story

Als Projektbeteiligter moechte ich aus README und kanonischem Backlog den gleichen Port- und Story-Status ablesen koennen, damit Entwicklung und externe Erwartungen nicht von veralteten Branch-/Lifecycle-Angaben abhaengen.

## 5. Akzeptanzkriterien

1. Root-README beschreibt nachvollziehbar, dass `ports/amigaos3/` auf `main` vorhanden ist, waehrend `amigaos32` als separater Entwicklungsbranch bestehen kann.
2. Kein Dokument behauptet, bereits einen lauffaehigen AmigaOS-PhotoCraft-Editor oder einen freigegebenen Produktcompiler zu haben.
3. Dokumente mit historischer PF-SP-002-Aussage verweisen eindeutig auf den seit 2026-10-11 kanonischen `blocked`-Status; historische Belege bleiben erhalten.
4. Dokumentations-/Link-/Backlog-Checks laufen ohne neue Fehler; Reviewer bestaetigt die Statuswahrheit anhand von `main`.

## 6. Architektur / Tests / Grenzen

- Betroffen: `README.md`, ggf. `ports/amigaos3/README.md` und nur eindeutige Statushinweise; `docs/backlog.md` ist kanonisch.
- Abhaengigkeit: dokumentierte PF-SP-002-Atomisierung und bestehende Backlog-States.
- Validierung: gezielte Suche nach veralteten Aussagen, Repo-Linkcheck und vorhandene Dokumentations-CI (tatsaechliches Ergebnis erst nach Ausfuehrung dokumentieren).
- Kein Build des PhotoCraft Rust-Kerns notwendig, wenn ausschliesslich Texte geaendert werden.

## 7. refinement_note / refinement_triage

- Eine Aenderungsachse: Dokumentationskonsistenz.
- `refinement_triage`: `pass` (belegte Readme-/Status-Diskrepanz, dokumentationsbeschraenkter Scope).
- `atomization_report.decision`: `keep_single_story` (Pre-ready-Review 2026-10-11).
- IP: ausschliesslich eigene Projektdokumentation, keine fremden Quellen/Assets kopieren.
- Die urspruengliche DoR-Freigabe setzte **keinen** Implementation-/Test-/Reviewer-PASS voraus; inzwischen erfolgten ein separates Implementation-Gate und ausgefuehrte Dokumentationschecks.

## 8. Testtraceability

- `PF-TN-015` wurde mit dokumentiertem Status-/Link-Audit und GitHub-Actions-PF-/Docs-Checks ausgefuehrt; die Ergebnisse beziehen sich auf die dort genannten Code-SHAs und keine AmigaOS-Runtime.
- Kein automatischer Merge-/Release-/Produkt-ABI-Gate durch diese Dokumentationspruefung erfuellt.

## 9. Review und Gate-Status

- `arch_review.initial`: `pass` (2026-10-11; [Pre-ready-Review](../reviews/PF-TD-002-pre-ready-review-2026-10-11.md))
- `arch_review.pre_ready_final`: `pass` (2026-10-11; ADRs und kanonische Statusquellen geprueft)
- `dor_check`: `pass` (2026-10-11; siehe Pre-ready-Review)
- `implementation_gate_report`: `pass` ([dokumentiertes Eintrittsgate](../reviews/PF-TD-002-implementation-gate-2026-10-11.md))
- `review_report`: `pending`
- `done`: `not_authorized`

## 10. Pre-ready-Abnahme und Restgates

- Evidenz: [Pre-ready-/DoR-Review](../reviews/PF-TD-002-pre-ready-review-2026-10-11.md), aktuellen `main`-/Port-/Backlog-/ADR-Zustand geprueft.
- `ip_provenance_impact: none` fuer Status-/Link-Dokumentation; keine Fremdcode-/Asset-Uebernahme.
- Architektur / OS-API / Capability / NDK / vxlibs: `n/a` fuer reine Markdown-Korrektur. Keine neue ADR noetig.
- Testfall `PF-TN-015` ist durch Status-/Linkchecks und GitHub-Actions-Preflight/Docs validiert; das aktuelle Ergebnis und die Scopegrenzen stehen in [PF-TN-015](../tests/PF-TN-015.md).
- `implementation_gate_report: pass` ([Gate](../reviews/PF-TD-002-implementation-gate-2026-10-11.md)); `review_report: pending`; keine automatische Done- oder Release-Freigabe.

## Priorisierung 2026-10-11

- Prioritaet: `P2` — Dokumentationskorrektur ist unabhaengig und ready, aber weniger kritisch als Core-/Speichersicherheit.
- Quelle: [Prioritaets-/Ready-Audit](../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine Implementation-/CI-/Reviewer-Gates.

## 11. Umsetzungsstand (2026-10-11)

- Umsetzung auf `docs/pf-td-002-readme-status`: Root-README benennt den realen `main`-Port, Port-README markiert historische 2026-10-09-Aussagen und verlinkt die kanonische PF-SP-002-`blocked`-Akte.
- Der ursprüngliche Rust-Upstream, alle binären APIs, Buildpfade und AmigaOS-Komponenten bleiben unverändert.
- Regression: [PF-TN-015](../tests/PF-TN-015.md), Docs-/Backlog-Checks und CI bereits fuer dokumentierte Commits ausgefuehrt; das finale PR-Review des aktualisierten Head bleibt separat.

## 12. Review-Handoff (2026-10-11)

- [PF-TN-015](../tests/PF-TN-015.md): acht kanonische Status-/Link-Checks und Docs-/PF-Preflight erfolgreich.
- [Change Summary](../reviews/PF-TD-002-change-summary-2026-10-11.md), [PR #10](https://github.com/HurricanVD/photocraft-amiga/pull/10).
- Allgemeiner final-head CI-/unabhaengiger Delta-Review noch offen; **kein** `review_report: approve`, `done` oder Release-Claim.
