# PF-BG-001 — PcRaster-Ownership beim Layer-Anhaengen absichern

- Story-ID: `PF-BG-001`
- Story-Art: `BG`
- Typ: `bug`
- Status: `draft` (nicht `ready`, keine Implementation autorisiert)
- Prioritaet: `P0` (potenzieller Double-Free / Use-after-free)
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Herkunft: statische Codepruefung von `main`, `ports/amigaos3/src/pc_document.c`
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-012` (geplant; noch nicht ausgefuehrt)

## 1. Problem / Nutzerwert

`pc_document_append_raster()` prueft eindeutige Layer-IDs, aber nicht, ob der uebergebene `PcRaster *` bereits einem Layer gehoert. Wird derselbe Pointer unter zwei verschiedenen IDs uebernommen, ruft `pc_document_destroy()` zweimal `pc_raster_destroy()` auf demselben Objekt auf. Moegliche Folgen sind Double-Free, Heap-Korruption und Absturz. Das ist ein **statischer Befund**, kein in CI reproduzierter Fehlernachweis. Ziel ist ein eindeutiger Ownership-Vertrag an dieser API-Grenze.

## 2. In Scope

- Klare, pruefbare Regeln fuer Eigentumsuebergabe, wiederholtes Anhaengen desselben Pointers sowie Fehler-Rueckgaben festlegen.
- Den Aliasfall innerhalb desselben Dokuments verhindern; Cross-Document-Aliasing in der Architekturentscheidung explizit behandeln, statt stillschweigend als sicher anzunehmen.
- Besitzer- und Layer-Zustand bei einem fehlgeschlagenen Append unveraendert lassen (kein implizites Freigeben oder Uebernehmen).
- Tests fuer Destroy, Clone und Fehlpfade des bestehenden flachen C99-Dokumentmodells.

## 3. Nicht-Ziele

- Gruppen-, Masken-, Compositor- oder History-Implementierung.
- Vollstaendige Neugestaltung von `PcRaster` oder Freigabe einer stabilen C-ABI.
- Aenderungen an originalen Rust-Crates; keine proprietaeren Dunkelkammer-Sourcen/Assets.

## 4. User Story

Als PhotoCraft-Portentwickler moechte ich Raster-Eigentum genau einmal uebertragen koennen, damit ungueltige Append-Aufrufe weder Heap-Korruption noch doppelte Freigaben verursachen.

## 5. Akzeptanzkriterien

1. Ein reproduzierbarer Regressionstest deckt zweimaliges Anhaengen desselben `PcRaster *` mit verschiedenen Layer-IDs ab: zweiter Aufruf wird kontrolliert abgewiesen; der erste Layer bleibt gueltig.
2. `pc_document_destroy()` und `pc_document_clone()` sind nach abgelehnten Appends speichersicher; keine Double-Free-/Use-after-free-/Leak-Diagnose unter ASan/UBSan.
3. Eine dokumentierte Entscheidung definiert das Ownership-Verhalten **ueber Dokumentgrenzen** und testet entweder die Ablehnung oder ein explizites, korrekt referenzgezaehltes Teilen.
4. Die API-Dokumentation nennt den Zeitpunkt der Eigentumsuebertragung und den Owner bei jeder Fehlerklasse eindeutig.
5. Hosttest und Sanitizer-Regression sind nachweisbar erfolgreich; m68k-HUNK/vamos O0/O2 wird fuer die betroffene C99-Teilmenge nach dokumentierter Toolchain-Gate-Entscheidung nachgezogen. Kein Test wird vor Ausfuehrung als PASS bezeichnet.

## 6. Architektur / Tests / Grenzen

- Betroffen: `ports/amigaos3/src/pc_document.c`, `include/pc_document.h`, `tests/pc_document_test.c` und ggf. eng begrenzte Ownership-Adapter.
- Ausgangspunkt: akzeptierte ADR-0001/0002/0003, speziell ADR-0002 (Owner, COW und Fehlerverhalten).
- Bestehende Referenz: `pc_document_append_raster`, `pc_document_destroy`, `pc_document_clone`.
- Rust-Original dient als Semantikreferenz; C-spezifisches Pointer-Aliasing hat keine zwingend identische Rust-Testsequenz.
- Kein AmigaOS-SDK oder MiniGL fuer den Host-Regressionstest notwendig.
- Evidenz: `not_run`; keine Implementierungs-PR.

## 7. refinement_note / refinement_triage

- Eine Aenderungsachse: Raster-Ownership an der Append-Grenze.
- `refinement_triage`: `pending` (Bug reproduzieren, Scope und Prioritaet bestaetigen).
- IP-Klassifikation: `reference_only`, keine Drittquellenuebernahme; `rights_status`: `not_required`.
- `atomization_report.decision`: `keep_single_story` (vorlaeufig; formale Pruefung offen).
- Keine stillschweigende `ready`-/`in_progress`-Freigabe.

## 8. Abhaengigkeiten

- Bestehende flache Dokumentbaseline auf `main`.
- Vor Integration von PF-SP-004 bis PF-SP-006 gegen deren modifizierte Dokument-API erneut pruefen.
- PF-TD-001 behandelt Allokations-Rollback/Tile-Index separat; diese Bugstory behandelt ausschliesslich mehrfaches Raster-Eigentum.

## 9. Review und Gate-Status

- `arch_review.initial`: `pending`
- `arch_review.pre_ready_final`: `pending`
- `dor_check`: `pending`
- `implementation_gate_report`: `pending`
- `review_report`: `pending`
- `done`: `not_authorized`
