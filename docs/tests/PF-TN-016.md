# PF-TN-016 — Dunkelkammer MIT provenance verification plan

- Story: [PF-SP-012](../stories/PF-SP-012.md)
- Date: 2026-10-12
- Status: `planned`, `not_run` for actual imported code; no completed implementation or GUI runtime proof.
- Static baseline: Dunkelkammer [MIT PR #9](https://github.com/HurricanVD/dunkelkammer/pull/9), license commit `70897e91c94214421b0595b3e858446d6b3586ea`, source `26295506ba363b1bd52bfca1852b0230ffb2bad9`.

| Gate | Check | Status |
|---|---|---|
| LIC-01 | MIT license, Virtual Dimension 2026 copyright, root `LICENSE` | `verified_static` |
| LIC-02 | Per-file revision and modified-ancestor MIT notices | `not_run` |
| LIC-03 | External fonts, binaries, NDK, ROMs and third-party assets excluded | `not_run` |
| LIC-04 | Actual PhotoCraft import-PR SPDX/attribution and release manifest | `not_run` |
| LIC-05 | Window-ownership and third-party code lifecycle check | `not_run` |

No automatic WinUAE tests were invoked; PF-TN-013/014 and GUI tests remain separate.
