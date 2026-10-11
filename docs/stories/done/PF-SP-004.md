# PF-SP-004 — Flat-Layer-Reihenfolge und Deckkraft

- Story-ID: `PF-SP-004`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `done` (2026-10-11; Flat-Layer-Shift/Opacity integriert, reviewt, getestet und gemergt)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Parent: `PF-SP-002`; [atomization_report](../../reviews/PF-SP-002-atomization-report-2026-10-11.md)
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
- Architekturquelle: akzeptierte ADR-0001/0002/0003; eigener [Child-Pre-ready-Review](../../reviews/PF-SP-004-ready-gates-2026-10-11.md) am 2026-10-11 mit `pass` vor der erneuten Integration dokumentiert.
- Test-Traceability: `PF-TN-005`.
- Implementierungszuordnung: [PR #3](https://github.com/HurricanVD/photocraft-amiga/pull/3) (historischer Implementierungsbeginn vor Atomisierung; neu gegatete Integration am 2026-10-11 erfolgreich gemergt).
- Test-Evidenz: ausgeführt auf PR-Head f998b9d; formaler Story-/Review-Abschluss fehlt.
- Host/ASan/UBSan/Rust-Differential und GCC13-vamos nur fuer eindeutig belegten Scope als PASS markieren.
- Manuelle AmigaOS/WinUAE/MiniGL-Pruefung: hier `n/a` als OS-freier Spike; fuer Produktintegration gesondert `required`.

## 7. refinement_note / refinement_triage

- Trennung: genau eine fachliche Aenderungsachse und eigene Testnachweise.
- `refinement_triage`: `pass`
- IP-/Third-Party-Klassifikation: `reference_only` (oeffentliche PhotoCraft-Rust-Originalsemantik; Code nur als Referenz, keine Uebernahme fremder geschuetzter Quellen).
- Shipping-Material aus Drittrepos: `no`; rights_status fuer proprietaere Dunkelkammer: `not_required` in diesem Scope.
- Spike-/Design-Bedarf: `retrospective accepted ADR audit; missing gate documentation`.
- Refinement-Ergebnis: historischer Zustand war `ready_for_architecture`; der eigene Child-DoR/Implementation-Gate wurde vor der neuen Integration dokumentiert und die Story nach Review abgeschlossen.

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
- `review_report`: `approve`
- `done`: `completed`
- Naechster Schritt: Child-Refinement und formale Lifecycle-Abnahme; bei PR #3–5 bereits erfolgte Aenderungen retrospektiv sauber auditieren.

## Priorisierung 2026-10-11

- Prioritaet: `P1` — PR #3 ist erster technisch bereits bearbeiteter Layer-Baustein der Abhaengigkeitskette.
- Quelle: [Prioritaets-/Ready-Audit](../../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine bestehenden Implementation-/CI-/Reviewer-Gates; dokumentierte Story-Statuswerte bleiben massgeblich.

## Child-Gates 2026-10-11

- Separater aktueller [Refinement-/Architektur- und DoR-Audit](../../reviews/PF-SP-004-ready-gates-2026-10-11.md). Keine Rueckdatierung der historischen PR3-Code-Commits.
- Bestehender Layer-Code wird auf aktuellem main mit PF-BG-001-Ownership erneut integriert und auf aktuellem Head neu getestet.

- [Implementation-Gate](../../reviews/PF-SP-004-implementation-gate-2026-10-11.md) erfasst; Scope: Overlay P0-Owner-Safety, Flat-Shift/Opacity, Rust-Oracle, Host/68k, keine Gruppen/Masken.

## Integrations-Review-Handoff 2026-10-11

- Post-Gate-C99-Code und Tests auf PR #3 integriert, P0-Claim erhalten. [Port-Run #38158436702](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38158436702) Host/ASan/UBSan, originale Rust-Parity, GCC13 HUNK/vamos O0/O2 success. [Root-CI #38158436706](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38158436706) noch als einzelne allgemeine Jobs in Bearbeitung zum Handoff; nicht als full PASS bezeichnet.
- Historischer Review-Handoff: Damals standen Abschlussreview und Archivierung noch aus; beides ist im späteren finalen Story-Abschluss unten nachgewiesen.

## Finaler Story-Abschluss — 2026-10-11

- [Finaler Reviewreport](../../reviews/PF-SP-004-final-review-2026-10-11.md): `review_report: approve` als fachliche, auf Scope und Eignung begrenzte Empfehlung, nicht als GitHub-Self-APPROVE-Submission.
- Testnachweis [PF-TN-005](../../tests/done/PF-TN-005.md), auf PR-Head `40ec4a17d023954a6e2752b4f676d77ba35a2143` [CI #38159331127](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159331127), [AmigaOS Host/GCC13/vamos #38159331128](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159331128), [Docs #38159331123](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159331123), [PF Preflight #38159331153](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159331153), [Packaging #38159331130](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159331130) jeweils `completed/success`.
- [PR #3](https://github.com/HurricanVD/photocraft-amiga/pull/3) gemergt nach `main`, Commit `e3b1c183ce1f1b3dece8a72728a8c811222bed77`; Rust-Original unveraendert, P0-Owner-Claim erhalten.
- Manuelle GUI-/WinUAE-Testpflicht fuer den isolierten headless C99-Teil: `n/a` mit Host/Sanitizer, Rust-Differential und GCC13/vamos-Regression als Fallback. Keine Editor- oder Release-Freigabe.
- Archiviert unter `docs/stories/done/PF-SP-004.md` und `docs/tests/done/PF-TN-005.md`.
