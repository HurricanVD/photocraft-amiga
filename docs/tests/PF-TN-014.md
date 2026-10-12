# PF-TN-014 — PF-SP-011 planned acceptance

- Story: [PF-SP-011](../stories/PF-SP-011.md)
- Date: 2026-10-12
- Status: `planned / not_run`. No implementation, build, runtime or manual native test PASS asserted.
- Scope: ReAction window/class opening, closing, repeated lifecycle, resize/refresh, Workbench/Shell and stack/OOM; manual WinUAE.

## Proposed verification

1. Fixture-positive and negative/boundary cases and repeatability.
2. Host C99 smoke/ASan/UBSan and unchanged original PhotoCraft Rust oracle for OS-free semantics; m68k/GCC13 -O0 and -O2 HUNK/vamos for target-portable code.
3. If GUI/RTG, direct manual AmigaOS 3.2 + WinUAE window/gadget visual and interaction test, exact binary SHA, hardware/RTG settings, screenshots and logs.
4. Invalid input, allocator fault, cleanup, rollback and RAM peak; no floating-point/byte-order assumptions outside documented contract.
5. No automated WinUAE UI/broker/capture flows while `disabled_by_process`.

- `test_evidence: not_run`
- `runtime_evidence: none`
- `release_authorization: no`

## Implementierungs-Nachweis 2026-10-12 (PR #15; eingeschraenkt)

