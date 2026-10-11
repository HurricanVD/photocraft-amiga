# PF-SP-005 — Verschachtelte Rastergruppen und Deep-COW

- Story-ID: `PF-SP-005`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `done` (2026-10-11; Gruppenmodell und Deep-COW integriert, getestet, reviewt, gemergt)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Parent: `PF-SP-002`; [atomization_report](../../reviews/PF-SP-002-atomization-report-2026-10-11.md)
- Backlog: `docs/backlog.md`

## 1. Problem / Nutzerwert

Tree aus Group/Raster, globale IDs, tiefe Owner-COW-Clones und sibling-only Moves. Die bisherige Sammelstory ist nicht atomar. Dieser Teil soll getrennt lieferbar und gegen das unveraenderte PhotoCraft-Rust-Original pruefbar sein.

## 2. In Scope

- Group/Raster append mit parent-ID
- depth<=100
- Duplicate/invalid-parent guards
- Deep clone und lifetime

## 3. Nicht-Ziele

- Ebenenmasken
- PassThrough-Composite
- Cross-Parent-Moves
- PSD
- Keine heimliche Produktcompiler-, GUI-, ABI- oder Release-Freigabe.
- Keine proprietaere Dunkelkammer-Quell-/Asset-Uebernahme.

## 4. User Story

Als PhotoCraft-Portentwickler moechte ich verschachtelte rastergruppen und deep-cow als eigenen pruefbaren Schritt, damit Risiko, Ownership und Testgate klar abgrenzbar bleiben.

## 5. Akzeptanzkriterien

1. Child-Order bottom-first und global IDs.
2. 100 Ebenen ja, 101 nein.
3. Keine Mutation bei Fehler.
4. C99/ASan, Rust Group-Differential, m68k O0/O2.

## 6. Architektur / Tests / Grenzen

- Betroffene Module: `ports/amigaos3/` und Original-Rust-Oracles (read-only); auf den PR-Scope begrenzt.
- Architekturquelle: akzeptierte ADR-0001/0002/0003; gesonderter aktueller [Child-Pre-ready-Review](../../reviews/PF-SP-005-ready-gates-2026-10-11.md) `pass`.
- Test-Traceability: `PF-TN-006`.
- Implementierungszuordnung: [PR #4](https://github.com/HurricanVD/photocraft-amiga/pull/4) (alte Implementierung vor Atomisierung, erneut gegatet und am 2026-10-11 in aktualisiertem Stack gemergt).
- Test-Evidenz: ausgeführt auf PR-Head f7ff7802; formaler Story-/Review-Abschluss fehlt.
- Host/ASan/UBSan/Rust-Differential und GCC13-vamos nur fuer eindeutig belegten Scope als PASS markieren.
- Manuelle AmigaOS/WinUAE/MiniGL-Pruefung: hier `n/a` als OS-freier Spike; fuer Produktintegration gesondert `required`.

## 7. refinement_note / refinement_triage

- Trennung: genau eine fachliche Aenderungsachse und eigene Testnachweise.
- `refinement_triage`: `pass`
- IP-/Third-Party-Klassifikation: `reference_only` (oeffentliche PhotoCraft-Rust-Originalsemantik; Code nur als Referenz, keine Uebernahme fremder geschuetzter Quellen).
- Shipping-Material aus Drittrepos: `no`; rights_status fuer proprietaere Dunkelkammer: `not_required` in diesem Scope.
- Spike-/Design-Bedarf: historisch vorgezogene Gruppen-Codebasis; aktuelle Nachintegration mit Pre-ready- und Implementation-Gates separat dokumentiert.
- Refinement-Ergebnis 2026-10-11: DoR ist bestanden, Gruppen-Nachintegration `in_progress`.

## 8. atomization_report

- decision: `keep_single_story` fuer diesen begrenzten Kandidaten, vorbehaltlich finalem Refinement-/Scope-Audit.
- parent_decision: `PF-SP-002: split_required`.
- Aenderungsachse: Verschachtelte Rastergruppen und Deep-COW.
- Dependencies: PF-SP-004 als Basis; ADR-0002.
- Lifecycle: initiales/finales Architekturreview, DoR und Implementation-Gate jetzt `pass`; CI auf neuem Head und finaler Review noch nicht abgeschlossen.

## 9. Review und Gate-Status

- `arch_review.initial`: `pass`
- `arch_review.pre_ready_final`: `pass`
- `dor_check`: `pass`
- `implementation_gate_report`: `pass`
- `review_report`: `approve`
- `done`: `completed`
- Umsetzung und komplette Testmatrix auf finalem PR-Head abgeschlossen, siehe Finalreview/Archivierung unten.

## Priorisierung 2026-10-11

- Prioritaet: `P1` — PR #4 baut auf PF-SP-004 auf und wird fuer Masken/Gruppen-Rendering benoetigt.
- Quelle: [Prioritaets-/Ready-Audit](../../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine bestehenden Implementation-/CI-/Reviewer-Gates; dokumentierte Story-Statuswerte bleiben massgeblich.

## Child-Gates 2026-10-11

- [Refinement-, Architektur- und DoR-Audit](../../reviews/PF-SP-005-ready-gates-2026-10-11.md) gilt ausschliesslich fuer die neue Integration mit PF-BG-001-Claim und PF-SP-004.
- Alte PR4-Commits waren unter PF-SP-002 ohne separate Child-Freigabe entstanden; diese Historie wird nicht rueckdatiert.

## Finaler Story-Abschluss — 2026-10-11

- [Finalreview](../../reviews/PF-SP-005-final-review-2026-10-11.md): `review_report: approve` als dokumentierte fachliche Empfehlung, keine selbst erteilte GitHub-APPROVE.
- [Testnachweis PF-TN-006](../../tests/done/PF-TN-006.md) auf finalem Code-PR-Head `b2f6290f9fa81d8f0ccd5c269972ea341d399cfa`: [CI #38159390811](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159390811), [AmigaOS Host #38159390900](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159390900), [Docs #38159390847](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159390847), [PF #38159390824](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159390824), [Packaging #38159390888](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38159390888): `completed/success`.
- [PR #4](https://github.com/HurricanVD/photocraft-amiga/pull/4) nach `main` gemergt unter `bbe1e019541c912b9d1abec3e129bd33d571c542`; `PF-SP-004` als vorherige Abhaengigkeit integriert, `PF-BG-001` Claim bleibt erhalten.
- Manuelle WinUAE/MiniGL-Tests `n/a` fuer reinen C99-Document-Tree, automatisierte Original-Rust-Oracle/Host-/m68k-Regression als Fallback. Kein kompletter PhotoCraft-Editor oder Release.
- Story archiviert unter `docs/stories/done/PF-SP-005.md`, Test unter `docs/tests/done/PF-TN-006.md`.
