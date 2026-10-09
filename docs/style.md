# Local Coding Style Overlay

Dieses Dokument beschreibt nur lokale Coding-Style-Ergaenzungen. Gemeinsame
Regeln stehen in `VD_PROCESS_REPO/docs/amiga-coding-style.md`.

## Lokale Modul- und Namenskonventionen

-

## Lokale Check- und Format-Kommandos

- Format-Check:
- Lint:
- Compiler-Warnings:

## Lokale Abweichungen

| Thema | Abweichung | Grund | Revisit |
|---|---|---|---|

## Rueckfuehrung in die Prozessbasis

Wiederverwendbare Regeln, die nicht produktspezifisch sind, sollen als
Prozess-Aenderung nach `VD_PROCESS_REPO` zurueckgefuehrt werden.

## PhotoCraft-Port-Overlay
- C99 allein unter `ports/amigaos3`; Original-Rust bleibt Rust.
- Große Tiles heapbasiert; O0/O2/vamos- und Sanitizer-Gates.
- Keine vendored NDK-/MiniGL-SDKs oder proprietären Fremdquellen.
