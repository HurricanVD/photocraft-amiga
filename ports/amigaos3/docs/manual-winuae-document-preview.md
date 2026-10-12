# PF native PcDocument / ReAction / M2 visual acceptance procedure

- Date: 2026-10-12
- Related: PF-SP-007 / PF-SP-010 / PF-SP-011, integration PR #18
- Classification: **manual target acceptance procedure**
- Current status: `not_run / runtime_blocked` in this environment (no
  configured AmigaOS 3.2 + WinUAE/CGX/P96 interactive desktop).
- This document is not evidence that the test was performed. Native
  HUNK linking and headless `vamos` are separate CI stages.

## Prerequisites

1. Run the GitHub Actions `AmigaOS 3.2 Host Core` workflow at the
   integration commit and confirm the native `gcc13-native-shell` step,
   `check-hunk.py`, host tests, Rust/C99 oracle and `vamos` checks.
2. Download the CI artifact `photocraft-amiga-document-preview-hunk`
   and transfer the executable to a writable AmigaDOS volume.
3. Boot **AmigaOS 3.2**, with suitable ReAction
   `window.class`, `layout.gadget` and `space.gadget` installed.
   Select a truecolour Picasso96/CyberGraphX RTG screen with
   `cybergraphics.library`. Note exact WinUAE version, CPU mode,
   installed libraries, screenmode, RAM and configuration.
4. Record the HUNK file hash and the source revision. Do **not**
   assume that success on WinUAE transfers to real 68040 hardware.

## Visible manual tests

| ID | Action | Expected observation | Result |
|---|---|---|---|
| GUI-01 | Shell: increase stack as needed, launch `photocraft-amiga-document-preview` | A ReAction window titled `PhotoCraft AmigaOS (native layers)`, no crash or missing-library alert | `not_run` |
| GUI-02 | Examine initial canvas | An approximately 192x144 centered multi-colour gradient image, orange translucent/masked rectangle and neutral checkerboard outside document; **not** the prior full-canvas synthetic test tile | `not_run` |
| GUI-03 | Expose/refresh repeatedly | Canvas restored from offscreen bitmap, controls/gadgets unaffected | `not_run` |
| GUI-04 | Resize down then up | Image recenters and redraws correctly, no stale out-of-bounds pixels, no corrupted gadgets | `not_run` |
| GUI-05 | Close, relaunch 3 times | All Window/Offscreen/Library resources released, Shell returns without an alert | `not_run` |
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
- `manual_target_result: not_run`; `image_screenshots: none`;
  `story_done: not_authorized`.

## Startup failure regression / diagnostic build v0.0.3 (2026-10-12)

A manual report showed that the first HUNK returned immediately to
`AmigaShell` without opening a window. The code review identified two
incorrect ReAction library names in the previous `pc_amiga_shell.c`:
`layout.gadget` and `space.gadget`. The supported AmigaOS ReAction
library locations (also used by MIT Dunkelkammer) are
`gadgets/layout.gadget` and `gadgets/space.gadget`.
The old shell also lacked explicit Intuition/Graphics library opening and
gave no error diagnostics when a prerequisite failed.

For the next build, the corrected start path writes its stage errors to
the Shell (starting with `PhotoCraft AmigaOS Document Preview - startup
diagnostic v0.0.3`). It now prints the failed component, required version,
and a final exit code 20 on failure; a user-triggered close prints exit 0.
Missing `cybergraphics.library` is a **warning**, not a reason to suppress
the ReAction window: the window may show only a neutral canvas fallback
on a planar screen.

Use the *new* build, **not** the previous
`e8a7dd29f41850d1...` executable. Transfer the new CI artifact
`photocraft-amiga-document-preview-hunk` after it has passed GCC13 HUNK
compilation. In AmigaShell:

```text
Stack 50000
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

- `visual_result: not_run` for the corrected build; report needed.
- This log-only fix does NOT establish graphics parity nor GUI lifecycle
  pass; manual WinUAE/AmigaOS 3.2 target evidence required.
