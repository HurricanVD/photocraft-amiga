# PF-SP-004 — Flat-Layer-Reihenfolge und Deckkraft

- Story-ID: `PF-SP-004`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `refining` (PF-SP-002 split_required; nicht `ready`)
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
- `refinement_triage`: `needs_architecture`.
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

- `arch_review.initial`: `pending`
- `arch_review.pre_ready_final`: `pending`
- `implementation_gate_report`: `missing/pending` (kein nachtraeglich erfundenes PASS)
- `review_report`: `pending`
- `done`: `not_authorized`
- Naechster Schritt: Child-Refinement und formale Lifecycle-Abnahme; bei PR #3–5 bereits erfolgte Aenderungen retrospektiv sauber auditieren.

## Retrospektiver Implementierungsnachweis (PR #3, keine historische DoR-Fiktion)

- Vor dem Atomisierungsbericht unter dem zu breiten Parent implementiert; nun der eigenstaendigen Story PF-SP-004 zugeordnet.
- Die experimentelle C99-Implementierung `pc_document.h/.c` bietet bottom-first Root-Reordering (`Document::shift`) und unabhaengige `opacity/fill_opacity` als IEEE-754 f32, ohne native UI oder stabilen ABI.
- Grenzen: Invalid-ID/INT_MIN/INT_MAX, NaN/INF und Bereichsfehler werden ohne mutation zurueckgewiesen; Clone-Ebenen teilen Raster nur als COW.
- Unveraendertes Rust-Original: `crates/doc/src/lib.rs`; PF-TN-005 und Rust/C99-Differential sowie C99/ASan/UBSan und GCC13 13.3 O0/O2 HUNK/vamos im früheren [CI #38033843306](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38033843306) PASS fuer den damaligen Stand.
- **Kein** automatisches `pre_ready_final`, DoR, Implementation-Gate oder `done` durch uebernommene Testevidenz. Aktuelle Child-Story bleibt `refining` bis zum getrennten Architektur-/Lifecycle-Review.
- Zusätzliche Zeilen im ursprünglichen Parent-PR wurden **nicht** in den kanonischen gesperrten Parent zurückgeschrieben; dessen historische Original-Raster-Evidenz bleibt erhalten.
