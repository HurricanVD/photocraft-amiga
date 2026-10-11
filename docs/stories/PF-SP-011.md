# PF-SP-011 — Minimale ReAction-Fenster- und Event-Shell

- Story-ID: `PF-SP-011`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `draft` (keine Implementierungsfreigabe)
- Prioritaet: `P2`
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
- Proprietären Dunkelkammer-ReAction-Code ohne Rechtefreigabe kopieren.
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
- `refinement_triage`: `needs_architecture`.
- `atomization_report.decision`: `keep_single_story` (vorlaeufig; formale Pruefung offen).
- `arch_review.initial`: `pending`.
- `rights_status`: `pending` fuer jede moegliche externe UI-Code- oder Asset-Uebernahme; no implementation authorized.

## 8. Testtraceability

- `PF-TN-014` geplant; WinUAE/AmigaOS-Test ist `required/not_run`.
- Build-/CI-/Reviewer-Gates muessen getrennt und belegbar abgeschlossen werden.

## 9. Review und Gate-Status

- `arch_review.pre_ready_final`: `pending`
- `dor_check`: `pending`
- `implementation_gate_report`: `pending`
- `review_report`: `pending`
- `done`: `not_authorized`
