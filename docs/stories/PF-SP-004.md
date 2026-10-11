# PF-SP-004 — Flat-Layer-Reihenfolge und Deckkraft

- Story-ID: `PF-SP-004`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `review` (Flat-Layer-Integration umgesetzt, Port-CI und GCC13/vamos bestanden; gesamte CI und Abschlussreview getrennt)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Parent: `PF-SP-002`; [atomization_report](../reviews/PF-SP-002-atomization-report-2026-10-11.md)
- Backlog: `docs/backlog.md`

## 1. Problem / Nutzerwert

Ebenenreihenfolge, eindeutige IDs, Besitzerwechsel und f32 opacity/fill_opacity. Die bisherige Sammelstory ist nicht atomar. Dieser Teil soll getrennt lieferbar und gegen das unveraenderte PhotoCraft-Rust-Original pruefbar sein.

## 2. In Scope

- bottom-first Shift und stabile IDs
- Layer-Owning und Clone-Isolation
- independent opacity/fill_opacity

## 3. Nicht-Ziele

- Gruppen
- Masken
- Compositor
- stabiler ABI/Produktcompiler
- Keine heimliche Produktcompiler-, GUI-, ABI- oder Release-Freigabe.
- Keine proprietaere Dunkelkammer-Quell-/Asset-Uebernahme.

## 4. User Story

Als PhotoCraft-Portentwickler moechte ich flat-layer-reihenfolge und deckkraft als eigenen pruefbaren Schritt, damit Risiko, Ownership und Testgate klar abgrenzbar bleiben.

## 5. Akzeptanzkriterien

1. Shift- und Fehlergrenzen halten Order/Owner stabil.
2. Endliche Deckkraft 0..1 und Snapshot-Isolation.
3. Host+Sanitizer+Rust-Differential.
4. GCC13 HUNK/vamos O0/O2.

## 6. Architektur / Tests / Grenzen

- Betroffene Module: `ports/amigaos3/` und Original-Rust-Oracles (read-only); auf den PR-Scope begrenzt.
- Architekturquelle: akzeptierte ADR-0001/0002/0003; gesonderter Child-`pre_ready_final`-Review `pending`.
- Test-Traceability: `PF-TN-005`.
- Implementierungszuordnung: [PR #3](https://github.com/HurricanVD/photocraft-amiga/pull/3) (bereits vor Atomisierung umgesetzt, noch nicht gemergt).
- Test-Evidenz: ausgeführt auf PR-Head f998b9d; formaler Story-/Review-Abschluss fehlt.
- Host/ASan/UBSan/Rust-Differential und GCC13-vamos nur fuer eindeutig belegten Scope als PASS markieren.
- Manuelle AmigaOS/WinUAE/MiniGL-Pruefung: hier `n/a` als OS-freier Spike; fuer Produktintegration gesondert `required`.

## 7. refinement_note / refinement_triage

- Trennung: genau eine fachliche Aenderungsachse und eigene Testnachweise.
- `refinement_triage`: `pass`
- IP-/Third-Party-Klassifikation: `reference_only` (oeffentliche PhotoCraft-Rust-Originalsemantik; Code nur als Referenz, keine Uebernahme fremder geschuetzter Quellen).
- Shipping-Material aus Drittrepos: `no`; rights_status fuer proprietaere Dunkelkammer: `not_required` in diesem Scope.
- Spike-/Design-Bedarf: `retrospective accepted ADR audit; missing gate documentation`.
- Refinement-Ergebnis: `ready_for_architecture`, **nicht** `ready`.

## 8. atomization_report

- decision: `keep_single_story` fuer diesen begrenzten Kandidaten, vorbehaltlich finalem Refinement-/Scope-Audit.
- parent_decision: `PF-SP-002: split_required`.
- Aenderungsachse: Flat-Layer-Reihenfolge und Deckkraft.
- Dependencies: Original-Rust doc::Document und Layer; akzeptierte ADR-0002/0003.
- Lifecycle-Blocker: initiales/finales Architekturreview, DoR/IP-Triage, Implementation-Gate und finales Reviewer-Gate offen.

## 9. Review und Gate-Status

- `arch_review.initial`: `pass`
- `arch_review.pre_ready_final`: `pass`
- `dor_check`: `pass`
- `implementation_gate_report`: `pass`
- `review_report`: `pending`
- `done`: `not_authorized`
- Naechster Schritt: Child-Refinement und formale Lifecycle-Abnahme; bei PR #3–5 bereits erfolgte Aenderungen retrospektiv sauber auditieren.

## Priorisierung 2026-10-11

- Prioritaet: `P1` — PR #3 ist erster technisch bereits bearbeiteter Layer-Baustein der Abhaengigkeitskette.
- Quelle: [Prioritaets-/Ready-Audit](../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine bestehenden Implementation-/CI-/Reviewer-Gates; dokumentierte Story-Statuswerte bleiben massgeblich.

## Child-Gates 2026-10-11

- Separater aktueller [Refinement-/Architektur- und DoR-Audit](../reviews/PF-SP-004-ready-gates-2026-10-11.md). Keine Rueckdatierung der historischen PR3-Code-Commits.
- Bestehender Layer-Code wird auf aktuellem main mit PF-BG-001-Ownership erneut integriert und auf aktuellem Head neu getestet.

- [Implementation-Gate](../reviews/PF-SP-004-implementation-gate-2026-10-11.md) erfasst; Scope: Overlay P0-Owner-Safety, Flat-Shift/Opacity, Rust-Oracle, Host/68k, keine Gruppen/Masken.

## Integrations-Review-Handoff 2026-10-11

- Post-Gate-C99-Code und Tests auf PR #3 integriert, P0-Claim erhalten. [Port-Run #38158436702](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38158436702) Host/ASan/UBSan, originale Rust-Parity, GCC13 HUNK/vamos O0/O2 success. [Root-CI #38158436706](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38158436706) noch als einzelne allgemeine Jobs in Bearbeitung zum Handoff; nicht als full PASS bezeichnet.
- `review_report` und `done` stehen bis finalem Scope-/Testreview und Archivierung aus.
