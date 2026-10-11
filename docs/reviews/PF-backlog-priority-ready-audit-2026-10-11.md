# PF backlog — Priorisierung und Readiness-Audit (2026-10-11)

- Basis: Fork-`main` 4f2b0cc9, kanonischer `docs/backlog.md`, Child-Stories PF-SP-004..009, PF-TD-001, `docs/adr/ADR-0001..0003` und Draft-PRs #3/#4/#5/#7.
- Anlass: expliziter Auftrag, offene und bereits mit Code angegangene Stories zu priorisieren und moegliche offene Stories nach `ready` zu bringen.
- Review-Typ: Dokumentation/Refinement- und Gate-Audit (kein ausfuehrender CI-/Build-/Runtime-Test, kein unabhaengiger Subagent).
- Prozessquelle: `HurricanVD/vd-amiga-dev-process/docs/process/story-lifecycle.md` (VD 0.2.1): `ready` verlangt `refinement_triage`, `arch_review.pre_ready_final`, `dor_check`; `in_progress` verlangt darueber hinaus `implementation_gate_report`.
- **Keine retrospektiv erfundenen PASS-Belege.** Nur zwei zeitgleich dokumentierte Pre-ready-Reviews empfehlen `ready`: [PF-BG-001](PF-BG-001-pre-ready-review-2026-10-11.md) und [PF-TD-002](PF-TD-002-pre-ready-review-2026-10-11.md).

## P0/P1/P2/P3-Schema

- **P0**: potenzieller Speicherkorruptions-/Crash-Fix; vor weiterer Layer-Integration klaeren.
- **P1**: kritischer Kernpfad, Speicherfundament und Abhaengigkeitskette zum ersten gerenderten AmigaOS-Pixel.
- **P2**: wichtiger anschliessender Workstream, optionaler Grafikpfad oder getrennte Produktfunktion.
- **P3**: bewusst spaete, noch abhaengige Funktion oder administrativer eingefrorener Parent.

## Gesamtergebnis: eine Prioritaet und ein Lifecycle-Status pro Story

| Story | Prioritaet | Status nach Audit | Technische / Prozess-Begruendung |
|---|---|---|---|
| PF-BG-001 | P0 | **ready** | Potenzieller Double-Free; Ownership-Kontrakt, Testplan, IP-/ADR- und heutiger DoR-Review dokumentiert; kein Test-PASS |
| PF-TD-001 | P1 | draft | Tile-Index/RAM/OOM grundlegend; Rollback-/Fehlallokator-Kontrakt und finales Architektur-/DoR-Gate ausstehend |
| PF-SP-004 | P1 | refining | Basis fuer Gruppen; PR #3 besitzt Code, aber Child-DoR/pre-ready-Review fehlt |
| PF-SP-005 | P1 | refining | Baut auf #3 auf; PR #4 Draft, kein separates formales Ready-Gate |
| PF-SP-006 | P1 | refining | Masken nach Gruppen; PR #5 Draft, keine Freigabe trotz technischer Teil-CI |
| PF-SP-007 | P1 | draft | CPU-Komposition ermoeglicht Kernrendering; Blend-/Memory-/Rust-Oracle-Matrix erst architektonisch entscheiden |
| PF-SP-010 | P1 | draft | Software-RTG-Viewport als MiniGL-unabhaengige Ausgabe; NDK/RTG-Target-Probe, Capability- und Pre-ready-Gate fehlen |
| PF-SP-008 | P2 | draft | Undo/Redo nach Layergrundlagen; Original-Command/History-Kontrakt und Speicher-Gate fehlen |
| PF-SP-011 | P2 | draft | ReAction-Event-Shell nach RTG; OS-Schnittstellen-/Provenienz- und Target-Evidenz fehlen |
| PF-TD-002 | P2 | **ready** | Dokumentationsabweichung eindeutig; IP none, OS n/a und dokumentierter initial/finaler Pre-ready-/DoR-Check |
| PF-SP-001 | P2 | blocked | Optionaler MiniGL-Zweig, m68k-SDK-/QuarkTex-Nachweis steht aus; keine Laufzeit-Freigabe |
| PF-SP-009 | P3 | draft | Persistenz erst nach Gruppen/Masken und Formatentscheidung; niedrigere Sofortprioritaet |
| PF-SP-002 | P3 | blocked | Historischer Parent mit `split_required`; keine neue Featurearbeit auf diesem Datensatz |

**Bereits begonnene Arbeit:** PR #3/#4/#5 enthaelt vorgezogene Implementierung, doch ihre formalen Child-Stories bleiben `refining`. Ein Draft-PR ist nicht gleich `in_progress`; es findet hier keine rueckdatierte Transition statt.

## Einordnung und Reihenfolge

1. **Sicherheitsfix PF-BG-001** mit seinem `implementation_gate_report`, danach verifizieren, dass PR #3–#5 den Ownership-Fix uebernehmen.
2. **PF-TD-001** Speicher-/OOM-/Tile-Index-Design und anschliessende Child-Readiness, parallel zur getrennten DoR-Nacharbeit fuer **PF-SP-004 → 005 → 006**.
3. **PF-SP-007** CPU-Compositor auf akzeptiertem Modell; **PF-SP-010** nach Presenter-/NDK-Review, fuer echte PhotoCraft-Pixel nach Compositor.
4. **PF-SP-011**, danach vollstaendige UI-/Command-/Formatarbeiten entsprechend ihren eigenen Gates.
5. **PF-SP-001** erst nach Nachweis verfuegbarer optionaler MiniGL-Umgebung; der RTG-Pfad darf nicht an MiniGL blockieren.

## Ready-Blocker je offener Story

- **PF-SP-004/005/006**: eigenes `refinement_triage: pass`, zeitgemaesses finales `arch_review.pre_ready_final: pass`, `dor_check: pass`, und Priorisierung der PR-Abhaengigkeiten. Historische Teil-CI ersetzt keine Child-Entry-Gates.
- **PF-TD-001**: Fehler-/Rollback-/RAM-Vertrag und atomare Testmatrix vor finaler Architekturfreigabe; bei Bedarf weitere Atomisierung.
- **PF-SP-007**: genaue minimale Render-/Blend-/Formatmatrix und Speichervertrag; Original-Rust-Orakel, Host/68k-Nachweise als Plan.
- **PF-SP-008**: konkrete Originaloperation, History-Reversibilitaet, Fehlermodell und endgueltiger Scope.
- **PF-SP-009**: Format-, Endianness-, Provenienz- und Roundtrip-Vertrag.
- **PF-SP-010**: geeigneter RTG/P96-Presenter- und NDK/API-Vertrag, Sandbox-/Ressourcen-/IP-Grenzen, initiales/finales Architekturreview.
- **PF-SP-011**: ReAction-/Intuition-/NDK-/Capability- und Rechtebasis, Presenter-Abhaengigkeit.
- **PF-SP-001**: bestehender Zielruntime-Blocker, unveraendert.
- **PF-SP-002**: `split_required` bleibt frozen; aus Prozessgruenden nicht promoten.

## Gate-Grundsatz und WIP

`PF-BG-001` und `PF-TD-002` haben dokumentierte gegenwaertige **DoR**-Entscheidungen, **kein** `implementation_gate_report` und keine ausgefuehrten neuen Tests. Es stehen **0** kanonische Stories auf `in_progress`; PR #3–#5 bleiben Draft. Das WIP-Limit maximal zwei wird durch reine Prioritaets-/Ready-Mutationen nicht erweitert. `main` erhaelt diesen Stand erst durch einen geprueften Merge des Dokumentations-PRs.
