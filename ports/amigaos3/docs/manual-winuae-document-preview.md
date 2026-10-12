# PF native PcDocument / ReAction / M2 visual acceptance procedure

- Date: 2026-10-12
- Related: PF-SP-007 / PF-SP-010 / PF-SP-011, integration PR #18
- Classification: **manual target acceptance procedure**
- Current status: `partial_manual_pass` with four user-observed v0.0.5 shell launch/close cycles, correct enlarged viewport and a user-waived low-Fast-RAM manual test (2026-10-12). Workbench icon startup, refresh and measured memory cleanup remain unverified. The assistant's environment cannot independently launch the GUI.
- This document is not evidence that the test was performed. Native
  HUNK linking and headless `vamos` are separate CI stages.

## Prerequisites

1. Run the GitHub Actions `AmigaOS 3.2 Host Core` workflow at the
   integration commit and confirm the native `gcc13-native-shell` step,
   `check-hunk.py`, host tests, Rust/C99 oracle and `vamos` checks.
2. Download the CI artifact `photocraft-amiga-document-preview-hunk`
   and transfer the executable to a writable AmigaDOS volume.
3. Boot **AmigaOS 3.2**, with suitable ReAction
   `window.class`, `gadgets/layout.gadget` and `gadgets/space.gadget` installed.
   Select a truecolour Picasso96/CyberGraphX RTG screen with
   `cybergraphics.library`. Note exact WinUAE version, CPU mode,
   installed libraries, screenmode, RAM and configuration.
4. Record the HUNK file hash and the source revision. Do **not**
   assume that success on WinUAE transfers to real 68040 hardware.

## Visible manual tests

| ID | Action | Expected observation | Result |
|---|---|---|---|
| GUI-01 | Shell: launch directly with the normal inherited stack, without a `Stack` command: `photocraft-amiga-document-preview` | A ReAction window titled `PhotoCraft AmigaOS (native layers)`, no crash or missing-library alert | `observed_pass` (screenshot of open native ReAction window; v0.0.4 shell text) |
| GUI-02 | Examine initial canvas | An approximately 192x144 centered multi-colour gradient image, orange translucent/masked rectangle and neutral checkerboard outside document; **not** the prior full-canvas synthetic test tile | `observed_pass` (checkerboard, gradient, masked orange layer all visible) |
| GUI-03 | Expose/refresh repeatedly | Canvas restored from offscreen bitmap, controls/gadgets unaffected | `not_run` |
| GUI-04 | Resize down then up | Image recenters and redraws correctly, no stale out-of-bounds pixels, no corrupted gadgets | `not_run` |
| GUI-05 | Close, relaunch 3 times | All Window/Offscreen/Library resources released, Shell returns without an alert | `observed_pass_four_cycles` for Shell launch/close without visible alert, each RC 0; memory leak measurements `not_run` |
| GUI-06 | Low Fast RAM or missing RTG capability | Controlled error/neutral canvas fallback; no hangs or document corruption | `skipped_by_owner` for low-Fast-RAM manual stress in this preview only; missing-RTG fallback still `not_run`; not a PASS |
| GUI-07 | Launch from Workbench where supported | No CLI/Workbench startup crash and window can be closed | `not_run` |

## Evidence to record

- `winuae_version`, `rom`, `amigaos_version`,
  `reactionsdk_classes`, `picasso96_or_cgx`, `screenmode_depth`,
  `cpu`, `fast_ram` and `chip_ram`.
- Source SHA and actual HUNK SHA-256; command/stack-size and exit status.
- At least two screenshots with display mode visible: initial window
  and after resize/expose. This test may be **manual only**; the
  shared process currently marks automated WinUAE UI broker/capture
  `disabled_by_process`.
- Before/after memory or alert observations; log unexpected
  exceptions, click/resize faults and repeat-reopen behavior.
- A final `PASS` requires **all** GUI-01..07 applicable checks,
  with exceptions explicitly justified, reviewed and scoped.

## Limitations and next steps

- CI's `vamos` can run `pc_canvas_doc_test` and validate
  `pc_compose_rgba8_region` in HUNK at -O0/-O2; it does **not** simulate
  a Picasso96 GUI or certify actual pixels displayed by window.class.