- GCC/Bebbo 13.3 HUNK-Link fuer `pc_amiga_shell.c` + M2 Presenter auf Revision `9b6f4c79c62ae28c8de07a0675f63cc236cdf777`: CI [GCC13.3 m68k HUNK + vamos vs Rust #38182781986](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38182781986) PASS; der Workflow prueft das erzeugte HUNK-Format.
- Bestehende Headless-C99, GCC/vamos und Rust-Differentialtests PASS; sie starten **nicht** die ReAction-Oberflaeche. Die GUI hat im Shell-MVP nur synthetische PhotoCraft-RGBA8-Pixel.
- Der erste GCC13-Compilefehler (fehlende `RA_OpenWindow`/`RA_CloseWindow`/`RA_HandleInput` Makrodeklarationen) wurde mit `reaction/reaction_macros.h` korrigiert.
- Offen: visueller manueller WinUAE-GUI-Lauf mit Workbench/Shell, Resize, Close/Repeated Open, RTG-Staging und Speicher-Fallback. `manual_target: not_run`; keine fertige Editor-App oder PF-SP-011-Done-Freigabe.

## Automatic private Exec StackSwap (v0.0.4)

- Native source: `ports/amigaos3/src/pc_amiga_shell.c`.
- `PC_APP_STACK_SIZE=200*1024`; static `StackSwapStruct`, static
  allocation pointer and status; `pc_main_body` is `noinline`.
- Original inherited Shell/Workbench stack is only used to allocate
  the private stack and call `StackSwap`. Real ReAction/C99 work runs
  on the private stack. The original is restored before `FreeMem`.
- Allocation failure is a controlled RC20 path, **not** an attempt
  to start the GUI on the inherited stack.
- `gcc13_native_HUNK: pending_ci`; `real_WinUAE_runtime: not_run`.
  Verify normal CLI without `Stack`, Workbench start, close/restart x3,
  and low Fast-RAM allocation failures before marking PASS.
- Reference: Dunkelkammer `src/app/main.c` @ `26295506`,
  MIT owner license merge `70897e91`; original copyright notice
  in `NOTICE-DUNKELKAMMER-MIT.txt`.

## User WinUAE evidence after StackSwap startup fix (2026-10-12)

- Two screenshots supplied by the project owner in the current chat:
  native `PhotoCraft AmigaOS (native layers)` window is open, and
  after closing the AmigaShell prints the `startup diagnostic v0.0.4
  (StackSwap 200 KiB)`, `Window closed normally (RC 0)`, and
  `Echo $RC = 0`.
- `GUI-01/native_window_launch: observed_pass`.
- `StackSwap_plain_Shell_launch: observed_pass` (diagnostic marker,
  no preparatory `Stack 50000` command visible for this invocation).
- `GUI-05/single_close: observed_pass`; repeated close and stack/RAM
  cleanup measurements **remain not_run**.
- `GUI-03/04_refresh_resize: not_run`;
  `GUI-06_OOM: not_run`; `GUI-07_Workbench_launch: not_run`.
- Screenshot files `grafik(20261012-073737).png` and
  `grafik(20261012-073749).png` are **conversation attachments,
  not checked into this repository**. Executable file SHA and exact
  emulator settings have not been independently captured.
- **Status: partial target manual PASS only**.
  All acceptance criteria of PF-SP-011 and story `done` remain open.

### ReAction resize regression (2026-10-12)

The user observed: downsized window renders; enlarged width ~1450 becomes grey; downsizing restores the image. This is consistent with explicit 1024x768 preview guards, **not** a StackSwap failure. The single-source 1920x1200 canvas cap is now shared by ReAction and the C99 portable rendering function. Regression fixtures test large -> small -> large render bands. `GUI-04_old: observed_fail`; `GUI-04_new: not_run`, pending corrected HUNK WinUAE manual retest. StackSwap normal startup remains `observed_pass` in v0.0.4.

## v0.0.5 resize regression retest — owner evidence (2026-10-12)

Owner's WinUAE screenshots `grafik(20261012-080717).png`
(enlarged ReAction document canvas with correct checkerboard,
gradient and overlay) and `grafik(20261012-080726).png`
(normal close, RC 0) confirm that the prior v0.0.4 failure
to render beyond the 1024px preview limit is repaired in v0.0.5
for the tested window size. Direct owner confirmation:
"Vergrößern klappt jetzt".

- `GUI-04_enlarged_v0.0.5: observed_pass`.
- `GUI-05_single_close_v0.0.5: observed_pass`.
- `GUI-05_triple_relaunch: not_run`;
  `GUI-06_low_RAM: not_run`;
  `GUI-07_Workbench_launch: not_run`;
  `resource_release_measurements: not_run`.
- Screenshot files are in the current conversation, not public git.
- `manual_review: partial_pass`, `story_done: not_authorized`.

## v0.0.5 four-cycle native ReAction lifecycle — owner verified (2026-10-12)

- User screenshot `grafik(20261012-081120).png`, conversation only:
  **four** `photocraft-amiga-document-preview` Shell invocations,
  each with the v0.0.5 `StackSwap 200 KiB` diagnostic and
  `Window closed normally (RC 0)`.
- Owner explicitly confirms repeated open/close works.
- `GUI-05_four_sequential_launch_close: observed_pass`.
- `GUI-05_no_visible_Guru_or_alert: observed_pass`.
- `GUI-05_heap_or_RTG_leak_free: not_measured` — normal
  RC 0 is not a proof of no leaks.
- Owner explicitly excludes the low-Fast-RAM stress test from
  the current preview: `GUI-06_low_ram: skipped_by_owner_preview_only`
  and **not** `pass`. No outstanding request to the owner
  to perform that manual stress scenario.
- `GUI-07_Workbench_launch: not_run`, `GUI-03_expose_stress: not_run`.
- `preview_target_result: partial_pass`, `PF-SP-011_done: not_authorized`.

## Owner WinUAE Workbench launch and normal-memory investigation — v0.0.6

- User screenshot `grafik(20261012-083211).png` (conversation attachment)
  shows a genuine double-click Workbench icon start with the document
  visible in a native ReAction window.
- `grafik(20261012-083217).png` shows the application closed normally
  while the output-console window persists. This is a **regression**
  in Workbench-mode output/lifecycle, not a clean full GUI closure.
- `grafik(20261012-083120).png` shows AmigaShell `Avail`
  before/after four closes: available total fell by 126,752 B
  (Fast -125,056 B; Chip -1,696 B). This does **not** by itself
  diagnose an unreleased allocation versus OS cache warm-up.
- Fix: process startup determines `pc_is_workbench=(argc==0)`;
  Workbench normal/failure console output suppressed, error requester
  via `EasyRequestArgs` when Intuition is available. Shell output
  remains and additionally prints before/after Fast/Chip
  `AvailMem` sample after the private 200KiB stack is freed.
- `GUI_07_v0.0.5_icon_launch: observed_pass`.
- `GUI_07_v0.0.5_no_orphan_console: observed_fail`.
- `GUI_07_v0.0.6_no_orphan_console: not_run`.
- `available_memory_delta: observed_minus_126752_B_after_four`.
- `true_memory_leak: unproven`, `new_probe: pending_manual_retest`.
- Owner's low-Fast-RAM stress waiver remains `skipped_by_owner_preview_only`.
- See [manual test](../../ports/amigaos3/docs/manual-winuae-document-preview.md).
