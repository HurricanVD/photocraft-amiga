# PF-SP-006 — Gray8-LayerMask-Sparse-COW

- Story-ID: `PF-SP-006`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `refining` (PF-SP-002 split_required; nicht `ready`)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Parent: `PF-SP-002`; [atomization_report](../reviews/PF-SP-002-atomization-report-2026-10-11.md)
- Backlog: `docs/backlog.md`

## 1. Problem / Nutzerwert

Sparse Gray8-Masken (default 255/0), enabled/linked, Attach/Detach und COW. Die bisherige Sammelstory ist nicht atomar. Dieser Teil soll getrennt lieferbar und gegen das unveraenderte PhotoCraft-Rust-Original pruefbar sein.

## 2. In Scope

- Gray/U8-NoAlpha-Guard
- Masken an Raster-/Gruppen-ID
- disable=255
- ownership/alias rollback
- fixed density=1 feather=0

## 3. Nicht-Ziele

- Masken-Density/Feather
- Composition
- PSD
- Produkt-ABI
- Keine heimliche Produktcompiler-, GUI-, ABI- oder Release-Freigabe.
- Keine proprietaere Dunkelkammer-Quell-/Asset-Uebernahme.

## 4. User Story

Als PhotoCraft-Portentwickler moechte ich gray8-layermask-sparse-cow als eigenen pruefbaren Schritt, damit Risiko, Ownership und Testgate klar abgrenzbar bleiben.

## 5. Akzeptanzkriterien

1. Gray8 defaults und negative Koordinaten wie Rust.
2. unpassende Formate/Doppelbesitz fail-closed.
3. Clone/Detach stabil.
4. Rust LayerMask Diff, ASan, HUNK/vamos.

## 6. Architektur / Tests / Grenzen

- Betroffene Module: `ports/amigaos3/` und Original-Rust-Oracles (read-only); auf den PR-Scope begrenzt.
- Architekturquelle: akzeptierte ADR-0001/0002/0003; gesonderter Child-`pre_ready_final`-Review `pending`.
- Test-Traceability: `PF-TN-007`.
- Implementierungszuordnung: [PR #5](https://github.com/HurricanVD/photocraft-amiga/pull/5) (bereits vor Atomisierung umgesetzt, noch nicht gemergt).
- Test-Evidenz: ausgeführt auf PR-Head 0af265e; formaler Story-/Review-Abschluss fehlt.
- Host/ASan/UBSan/Rust-Differential und GCC13-vamos nur fuer eindeutig belegten Scope als PASS markieren.
- Manuelle AmigaOS/WinUAE/MiniGL-Pruefung: hier `n/a` als OS-freier Spike; fuer Produktintegration gesondert `required`.

## 7. refinement_note / refinement_triage

- Trennung: genau eine fachliche Aenderungsachse und eigene Testnachweise.
- `refinement_triage`: `needs_architecture`.
- IP-/Third-Party-Klassifikation: `reference_only` (oeffentliche PhotoCraft-Rust-Originalsemantik; Code nur als Referenz, keine Uebernahme fremder geschuetzter Quellen).
- Shipping-Material aus Drittrepos: `no`; rights_status fuer proprietaere Dunkelkammer: `not_required` in diesem Scope.
- Spike-/Design-Bedarf: `retrospective accepted ADR audit; missing gate documentation`.
- Refinement-Ergebnis: `ready_for_architecture`, **nicht** `ready`.

## 8. atomization_report

- decision: `keep_single_story` fuer diesen begrenzten Kandidaten, vorbehaltlich finalem Refinement-/Scope-Audit.
- parent_decision: `PF-SP-002: split_required`.
- Aenderungsachse: Gray8-LayerMask-Sparse-COW.
- Dependencies: PF-SP-005 und PF-SP-004; ADR-0002.
- Lifecycle-Blocker: initiales/finales Architekturreview, DoR/IP-Triage, Implementation-Gate und finales Reviewer-Gate offen.

## 9. Review und Gate-Status

- `arch_review.initial`: `pending`
- `arch_review.pre_ready_final`: `pending`
- `implementation_gate_report`: `missing/pending` (kein nachtraeglich erfundenes PASS)
- `review_report`: `pending`
- `done`: `not_authorized`
- Naechster Schritt: Child-Refinement und formale Lifecycle-Abnahme; bei PR #3–5 bereits erfolgte Aenderungen retrospektiv sauber auditieren.
