# PF-TN-012 — Raster-Ownership-Regression (Host verifiziert)

- Story: [PF-BG-001](../stories/PF-BG-001.md)
- Status: `partial_verified` (Host/ASan/UBSan sowie bestehende Rust-Oracle PASS am 2026-10-11; GCC13/vamos und finaler Head-Review offen)
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
- Ausführung/Exitcode: `PASS` (AmigaOS-Host-Run #38125707879, Job 114429524447)
- Assertion-/Sanitizer-Ergebnis: `PASS` (Host/ASan/UBSan laut ausgefuehrtem Joblog)
- m68k/vamos: `not_run`
- Regression-Test-Review: `pending`

Es ist **keine** produktive AmigaOS-GUI, MiniGL- oder PSD-Testabdeckung behauptet.

## Ausgefuehrte Regression 2026-10-11 — GitHub Actions

- Code-/Teststand: `ae3102f10be2bd4d021f356ad023826243d6e4e6`
- [AmigaOS 3.2 Host Core Run #38125707879](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38125707879), Job `Portable C99 + ASan/UBSan` ID `114429524447`: **success**.
- `make -C ports/amigaos3 host-test`: PASS; `pc_document_test` mit neuen gleichen/dokumentuebergreifenden Raster-Claim-Faellen sowie Clone/Destroy PASS.
- `make -C ports/amigaos3 host-core-sanitize`: PASS mit `-fsanitize=address,undefined`, einschliesslich `pc_document_test_san`. Keine Fehlerdiagnose im ausgefuehrten Joblog.
- Job `Rust originals vs C99 port` ID `114429524610`: **success**, `PASS: original PhotoCraft Rust crates match C99 port and typed oracle`. Die Rust/C99-Oracles decken weiterhin ihren bestehenden Scope ab; der C-exklusive Ownership-Test besitzt keinen neuen Rust-originaeren Ownership-Oracle.
- m68k GCC13/vamos: zum Zeitpunkt dieses Eintrags **in_progress**, noch kein PASS.
- OOM-Allocator-Fault-Injection: **not_run / ausserhalb dieses Bugfix-Regressionsjobs**; bleibt in `PF-TD-001` fuer spaetere deterministische Allokationsfehler offen.
- Ein finaler unabhängiger Review, vollstaendiges CI und PR-/Merge-Freigabe fehlen weiterhin.
