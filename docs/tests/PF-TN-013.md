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

### Window-growth regression and bounded fix (2026-10-12)

The user-provided WinUAE screenshots demonstrate **failure** at approximately 1450px window width: a uniform grey fallback, while reduced window widths draw correctly. Direct source inspection identified hard-coded 1024x768 guards both in the shell and the portable document band renderer. PR #18 changes the shared maximum to 1920x1200 and adds C99 large/small/large regression checks. The old screenshot is valid evidence of the failure, **not** evidence that the new code passes. `GUI-04_old: observed_failure`; `GUI-04_new_v0.0.5: not_run`; normal host, m68k and manual screen evidence required.

## v0.0.5 large-window visual retest — observed PASS (2026-10-12)

The project owner reported "Vergrößern klappt jetzt" and supplied
`grafik(20261012-080717).png` showing the dark checkerboard,
centered document gradient and masked orange layer inside the
enlarged native ReAction canvas. The v0.0.4 uniformly grey failure
did **not** recur at the pictured width. The second screenshot
`grafik(20261012-080726).png` shows v0.0.5 startup diagnostics
and `Window closed normally (RC 0)`.

- `GUI-04_v0.0.4_enlarge: observed_fail` (prior regression).
- `GUI-04_v0.0.5_enlarge: observed_pass` (manual visual evidence).
- `RTG_presenter_enlarged_screen: observed_pass`.
- `full_resize_cycle_and_repaint: partially_observed`;
  `pixel_exact_capture: not_run`;
  `OOM_memory_leak: not_run`.
- Screenshot images are available only in the conversation, **not
  checked into git**. This is not an automated WinUAE PASS, full
  presenter acceptance, or story `done` authorization.

## Four start/close cycles — v0.0.5 evidence (2026-10-12)

The owner supplied one more AmigaShell screenshot
(`grafik(20261012-081120).png`, chat attachment, not in git)
showing **four** complete native Shell start/close cycles,
each with `Window closed normally (RC 0)`. This supports the
basic joint ReAction/M2 lifecycle but does not measure bitmap
or heap release.

- `GUI-05_four_start_close_RC0: observed_pass`;
  `RTG_bitmap_leak_measured: not_run`.
- Owner opts out of low-Fast-RAM stress for current preview:
  `GUI-06_low_fast_ram: skipped_by_owner_preview_only`,
  **not a PASS**. Keep allocation guards.
- No change to `GUI-04_v0.0.5_enlarge: observed_pass`.
- `full_presenter_release_acceptance: pending`; no `done` claim.
