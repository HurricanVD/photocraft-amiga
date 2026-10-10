# atomization_report — PF-SP-002 (retrospektiv)

- Datum: 2026-10-11
- Rolle: `ATOMIZE_STORY` (read-only Scopetaudit; Schreibkorrektur erfolgt getrennt im anschliessenden Refinement-/Dokumentationsschritt)
- Prozesspin: VD Amiga 0.2.1; Quelle: `skills/vd-amiga-atomize-story/SKILL.md`, `docs/process/story-lifecycle.md`
- Source-Freeze: PhotoCraft `main 1fb5dd87b5a6ef912b187f43b711ce22966b5deb`; GitHub-PR #3 `f998b9d`, #4 `f7ff7802`, #5 `0af265e`
- Verfuegbarkeit: kein nativer delegierbarer Reviewer-/Atomisierungs-Subagent in dieser Chat-Runtime; dokumentierter getrennter Sequenzfallback, kein unabhaengiges Subagent-Review
- **decision: `split_required`**
- **parent_status: `blocked`** fuer weitere Arbeit und `ready`-/`done`-Promotion; historischer in_progress-/CI-Stand bleibt nachvollziehbar

## Befund und Ursachen

Der bisherige Sammel-Spike PF-SP-002 enthielt unabhaengig lieferbare Aenderungsachsen: geometrische/typed Sparse-Raster-Paritaet, Flat-Layer/Deckkraft, Gruppenbaum, LayerMask, CPU-Compositor, Commands/History, Persistenz sowie Allokation/Tile-Indexing. Fuer die letzteren werden separate Architektur-/Performancevertraege verlangt. Eine einzelne DoR/Review/Done-Entscheidung kann all diese Gegenstaende nicht abdecken; die gestapelten PRs und PF-TN-005/006/007 sind selbst bereits die Evidenz fuer separate Umfangsgrenzen.

**Prozessabweichung:** PF-SP-002 war `in_progress`, ohne in der kanonischen Story belegtes `atomization_report`, `refinement_triage`, finales `pre_ready_final` oder `implementation_gate_report`. Grüne CI ist **keine** nachtraegliche Autorisierung der Statuspromotion. Keine rueckdatierte Freigabe behaupten.

## Aenderungsachsen und Teilstory-Zuordnung

| Achse | Kindstory | Implementierungsstand | Tests/PR | Voraussetzung |
|---|---|---|---|---|
| Urspruenglicher raw typed sparse Raster/COW-Spike | PF-SP-002 (historische Baseline) | auf main, Teil-Evidenz | PF-TN-002/004 | Governanceabschluss separat |
| Root-Layer-Metadaten und Reorder | PF-SP-004 | Implementierungs-PR offen | PF-TN-005 / [PR #3](https://github.com/HurricanVD/photocraft-amiga/pull/3) | PF-SP-002 Core-Baseline |
| Gruppen, Tiefengrenze, Clone | PF-SP-005 | gestapelter Implementierungs-PR | PF-TN-006 / [PR #4](https://github.com/HurricanVD/photocraft-amiga/pull/4) | PF-SP-004 |
| Gray8 LayerMask | PF-SP-006 | gestapelter Implementierungs-PR | PF-TN-007 / [PR #5](https://github.com/HurricanVD/photocraft-amiga/pull/5) | PF-SP-005 |
| CPU-Compositor | PF-SP-007 | draft | PF-TN-008 geplant | PF-SP-004/005 und ggf. 006 |
| Modell-Commands/Undo | PF-SP-008 | draft | PF-TN-009 geplant | PF-SP-004/005 |
| Dokument-Persistenz | PF-SP-009 | draft | PF-TN-010 geplant | PF-SP-005/006 und Formatentscheid |
| Tile-Suche, OOM/Fault/Heapbudget | PF-TD-001 | draft | PF-TN-011 geplant | typed Raster-Baseline |

Die Child-Story-Dateien enthalten Scope, ausdrueckliche Nicht-Ziele, Akzeptanzkriterien, Abhaengigkeiten und Testtraceability. PF-SP-004/005/006 bleiben `refining`, nicht `in_progress`, obwohl **historische Codeaenderungen** bereits in ungemergten PRs liegen. Diese Aenderungen duerfen nur nach echter separater Gate-/Abnahmeentscheidung gemergt werden; ein nachtraegliches Artefakt macht die fruehere Implementation nicht automatisch prozesskonform.

## Lifecycle-Blocker und WIP

- Alle Kindstorys benoetigen eigenes `refinement_triage`-Audit, Child-Atomisierung, initiales/finales Architekturreview und vor weiterer Implementierung ein formales Implementation-Gate.
- Reale erste Tests und CI-Nachweise erhalten **nur den existierenden Scope**, keinen vollen Produkteffekt und keinen impliziten `done`-Status.
- Vorher `ready` erst bei belegtem DoR. Keine `in_progress`-Child-Story ohne Entry-Gate. Das zentrale WIP-Limit von maximal 2 bleibt eingehalten: hier werden 0 Child-Stories aktiviert.
- Bestandsschutz: PF-SP-003 bleibt `done`; PF-SP-001 bleibt `blocked`; kein Eingriff in Upstream, Release, ABI oder WinUAE-Automation.
- Port-lokale PF-SP-002-Datei ist historischer Stand; **kanonischer Status** liegt unter `docs/stories/PF-SP-002.md`.
- Kein bestehender PR darf als freigegeben gelten, nur weil das Review mehrerer heterogener Aenderungsachsen zufaellig in einer Story dokumentiert wurde.

## Gepruefte Quellen und Offenpunkte

- `docs/stories/PF-SP-002.md`, `docs/backlog.md`, `docs/tests/PF-TN-002.md` und `PF-TN-004/005/006/007`.
- PhotoCraft ADR-0001/0002/0003, `ports/amigaos3/` sowie die bestehenden PRs #3–#5.
- VD Prozess: `skills/vd-amiga-atomize-story/SKILL.md`, `docs/process/story-lifecycle.md`, `docs/templates/story.md`.
- Kein lokaler Worktree in diesem Connectorpfad vorhanden. Staging-Grundlage sind gefrorener remote-main SHA und der vor Mutationen gelesene PR-/Branchzustand.
- Entscheidung zu genauer Compositor-Matrix, History-Grenze, Serialisierungsformat und Allokatorrollback steht aus; neuer tragfaehiger Architekturvertrag/DoR erforderlich.

## Naechste Rolle / naechster Schritt

`Product Owner / Refinement`: sieben Teilstorys unter kanonischen PF-IDs verfeinern, danach Architektur- und Lifecycle-Gates. Die PRs #3/#4/#5 werden auf PF-SP-004/005/006 verwiesen und bis dahin als nicht mergefreigegebene Drafts gefuehrt. PF-SP-002 selbst wird nicht als `done` archiviert.
