# PF-SP-004/005/006 — Getrennter Pre-ready-/Integrationsreview

- Datum: 2026-10-11
- Basis: `main` 4f2b0cc9eb18d342d4cf63e4cc3668ef595a127a
- Prozess: VD-Amiga 0.2.1, `ready` erst nach nachweisbarem `refinement_triage: pass`, eigenstaendigem **`arch_review.initial: pass`**, nachfolgendem **`arch_review.pre_ready_final: pass`** und `dor_check: pass`. Beide Architekturreview-Stufen sind unabhaengige Gates.
- Reviewform: statisches Delta-/Gate-Audit mit GitHub-Diffs und realen CI-Run-/Job-Metadaten; kein lokaler Full-Build oder unabhängiges menschliches Review
- Ausfuehrung: GitHub-Review-Kommentare in PR #3/#4/#5 vorhanden.
- **Entscheidung fuer alle drei Child-Stories:** `arch_review.initial: pending_remediation`; `arch_review.pre_ready_final: pending_remediation`; `dor_check: fail/pending_remediation`; `status_after_review: refining`; `merge_allowed: no`.
- Grund: ungeklärte dokumentuebergreifende Pointer-Ownership-Integration, noch nicht auf aktualisierten Basis-/Fix-Commit integriert, formale Child-Gates fehlen. Keine rückdatierte Zustimmung.

## Aktuelle technische CI-Evidenz (unabhaengig von Lifecycle)

