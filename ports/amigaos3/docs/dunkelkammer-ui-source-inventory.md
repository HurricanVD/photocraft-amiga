# Dunkelkammer MIT source inventory — PhotoCraft-Amiga

- Status: `planning`; **no code import** and no PhotoCraft GUI test yet.
- Source snapshot: private `HurricanVD/dunkelkammer` @ `26295506ba363b1bd52bfca1852b0230ffb2bad9`.
- License evidence: `HurricanVD/dunkelkammer` @ `70897e91c94214421b0595b3e858446d6b3586ea`, [MIT merge PR #9](https://github.com/HurricanVD/dunkelkammer/pull/9); `LICENSE` grants MIT for Virtual Dimension-controlled source and original materials, keeping Third-Party licenses separate.
- Other provenance: `HurricanVD/virtualcommander` is MIT (UI draw / context popup ancestor); `HurricanVD/vxplatform` is MIT but its surface API depends on its process-global display owner. ReAction owns PhotoCraft's window.

| Dunkelkammer source file | PhotoCraft story | Integration and notice check |
|---|---|---|
| `src/platform/dk_present.c/.h` | PF-SP-010 | M2 RTG presenter; no `DkCore`/ARGB aliasing, explicit RGBA8 conversion; MIT and vxplatform ancestry notice |
| `src/ui/dk_ui_layer_list.c/.h` | PF-SP-013 | BOOPSI independent list; remove 64-row cap, support PhotoCraft group IDs |
| `src/ui/dk_ui_layer_panel.c/.h` | PF-SP-013 | Rewrite adapter; no 16-layer DkDocument assumption |
| `src/ui/dk_ui_draw.c/.h` | PF-SP-013/016 | Retain VirtualCommander MIT notice; text width/truncation helpers |
| `src/ui/dk_ctx_menu.c/.h` | PF-SP-016 | Retain VirtualCommander MIT notice; decouple action IDs |
| `src/ui/dk_ui_dialogs.c` | PF-SP-016 | Extract modal behavior without DkSession; Cancel without mutation |
| `src/ui/dk_ui_event.c` | PF-SP-014 | Move/down/up queue independent of global `s_ui` |
| `src/canvas/dk_canvas.c/.h` | PF-SP-014 | Reuse zoom/pan/dirty math only; PhotoCraft 256x256 tiles stay authoritative |
| `src/ui/dk_ui.c` and `src/ui/dk_ui_internal.h` | PF-SP-011/015 | Select small ReAction lifecycle and action snippets, **no whole-monolith copy** |
| `src/ui/dk_ui_render.c` | PF-SP-015 | ReAction hooks only; no Dunkelkammer icons/assets |
| `src/app/main.c` | PF-SP-011 | Workbench/StackSwap startup lifecycle, PhotoCraft-owned app state |

## Gate before any public source import

- [x] Owner MIT license verified in Dunkelkammer.
- [ ] Examine imported exact source revisions, modifications, original copyright notices, VirtualCommander/vxplatform provenance.
- [ ] Prevent copying third-party libraries, fonts, NDK/SDK/ROM, product artwork and unlicensed binaries.
- [ ] Confirm window-neutral M2 presenter avoids `vxp_display_open` double-display ownership.
- [ ] Add SPDX/copyright/license text and source pin in actual integration PR.
- [ ] Run host/m68k HUNK/vamos and manual WinUAE tests on PhotoCraft integrations.

No shipping permission for third-party materials is inferred from the MIT change.