- This is the first document-backed viewport integration, not
  PF-SP-017 PNG import, PF-SP-019 brush, PF-SP-008 Undo or
  PF-SP-020 end-to-end native editor acceptance.
- `manual_target_result: partial_pass`; `image_screenshots: two user-provided conversation attachments (not included in repository)`; `story_done: not_authorized`.

## Startup failure regression / diagnostic builds v0.0.3–0.0.4 (2026-10-12)

A manual report showed that the first HUNK returned immediately to
`AmigaShell` without opening a window. The code review identified two
incorrect ReAction library names in the previous `pc_amiga_shell.c`:
`layout.gadget` and `space.gadget`. The supported AmigaOS ReAction
library locations (also used by MIT Dunkelkammer) are
`gadgets/layout.gadget` and `gadgets/space.gadget`.
The old shell also lacked explicit Intuition/Graphics library opening and
gave no error diagnostics when a prerequisite failed.

For v0.0.3 and later, the corrected start path writes its stage errors to
the Shell (starting with `PhotoCraft AmigaOS Document Preview - startup
diagnostic v0.0.4 (StackSwap 200 KiB)`). It now prints the failed component, required version,
and a final exit code 20 on failure; a user-triggered close prints exit 0.
Missing `cybergraphics.library` is a **warning**, not a reason to suppress
the ReAction window: the window may show only a neutral canvas fallback
on a planar screen.

Use the *new* build, **not** the previous
`e8a7dd29f41850d1...` executable. Transfer the new CI artifact
`photocraft-amiga-document-preview-hunk` after it has passed GCC13 HUNK
compilation. In AmigaShell:

```text
photocraft-amiga-document-preview
Echo $RC
```

Capture and send **all PhotoCraft diagnostic lines**.
If the new build still returns without a window, the log should identify
`intuition.library`, `graphics.library`, `window.class`,
`gadgets/layout.gadget`, `gadgets/space.gadget`,
`LAYOUT_GetClass/NewObjectA`, `SPACE_GetClass/NewObjectA`,
`WINDOW_GetClass/NewObjectA`, `RA_OpenWindow` or
`WINDOW_SigMask`, or an error in the local `PcDocument` fixture.

- `visual_result: observed_pass` for v0.0.4 launch / initial image, but remaining checks need reports.
- This log-only fix does NOT establish graphics parity nor GUI lifecycle
  pass; manual WinUAE/AmigaOS 3.2 target evidence required.

## Automatic Exec StackSwap (native v0.0.4, 2026-10-12)

The Shell/Workbench inherited stack no longer needs to be increased by
`Stack 50000`: the program itself reserves **200 KiB** through
`AllocMem(MEMF_ANY)` and uses Exec `StackSwap()` before starting
`pc_main_body()`. `pc_main_body` is marked `noinline` to prevent the
compiler from folding the native GUI call back into the entry stack.

The StackSwap record, new-stack pointer and return result are **static**
because the call frame changes stacks. The second `StackSwap` restores
the exact inherited task stack before the 200 KiB allocation is freed.
All of the window, library, document and RTG cleanup still occurs on
the private stack *before* swapping back. A failed 200 KiB allocation
prints a startup error and returns RC 20; it does **not** fall back to
the potentially undersized original stack.

The 200 KiB extra allocation is an implementation budget, not a
measurement of minimum RAM for the complete editor.

### Native regression checks (manual target)

- Launch without `Stack` shell modification. Confirm the startup
  diagnostic includes `StackSwap 200 KiB` and the window opens.
- Close normally; confirm Shell RC 0. Repeat at least three times.
- Launch through Workbench (if valid icon is available), confirm the
  same behavior despite a small inherited stack.
- Under low Fast RAM, confirm an out-of-memory message with RC 20
  instead of GUI startup on an undersized fallback stack.
- Log any Guru, address-error or library-close failure and the exact
  HUNK SHA. **No manual target PASS is asserted** by this document.

`native_gui_validation: partial_manual_pass`; `stack_swap_cross_build: ci_success`.

## Actual user-provided WinUAE observations — 2026-10-12

