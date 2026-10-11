# PF-TN-012 — Raster-Ownership-Regression (geplant)

- Story: [PF-BG-001](../stories/PF-BG-001.md)
- Status: `planned`; Execution: `not_run` (2026-10-11)
- Gate: Regression nach Implementierung, **kein PASS-Nachweis durch die DoR-Freigabe**.

## Testfaelle

1. Ein frisches `PcRaster*` an Layer-ID 1 anhängen; zweite Übergabe desselben Pointers an andere Layer-ID desselben Dokuments muss kontrolliert fehlschlagen. Anzahl, Namen, Layer-ID und Besitzerzustand unverändert.
2. Dasselbe `PcRaster*` in zwei unabhängige Dokumente einhaengen: nur der erste erfolgreiche Owning-Transfer ist erlaubt. Nach abgelehntem Claim kein fehlerhaftes Caller-Free des bereits vom ersten Dokument besessenen Objektes; den Return-/Owner-Vertrag testen.
3. `pc_document_clone` erzeugt unabhängige Raster-Owner und teilt nur Copy-on-Write-Tile-Daten; beide Dokumente sicher freigeben, ohne UAF/Double-Free.
4. `NULL`, duplicate Layer-ID, invalid ID und injizierter OOM-Fehler: Fehlrückgabe ohne doppelten Claim, unbeabsichtigte Freigabe oder logische Teilmutation.
5. ASan/UBSan und leak detection auf allen oben genannten Fällen; Host-`make -C ports/amigaos3 host-test host-core-sanitize`.
6. Separates GCC13 68k HUNK/`vamos` O0/O2-Regressionsergebnis und Bezug auf Original-Rust-Semantik festhalten, sofern der zugehörige Target-Testfall anwendbar ist.

## Nachweispflicht

- Test-Commit SHA, Runner/Compiler-Version und exakte Kommandos:
- Ausführung/Exitcode: `not_run`
- Assertion-/Sanitizer-Ergebnis: `not_run`
- m68k/vamos: `not_run`
- Regression-Test-Review: `pending`

Es ist **keine** produktive AmigaOS-GUI, MiniGL- oder PSD-Testabdeckung behauptet.
