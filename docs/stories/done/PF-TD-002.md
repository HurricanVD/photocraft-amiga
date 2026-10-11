# PF-TD-002 — README-/Story-Status mit kanonischem Backlog abgleichen

- Story-ID: `PF-TD-002`
- Story-Art: `TD`
- Typ: `tech_debt`
- Status: `done` (2026-10-11; kanonischer README-/Backlog-Abgleich validiert, reviewt und gemergt)
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

- `PF-TN-015` wurde mit dokumentiertem Status-/Link-Audit und GitHub-Actions-PF-/Docs-/CI-Checks auf dem finalen PR-Head ausgefuehrt; die Ergebnisse beziehen sich auf die in der archivierten PF-TN-015 genannten SHAs und keine AmigaOS-Runtime.
- Der eigene Dokumentations-PR #10 ist gemergt; daraus folgt keine Release-/Produkt-ABI-Freigabe.

## 9. Review und Gate-Status

- `arch_review.initial`: `pass` (2026-10-11; [Pre-ready-Review](../../reviews/PF-TD-002-pre-ready-review-2026-10-11.md))
- `arch_review.pre_ready_final`: `pass` (2026-10-11; ADRs und kanonische Statusquellen geprueft)
- `dor_check`: `pass` (2026-10-11; siehe Pre-ready-Review)
- `implementation_gate_report`: `pass` ([dokumentiertes Eintrittsgate](../../reviews/PF-TD-002-implementation-gate-2026-10-11.md))
- `review_report`: `approve`
- `done`: `completed` (2026-10-11, reine Dokumentationskorrektur; keine Produktfreigabe)

## 10. Pre-ready-Abnahme und Restgates

- Evidenz: [Pre-ready-/DoR-Review](../../reviews/PF-TD-002-pre-ready-review-2026-10-11.md), aktuellen `main`-/Port-/Backlog-/ADR-Zustand geprueft.
- `ip_provenance_impact: none` fuer Status-/Link-Dokumentation; keine Fremdcode-/Asset-Uebernahme.
- Architektur / OS-API / Capability / NDK / vxlibs: `n/a` fuer reine Markdown-Korrektur. Keine neue ADR noetig.
- Testfall `PF-TN-015` ist durch Status-/Linkchecks und GitHub-Actions-Preflight/Docs validiert; das aktuelle Ergebnis und die Scopegrenzen stehen in [PF-TN-015](../../tests/done/PF-TN-015.md).
- Das Implementation-Gate wurde auf dem Arbeitsbranch vor der Umsetzung dokumentiert. Finaler fachlicher Review und Archivierung sind abgeschlossen; Release-Freigabe separat.

## Priorisierung 2026-10-11

- Prioritaet: `P2` — Dokumentationskorrektur ist unabhaengig und ready, aber weniger kritisch als Core-/Speichersicherheit.
- Quelle: [Prioritaets-/Ready-Audit](../../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine Implementation-/CI-/Reviewer-Gates.

## 11. Umsetzungsstand (2026-10-11)

- Umsetzung auf `docs/pf-td-002-readme-status`: Root-README benennt den realen `main`-Port, Port-README markiert historische 2026-10-09-Aussagen und verlinkt die kanonische PF-SP-002-`blocked`-Akte.
- Der ursprüngliche Rust-Upstream, alle binären APIs, Buildpfade und AmigaOS-Komponenten bleiben unverändert.
- Regression: [PF-TN-015](../../tests/done/PF-TN-015.md), Docs-/Backlog-Checks, CI auf finalem PR-Head erfolgreich; finaler Review und Merge erfolgt.

## 12. Review-Handoff (2026-10-11)

- [PF-TN-015](../../tests/done/PF-TN-015.md): acht kanonische Status-/Link-Checks und Docs-/PF-Preflight erfolgreich.
- [Change Summary](../../reviews/PF-TD-002-change-summary-2026-10-11.md), [PR #10](https://github.com/HurricanVD/photocraft-amiga/pull/10).
- Historischer Handoff; finaler CI und getrennter Reviewbericht liegen vor. Produkt-Release bleibt nicht freigegeben.

## 13. Finaler Story-Abschluss — 2026-10-11

- `review_report: approve` — [Finalreview](../../reviews/PF-TD-002-final-review-2026-10-11.md) als begrenzte fachliche Empfehlung, **nicht** als vom GitHub-Eigentuemer selbst erteilte Plattform-APPROVE.
- `test_evidence: pass` — [PF-TN-015](../../tests/done/PF-TN-015.md), finaler PR-Head `4af4441828f431c163a4d27013b378fb57e14f5f`.
- [CI #38141046023](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38141046023), [Docs #38141046048](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38141046048), [PF Preflight #38141046033](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38141046033), [AmigaOS Host #38141046042](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38141046042): `completed/success`.
- [PR #10](https://github.com/HurricanVD/photocraft-amiga/pull/10) gemergt unter `7621c7fe814d73aec58ec7561e7ef612e23e185e`. Endgueltige Haupt-README und Port-README vorhanden; PF-SP-002 bleibt `blocked`.
- `manual_application_test: n/a` fuer Markdown-Statuskorrektur, ersetzt durch dokumentierten automatisierten PF-/Docs-/CI- und Backlog-Regressionstest. Kein Produktrelease, keine OS-/API-Aenderung.
- Archiv: `docs/stories/done/PF-TD-002.md`, Test: `docs/tests/done/PF-TN-015.md`, Status nur noch in `docs/backlog-done.md`.