**Evidence source:** two screenshots uploaded directly by the user in the PhotoCraft-Amiga
conversation, filenames `grafik(20261012-073737).png` (native window) and
`grafik(20261012-073749).png` (return to Shell). They are **not attached to
this public GitHub repository**, so the image pixels and their provenance
cannot be independently rechecked by a GitHub reviewer. This is user-reported
manual interactive evidence, not a CI-simulated GUI.

Observed in the first screenshot:
- Window title: `PhotoCraft AmigaOS (native layers)`.
- Native ReAction window visibly opens above Workbench/AmigaShell.
- A dark checkerboard fills the viewport, with a centered blue/green
  gradient document and orange translucent/masked area consistent with
  the locally generated `PcDocument` fixture.
- A snippet of the diagnostic `StackSwap 200 KiB` is visible behind the
  window. This confirms the diagnostic build was launched; proving the
  exact stack-pointer/bounds swap independently is outside the screenshot.

Observed in the second screenshot after closing the window:
- `PhotoCraft AmigaOS Document Preview - startup diagnostic v0.0.4 (StackSwap 200 KiB)`.
- `PhotoCraft: Window closed normally (RC 0).`.
- `Echo $RC` yields `0`.
- Only **one** open/close iteration is documented. Repeated open/close
  and resource-free verification are **not** established.

Acceptance disposition:
- `GUI-01: observed_pass`; `GUI-02: observed_pass`.
- `GUI-05: partial_pass_single_close_only`.
- `GUI-03: not_run`; `GUI-04: not_run`; `GUI-06: not_run`;
  `GUI-07: not_run`.
- `StackSwap_simple_launch_without_Stack_command: observed_pass`, subject to
  the screenshot's limited ability to prove memory layout internals.
- `artifact_sha256: unconfirmed_by_user`; `WinUAE_version: not_recorded`;
  `RTG_driver_and_screenmode: not_recorded`;
  `Peak_RAM_and_leak_measurement: not_run`.
- `overall_manual_acceptance: partial_pass`, `story_done: not_authorized`.

Next tests requested: uncover/expose window, resize smaller/larger,
close/restart **three** times, report WinUAE CPU/RAM/P96 configuration and
any crash/alert; attach actual binary SHA-256 if possible.

## Window resize regression from real WinUAE screenshots (2026-10-12)

The project owner provided three screenshots in this conversation:
`grafik(20261012-075140).png` (reduced viewport visibly correct),
`grafik(20261012-075201).png` (enlarged ~1450-pixel-wide viewport
rendered as a uniform grey area), and
`grafik(20261012-075229).png` (reduced viewport again visibly correct).
They remain **conversation attachments**, not stored as image evidence in GitHub.
These observations change `GUI-04` from `not_run` to
`observed_fail_on_enlarge`, followed by `observed_pass_on_shrink`.

Cause verified in the code at the tested v0.0.4 source: both
`pc_amiga_shell.c` and `pc_canvas_doc.c` rejected canvas dimensions
exceeding **1024×768**, and the ReAction render hook painted a neutral
grey fallback. This was an intentional preview ceiling but is below
the physical screen/window size in the user's WinUAE configuration.

The v0.0.5 code change lifts the unified, shared viewport ceiling to
**1920×1200** with the existing caller-owned 8-row RGBA8 scratch
buffer, preserving overflow/capacity rejection and avoiding a full
RGBA8 document framebuffer. It still uses M2's existing ARGB32
full-size staging plus offscreen bitmap (~17.6 MiB combined at
1920×1200, excluding device/allocator overhead). This is a bounded
preview accommodation, not an unrestricted canvas or proven
32-MiB-RAM minimum. The C99 regression test exercises
1600×900 -> 320×256 -> 1920×1200 -> 1450×700, verifies document
pixels and destination-buffer guards, and rejects out-of-limit sizes.

**Status:** `GUI-04_previous_build: observed_fail_enlarge`;
`fix_code: implemented_in_PR18`;
`v0.0.5_manual_winuae_retest: not_run`;
`PF_SP_010_011_done: not_authorized`.

