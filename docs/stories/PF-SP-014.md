# PF-SP-014 — Canvas-Navigation und Pointer-Queue aus Dunkelkammer adaptieren

- Story-ID: `PF-SP-014`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `draft` (keine Implementierungsfreigabe)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-12
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-018` (geplant, `not_run`)
- Dunkelkammer Quell-Pin: `26295506ba363b1bd52bfca1852b0230ffb2bad9` (`main`, 2026-08-20)

## 1. Problem / Nutzerwert

Das PhotoCraft-CPU- und Dokumentmodell enthaelt keine native Navigation und keine robuste ReAction-Pointer-Erfassung. Dunkelkammer bietet Zoom/Pan, Region-Invalidierung und eine begrenzte Pointer-Queue mit Bewegungsevent-Dedup.

## 2. In Scope

- Nur viewportbezogene Geometrie aus src/canvas/dk_canvas.c/.h adaptieren: Canvas/Document-Koordinaten, Zoom, Pan, Resize, Cache-Invalidierung und Dirty-Regionen auf PhotoCraft beziehen.
- Die Pointer-Queue aus src/ui/dk_ui_event.c in eine eigene UI-Komponente uebernehmen und Datenvertrag fuer Down/Move/Up, Qualifier, Fensterverluste und Overflow definieren.
- RGBA/ARGB-Staging zum PF-SP-010-Presenter und CPU-/Render-Regionen abgrenzen; PhotoCrafts 256x256-Tiles und negativen Dokumentkoordinaten folgen.
- Initialer Scope ausschliesslich Hand/Zoom/Pan und Navigation; Interaktion muss ohne MiniGL funktionieren.

## 3. Nicht-Ziele

- Keine Dunkelkammer-Pixel-/Tile-/Dokumentalgorithmen uebernehmen.
- Kein kompletter Brush-, Selection- oder Transform-Tool-Stack.
- Keine Full-Frame-Kopie je Pointer-Move; keine GPU-Anforderung.

## 4. User Story

Als PhotoCraft-Amiga-Nutzer moechte ich mit Maus und Tastatur flessig zoomen und verschieben koennen, ohne dass der Canvas fehlerhaft neu gezeichnet wird.

## 5. Akzeptanzkriterien

1. Canvas-zu-Dokument-Koordinaten stimmen an 256er-Tile-Grenzen, bei negativen Offsets sowie nach Resize und Zoom.
2. Pointer-Queue behaelt Press/Release-Folgen bei Last und verwirft nur redundante Move-Events; Fokusverlust/Cancel hinterlaesst keinen klemmenden Drag.
3. Pan/Zoom verwenden begrenzte Dirty-Regionen und gecachten Present; keine Pixel neben dem Canvas werden beschrieben.
4. Host-Navigations-/Eventtests, m68k-Smoke und manueller WinUAE-Refresh-/Resize-/Drag-Nachweis liegen mit RAM-Messungen vor.

## 6. Architektur, Abhaengigkeiten und Grenzen

- Ziel: `ports/amigaos3/` (C99-Ausnahme), AmigaOS 3.2, 68040+, RTG/P96, 32 MiB Fast-RAM-Baseline.
- Abhaengigkeiten: PF-SP-010 (Presenter), PF-SP-011 (Shell), PF-SP-012 (Freigabe). Echtes Compositorbild nach PF-SP-007; synthetische Testgrafik bereits vorher erlaubt.
- PhotoCraft Rust bleibt semantische Referenz; 256x256-Sparse-Tiles, Gruppen, Masken und Commands gehoeren dem PhotoCraft-Kern. Kein Ersatz durch Dunkelkammer-Dokument-/Rastermodell.
- Provenienz: Dunkelkammer src/canvas/dk_canvas.c/.h und src/ui/dk_ui_event.c @ source pin; keine DK_CORE_TILES oder DK-Dokumentstruktur.
- Explizite Eigentuemeraussage zur Wiederverwendung liegt vor; bei direkter Veroeffentlichung kopierter proprietaerer Quelldateien ist der dateibezogene Lizenz- und Notice-Nachweis aus PF-SP-012 vor der Codeuebernahme erforderlich.
- Der Code-Import ist kein Scope dieses Dokumentations-PRs. Kein MiniGL als harte Abhaengigkeit; keine Produkt-/ABI-/Release-Freigabe.

## 7. Tests und Evidenz

- PF-TN-018: Roundtrip-Koordinaten, Queue-Stress, Drag-Abbruch, Resize, Dirty-Rect-Guard, WinUAE-Mausnavigation.
- Host-/ASan/UBSan-Tests fuer portablen Teil, echte m68k-HUNK/vamos-O0/O2-Smokes fuer anwendbaren Teil; GUI/Presenter erfordert zusaetzlich manuellen WinUAE-AmigaOS-3.2-Nachweis.
- Prozessgesteuerte automatisierte WinUAE-UI-/Broker-/Capture-Skripte bleiben `disabled_by_process` und werden nicht ausgefuehrt.
- Alle neuen Tests und Runtime-Checks: `not_run`; keine PASS-Evidenz aus Dunkelkammer wird als PhotoCraft-Port-PASS ausgegeben.

## 8. Refinement / atomization_report

- `refinement_triage: needs_architecture`; `atomization_report.decision: keep_single_story` (vorlaeufig, formaler Scope-/IP-/Architekturreview offen).
- Aenderungsachse: Canvas-Navigation und Pointer-Queue aus Dunkelkammer adaptieren.
- `rights_status: pending_PF-SP-012`.
- `arch_review.initial: pending`.
- Vor `ready`: separate `arch_review.pre_ready_final: pass` und `dor_check: pass` mit echten Belegen erforderlich; WIP-Limit bleibt unveraendert.

## 9. Review-/Implementierungsgates

- `arch_review.pre_ready_final: pending`
- `dor_check: pending`
- `implementation_gate_report: missing/pending`
- `review_report: pending`
- `test_evidence: not_run`
- `done: not_authorized`
