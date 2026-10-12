# PF-TN-013 — PF-SP-010 planned acceptance

- Story: [PF-SP-010](../stories/PF-SP-010.md)
- Date: 2026-10-12
- Status: `planned / not_run`. No implementation, build, runtime or manual native test PASS asserted.
- Scope: Synthetic RGBA8 CPU band -> ARGB32 RTG M2, dirty clipping, OOM, bitmap lifetime; manual WinUAE.

## Proposed verification

1. Fixture-positive and negative/boundary cases and repeatability.
2. Host C99 smoke/ASan/UBSan and unchanged original PhotoCraft Rust oracle for OS-free semantics; m68k/GCC13 -O0 and -O2 HUNK/vamos for target-portable code.
3. If GUI/RTG, direct manual AmigaOS 3.2 + WinUAE window/gadget visual and interaction test, exact binary SHA, hardware/RTG settings, screenshots and logs.
4. Invalid input, allocator fault, cleanup, rollback and RAM peak; no floating-point/byte-order assumptions outside documented contract.
5. No automated WinUAE UI/broker/capture flows while `disabled_by_process`.

- `test_evidence: not_run`
- `runtime_evidence: none`
- `release_authorization: no`

## User visual evidence: ReAction + M2 document preview (2026-10-12)

A project-owner-provided WinUAE screenshot
(`grafik(20261012-073737).png`, **conversation-only, not in git**)
shows the native ReAction window displaying a centered gradient document,
orange translucent/masked rectangle and dark checkerboard.
This is consistent with the actual `PcDocument` -> C99 Normal compositor
-> RGBA8 staging -> M2/RTG presentation path.
A second screenshot shows Shell RC 0 after closing the same window.

- `RTG_initial_visible_document: observed_pass` from user manual screenshot.
- `Presenter_dirty_region_correctness: not_run`.
- `Refresh_and_resize: not_run`.
- `Non_RTG_and_OOM_fallback: not_run`.
- `Pixel_exact_screen_capture: not_run`.
- `Actual_WinUAE_screenmode_and_P96_details: not_recorded`.
- No blanket PF-SP-010 story `done` or full target acceptance claim.
