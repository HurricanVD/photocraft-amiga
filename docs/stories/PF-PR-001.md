# PF-PR-001 — Upstream-Delta und Original-Rust-Oracle fortlaufend pruefen

- Story-ID: `PF-PR-001`
- Story-Art: `PR`
- Typ: `process/verification`
- Status: `ready` (2026-10-12 DoR, keine Implementation)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-12
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-026` (geplant / `not_run`)
- Review: `docs/reviews/PF-all-open-ready-review-2026-10-12.md`

## 1. Problem / Nutzerwert

Als Nutzer/Maintainer moechte ich neue Upstream-Aenderungen sichtbar machen und die C99-Port-Differenzen automatisch belegen, damit eine vollstaendig autarke PhotoCraft-Amiga-Version entsteht, die den originalen Rust-Upstream nachweisbar respektiert.

## 2. In Scope

- Upstream main exakt pinnen und bei neuer Rust-Revision einen reproduzierbaren Delta-Report fuer geom/color/raster/doc/compose/ops/paint/engine produzieren.
- Original-Rust Fixture API/semantische Deltas und C99/m68k HUNK-vamos-Paeritaet in getrennten CI-Stufen markieren; defekte/missing Features nicht als Erfolg verkaufen.
- Automatisch nur PR/Issue oder Bericht erstellen; kein unueberwachtes Merge, keine Rewrite/Cherry-Pick von fremden Quellen oder automatische Aenderungen an main.

## 3. Nicht-Ziele

- Keine automatische Uebersetzung Rust→C, keine unbeaufsichtigten Upstream-Merges, keine Release-Promotion.
- Keine Erweiterung der originalen Rust-Crates und keine Remote-/Cloud-Pflicht.
- `ready` ist **keine** Implementierungs-, Release- oder WinUAE-Freigabe.

## 4. Akzeptanzkriterien

1. Synthetic upstream-change fixture reports changed files, impacted port IDs, unknown API fields and outdated oracles.
2. Workflow mit PR-only App-Tokens/least privilege und Notifikation testen; no-secrets logs.
3. Die Upstream-Quelle bleibt original; kein m68k-std oder Remote-Engine erforderlich.
4. Ungueltige Daten/OOM/Fehlerpfade liefern kontrollierte Ergebnisse; alle genutzten Datenpuffer und OS-Libraries werden trotz Fehler vollstaendig freigegeben. 32 MiB Fast RAM bleibt gepruefte Testbaseline, keine unbelegte Minimumspec.
5. Eigene C99-Algorithmen bleiben durch originale Rust-Semantik und benannte Oracles differenziert; WinUAE/GUI-Status wird nur nach echter manueller Zielausfuehrung PASS.

## 5. Architektur / Betroffene Module

- .github/workflows/ + ports/amigaos3/tests oracles; keine Upstream-Rust-Crates aendern.
- Gate-Quellen: akzeptierte ADR-0001/0002/0003, [Port-Architektur](../../ports/amigaos3/docs/architecture.md) und [Pre-ready-Review](../reviews/PF-all-open-ready-review-2026-10-12.md).
- Capability: n/a (OS-unabhaengiger CI-Prozess); AmigaOS/NDK 3.2 und GCC/Bebbo Testprofil gem. ADR-0003; reale Produktcompilerfreigabe getrennt.
- IP-Provenienz: `reference_only`. Dunkelkammer-Eigenanteile sind MIT (PR #9, Lizenzhead `70897e91`), fremde Quellen/Assets unter eigener Lizenz. Alle direkt importierten Dateien benoetigen SHA/Copyright/Notice im Portregister.
- Implementierungsabhaengigkeiten: Original Rust und PhotoCraft fork refs, CI credentials review, Phase-1 Oracles; Upstream-Track als separater PR von Kernimplementierung.

## 6. Testplanung

- [PF-TN-026](../tests/PF-TN-026.md): Testfaelle und Negativpfade vor Umsetzung, `not_run`.
- Reproduzierbare Host-/ASan-/UBSan- und original Rust-Differential-Tests bei CPU/Modell, genuine m68k HUNK/vamos O0/O2; zusaetzlich manuelle WinUAE-ReAction-/RTG-Abnahme bei UI.
- Private automatisierte WinUAE-Capture/Broker-Flows bleiben `disabled_by_process`.

## 7. Refinement / atomization_report

- `atomization_report.decision`: `keep_single_story` (eine abgegrenzte Veraenderungsachse, keine versteckte Zweitimplementierung).
- `refinement_triage`: `pass`
- `arch_review.initial`: `pass`
- `arch_review.pre_ready_final`: `pass`
- `dor_check`: `pass`
- Quelle: [PF-all-open-ready-review 2026-10-12](../reviews/PF-all-open-ready-review-2026-10-12.md), statische Planungs-/DoR-Analyse, kein Test-PASS.

## 8. Implementation-/Review-Gates

- `implementation_gate_report`: `pending` (Vorbedingungen direkt vor Umsetzung erneut pruefen)
- `review_report`: `pending`
- `test_evidence`: `not_run`
- `done`: `not_authorized`
