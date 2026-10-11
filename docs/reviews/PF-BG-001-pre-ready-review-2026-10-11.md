# PF-BG-001 — Refinement, initiales und finales Pre-ready-Architekturreview

- Datum: 2026-10-11
- Story: [PF-BG-001](../stories/PF-BG-001.md)
- Review-Typ: dokumentierter sequenzieller Eigenreview der vorhandenen Quellen; **kein unabhaengiger Subagent-/Maintainer-Review**
- Basis: `main` 4f2b0cc9, `ports/amigaos3/src/pc_document.c`, `src/pc_raster.c`, `include/pc_document.h`, `tests/pc_document_test.c`, `docs/adr/ADR-0001..0003`, `ports/amigaos3/docs/ip-provenance-register.md`
- Pruefumfang: **Readiness/Design**, keine Codeumsetzung, kein CI-/Target-PASS
- `refinement_triage: pass`
- `atomization_report.decision: keep_single_story`
- `arch_review.initial: pass`
- `arch_review.pre_ready_final: pass`
- `dor_check: pass`
- Entscheidung: `ready` fuer den klar begrenzten Bugfix, **nicht** `in_progress` oder `done`

## 1. Findings und Refinement

`pc_document_append_raster()` prueft die Einzigartigkeit der ID, nicht aber die Einmaligkeit des uebergebenen `PcRaster*`. Dieselbe Adresse kann unter zwei Layer-IDs (auch dokumentuebergreifend) gespeichert und bei `pc_document_destroy()` mehrfach freigegeben werden. Die Behauptung folgt aus dem Quellcode; ein ausfuehrender Regressionstest steht noch aus und wird nicht als PASS ausgewiesen. Dieses reproduzierbar formulierbare Fehlermodell besitzt einen abgegrenzten Fix.

- Ursache: Ownership-Transfer ohne Raster-internen Besitzzustand oder Double-Append-Pruefung.
- Kleinste ausreichende Ursachenbehebung: Owner-Bindung beim Transfer atomar nur einmal zulassen, doppelte Claims zurueckweisen. Implementierungsdetail (z. B. privates `owner`/claimed Feld im opaken `PcRaster`) bleibt dem Umsetzungsgate, solange der dokumentierte Kontrakt eingehalten wird.
- Geltungsbereich: flache Dokument-API `pc_document_append_raster` und `pc_raster_destroy`, einschliesslich dokumentuebergreifender fehlerhafter Transfers. Keine Layer-Gruppen-/History-/Memory-Budget-Umgestaltung.
- Zusaetzliche Regel: Nach Erfolg darf der externe Aufrufer das uebergebene Raster weder erneut uebergeben noch selbst freigeben. Nach Fehler verbleibt das Raster beim Aufrufer. Ein Dokument-Clone besitzt eigene `PcRaster`-Objekte, deren Tiles COW teilen duerfen.
- Keine temporäre Workaround-Freigabe; Wiederholungsmuster: eine Owning-Append-Grenze, gesonderte OOM-Faelle in PF-TD-001.
- Prioritaet: P0 wegen moeglichem Double-Free. Test/ASan noch `not_run`.

## 2. Initiales Architekturreview (heute)

- PhotoCraft-Amiga-Seam (ADR-0001): **pass**, portable C99-Ownership-Grenze innerhalb des isolierten Ports.
- Native Tile/Memory Contract (ADR-0002): **pass**, erhaelt 256x256-Tiles, rohe Pixelbytes und COW-Snapshot-Semantik; Owner des Raster-Objektes ist davon getrennt.
- Testprofile (ADR-0003): **pass**, Host/Sanitizer, danach opt-in GCC13 HUNK/vamos bei faehiger Toolchain; Produktcompiler wird nicht gewaehlt.
- Architekturbeschreibung `ports/amigaos3/docs/architecture.md`: **pass** fuer den begrenzten C99-Raster-/Document-Seam.
- ReAction, NDK, MiniGL, OS-Capabilities, vxlibs: **n/a**, da rein portables Ownership ohne OS-API-/Library-Abhaengigkeit; keine neue OS-Abweichung.
- Heap-vs-Stack: kein frame-size-getriebenes Scratch-Array; nur kleiner privater Owner-Zustand, keine zusaetzliche Kopie der Tile-Daten gefordert.
- IP: `reference_only`; Rust-Original und der Port-Quellbestand als Verhalten/Modellreferenz, keine fremden Quell-/Asset-Uebernahmen. Existing provenance in `ports/amigaos3/docs/ip-provenance-register.md`. `rights_status: not_required` fuer diesen Scope, keine neue Shipping-Pflicht.
- ADR-Aktion: `n/a`; kein neues weitreichendes ABI-/Produktversprechen; eine konkrete Umsetzung, die ein neues oeffentliches Interface benoetigt, ist separat zu reviewen.

## 3. Finales Pre-ready-Review (nach obigem Refinement)

- Testbar: doppeltes Append in einem Dokument, zweites Dokument, Owner nach Fehler, Clone-Isolation, Destroy genau einmal, reproduzierbarer ASan-Regressionstest.
- Fehlpfade: falsche ID, `NULL`, Memory-Allocation-Failure und bereits beanspruchtes Raster muessen den Besitz beim Aufrufer belassen und bereits gespeicherte Layer erhalten.
- Regressionstests: `PF-TN-012` (geplant), `make -C ports/amigaos3 host-test` und `host-core-sanitize`; m68k/vamos O0/O2 nur bei realem Build und ausgefuehrtem Test als PASS.
- Scope/Non-Scope, Module, Fehlerverhalten, IP- und ADR-Grenze: pruefbar und vollstaendig genug fuer Umsetzung.
- `arch_review.pre_ready_final: pass`; `dor_check: pass`. Die Pre-ready-Freigabe ist **keine** technische Abnahme des Fixes.
- `implementation_gate_report: pending`; WIP-Gate und Branch-/PR-Scope vor Eintritt in `in_progress` erneut pruefen.
- `review_report: pending`; `done: not_authorized`.

## 4. Risiken und Revisit

Eine Implementierung, die Layer-Pointer global registriert, zusaetzliche Allokationen einfuehrt oder die oeffentliche `PcRaster`-ABI/Copy-Semantik aendert, muss die Ownership-, OOM- und Architekturfolgen vor der Codefreigabe neu pruefen. Vor PR #3–#5 Merge mit den jeweiligen Layer-/Group-/Mask-APIs erneut regressionspruefen. Fehlende Tests bleiben bewusst offen.
