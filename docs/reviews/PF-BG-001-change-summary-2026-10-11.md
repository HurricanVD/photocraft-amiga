# PF-BG-001 — change_summary und technischer Teststand

> Historischer Bericht aus der Implementierungs-/Review-Phase. Die unten genannten offenen Freigaben beschreiben den damaligen Zeitpunkt; seit 2026-10-11 liegt der [finale Review](PF-BG-001-final-review-2026-10-11.md) mit `recommendation: approve`, die gemergte PR #8 und die Archivierung vor.

- Datum: 2026-10-11
- Story: [PF-BG-001](../stories/done/PF-BG-001.md), P0
- Implementierungs-PR: [#8](https://github.com/HurricanVD/photocraft-amiga/pull/8), Stackbasis [#7](https://github.com/HurricanVD/photocraft-amiga/pull/7)
- Freigabe: [Implementation-Gate](PF-BG-001-implementation-gate-2026-10-11.md) vor eigentlichen Codeaenderungen
- Scope: `ports/amigaos3/` C99-Document-Ownership und Port-CI-Ausloesung, keine Rust-/GUI-/PSD-/ABI-Versprechen

## Umsetzung

1. Privates `document_owned`-Flag in `src/pc_raster.c` und rein internes `src/pc_raster_owner_internal.h` mit `pc_raster_claim_document_ownership()`.
2. `pc_document_append_raster()` beansprucht das Raster **erst nach erfolgreichen fehlbaren Allokationen**. Bereits in anderem Dokument beanspruchte Raster werden ohne zweiten Owner oder Freigabe abgewiesen.
3. `pc_raster_clone()` erzeugt ein unabhaengiges und zunaechst unbeanspruchtes Rasterobjekt; vorhandene COW-Tiles werden weiterhin geteilt, aber nicht das Objekt-Lifetime-Ownership.
4. `include/pc_document.h` dokumentiert `on success transfer / on failure remain with caller`. Ein bereits durch andere Dokumente beanspruchtes Objekt bleibt beim urspruenglichen Owner, nicht beim fehlschlagenden Caller.
5. `tests/pc_document_test.c` ergaenzt Regressionen fuer Double-Append im selben und einem anderen Dokument, Invalid-ID/-Parameter, unterschiedliche legitime Owner, Clone-Isolation, Destroy.
6. `Makefile` fuehrt neuen privaten Header als Dokumenttest-Abhaengigkeit.
7. `.github/workflows/amigaos3-host.yml`: Pull Request mit Portaenderung triggert vorhandene Host/Rust-Oracle/GCC13-vamos-Lanes unabhaengig vom Basisbranch; Gate-Erweiterung vor diesem Schritt dokumentiert.

## Ausgefuehrte Evidenz

- Code SHA: `ae3102f10be2bd4d021f356ad023826243d6e4e6`.
- [AmigaOS Host Run #38125707879](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38125707879) **success**:
  - `host-test` inkl. PF-BG-001-Regressionscases PASS.
  - `host-core-sanitize` (ASan+UBSan) mit Dokumenttest PASS, kein gemeldeter Fehler.
  - Unveraenderte originale Rust geom/color/raster Tests und C99-Oracles PASS.
  - GCC/Bebbo 13.3 echte m68k HUNK Tests/Oracles unter `vamos`, `-O0` und `-O2`, inklusive `pc_document_test`, PASS; Job ID 114429621096.
- Diese Evidenz bezieht sich auf den gelisteten **Code-Commit**. Spaetere Dokumentations-Commits muessen beim finalen Head-Review gesondert auf Checkstatus geprueft werden.
- Keine echte WinUAE-/ReAction-/MiniGL-/PiStorm3D-Laufzeitevidenz erforderlich oder behauptet: diese Story ist ein plattformfreier C99-Bugfix.
- Allocator-Fault-Injection und transaktionales Multi-Tile-OOM bleiben `PF-TD-001` und sind nicht als PF-BG-001-PASS deklariert.
- GitHub-Reviews zu PR #3/#4/#5 haben den neuen Owner-Vertrag als Integrationsvoraussetzung identifiziert; keine stille Child-Implementierung, kein Merge.

## Offene Finalisierung

- Unabhaengiges Code-/Delta-Review auf dem finalen PR-Head, bei Bedarf Korrektur und erneute Target-Regression.
- Dokumentations-/CI-Final-Head inkl. nicht abgeschlossener general CI und eventueller Cancelled-Runs beurteilen.
- PR #7 (Story-Governance) zuerst integrieren, #8 passend retargeten und den aktualisierten `main`-Stand validieren.
- `PF-SP-004/005/006` Gruppen-/Masken-APIs muessen `claim/clone/detach` ausdruecklich in ihrer **eigenen** Implementierung anpassen, bevor diese PRs gemergt werden.
- `review_report: pending`, `done: not_authorized`, keine Produktrelease-Freigabe.


## Finaler Nachtrag 2026-10-11

- PR #8 ist gemergt; die Testläufe [CI #38140374511](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374511) und [AmigaOS Host #38140374487](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374487) sind abgeschlossen mit `success`.
- Die fachliche `review_report: approve`-Empfehlung und ausgenommene Bereiche stehen im [Finalreview](PF-BG-001-final-review-2026-10-11.md); Story und Test unter `done/`, kein Editor-/Release-PASS.
