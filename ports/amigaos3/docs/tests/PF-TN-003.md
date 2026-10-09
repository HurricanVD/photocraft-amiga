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
