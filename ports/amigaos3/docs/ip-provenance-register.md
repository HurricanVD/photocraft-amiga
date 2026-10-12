# Port IP / provenance register

All code in this bootstrap is newly authored for this fork. No proprietary Dunkelkammer implementation, graphics SDK, driver binary, font, ROM or non-relicensed asset has been copied.

| Source | Intended use | Status | Licence / notes |
|---|---|---|---|
| `storytold/photocraft` (the fork's upstream) | original Rust core and authoritative semantics | present (upstream code) | MIT OR Apache-2.0; retain source notices |
| `HurricanVD/dunkelkammer` (private) @ `26295506ba363b1bd52bfca1852b0230ffb2bad9` | ReAction, M2-RTG and Amiga I/O candidates | **reference only**, no copied code | MIT for Virtual Dimension-owned sources verified by [Merge PR #9](https://github.com/HurricanVD/dunkelkammer/pull/9), license commit `70897e91c94214421b0595b3e858446d6b3586ea`; original MIT copyright notices and independent third-party rights still apply |
| `HurricanVD/vd-amiga-dev-process` (private) | process guidance and independent build/test methodology | **reference only** | private; no process files vendored |
| `Sdursun/QuarkTexNG` | optional external WinUAE runtime/diagnostic backend | reference and separately installed runtime, not bundled | LGPL-3.0 upstream; validate linking, licensing and distribution before shipping |
| `SteffenHaeuser/MiniGL_Library_68k` / PiStorm3D | optional separately installed shared `minigl.library` | reference/runtime only | Hyperion MiniGL Open Source License plus component notices; SDK import library to be reviewed |
| `vxcore`, `vxplatform` | prospective shared Amiga helpers | not integrated | use only pinned, licence-reviewed versions |

Before public code copy from private repositories: record exact source commit/file, ownership, explicit public distribution permission, copyright statement, licence/SPDX, modifications, and release notice obligations. Public fork visibility cannot be treated as permission to relicense third-party code.

## Dunkelkammer MIT-Nachweis 2026-10-12

- Lizenzwechsel auf eigenem Quellcode: [Dunkelkammer PR #9](https://github.com/HurricanVD/dunkelkammer/pull/9), Commit `70897e91c94214421b0595b3e858446d6b3586ea`. Vollstaendige Standard-MIT-Lizenz in `LICENSE`, Scope `LICENSE.md`.
- Konkrete Quell-Dateien bleiben `not_imported`; Copyright, Notices und Drittcode werden vor erstem Import ueber [Quellmatrix](dunkelkammer-ui-source-inventory.md) validiert. Keine MIT-Umlizenzierung von TTEngine, FreeType, Vera, ROMs/NDK/Third-Party-Assets.

## Actual code-PR source reuse candidates (2026-10-12)

- [Provenance review](reviews/PF-SP-012-mit-import-audit-2026-10-12.md):
  native M2 presenter (PR #14) and ReAction shell (PR #15) are proposed
  with explicit MIT source/notice references. Neither is merged into main
  at the time this record is written; first import review and eventual
  release manifest update remain separate gates.
