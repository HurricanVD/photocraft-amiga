# PF-SP-005 — Verschachtelte Rastergruppen und Deep-COW

- Story-ID: `PF-SP-005`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `refining` (PF-SP-002 split_required; nicht `ready`)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Parent: `PF-SP-002`; [atomization_report](../reviews/PF-SP-002-atomization-report-2026-10-11.md)
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
- Architekturquelle: akzeptierte ADR-0001/0002/0003; gesonderter Child-`pre_ready_final`-Review `pending`.
- Test-Traceability: `PF-TN-006`.
- Implementierungszuordnung: [PR #4](https://github.com/HurricanVD/photocraft-amiga/pull/4) (bereits vor Atomisierung umgesetzt, noch nicht gemergt).
- Test-Evidenz: ausgeführt auf PR-Head f7ff7802; formaler Story-/Review-Abschluss fehlt.
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
- Aenderungsachse: Verschachtelte Rastergruppen und Deep-COW.
- Dependencies: PF-SP-004 als Basis; ADR-0002.
- Lifecycle-Blocker: initiales/finales Architekturreview, DoR/IP-Triage, Implementation-Gate und finales Reviewer-Gate offen.

## 9. Review und Gate-Status

- `arch_review.initial`: `pending`
- `arch_review.pre_ready_final`: `pending`
- `implementation_gate_report`: `missing/pending` (kein nachtraeglich erfundenes PASS)
- `review_report`: `pending`
- `done`: `not_authorized`
- Naechster Schritt: Child-Refinement und formale Lifecycle-Abnahme; bei PR #3–5 bereits erfolgte Aenderungen retrospektiv sauber auditieren.

## Technischer Implementierungsstand aus PR #4 (retrospektiv)

- Bereits unter dem alten Sammelspike implementiert; nach formaler Atomisierung ausschließlich PF-SP-005 zugeordnet.
- Experimentelle Gruppen-Raster-Baumstruktur, globale eindeutige Layer-IDs, bottom-first Child-Order, Gruppentiefe maximal 100, sibling-only Shift, Owned-Group-Arrays und COW-Raster-Clones.
- Fehlender oder falscher Parent sowie doppelte ID werden ohne Ownership-Uebergang zurückgewiesen. Kein gruppenweises Rendering, Cross-Parent-Move, Masken-/PSD-/History-Verhalten freigegeben.
- Echte Host C99/ASan+UBSan/51 Original-Rust-Tests, Rust-vs-C Group-Differential und GCC13 13.3 HUNK/vamos O0/O2 im [CI #38042621636](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38042621636) erfolgreich auf früherem Implementierungssnapshot; PF-TN-006 konserviert das.
- `pre_ready_final` und Implementation-Gate für diesen Child-Scope bleiben nachzuholen. Keine Rückdatierung der Freigabe, keine Änderung am administrativ gesperrten Parent PF-SP-002.
