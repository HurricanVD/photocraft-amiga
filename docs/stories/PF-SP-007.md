# PF-SP-007 — Kleiner CPU-Compositor-Paritätsspike

- Story-ID: `PF-SP-007`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `ready` (Pre-ready-Audit 2026-10-12; no implementation)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Parent: `PF-SP-002`; [atomization_report](../reviews/PF-SP-002-atomization-report-2026-10-11.md)
- Backlog: `docs/backlog.md`

## 1. Problem / Nutzerwert

Begrenzte Referenzpixel-Komposition nach originalem PhotoCraft Rust. Die bisherige Sammelstory ist nicht atomar. Dieser Teil soll getrennt lieferbar und gegen das unveraenderte PhotoCraft-Rust-Original pruefbar sein.

## 2. In Scope

- RGBA8 Normal Blend als vorgeschlagene Startmatrix
- Sichtbarkeit/Opacity
- Row/Dirty-Band-Ausgabe
- deterministische Rust/C-Fixtures

## 3. Nicht-Ziele

- voller Blend-Modus-Katalog
- ICC/HDR
- GUI/MiniGL
- PSD
- Keine heimliche Produktcompiler-, GUI-, ABI- oder Release-Freigabe.
- Keine proprietaere Dunkelkammer-Quell-/Asset-Uebernahme.

## 4. User Story

Als PhotoCraft-Portentwickler moechte ich kleiner cpu-compositor-paritätsspike als eigenen pruefbaren Schritt, damit Risiko, Ownership und Testgate klar abgrenzbar bleiben.

## 5. Akzeptanzkriterien

1. Kleinste Blend-Matrix vor Implementation dokumentiert.
2. berechnete Referenzpixel Rust/C gleich.
3. bounded host/m68k regression.

## 6. Architektur / Tests / Grenzen

- Betroffene Module: `ports/amigaos3/` und Original-Rust-Oracles (read-only); auf den PR-Scope begrenzt.
- Architekturquelle: akzeptierte ADR-0001/0002/0003; gesonderter Child-`pre_ready_final`-Review `pending`.
- Test-Traceability: `PF-TN-008 (geplant)`.
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
- Aenderungsachse: Kleiner CPU-Compositor-Paritätsspike.
- Dependencies: PF-SP-004/005 und fuer Maskenintegration PF-SP-006.
- Lifecycle-Blocker: initiales/finales Architekturreview, DoR/IP-Triage, Implementation-Gate und finales Reviewer-Gate offen.

## 9. Review und Gate-Status

- `arch_review.initial`: `pass`
- `arch_review.pre_ready_final`: `pass`
- `implementation_gate_report`: `pending`
- `review_report`: `pending`
- `done`: `not_authorized`
- Naechster Schritt: Child-Refinement und formale Lifecycle-Abnahme; bei PR #3–5 bereits erfolgte Aenderungen retrospektiv sauber auditieren.

## Priorisierung 2026-10-11

- Prioritaet: `P1` — Erster CPU-Compositor ist Voraussetzung fuer echten PhotoCraft-Viewport.
- Quelle: [Prioritaets-/Ready-Audit](../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine Implementation-/CI-/Reviewer-Gates.

- `dor_check`: `pass`
## Pre-ready delta 2026-10-12

- Reviewed design and acceptance: Local RGBA8 Normal/opacity/mask compositor; row/dirty band output; original Rust differential fixture; no GUI/MiniGL/ICC.
- Architecture: accepted ADR-0001/0002/0003; native offline 68040+ / 32 MiB RAM. Rust upstream remains the oracle, never a remote runtime. This limited, independent spike is `keep_single_story`.
- Evidence: [PF-all-open-ready-review](../reviews/PF-all-open-ready-review-2026-10-12.md). Initial architecture / scope refinement / separate final pre-ready review and DoR pass are **planning checks**, not product runtime results.
- IP: original PhotoCraft semantics are reference-only; any reused Dunkelkammer source is MIT for owner material (merge PR #9), retain each copyright and third-party notice.
- Next stage: individual `implementation_gate_report` before work, unmet dependency verification as applicable. All actual native tests and target GUI checks still `not_run`.
