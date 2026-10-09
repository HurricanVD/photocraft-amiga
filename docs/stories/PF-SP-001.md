<!-- VD canonical story/test record. Source port record kept intact at ports/amigaos3/docs/stories/PF-SP-001.md. -->
# PF-SP-001 – MiniGL/QuarkTex NG Build- und Laufzeitvalidierung

- Story-ID: PF-SP-001
- Typ: technischer Spike (SP)
- Status: **blocked** (Hostanteil umgesetzt, Zielsystem-Evidenz fehlt)
- Priorität: P1
- Datum: 2026-10-08
- Auftrag: "setze das um"
- Architektur: akzeptierte ADR-0001, port-lokaler VD-Prozess 0.2.1
- Test-ID: PF-TN-001

## Ziel
Ein natives 68k/MiniGL-Testprogramm für QuarkTex NG und später PiStorm3D mit reproduzierbarer SDK-Prüfung, eindeutiger PASS/FAIL-Ausgabe und Pixeltests. **Kein** PhotoCraft-Editor, keine ReAction-Einbindung, keine Änderung an Rust-Crates.

## Akzeptanzkriterien
- Hosttest: ARGB/RGBA-Konvertierung und 256x256-Tile (GL-00).
- m68k GCC und offizielles MiniGL-SDK vor Build prüfen (kein impliziter Download).
- Genuine m68k-HUNK erzeugen und in WinUAE mit QuarkTex NG ausführen (GL-01).
- RGBA-Upload und Kanal-/Achsenorientierung per glReadPixels prüfen (GL-02/03).
- glTexSubImage2D 16x16 Patch und unveränderte Nachbarpixel prüfen (GL-04).
- Versionen, Einstellungen, Binär-SHA und Terminal-/Bild-Evidenz im Testbericht festhalten.

## Technischer Scope
`src/pc_minigl_smoke.c` verwendet `MiniGLOpen/MiniGLClose`, eigenes MiniGL-Testfenster, 256x256 RGBA8-Textur, zwei unterschiedliche Pixel-Samples sowie Subtexture-Update und RGB-Readback. `Makefile` enthält host-test, amiga-preflight und amiga-smoke.

## Grenzen, Lizenz und Nachweise
- C99 nur unter `ports/amigaos3/`, unveränderter PhotoCraft-Rust-Core.
- QuarkTex NG und PiStorm3D nur als externe optionale Laufzeit, SDK nicht im Fork.
- Keine proprietären Dunkelkammer-Dateien übernommen; Impact `reference_only`, Provenienzregister vorhanden.
- WinUAE-Automatisierung `disabled_by_process` und wird nicht gestartet.
- Zielruntime hier nicht verfügbar (kein m68k-amigaos-gcc, SDK, WinUAE). Das ist ein Blocker, kein Test-PASS.
- Review/Closure nach manueller WinUAE-Evidenz und m68k-Build zu wiederholen.

## Fortsetzung
Im WSL2-Workspace SDK und Bebbo-GCC bereitstellen, `make amiga-preflight` und `make amiga-smoke` ausführen und unter WinUAE GL-01..04 prüfen. PF-TN-001 nach realem Run ergänzen. Erst danach die Story zum Review/finalen Abschluss bringen.
