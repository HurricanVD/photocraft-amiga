# MiniGL / QuarkTex NG – PF-SP-001

Status: **host pass; native build/runtime blocked**, see [PF-TN-001](tests/PF-TN-001.md) and [story](stories/PF-SP-001.md).

Das MiniGL-Testprogramm ist ein **separater 68k-Grafik-Smoke**, kein PhotoCraft-Editor. Es erzeugt 256x256 RGBA8, lädt es mit glTexImage2D, liest drei Pixel über glReadPixels, ersetzt per glTexSubImage2D einen 16x16-Bereich und prüft danach die ersetzten und unveränderten Pixel. Rückgabecodes: 0=API/Pixel-PASS, 5=Pixel/GL-FAIL, 20=Kontext/SDK-Runtime-Fehler.

Die GPU-Pixeltests wurden **nicht ausgeführt**. Nur der Host-Test des RGBA-Generators ist nachgewiesen. Weitere Tests (ReAction-Offscreen-Bitmap, Dirty Cache, RTG-Performance, PiStorm3D-Hardware) gehören nicht zu diesem Spike.

## Build

```sh
make -C ports/amigaos3 host-test
make -C ports/amigaos3 amiga-preflight CC68K=m68k-amigaos-gcc MINIGL_INCLUDE=/sdk/dev/include MINIGL_IMPORT_LIB=/sdk/libminigl.a
make -C ports/amigaos3 amiga-smoke CC68K=m68k-amigaos-gcc MINIGL_INCLUDE=/sdk/dev/include MINIGL_IMPORT_LIB=/sdk/libminigl.a
```

`/sdk` sind illustrative SDK-Pfade. Das SDK mit `proto/minigl.h` und die passende `libminigl.a` müssen extern bereitgestellt werden. Keine proprietären SDK-Dateien im öffentlichen Fork.

## Manuelle WinUAE-Tests

WinUAE 6.x mit AmigaOS 3.2, RTG/P96, 32-Bit Workbench, 68040/68060-FPU **Host (80-bit)**, JIT und **Allow native code**. QuarkTex NG Windows-DLLs und `LIBS:minigl.library` müssen aus demselben Release kommen. Testbinary in ein separates AmigaDOS-Volume kopieren und von der Shell aus starten. GL-01..04 müssen im Shell-Output als PASS erscheinen und das Testbild muss sichtbar korrekt sein.

[QuarkTex-NG-Referenztest](https://github.com/Sdursun/QuarkTexNG/blob/main/tests/m01_minigl.c) · [PiStorm3D-MiniGL-Dokumentation](https://github.com/SteffenHaeuser/MiniGL_Library_68k/blob/main/Readme.md).

Die zentrale VD-WinUAE-Automatisierung ist `disabled_by_process`; manuelle Tests sind zulässig. Fehlende m68k-/WinUAE-Evidenz **nicht** als bestandene Tests ausgeben.
