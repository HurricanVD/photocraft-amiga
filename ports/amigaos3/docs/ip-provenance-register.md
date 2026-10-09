# Port IP / provenance register

All code in this bootstrap is newly authored for this fork. No proprietary Dunkelkammer implementation, graphics SDK, driver binary, font, ROM or non-relicensed asset has been copied.

| Source | Intended use | Status | Licence / notes |
|---|---|---|---|
| `storytold/photocraft` (the fork's upstream) | original Rust core and authoritative semantics | present (upstream code) | MIT OR Apache-2.0; retain source notices |
| `HurricanVD/dunkelkammer` (private) | ReAction, M2-RTG and Amiga I/O patterns; possible future licensed extraction | **reference only**, no copied code | Dunkelkammer is proprietary; owner approval and explicit relicensing/redistribution scope are required before publishing copied files |
| `HurricanVD/vd-amiga-dev-process` (private) | process guidance and independent build/test methodology | **reference only** | private; no process files vendored |
| `Sdursun/QuarkTexNG` | optional external WinUAE runtime/diagnostic backend | reference and separately installed runtime, not bundled | LGPL-3.0 upstream; validate linking, licensing and distribution before shipping |
| `SteffenHaeuser/MiniGL_Library_68k` / PiStorm3D | optional separately installed shared `minigl.library` | reference/runtime only | Hyperion MiniGL Open Source License plus component notices; SDK import library to be reviewed |
| `vxcore`, `vxplatform` | prospective shared Amiga helpers | not integrated | use only pinned, licence-reviewed versions |

Before public code copy from private repositories: record exact source commit/file, ownership, explicit public distribution permission, copyright statement, licence/SPDX, modifications, and release notice obligations. Public fork visibility cannot be treated as permission to relicense third-party code.
