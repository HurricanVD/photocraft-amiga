# MiniGL spike: QuarkTex NG first, PiStorm3D later

Status: **planned; not executed**. All performance and visual acceptance checks below require real runs.

## Purpose

Prove a separately compiled AmigaOS/m68k application can use the modern shared `minigl.library` ABI and upload a 256x256 RGBA8 texture without colour-channel corruption. This is the *first graphics smoke*, NOT proof the PhotoCraft editor has been ported.

## Test environment

- AmigaOS 3.2, 68040/68060 with FPU configured to WinUAE **Host (80-bit)**.
- WinUAE 6.x, Picasso96 / UAE RTG (32-bit display), **Allow native code** enabled.
- Use matching QuarkTex NG DLL and `LIBS:minigl.library` from the same QuarkTex NG build.
- MiniGL SDK header `proto/minigl.h` plus corresponding `libminigl.a` from a legally sourced SDK.
- Do NOT mix QuarkTex NG's own `minigl.library` with PiStorm3D's library or an older Warp3D MiniGL in one target image.

## Minimum test matrix

| ID | Scenario | Pass criteria | State |
|---|---|---|---|
| GL-01 | Library and context | Window appears; no startup failure | not run |
| GL-02 | RGBA8 upload | Reference checker colours and transparency as expected | not run |
| GL-03 | Channel endian correctness | No R/B exchange; alpha interpreted correctly | not run |
| GL-04 | Texture re-upload | glTexSubImage2D updates only changed texture rows | not implemented |
| GL-05 | Off-screen bitmap context | ReAction sidebars remain intact; clipping correct | not implemented |
| GL-06 | Dirty cache | Unchanged frames have no texture re-uploads | not implemented |
| GL-07 | RTG vs MiniGL | A/B performance, quality, resize and pan | not implemented |
| GL-08 | PiStorm3D | Repeat on supported Pi 4/CM4 hardware | not run |

`src/pc_minigl_smoke.c` currently addresses **GL-01 and a visual portion of GL-02**. A return code of zero does not establish full framebuffer parity.

## Useful sources

- [QuarkTex NG README and m01 MiniGL reference test](https://github.com/Sdursun/QuarkTexNG/blob/main/tests/m01_minigl.c)
- [PiStorm3D library integration notes](https://github.com/SteffenHaeuser/MiniGL_Library_68k/blob/main/Readme.md)
- Private internal reference: HurricanVD/dunkelkammer, `src/platform/dk_present.c`, `docs/adr/ADR-0007-rtg-present-model.md` (no code copied).
- Private process: HurricanVD/vd-amiga-dev-process, `docs/process/winuae-test-automation.md`. Automation status **disabled_by_process**; tests must remain manual until policy is changed.