Retest with the *newly generated v0.0.5 HUNK*: open a small window,
resize past 1024 pixels width, enlarge to the previous failing
screen size, shrink again, and repeat several times. Note if the
image remains centered with mask/alpha preserved and no grey fallback.

## v0.0.5: user-confirmed larger viewport fix (2026-10-12)

Two new project-owner screenshots were provided in the conversation:

- `grafik(20261012-080717).png`: the native `PhotoCraft AmigaOS
  (native layers)` window is enlarged substantially beyond the old
  1024-pixel canvas-width guard. Its dark checkerboard and centered
  blue/green PhotoCraft document with orange masked overlay **remain
  visibly present and correctly positioned**, rather than becoming
  uniformly grey.
- `grafik(20261012-080726).png`: after close, the AmigaShell shows
  `startup diagnostic v0.0.5 (StackSwap 200 KiB, extended viewport)`
  and `PhotoCraft: Window closed normally (RC 0).`.

Owner's direct report: **"Vergrößern klappt jetzt"**.

These screenshots are user-provided conversational evidence, **not
automated WinUAE/CI results**, and are not copied into the public
GitHub repository. They do not certify a pixel-for-pixel screen oracle.
The enlargement that previously failed in v0.0.4 now **visibly works**.

Status changes relative to the older v0.0.4 observations above:

- `GUI-04_v0.0.4_enlarged: observed_fail` (historical, retained).
- `GUI-04_v0.0.5_enlarged: observed_pass` (image visible, centered).
- `GUI-04_v0.0.5_full_shrink_enlarge_cycle: not_recorded`
  (the uploaded images confirm enlargement, not every step of a
  complete resize-cycle sequence).
- `GUI-01_v0.0.5_window_open: observed_pass`.
- `GUI-05_v0.0.5_single_close_RC0: observed_pass`;
  `relaunch_three_times: not_run`; `leak_measurement: not_run`.
- `WinUAE_version/driver/settings: not_recorded`;
  `image_evidence_in_git: no`; `Workbench_icon_launch: not_run`;
  `OOM_fallback: not_run`.
- `manual_target_result: partial_pass_with_enlargement_confirmed`;
  `story_done: not_authorized`.

This evidence **closes the reported grey-on-enlarge regression
for the tested viewport size**. The full GUI acceptance gate remains
open for resource/low-memory and repeated-lifecycle tests.

## v0.0.5: four successful Shell start/close cycles — owner evidence (2026-10-12)

The project owner supplied a screenshot named
`grafik(20261012-081120).png` and stated:
"Mehrfaches öffnen und schließen ok, den Test mit wenig Fast ram überspringen wir."

The AmigaShell screenshot displays **four** consecutive invocations of
`photocraft-amiga-document-preview`. Each invocation prints
`PhotoCraft AmigaOS Document Preview - startup diagnostic v0.0.5
(StackSwap 200 KiB, extended viewport)` and ends with
`PhotoCraft: Window closed normally (RC 0).`
No crash or Guru alert is visible, and the AmigaShell prompt
returns after the fourth close.

This screenshot is a *conversation-only user attachment*; it is
not copied into this public repository. The user-observed lifecycle
result is distinct from actual allocation tracing: it cannot
prove that each ReAction object, RTG bitmap or allocated stack
was released without a leak.

**Owner-directed test exclusion:** low-Fast-RAM stress is
`skipped_by_owner_preview_only`. The absence of a test is not
an OOM pass. The application still must maintain controlled
allocation-failure handling; a later final-release, robustness
or fault-injection acceptance may reopen this check. No request
for another low-RAM manual test is pending from the owner.

### Latest authoritative status for this preview

- `GUI-01_shell_startup: observed_pass`.
- `GUI-02_document_visible: observed_pass` (earlier screenshots).
- `GUI-04_resize_enlarge: observed_pass_v0.0.5` (earlier screenshots).
- `GUI-05_4x_launch_close_RC0: observed_pass`.
- `GUI-05_resource_leak_measurement: not_run`.
- `GUI-06_low_fast_ram_manual: skipped_by_owner_preview_only`.
- `GUI-06_no_RTG_fallback: not_run`.
- `GUI-03_expose_refresh_stress: not_run`.
- `GUI-07_Workbench_icon_launch: not_run`.
- `runtime_4x_shell_lifecycle: observed_pass`;
  `overall_preview_manual_acceptance: partial_pass`;
  `story_done: not_authorized`.

