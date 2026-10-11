# PF-SP-010 — Optionalen RTG-Software-Viewport validieren

- Story-ID: `PF-SP-010`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `draft` (keine Implementierungsfreigabe)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-013` (geplant; `not_run`)

## 1. Problem / Nutzerwert

Der experimentelle Port besitzt einen MiniGL-Smoke, aber noch keine nutzbare native Darstellung eines PhotoCraft-CPU-Frames. Da MiniGL/QuarkTex NG in PF-SP-001 weiterhin `blocked` ist, wird ein eigenstaendiger, optionaler RTG-Software-Ausgabepfad gebraucht.

## 2. In Scope

- Port-eigene Present-Schnittstelle: RGBA8-CPU-Ausgabe bzw. begrenzte Dirty-Bands in ein AmigaOS-RTG-Ziel uebertragen.
- `WritePixelArray` und Offscreen-/Cached-Blit-Strategie in einem kleinen Spike evaluieren; exakte Pixel-/Stride-/Kanal-Konvention und Kopiergrenzen vorab festlegen.
- Sichtbare Ausgabe, Clipping, Refresh und Freigabe aller temporären Ressourcen mit reproduzierbarer Target-Evidenz nachweisen.
- Software-Pfad ohne `minigl.library`; gemeinsame Oberflaeche fuer spaeteren optionalen MiniGL-Backendwechsel vorsehen.

## 3. Nicht-Ziele

- Fertige ReAction-Editor-UI, Menues, PSD-Import, Effekte oder GPU-Filter.
- Ersatz von PhotoCraft-256x256-Tiles durch ein anderes Dokumentmodell.
- Behauptete WinUAE-Laufzeitergebnisse ohne manuellen Test; keine Dunkelkammer-Quellen/Assets ohne dokumentierte oeffentliche Lizenz-/Provenienzfreigabe uebernehmen.

## 4. User Story

Als AmigaOS-3.2-Nutzer moechte ich gerenderte PhotoCraft-Pixel auch ohne MiniGL in einem RTG-Fenster sehen, damit der Editor ueber einen belastbaren Software-Ausgabepfad verfuegt.

## 5. Akzeptanzkriterien

1. Ein schriftlicher Pixel-/Stride-/Ownership-Vertrag trennt CPU-Raster und AmigaOS-spezifischen Presenter; keine Amiga-APIs im portablen CPU-Core.
2. Eine definierte Testgrafik und mindestens eine Dirty-Rect-Aenderung werden bei passender RTG/P96-Konfiguration korrekt angezeigt, ohne Nachbarpixel zu veraendern.
3. Invalid Stride, Clipping, Fensterverlust und fehlende RTG-Faehigkeit liefern kontrolliertes Verhalten; Ressourcen werden fehlerfrei freigegeben.
4. Peak-RAM fuer Testfenster/-bands ist erfasst und mit PF-TD-001 abgestimmt; keine dauerhafte unnoetige Full-Frame-Kopie.
5. Hostseitige Pixel-/Conversion-Tests und echte m68k-/WinUAE-Laufzeitevidenz mit Einstellungen, Binary-SHA und Log/Screenshot sind dokumentiert. Solange Target nicht vorhanden: `blocked/not_run`, niemals PASS.

## 6. Architektur / Tests / Grenzen

- Betroffen: ausschliesslich `ports/amigaos3/`; Rueckgriff auf akzeptierte ADR-0001/0002/0003.
- Abhaengigkeiten: PF-SP-007 fuer echte PhotoCraft-Compositor-Pixel; eine festgelegte Testgrafik darf den Presenter vorher isoliert pruefen. PF-TD-001 fuer Speicherbudget.
- PF-SP-001 ist **keine** harte Runtime-Abhaengigkeit des Software-Pfads.
- Kein automatisierter WinUAE-Broker/Capture-Test solange `disabled_by_process`.
- IP-Provenienz: RTG-/ReAction-Design ggf. aus dokumentierter oeffentlicher Schnittstelle; Dunkelkammer nur nach separater Rechtefreigabe.

## 7. refinement_note / refinement_triage

- Aenderungsachse: reine RTG-Software-Praesentation, nicht die UI-Shell und nicht die CPU-Komposition.
- `refinement_triage`: `needs_architecture`.
- `atomization_report.decision`: `keep_single_story` (vorlaeufig; formales Audit offen).
- `arch_review.initial`: `pending`; exakte RTG-/OS-Baseline noch festzulegen.
- Spike-Bedarf: ja; `no implementation authorized`.

## 8. Abhaengigkeiten und Testnachweis

- Plattformvoraussetzung: m68k-GCC/NDK plus manuell geprueftes WinUAE mit RTG/P96.
- `PF-TN-013` reserviert als geplanter Testnachweis, nicht als durchgefuehrter Test.
- Keine PR-/CI-Evidenz vorhanden; Produkt-ABI und Release bleiben unfreigegeben.

## 9. Review und Gate-Status

- `arch_review.pre_ready_final`: `pending`
- `dor_check`: `pending`
- `implementation_gate_report`: `pending`
- `review_report`: `pending`
- `done`: `not_authorized`

## Priorisierung 2026-10-11

- Prioritaet: `P1` — Native RTG-Softwareausgabe ist die MiniGL-unabhaengige Voraussetzung fuer AmigaOS-Viewport.
- Quelle: [Prioritaets-/Ready-Audit](../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine bestehenden Implementation-/CI-/Reviewer-Gates; dokumentierte Story-Statuswerte bleiben massgeblich.

## Dunkelkammer-M2-Wiederverwendung (Planung 2026-10-12)

- Ausgangsquelle: privates `HurricanVD/dunkelkammer` @ `26295506ba363b1bd52bfca1852b0230ffb2bad9`, `src/platform/dk_present.c/.h` und `docs/adr/ADR-0007-rtg-present-model.md`; die unter MIT freigegebene, historische `libamiga2d`-/`vxplatform`-Present-Variante ist vor einer weiteren Kopie zu bevorzugen, sofern sie dieselben erforderlichen Operationen abdeckt.
- Wiederverwendung ist vom Eigentuemer fuer Entwicklung erlaubt; direkt uebernommener Code benoetigt vor oeffentlicher Publikation den dateibezogenen Lizenz- und Provenienznachweis aus [PF-SP-012](PF-SP-012.md). Kein Source-Import in dieser Story-Aktualisierung.
- Der PhotoCraft-RGBA8-CPU-/256x256-Tile-Kern bleibt Eigentuemer der Pixel; RTG/M2 ist nur Presenter. Der Adapter stellt explizit RGBA8 -> ARGB32, Stride, Buffer-Ownership, Clip/Overflow, OOM/Fallback und Cache-Invalidierung sicher.
- Das Dunkelkammer-Benchmark unter WinUAE ist eine historische Fremdprojekt-Messung, **kein** PhotoCraft-Laufzeitnachweis. Ein synthetisches Canvas-Muster darf den Presenter vor [PF-SP-007](PF-SP-007.md) testen, echte Composite-Pixel erst nach dessen abgeschlossenem Gate.
- Keine zusaetzliche RTG-Implementierungsstory neben PF-SP-010; PF-TN-013 bleibt unveraendert `not_run`. 