| Story | Draft-PR | Finaler PR-Head zum Audit | Aktuelle CI | Beobachtung |
|---|---|---|---|---|
| PF-SP-004 | [#3](https://github.com/HurricanVD/photocraft-amiga/pull/3) | `195690e50f7a1bbe9933524e9af9f4c609aa3def` | [#38094893931](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38094893931) — `success` | Rust/C99-Document-Parity, GCC13/vamos O0/O2, Linux/macOS/Windows bestanden |
| PF-SP-005 | [#4](https://github.com/HurricanVD/photocraft-amiga/pull/4) | `0ee7d99ee056aabfec7f88f0cc2acb1becdc6eb9` | [#38095522427](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38095522427) — `success` | Rust/C99-Group-Parity, GCC13/vamos O0/O2, Linux/macOS/Windows bestanden |
| PF-SP-006 | [#5](https://github.com/HurricanVD/photocraft-amiga/pull/5) | `7bc7d604ea5a5f3e04c32c6e7a19d49bf5c31540` | [#38095619323](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38095619323) — `success` | Rust/C99-Mask-Parity, GCC13/vamos O0/O2, Linux/macOS/Windows bestanden |

Die erfolgreichen Runs sind **echte historische Nachweise auf den obigen Heads**. Ein Rebasing/Cherry-Pick des P0-Owner-Fixes erzeugt einen neuen Commit und erfordert eine **erneute** final-head-Ausfuehrung. Sie sagen zudem nichts über das noch nicht vorhandene Cross-Document-Ownership-Regressionstestszenario im Gesamtstapel aus.

## 1. PF-SP-004 — Flat Layers / Opacity (PR #3)

- Umfasst Layer-Shift, Opacity und Fill-Opacity als klar abgegrenzten C99-Teilumfang.
- Bewahren: stabile IDs, Reihenfolge bottom-first, F32-Metadaten, Rust-Oracle-Verhalten und Soft-Float-Testprofil.
- [PR #6](https://github.com/HurricanVD/photocraft-amiga/pull/6) ist bereits nach `main` gemergt. #3-Diff enthaelt dennoch Governance-/Statusdateien aus dem frueheren Stack. Beim Rebase keine `PF-SP-002 blocked`- bzw. neuen Prioritaetsentscheidungen ueberschreiben.
- Der P0-Fix [PF-BG-001 / #8](https://github.com/HurricanVD/photocraft-amiga/pull/8) erweitert die gemeinsame `pc_document_append_raster()`-Owning-Grenze. #3 muss gegen den **integrierten** Fix samt neuer Regression und finalem CI neu validiert werden.
- Entry-Gate: Kindstory-Refinement, ein eigenstaendiges initiales `arch_review.initial` und das danach ausgefuehrte finale `arch_review.pre_ready_final` fehlen formal noch; derzeit `refining`.

## 2. PF-SP-005 — Gruppen / Deep-COW (PR #4)

- Der Gruppenbaum besitzt mehrere Appends ueber `pc_append()` (Root- und Child-Raster) und rekursive `pc_layer_clone()`-Pfade.
- **Blockierender Sicherheitsbefund:** Vor der neuen P0-Integration ist keine Dokument-uebergreifende Owner-Claim-Pruefung enthalten. Ein `PcRaster*` kann mehreren `PcDocument`-Instanzen gehoeren. Nach Anwendung von #8 muessen **auch** Child-Appends und die direkt zugewiesenen Clones als eigene Owner registriert werden.
- Die rekursive Clone-Implementierung darf keine unclaimed Clones in Owning-Knoten belassen. Dies muss in ASan/UBSan-Tests fuer Root-/Nested-/Cross-Document- und Clone/Destroy-Szenarien nachgewiesen werden.
- Erst #3, dann #4 integrieren/retargeten und auf finalem Head erneut alle diff/target/CI-Gates ausfuehren.
- Formales `arch_review.initial`, anschliessendes `arch_review.pre_ready_final` und Child-DoR-Gate weiterhin `pending`.

## 3. PF-SP-006 — Layer Masks (PR #5)

- `pc_owns_surface()` aus #5 scannt nur die **vorhandenen** Layer/Masken eines Dokuments. Es verhindert weder ein zweites Owning in einem anderen Dokument noch bereits anderweitig beanspruchte Raster-Klone.
- `pc_document_attach_mask()` ist damit ohne Integration von #8 potenziell Double-Free-gefaehrdet. Der P0-Owner-Claim muss auch Mask-Attach abdecken.
- `pc_document_detach_mask()` gibt derzeit den Mask-Pointer zurueck; nach Ownership-Claim-Integration muss ein sauber definierter `release`-Pfad den Owner an den Caller zurueckuebertragen (ohne Double-Free, Reattach an ein anderes Dokument muss moeglich bleiben).
- Rekursiver Mask-Clone: eigener Owner und COW-Isolation; kein unregistriertes zweites Owning.
- Validierung: derselbe Mask-Pointer in zwei Dokumenten, als Layer-Raster und Maske, Detach/Reattach, Invalid-Format, Clone/Destroy, ASan/UBSan, Rust-Differential und GCC13/vamos auf aktuellem finalen Head.
- Formales `arch_review.initial`, anschliessendes `arch_review.pre_ready_final` und Child-DoR-Gate `pending`.

## Freigabeplan (keine Freigabe vorgezogen)

1. **Erledigt 2026-10-11:** [PR #7](https://github.com/HurricanVD/photocraft-amiga/pull/7) ist auf `main` gemergt (Merge-SHA `2723b00664ca2ef53d2ab5dc34431e3f097ddd4e`). Kanonische Story-/Prioritaetsakten sind jetzt auf `main` vorhanden.
2. PR #8 (PF-BG-001) nach vollstaendigen Host-/m68k-/Review-Gates eigenstaendig integrieren und auf `main` erneut pruefen.
3. #3 auf aktualisiertem `main` mit P0-Fix und ohne veraltete Governance-Rueckschreibungen ausrichten. Child-`refinement_triage`, `arch_review.initial`, `arch_review.pre_ready_final`, `dor_check` einzeln und in dieser Reihenfolge bestaetigen; dann erst `ready` und anschliessend per Implementation-Gate `in_progress` bzw. `review` dokumentieren.
4. #4 auf der akzeptierten #3-Basis aktualisieren, Owner-Claim und Clone-Gates implementieren, Child-`arch_review.initial` und `arch_review.pre_ready_final`, DoR und erneute komplette CI.
5. #5 zuletzt auf #4 aktualisieren, Owner-Claim/Detach/Mask-Clone sicher machen, Child-`arch_review.initial` und `arch_review.pre_ready_final`, DoR und erneute komplette CI.
6. Für jedes Child separate unabhängige Delta- und Abschlussreviews; keine pauschale Freigabe nur aufgrund frueherer Gruen-Laeufe. WIP-Limit 2 wahren.

## Gate-Fazit

- `PF-SP-004: refining`, `PF-SP-005: refining`, `PF-SP-006: refining`.
- `merge_approved: no`; `release_approved: no`.
- Bestehende CI-Ergebnisse: `success` auf den aufgefuehrten Heads; neuer integrierter Stand: `not_run`.
- Keine Aenderung an Produktcompiler, Rust-Upstream, Amiga-GUI oder privatem Dunkelkammer-Code.

## Follow-up 2026-10-11 — Sicherheitsfix und aktuelle Abhaengigkeiten

- [PF-BG-001 / PR #8](https://github.com/HurricanVD/photocraft-amiga/pull/8) wurde gegen `main` retargetet, befindet sich in `review` und hat [gruenen AmigaOS-C99-/ASan-/Rust-/GCC13-vamos-Nachweis](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38126076875). Es ist **noch nicht gemergt**, waehrend allgemeiner CI und Review kontrolliert ausstehen.
- Die konkrete Integrationsarbeit von #3/#4/#5 ist unveraendert blockiert, solange der neue Ownership-Claim nicht auf deren finalen Head portiert und neu getestet wurde.
- Die frueheren Whole-PR-Runs #38094893931, #38095522427, #38095619323 bleiben nachweislich gruen fuer die alten Heads; keine Behauptung ueber den nach P0-Integration neu entstehenden Head.
- Kein neues `ready`-Gate fuer die drei Child-Stories durch diesen Dokumentations-Nachtrag.
