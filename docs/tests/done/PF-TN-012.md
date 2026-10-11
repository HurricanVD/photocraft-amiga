# PF-TN-012 — Raster-Ownership-Regression (Host verifiziert)

- Story: [PF-BG-001](../../stories/done/PF-BG-001.md)
- Status: `pass` (2026-10-11; Host/ASan/UBSan, Rust-Original-Oracles und GCC13/vamos O0/O2 auf finalem PR-Head bestanden; archiviert)
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
- m68k/vamos: `PASS` (GCC13.3 HUNK/vamos O0 und O2, Job 114429621096, CI #38125707879)
- Regression-Test-Review: `approve` ([finaler Review](../../reviews/PF-BG-001-final-review-2026-10-11.md))

Es ist **keine** produktive AmigaOS-GUI, MiniGL- oder PSD-Testabdeckung behauptet.

## Ausgefuehrte Regression 2026-10-11 — GitHub Actions

- Code-/Teststand: `ae3102f10be2bd4d021f356ad023826243d6e4e6`
- [AmigaOS 3.2 Host Core Run #38125707879](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38125707879), Job `Portable C99 + ASan/UBSan` ID `114429524447`: **success**.
- `make -C ports/amigaos3 host-test`: PASS; `pc_document_test` mit neuen gleichen/dokumentuebergreifenden Raster-Claim-Faellen sowie Clone/Destroy PASS.
- `make -C ports/amigaos3 host-core-sanitize`: PASS mit `-fsanitize=address,undefined`, einschliesslich `pc_document_test_san`. Keine Fehlerdiagnose im ausgefuehrten Joblog.
- Job `Rust originals vs C99 port` ID `114429524610`: **success**, `PASS: original PhotoCraft Rust crates match C99 port and typed oracle`. Die Rust/C99-Oracles decken weiterhin ihren bestehenden Scope ab; der C-exklusive Ownership-Test besitzt keinen neuen Rust-originaeren Ownership-Oracle.
- m68k GCC13/vamos: **success**, Job ID `114429621096` in CI #38125707879. GCC/Bebbo 13.3, 12 echte HUNK-Binaries, O0/O2 über `vamos` ausgefuehrt. Überpruefte Doc-Regression meldete jeweils `PASS: PhotoCraft flat raster-layer document ownership/order/COW`; Rust/Core/Typed-Oracles bei O0/O2 stimmten byteidentisch ueberein.
- OOM-Allocator-Fault-Injection: **not_run / ausserhalb dieses Bugfix-Regressionsjobs**; bleibt in `PF-TD-001` fuer spaetere deterministische Allokationsfehler offen.
- Final-PR-Head: alle einschlaegigen CI-Lanes green, Codex-P2-Finding geloest, PR #8 gemergt; keine Produktfreigabe.

## Abschlussgrenze

- Der final gepruefte Code ist nach `main` gemergt. Die GitHub-Eigenfreigabe per APPROVE ist nicht moeglich; separater Reviewbericht dokumentiert die fachliche Empfehlung.
- Native GUI-/Minigl-/WinUAE-/PiStorm-Runtime bleibt ohne Testnachweis (nicht Teil dieses Bugfixes).

## Finaler Testabschluss — 2026-10-11

- PR-Head: `5c862a3ca149b26af0eb9fd20b762f671f7b9217`.
- [CI #38140374511](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374511): completed/success.
- [AmigaOS 3.2 Host #38140374487](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374487): completed/success, C99 + ASan/UBSan, Rust-Original-Oracle, GCC13.3 HUNK + `vamos` O0/O2.
- [Documentation #38140374504](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374504), [PF Preflight #38140374455](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374455), [Packaging #38140374462](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374462): completed/success.
- PR-Merge-Commit: `4fa6ca6a5b5a5b1c48257835fabacb7922c3b03e`.
- Bewusst nicht als bestanden verbucht: deterministische OOM-Fault-Injection, WinUAE/GUI/MiniGL, Gruppen-/Maskenintegration; eigene Stories/Gates.
