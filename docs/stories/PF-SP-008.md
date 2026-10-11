# PF-SP-008 — Kommando- und Undo/Redo-Prototyp

- Story-ID: `PF-SP-008`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `ready` (Pre-ready-Audit 2026-10-12; no implementation)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Parent: `PF-SP-002`; [atomization_report](../reviews/PF-SP-002-atomization-report-2026-10-11.md)
- Backlog: `docs/backlog.md`

## 1. Problem / Nutzerwert

Ein isoliertes, reversibles Dokumentkommando mit originaler History-Semantik. Die bisherige Sammelstory ist nicht atomar. Dieser Teil soll getrennt lieferbar und gegen das unveraenderte PhotoCraft-Rust-Original pruefbar sein.

## 2. In Scope

- Eine benannte Modelloperation
- apply/undo/redo
- Invalid-ID/No-op
- bounded History-/Fault-Verhalten

## 3. Nicht-Ziele

- komplette Editor-Kommandos, GUI Actions, jede History-Aktion
- Keine heimliche Produktcompiler-, GUI-, ABI- oder Release-Freigabe.
- Keine proprietaere Dunkelkammer-Quell-/Asset-Uebernahme.

## 4. User Story

Als PhotoCraft-Portentwickler moechte ich kommando- und undo/redo-prototyp als eigenen pruefbaren Schritt, damit Risiko, Ownership und Testgate klar abgrenzbar bleiben.

## 5. Akzeptanzkriterien

1. Konkrete originale Rust-Referenzoperation vor Design gewaehlt.
2. Deterministische Undo/Redo-Gleichheit.
3. klare Speicher-/Fehlergrenzen.

## 6. Architektur / Tests / Grenzen

- Betroffene Module: `ports/amigaos3/` und Original-Rust-Oracles (read-only); auf den PR-Scope begrenzt.
- Architekturquelle: akzeptierte ADR-0001/0002/0003; gesonderter Child-`pre_ready_final`-Review `pending`.
- Test-Traceability: `PF-TN-009 (geplant)`.
- Implementierungszuordnung: kein PR / Implementierung not_run.
- Test-Evidenz: nicht implementiert / not_run.
- Host/ASan/UBSan/Rust-Differential und GCC13-vamos nur fuer eindeutig belegten Scope als PASS markieren.
- Manuelle AmigaOS/WinUAE/MiniGL-Pruefung: hier `n/a` als OS-freier Spike; fuer Produktintegration gesondert `required`.

## 7. refinement_note / refinement_triage

- Trennung: genau eine fachliche Aenderungsachse und eigene Testnachweise.
- `refinement_triage`: `pass`
- IP-/Third-Party-Klassifikation: `reference_only` (oeffentliche PhotoCraft-Rust-Originalsemantik; Code nur als Referenz, keine Uebernahme fremder geschuetzter Quellen).
- Shipping-Material aus Drittrepos: `no`; rights_status fuer proprietaere Dunkelkammer: `not_required` in diesem Scope.
- Spike-/Design-Bedarf: `yes; no implementation authorized`.
- Refinement-Ergebnis: `ready_for_architecture`, **nicht** `ready`.

## 8. atomization_report

- decision: `keep_single_story` fuer diesen begrenzten Kandidaten, vorbehaltlich finalem Refinement-/Scope-Audit.
- parent_decision: `PF-SP-002: split_required`.
- Aenderungsachse: Kommando- und Undo/Redo-Prototyp.
- Dependencies: PF-SP-004/005; maskenspezifische Commands erst nach PF-SP-006.
- Lifecycle-Blocker: initiales/finales Architekturreview, DoR/IP-Triage, Implementation-Gate und finales Reviewer-Gate offen.

## 9. Review und Gate-Status

- `arch_review.initial`: `pass`
- `arch_review.pre_ready_final`: `pass`
- `implementation_gate_report`: `pending`
- `review_report`: `pending`
- `done`: `not_authorized`
- Naechster Schritt: Child-Refinement und formale Lifecycle-Abnahme; bei PR #3–5 bereits erfolgte Aenderungen retrospektiv sauber auditieren.

## Priorisierung 2026-10-11

- Prioritaet: `P2` — Undo/Redo wichtig, aber nach Ownership-/Layer-Stabilisierung und gesondertem Command-Kontrakt.
- Quelle: [Prioritaets-/Ready-Audit](../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine bestehenden Implementation-/CI-/Reviewer-Gates; dokumentierte Story-Statuswerte bleiben massgeblich.

- `dor_check`: `pass`
## Pre-ready delta 2026-10-12

- Reviewed design and acceptance: One reversible PhotoCraft command with apply/undo/redo; stable layer ID, no-op, fault/OOM rollback and bounded history. Other commands follow.
- Architecture: accepted ADR-0001/0002/0003; native offline 68040+ / 32 MiB RAM. Rust upstream remains the oracle, never a remote runtime. This limited, independent spike is `keep_single_story`.
- Evidence: [PF-all-open-ready-review](../reviews/PF-all-open-ready-review-2026-10-12.md). Initial architecture / scope refinement / separate final pre-ready review and DoR pass are **planning checks**, not product runtime results.
- IP: original PhotoCraft semantics are reference-only; any reused Dunkelkammer source is MIT for owner material (merge PR #9), retain each copyright and third-party notice.
- Next stage: individual `implementation_gate_report` before work, unmet dependency verification as applicable. All actual native tests and target GUI checks still `not_run`.
