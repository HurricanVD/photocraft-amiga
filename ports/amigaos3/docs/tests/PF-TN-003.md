# PF-TN-003 — GCC13.3/m68k/vamos test evidence

- Date: 2026-10-09
- Story: PF-SP-003
- Test status: **not yet executed/verified** at time of initial CI commit
- Compiler baseline: opt-in GCC/Bebbo 13.3.0; root explicit and version-checked, independent of GCC16 vxplatform production profile.
- Emulation: amitools vamos + pinned machine68k; AmigaOS system libraries only as available in vamos.
- CI: `.github/workflows/amigaos3-host.yml`, job `gcc13-vamos`.

| Gate | Test | Status |
|---|---|---|
| Target compiler guard | GCC13.3 target and libgcc in same root | pending |
| HUNK build | three tests × O0/O2 = 6 HUNK binaries | pending |
| HUNK signature | all six start with HUNK_HEADER 0x000003f3 | pending |
| Runtime | run six HUNK binaries under vamos | pending |
| Pixel staging | C99 tile conversion tests on 68k | pending |
| Core operations | geometry, format, RGBA8 COW tests on 68k | pending |
| Parity | O0 and O2 m68k C oracle vs original Rust output | pending |
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
