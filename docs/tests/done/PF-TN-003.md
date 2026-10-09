<!-- VD canonical story/test record. Source port record kept intact at ports/amigaos3/docs/tests/done/PF-TN-003.md. -->
# PF-TN-003 — GCC13.3/m68k/vamos test evidence

- Date: 2026-10-09
- Story: PF-SP-003
- Test status: **pass**, archived with PF-SP-003 after review (2026-10-09); GCC13.3 m68k/vamos O0/O2 and original Rust differential verified in CI #37915607402
- Compiler baseline: opt-in GCC/Bebbo 13.3.0; root explicit and version-checked, independent of GCC16 vxplatform production profile.
- Emulation: amitools vamos + pinned machine68k; AmigaOS system libraries only as available in vamos.
- CI: `.github/workflows/amigaos3-host.yml`, job `gcc13-vamos`.

| Gate | Test | Status |
|---|---|---|
| Target compiler guard | GCC13.3 target and libgcc in same root | **pass** |
| HUNK build | three tests × O0/O2 = 6 HUNK binaries | **pass** |
| HUNK signature | all six start with HUNK_HEADER 0x000003f3 | **pass** |
| Runtime | run six HUNK binaries under vamos | **pass** |
| Pixel staging | C99 tile conversion tests on 68k | **pass O0/O2** |
| Core operations | geometry, format, RGBA8 COW tests on 68k | **pass O0/O2** |
| Parity | O0 and O2 m68k C oracle vs original Rust output | **pass byte-identical** |
| Hardware | WinUAE/QuarkTex NG and PiStorm3D | not covered |

When CI completes, record run id, commit, compiler `--version`/target, Docker digest, native logs, number of tests, exact comparison result, failures and remedies. No success until verified from job logs.

## Initial CI run 37910690849 (2026-10-09)

- Host-C99/sanitizers: pass.
- Rust-original comparison: pass.
- GCC13.3 image pull/version/preflight: pass; image digest recorded in CI logs.
- GCC13.3 native build at O0 and O2: **pass**.
- Six binary HUNK_HEADER checks: **pass**, binary sizes logged.
- Pinned machine68k + amitools/vamos installation: pass.
- First vamos invocation: **fail in emulator configuration** with `Too much RAM allocated with hw access enabled!` at `-m 16384`. No product assertion was executed in this run.
- Correction: reduce documented vamos RAM from 16384 KiB to 8192 KiB (same order used by VD/amiport examples); rerun CI. **Do not record native runtime parity as passed until actual execution succeeds.**

Run: https://github.com/HurricanVD/photocraft-amiga/actions/runs/37910690849

## Zweiter CI-Lauf 37911180171 (2026-10-09)

- Commit: `c6ea8f2361579188ebd075570072f93cd2457076`.
- Host-C99 inklusive Sanitizer: **pass**.
- Rust-original-Test und C99-Oracle: **pass**.
- GCC/Bebbo 13.3: Build mit `-O0` und `-O2` **pass**; alle sechs Amiga-HUNK-Header **pass**.
- Pinned `machine68k`/`amitools` Installation: **pass**.
- `vamos` mit 8192 KiB startet, jedoch ist der erste `pc_core_test` bei der Schreiboperation im Boundary-Matrix-Test fehlgeschlagen (`tests/pc_core_test.c:114`), nachdem die monolithische 13x13-Matrix ca. 9 MiB Tile-Speicher beanspruchte.
- Korrektur im nächsten Commit: Alle 169 x/y-Paare unverändert prüfen, aber pro Y-Koordinate eine neue Surface mit genau sechs belegten X-Tiles verwenden (max. ca. 1,5 MiB Tile-Daten je Surface). Nach jedem Row-Test Surface freigeben; Defaults und Tile-Count zusätzlich kontrollieren. Danach beide Optimierungsstufen erneut unter `vamos` und gegen Rust prüfen.
- Keine Aussage zum tatsächlichen m68k/Rust-Paritäts-PASS bis zum nächsten verifizierten CI-Lauf.

[Actions-Lauf 37911180171](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37911180171).

## Dritter CI-Lauf: alle GCC13-/vamos-Gates bestanden (2026-10-09)

- [GitHub Actions #37915607402](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37915607402)
- Getesteter Code-Commit: `622cfbae25a8522126a3635235a62720feeaa19d`.
- Gesamtworkflow: **completed / success**; Jobs `Portable C99 + ASan/UBSan`, `Rust originals vs C99 port` und `GCC13.3 m68k HUNK + vamos vs Rust` jeweils **success**.
- Bebbo-GCC: **13.3.0**, Target `m68k-amigaos`, Root `/opt/amiga-13.3`, `libgcc.a` aus demselben Root.
- Image (aufgelöster Digest): `stefanreinauer/amiga-gcc@sha256:f9d09422a89f317a2f59d5db46227ad9bd8d753d5c9fe41553fcfdc6ecf60ce8`; Tag im Workflow `gcc-v13.3-20260622`.
- `machine68k`: Commit `61600df53cb007e06b9f3c576d1c4b1d19042ef4`; `amitools`: Commit `3b57f2052ee76c28bbc5e4256227f62dca7b1c9f`; `greenlet`: 3.5.6 im Runner installiert.
- Compilerflags: C99, `-m68020 -msoft-float -fno-omit-frame-pointer -noixemul`, jeweils `-O0` bzw. `-O2`.
- Sechs native Amiga-HUNK-Binärdateien durch `check-hunk.py` bestätigt:
  - O0: `pc_core_test` 19532 B, `pc_tile_convert_test` 14432 B, `core_oracle` 18344 B.
  - O2: `pc_core_test` 18360 B, `pc_tile_convert_test` 14072 B, `core_oracle` 17480 B.
- `vamos -S -C 20 -m 8192 -s 128`: beide Kern-Suites und beide Staging-Suites **PASS**.
- `core_oracle` wurde unter `vamos` bei **O0 und O2** ausgeführt. `diff -u` gegen die im separaten Rust-Job durch *originale PhotoCraft-Crates* erzeugte Referenzdatei ergab **keinen Unterschied**.
- Wörtlicher Ergebnisindikator aus dem Job-Log:
  - `PASS: GCC13 O0 m68k/vamos vs original PhotoCraft Rust`
  - `PASS: GCC13 O2 m68k/vamos vs original PhotoCraft Rust`
- Der Boundary-Test prüft weiterhin alle 169 Koordinatenpaare. Statt 36 Tiles gleichzeitig existieren pro Zeile nur noch 6 × 256 KiB = ca. 1,5 MiB Tile-Puffer. Quellcode der PhotoCraft-Core-Portierung wurde für diese Korrektur nicht geändert.

### Evidenzgrenze

Diese Tests beweisen die **definierten C99-Core-Fixtures auf emuliertem 68020** mit GCC13, einschließlich Rust-Oracle-Vergleich. Sie beweisen *nicht* komplette Rust-Rastersemantik, PSD/Dokument-/Layer-Engine, echtes AmigaOS/Workbench, MiniGL, QuarkTex NG, RTG, PiStorm3D oder eine GCC13-Produkt-/Releasefreigabe.

Die technische Testphase ist **pass**; der formale Story-Abschluss bleibt an den VD-Prozessreview gebunden.

## Archival / closure (2026-10-09)

Technical scope accepted as `pass` and archived in the port-local `tests/done/`. Final review: [PF-SP-003-final-review](../../../ports/amigaos3/docs/reviews/PF-SP-003-final-review.md). The subsequent CI run #37916110300 also passed on the documentation/evidence commit. The tests do **not** certify MiniGL or the completed PhotoCraft editor.
