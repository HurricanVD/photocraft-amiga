# PF-SP-011 — Minimale ReAction-Fenster- und Event-Shell

- Story-ID: `PF-SP-011`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `ready` (Pre-ready-Audit 2026-10-12; no implementation)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-014` (geplant; `not_run`)

## 1. Problem / Nutzerwert

Fuer AmigaOS 3.2 fehlt eine native Window-/Event-Shell. Sie soll mit einem austauschbaren, CPU-getriebenen Viewport beginnen, ohne die noch blockierte MiniGL-Laufzeit zur Pflicht zu machen.

## 2. In Scope

- Minimaler ReAction-/Intuition-Host: Fenster oeffnen/schliessen, Refresh und Resize verarbeiten, Ereignisloop sauber beenden.
- Einen *bereitgestellten* Viewport-Inhalt ueber den Port-eigenen Presenter anzeigen; keine Portierung des Rust-egui-Layouts.
- Fehlerpfade fuer fehlende Klassen/Bibliotheken, Fenstererzeugung und Presenter-Bindung dokumentieren.
- Ressourcen-/Lifetime-Vertrag zwischen Fenster, Presenter und CPU-Pixelpuffer festlegen.

## 3. Nicht-Ziele

- Bildbearbeitungs-Menues, Werkzeuge, Dokumentdialoge, Engine-Command-Bridge oder PSD-Funktionalitaet.
- GPU-Auswahl-/MiniGL-Integration; das ist eine getrennte Folgearbeit.
- Dunkelkammer-ReAction-Code ohne dokumentierte oeffentliche Weitergabe-/Provenienzfreigabe uebernehmen.
- Fertiger AmigaOS-Editor, Produktcompiler- oder Releasefreigabe.

## 4. User Story

Als PhotoCraft-Portentwickler moechte ich eine kleine AmigaOS-Fenster- und Event-Shell mit austauschbarem Viewport, damit spaetere Editorfunktionen auf einer geprueften nativen Host-Schicht aufbauen.

## 5. Akzeptanzkriterien

1. In einem echten AmigaOS-3.2-Target oeffnet sich ein Fenster, zeichnet die definierte Testansicht und kann ohne Absturz geschlossen werden.
2. Refresh/Resize fuehren zu konsistenter Neudarstellung; auch wiederholtes Oeffnen/Schliessen hinterlaesst keine verwaisten Ressourcen.
3. Fehlende erforderliche OS-Komponenten und Presenter-Fehler fuehren zu kontrollierten Meldungen statt Crash.
4. Die Event-/UI-Schicht ruft keine plattformabhaengigen APIs aus dem portablen Raster-/Dokument-Core auf.
5. Manuelle WinUAE-Laufzeitevidenz mit Testmatrix, Build-SHA, Logs und Screenshots ist vorhanden; automatische WinUAE-Steuerung bleibt deaktiviert, solange der Prozess dies verlangt.

## 6. Architektur / Tests / Grenzen

- Betroffen: neuer begrenzter AmigaOS-Host unter `ports/amigaos3/`, akzeptierte ADR-0001/0002/0003.
- Abhaengigkeiten: PF-SP-010 (Software-Presenter) und fuer echte Bildinhalte PF-SP-007; MiniGL PF-SP-001 nur optional.
- Vor Integration: ReAction/Intuition-API, NDK-Version, Rechte-/Provenienzregister und externe Toolchain benoetigen gesondertes Review.
- Kein Host-Test ersetzt den manuellen GUI-Zieltest.
- Keine Implementierungs-PR und noch keine CI-/Runtime-Evidenz.

## 7. refinement_note / refinement_triage

- Eine Aenderungsachse: native Fenster-/Ereignis-Shell, keine Renderalgorithmen.
- `refinement_triage`: `pass`
- `atomization_report.decision`: `keep_single_story` (vorlaeufig; formale Pruefung offen).
- `arch_review.initial`: `pass`
- `rights_status`: `pending` fuer jede moegliche externe UI-Code- oder Asset-Uebernahme; no implementation authorized.

## 8. Testtraceability

- `PF-TN-014` geplant; WinUAE/AmigaOS-Test ist `required/not_run`.
- Build-/CI-/Reviewer-Gates muessen getrennt und belegbar abgeschlossen werden.

## 9. Review und Gate-Status

- `arch_review.pre_ready_final`: `pass`
- `dor_check`: `pass`
- `implementation_gate_report`: `pending`
- `review_report`: `pending`
- `done`: `not_authorized`

## Priorisierung 2026-10-11

- Prioritaet: `P1` (Planungsanpassung 2026-10-12) — Native Shell ist auf dem kritischen Pfad zum ersten sichtbaren autarken Amiga-Editor; OS-Capability/IP-/Target-Gates bleiben offen.
- Quelle: [Prioritaets-/Ready-Audit](../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine Implementation-/CI-/Reviewer-Gates.

## Dunkelkammer-ReAction-Wiederverwendung (Planung 2026-10-12)

- Quellkandidaten: `HurricanVD/dunkelkammer` @ `26295506ba363b1bd52bfca1852b0230ffb2bad9`, `src/ui/dk_ui.c` (nur Fenster-/Gadget-Lifecycle-Ausschnitte), `src/app/main.c` (Workbench/StackSwap) und bei Bedarf `src/ui/dk_ui_splash.c/.h` (Splash ist optional, nicht MVP-Blocker).
- **Keine Monolith-Uebernahme** von `dk_ui.c`: Dunkelkammer-`DkDocument`, `DkSession`, `DkHistory`, Werkzeug- und Datenmodell bleiben draussen. Eigenes kleines `pc_ui_shell`/Adaptermodul; ein Fenster-Owner, klare Disposition von BOOPSI-Klassen und Screen/RTG-Ressourcen.
- Entwicklungserlaubnis zur Wiederverwendung vom Eigentuemer ist gegeben; oeffentliche Lizenz-/Provenienzpruefung vor direkter Codekopie als [PF-SP-012](PF-SP-012.md). Keine vererbten Logos/Icons/Fonts/SDKs.
- Der Shell-Test darf einen synthetischen Presenter-Frame anzeigen, unabhaengig vom noch offenen CPU-Compositor [PF-SP-007](PF-SP-007.md). Der native Layer-/Pointer-/Command-Ausbau erfolgt in [PF-SP-013](PF-SP-013.md), [PF-SP-014](PF-SP-014.md) und [PF-SP-015](PF-SP-015.md), Kontextmenues/Dialoge in [PF-SP-016](PF-SP-016.md).
- ReAction/Intuition-Open/Close, Resize, Refresh, Workbench-Start sowie kontrollierter Stack-/OOM-Fallback muessen auf echter AmigaOS-3.2-Zielumgebung nachgewiesen werden. PF-TN-014 bleibt bis dahin `not_run`.

## MIT-Nachweis 2026-10-12

- Die Lizenz der Dunkelkammer-Eigenanteile ist seit [Merge PR #9](https://github.com/HurricanVD/dunkelkammer/pull/9) am Commit `70897e91c94214421b0595b3e858446d6b3586ea` **MIT** (Copyright 2026 Virtual Dimension). UI-Quellenstand: `26295506ba363b1bd52bfca1852b0230ffb2bad9`.
- Kopien lizenzierter Eigenanteile sind unter den MIT-Bedingungen moeglich. Vor Quellimport: Herkunft je Datei, MIT-Copyright-/Permission-Notice, VirtualCommander-/vxplatform-Anteile und unlizenzierte Fremdassets kontrollieren; [Quellmatrix](../../ports/amigaos3/docs/dunkelkammer-ui-source-inventory.md).
- Keine kopierten Quellen oder Tests in diesem Dokumentations-PR. Story weiterhin `draft`, Umsetzungsgates und WinUAE-Evidenz `not_run`.

## Pre-ready delta 2026-10-12

- Reviewed design and acceptance: One ReAction window owner, native shell, Workbench/Shell start, resize/refresh/close, stack/cleanup/error tests and a synthetic test image.
- Architecture: accepted ADR-0001/0002/0003; native offline 68040+ / 32 MiB RAM. Rust upstream remains the oracle, never a remote runtime. This limited, independent spike is `keep_single_story`.
- Evidence: [PF-all-open-ready-review](../reviews/PF-all-open-ready-review-2026-10-12.md). Initial architecture / scope refinement / separate final pre-ready review and DoR pass are **planning checks**, not product runtime results.
- IP: original PhotoCraft semantics are reference-only; any reused Dunkelkammer source is MIT for owner material (merge PR #9), retain each copyright and third-party notice.
- Next stage: individual `implementation_gate_report` before work, unmet dependency verification as applicable. All actual native tests and target GUI checks still `not_run`.
