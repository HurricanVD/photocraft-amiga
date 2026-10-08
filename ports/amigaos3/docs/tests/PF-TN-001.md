# PF-TN-001 — PF-SP-001 Testnachweis

- Datum: 2026-10-08
- Scope: PhotoCraft AmigaOS isolierter MiniGL-Test
- Gesamtstatus: **partial / runtime_blocked**
- Host: GNU make + C99 GCC in Linux-Container
- Native m68k: nicht ausgeführt, Compiler und MiniGL-SDK fehlen
- WinUAE QuarkTex NG: nicht ausgeführt, WinUAE nur unter Windows verfügbar
- WinUAE-Automatisierung: `disabled_by_process` (nicht verwendet)
- Runtime-Pass nicht aus Hosttest ableiten.

| ID | Fall | Status | Evidenz |
|---|---|---|---|
| GL-00 | ARGB/RGBA inklusive kompletter 256x256-Tile-Generator | pass (Host) | `gcc -std=c99 -Wall -Wextra -Werror -pedantic ...` -> `PASS: portable ARGB/RGBA staging checks`; Host-Staging auf Quelltextstand geprüft |
| SDK-00 | `amiga-preflight` ohne Cross-Compiler | blocked | Umgebung hat keinen `m68k-amigaos-gcc` |
| GL-01 | MiniGL-Library und Kontext | not_run | echter m68k-HUNK-/WinUAE-Lauf erforderlich |
| GL-02 | RGBA8-Textur readback | not_run | QuarkTex-NG-Runtime erforderlich |
| GL-03 | RGB- und Achsenorientierung | not_run | QuarkTex-NG-Runtime erforderlich |
| GL-04 | glTexSubImage2D Patch | not_run | QuarkTex-NG-Runtime erforderlich |
| GL-05..08 | ReAction-Offscreen, Cache, Benchmark, PiStorm3D | out_of_scope | separate Folgespikes |

## Manuelle Ausführung (ausstehend)

```sh
make -C ports/amigaos3 host-test
make -C ports/amigaos3 amiga-preflight CC68K=m68k-amigaos-gcc MINIGL_INCLUDE=/sdk/dev/include MINIGL_IMPORT_LIB=/sdk/libminigl.a
make -C ports/amigaos3 amiga-smoke CC68K=m68k-amigaos-gcc MINIGL_INCLUDE=/sdk/dev/include MINIGL_IMPORT_LIB=/sdk/libminigl.a
```

WinUAE 6.x, AmigaOS 3.2, RTG/P96 32 Bit, 68040/68060, FPU Host (80-bit), Allow native code=on. Nur zusammengehörige QuarkTex-NG-DLL und `LIBS:minigl.library` verwenden. In Amiga Shell `pc_minigl_smoke` starten; Shell-Output/Returncode, Versionsangaben, Binary-Hash und Screenshot hier mit Datum ergänzen. Ohne diese Daten bleibt PF-SP-001 `blocked`.