The older provisional status values earlier in this file are
chronological snapshots, superseded by this section. There is
no claim of complete WinUAE, release or memory-lifetime approval.

## Workbench stdout and AvailMem differential — v0.0.6 (2026-10-12)

A project-owner WinUAE screenshot demonstrates that **Workbench icon launch
works**: the native ReAction window and document are visible; after closing,
the GUI exits with RC 0. A separate, unexpected AmigaShell/CON: window remains
open and contains the `v0.0.5` startup diagnostics. Previous code
unconditionally called `puts()/printf()/fflush(stdout)` in GUI startup,
shutdown and error paths. On a Workbench task, the C runtime may allocate a
CON: output window when writing to stdout.

The v0.0.6 fix changes `main` to `main(int argc, char **argv)`:
AmigaOS Workbench startup uses `argc == 0` (as in the existing MIT
Dunkelkammer startup flow), and `pc_is_workbench` is stored in static BSS.
**All** normal/diagnostic stdout output is suppressed in that mode.
If Intuition has already opened and a required component cannot start,
`EasyRequestArgs()` displays a native error requester instead of writing
to a console. For Shell launches the diagnostics remain available, and
the application continues to use an internal 200 KiB Exec `StackSwap()`.

### Memory observation and investigation

The same user supplied an AmigaShell `Avail` comparison before and
after four successful start/close cycles:

| Area | Available before | Available after | Delta |
|---|---:|---:|---:|
| Chip | 2,025,320 B | 2,023,624 B | -1,696 B |
| Fast | 127,536,184 B | 127,411,128 B | -125,056 B |
| Total | 129,561,504 B | 129,434,752 B | -126,752 B |

The difference is **observed**; its cause is not yet established.
AmigaOS/CGX/ReAction may lazily retain class, font, RTG or runtime
allocations when first used, even after the application frees its own
resources. A single pre/post four-run snapshot cannot distinguish
system warm-up/caching from a linear, per-invocation leak. The existing
source calls `FreeVec`, `FreeBitMap`, `CloseLibrary`,
`pc_document_destroy`, `RA_CloseWindow`, `DisposeObject`
and `FreeMem` on the ordinary close path; none proves the absence
of other leaks or cached system allocations.

**New Shell-only diagnostics:** v0.0.6 measures
`AvailMem(MEMF_FAST)` and `AvailMem(MEMF_CHIP)` immediately
before private-stack allocation and after it is freed. Per-run
signed differences are printed after normal close. These readings
remain advisory (OS cache/allocator/libraries may affect the counts).
They do **not** appear at all for Workbench starts.

### Next manual tests — no low-Fast-RAM stress requested

1. Launch from the Workbench icon with v0.0.6 and close the application.
   The previously unexpected `AmigaShell` output window **must not**
   be created or left open. Confirm that the ReAction viewport still
   displays PhotoCraft pixels.
2. In a regular Shell, run the v0.0.6 program **five** times, normally
   closing each window. Record each
   `PhotoCraft: post-shutdown available Fast ... Chip ...` line.
   Compare runs after the first warmed-up invocation. Consistent
   progressively negative deltas require a true resource leak
   investigation; first-run-only changes may indicate caches.
3. If relevant, run the command `Avail` once before and after the
   five cycles, **without other applications opening/closing**.
   Report the figures and emulator/RTG setup.
4. Do **not** perform the previously excluded low-Fast-RAM stress
   test; this is a normal-memory baseline investigation.

- `GUI-07_Workbench_icon_launch_v0.0.5: observed_pass`.
- `Workbench_console_orphan_v0.0.5: observed_fail`.
- `GUI-07_Workbench_no_console_v0.0.6: not_run`.
- `Fast_Chip_memory_change_v0.0.5: observed`, `leak_root_cause: unconfirmed`.
- `v0.0.6_mem_probe: not_run`.
- `release_acceptance: no`; `PF-SP-011_done: not_authorized`.
