# PF-TD-002 — README-/Story-Status mit kanonischem Backlog abgleichen

- Story-ID: `PF-TD-002`
- Story-Art: `TD`
- Typ: `tech_debt`
- Status: `draft` (nicht `ready`)
- Prioritaet: `P2`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-015` (geplant; Dokumentationsvalidierung `not_run`)

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
- `refinement_triage`: `pending`.
- `atomization_report.decision`: `keep_single_story` (vorlaeufig).
- IP: ausschliesslich eigene Projektdokumentation, keine fremden Quellen/Assets kopieren.
- Kein impliziter Status- oder Reviewer-PASS.

## 8. Testtraceability

- `PF-TN-015` als geplanter Check notiert, noch nicht ausgefuehrt.
- Kein Merge-/Release-/Produkt-ABI-Gate durch diesen Draft erfuellt.

## 9. Review und Gate-Status

- `arch_review.initial`: `pending`
- `arch_review.pre_ready_final`: `pending`
- `dor_check`: `pending`
- `implementation_gate_report`: `pending`
- `review_report`: `pending`
- `done`: `not_authorized`
