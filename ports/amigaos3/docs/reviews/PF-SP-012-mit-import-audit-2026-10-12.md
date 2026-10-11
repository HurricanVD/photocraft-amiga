# PF-SP-012: concrete Dunkelkammer MIT source reuse audit

- Date: 2026-10-12
- Type: static file/third-party/license review, **not a native runtime test**.
- Owner: Virtual Dimension, Dunkelkammer MIT license confirmed by
  [merge PR #9](https://github.com/HurricanVD/dunkelkammer/pull/9),
  `70897e91c94214421b0595b3e858446d6b3586ea`.
- Selected original source revision:
  `HurricanVD/dunkelkammer@26295506ba363b1bd52bfca1852b0230ffb2bad9`.
- Exact MIT copyright text: `Copyright (c) 2026 Virtual Dimension`;
  permission text in Dunkelkammer `LICENSE`.
- Port target: `ports/amigaos3/` only. Original PhotoCraft Rust crates untouched.

## Actual selected source implementation

| Planned PhotoCraft file | Origin | Rights and limitations | State |
|---|---|---|---|
| `include/pc_present_convert.h`, `src/pc_present_convert.c` | New port-owned conversion helpers, informed by PhotoCraft RGBA8 and numeric DK M2 ARGB | No direct third-party source transplant; standard fork license applies | Proposed in PR #14 |
| `src/pc_present_m2.c`, `include/pc_present_m2.h` | Dunkelkammer `src/platform/dk_present.c/.h`, exact source SHA above | MIT-owned M2 algorithm; retain Virtual Dimension notice and root `NOTICE-DUNKELKAMMER-MIT.txt`; no Dunkelkammer document structs | Proposed in PR #14 |
| `src/pc_amiga_shell.c` | Dunkelkammer `src/ui/dk_ui.c` ReAction lifecycle and `src/app/main.c` context | MIT-owned UI control ideas adapted into independent minimal shell, Virtual Dimension notice | Proposed in PR #15 |
| `src/ui/dk_ui_draw.c/.h` and popup | Historical VirtualCommander MIT origin | No source copied yet. When copied: keep both MIT and original copyright/permission notice. | Not imported |
| `src/ui/dk_ui_layer_list.c/.h` | Dunkelkammer MIT source | No code copied yet. Layer/UI ownership to be independently reviewed at implementation. | Not imported |
| `src/canvas/dk_canvas.c/.h` | Dunkelkammer source | No code copied yet; no 128x128 model substituted. | Not imported |
| `assets/**`, fonts, external libraries | Possibly independently licensed | No blanket MIT transfer. No product assets, NDK, driver DLLs, ROMs, TTEngine/FreeType/Vera copied. | Excluded |

## Static license check

- `LIC-01: verified`: Dunkelkammer own source MIT, root LICENSE / scope LICENSE.md.
- `LIC-02: verified_for_PR14_15_source_headers`: origin, source revision, owner copyright and separate notice in proposed native C files; recheck exact final diffs at merge.
- `LIC-03: verified_scope`: no VirtualCommander UI-source pasted in proposed files; historical provenance in README and license.
- `LIC-04: no_product_assets_imported`: no SDK/ROM/font/assets planned in the two code PRs.
- `LIC-05: pending`: final import-PR review and notice distribution on actual merged files.
- `LIC-06: pending`: release IP manifest needs update once code merges.

## Platform/architecture review

Dunkelkammer M2 accesses CyberGraphX/P96 and RTG with
`AllocBitMap` Friend BitMap, `WritePixelArray` and
`BltBitMapRastPort`. ReAction `window.class` remains sole
Window/Screen owner. The new `pc_present_m2` does not call
`vxp_display_open` or own a screen; it fails safely if RTG is unavailable.
The original DK document/ARGB32 raster is not copied into PhotoCraft.

This review validates **source-rights and static ownership boundaries**;
it does NOT certify GUI lifecycle, HUNK linking, pixel quality or
AmigaOS 3.2 WinUAE operation. All such evidence remains in PF-SP-010/011
and their associated PF-TN-013/014.

`review_result: approve_scoped_provenance`
`implementation_gate_report: pending`
`runtime_test: not_run`
`PF-SP-012_story_status: ready` (unchanged until formal closure gates)
