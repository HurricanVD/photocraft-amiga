# PF-TD-001 — Tile-Lookup, Speicherbudget und OOM-Rollback

- Story-ID: `PF-TD-001`
- Story-Art: `TD`
- Typ: `tech_debt`
- Status: `ready` (Pre-ready-Audit 2026-10-12; no implementation)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Parent: `PF-SP-002`; [atomization_report](../reviews/PF-SP-002-atomization-report-2026-10-11.md)
- Backlog: `docs/backlog.md`

## 1. Problem / Nutzerwert

Lineare Tile-Suche, Heapbudgets und mehrkachelige Fehlerpfade absichern. Die bisherige Sammelstory ist nicht atomar. Dieser Teil soll getrennt lieferbar und gegen das unveraenderte PhotoCraft-Rust-Original pruefbar sein.

## 2. In Scope

- Allocator-Fault-Injection
- OOM/Rollback-Definition
- indexed-lookup Benchmark
- 32MiB row-/band-limited RAM

## 3. Nicht-Ziele

- neues 128x128 Tile-Modell
- fixes Layerlimit
- volle f32 Framebuffer
- Produktrelease
- Keine heimliche Produktcompiler-, GUI-, ABI- oder Release-Freigabe.
- Keine proprietaere Dunkelkammer-Quell-/Asset-Uebernahme.

## 4. User Story

Als PhotoCraft-Portentwickler moechte ich tile-lookup, speicherbudget und oom-rollback als eigenen pruefbaren Schritt, damit Risiko, Ownership und Testgate klar abgrenzbar bleiben.

## 5. Akzeptanzkriterien

1. Fehlallokationen deterministisch und leakfrei.
2. Peak-/Tile-Größen dokumentiert.
3. indexed COW-Parität zu Original-Rust.

## 6. Architektur / Tests / Grenzen

- Betroffene Module: `ports/amigaos3/` und Original-Rust-Oracles (read-only); auf den PR-Scope begrenzt.
- Architekturquelle: akzeptierte ADR-0001/0002/0003; gesonderter Child-`pre_ready_final`-Review `pending`.
- Test-Traceability: `PF-TN-011 (geplant)`.
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
- Aenderungsachse: Tile-Lookup, Speicherbudget und OOM-Rollback.
- Dependencies: PF-SP-002 Raw-Raster-Baseline; ADR-0002 implementation follow-up.
- Lifecycle-Blocker: initiales/finales Architekturreview, DoR/IP-Triage, Implementation-Gate und finales Reviewer-Gate offen.

## 9. Review und Gate-Status

- `arch_review.initial`: `pass`
- `arch_review.pre_ready_final`: `pass`
- `implementation_gate_report`: `pending`
- `review_report`: `pending`
- `done`: `not_authorized`
- Naechster Schritt: Child-Refinement und formale Lifecycle-Abnahme; bei PR #3–5 bereits erfolgte Aenderungen retrospektiv sauber auditieren.

## Priorisierung 2026-10-11

- Prioritaet: `P1` — Kernrisiko Tile-Suche, RAM und OOM; notwendiges Architektur- und Testfundament fuer groessere Dokumente.
- Quelle: [Prioritaets-/Ready-Audit](../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine bestehenden Implementation-/CI-/Reviewer-Gates; dokumentierte Story-Statuswerte bleiben massgeblich.

- `dor_check`: `pass`
## Pre-ready delta 2026-10-12

- Reviewed design and acceptance: Deterministic allocator fault injection, transactional multi-tile write preparation/commit, tile index complexity and <=32 MiB fast RAM working budget; COW semantics remain.
- Architecture: accepted ADR-0001/0002/0003; native offline 68040+ / 32 MiB RAM. Rust upstream remains the oracle, never a remote runtime. This limited, independent spike is `keep_single_story`.
- Evidence: [PF-all-open-ready-review](../reviews/PF-all-open-ready-review-2026-10-12.md). Initial architecture / scope refinement / separate final pre-ready review and DoR pass are **planning checks**, not product runtime results.
- IP: original PhotoCraft semantics are reference-only; any reused Dunkelkammer source is MIT for owner material (merge PR #9), retain each copyright and third-party notice.
- Next stage: individual `implementation_gate_report` before work, unmet dependency verification as applicable. All actual native tests and target GUI checks still `not_run`.

## Review-Priorisierung / Bestandsabgleich 2026-10-12

- **Prioritaet bleibt P1 (Reihenfolge A, zuerst).** Es gibt keinen bestaetigten P0-Defekt aus der Review-Beobachtung allein.
- Die konkreten Befunde zu nicht-atomarem \`pc_raster_write_region\` und linearem \`pc_find\` sind bereits im Story-Scope. Keine weiteren Raster-Bug-Dubletten anlegen; Implementierung/Fault-Injection/Benchmark ausstehend.
- Review 2026-10-12 ist Planungs-/Evidence-Abgleich, kein neuer Implementierungs-, DoR-, Runtime-PASS- oder Release-Beschluss.

## Implementation checkpoint 2026-10-12 (nicht done)

- Arbeitsbranch: `implementation/pf-td-001-raster-atomic-index`.
- Vorhanden: geordnete Tile-Vektoren mit binaerer Suche und rollback-sichere Region-Writes durch COW-Staging mit abschliessendem Commit; Allokations-Fault-Injection-Test und neue Build-Testziele.
- Verbleibend: objektive Tile-Index-Benchmarks, 32-MiB-Peak-Budget am Target, Ausfuehrung aller Host/ASan/UBSan/Rust-C99/GCC13-vamos-Tests und formale Review-Gates.
- Die neue Implementierung vergroessert je nach betroffenem Tile-Set den transienten Speicherbedarf; keine Behauptung, das 32-MiB-Akzeptanzkriterium sei bereits erreicht.
- `implementation_gate_report: pending`; `test_evidence: not_run`; `review_report: pending`; `done: not_authorized`. Der kanonische Status `ready` wird nicht eigenmaechtig auf `done` gesetzt.
