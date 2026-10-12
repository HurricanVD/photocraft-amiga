# PF native PcDocument / ReAction / M2 visual acceptance procedure

- Date: 2026-10-12
- Related: PF-SP-007 / PF-SP-010 / PF-SP-011, integration PR #18
- Classification: **manual target acceptance procedure**
- Current status: `partial_manual_pass` based on two user-provided WinUAE/AmigaOS screenshots dated 2026-10-12; remaining interactive checks are `not_run`. The assistant's environment cannot independently launch the GUI.
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
| GUI-05 | Close, relaunch 3 times | All Window/Offscreen/Library resources released, Shell returns without an alert | `partial`: one normal close and Shell RC=0 observed; 3x relaunch and memory-release checks `not_run` |
| GUI-06 | Low Fast RAM or missing RTG capability | Controlled error/neutral canvas fallback; no hangs or document corruption | `not_run` |
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
