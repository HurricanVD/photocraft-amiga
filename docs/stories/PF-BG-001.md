# PF-BG-001 — PcRaster-Ownership beim Layer-Anhaengen absichern

- Story-ID: `PF-BG-001`
- Story-Art: `BG`
- Typ: `bug`
- Status: `in_progress` (Implementation-Gate 2026-10-11 dokumentiert; Merge/Review/Done noch ausstehend)
- Prioritaet: `P0`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Herkunft: statische Codepruefung von `main`, `ports/amigaos3/src/pc_document.c`
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-012` (Host und ASan/UBSan ausgefuehrt PASS; 68k/vamos noch offen)

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
- Evidenz: Host-/ASan-Run #38125707879 (scope-limitiert PASS); [Implementierungs-PR #8](https://github.com/HurricanVD/photocraft-amiga/pull/8) Draft; kein finaler Merge-/Review-Abschluss.

## 7. refinement_note / refinement_triage

- Eine Aenderungsachse: Raster-Ownership an der Append-Grenze.
- `refinement_triage`: `pass` (statische Ursache, abgrenzbarer Ownership-Kontrakt und Testplan geprueft; Laufzeittest weiterhin `not_run`).
- IP-Klassifikation: `reference_only`, keine Drittquellenuebernahme; `rights_status`: `not_required`.
- `atomization_report.decision`: `keep_single_story` (Pre-ready-Review 2026-10-11).
- `ready`-Freigabe nur fuer den beschriebenen Ownership-Bugfix; kein Implementation-Gate, CI-/Merge- oder Release-PASS.

## 8. Abhaengigkeiten

- Bestehende flache Dokumentbaseline auf `main`.
- Vor Integration von PF-SP-004 bis PF-SP-006 gegen deren modifizierte Dokument-API erneut pruefen.
- PF-TD-001 behandelt Allokations-Rollback/Tile-Index separat; diese Bugstory behandelt ausschliesslich mehrfaches Raster-Eigentum.

## 9. Review und Gate-Status

- `arch_review.initial`: `pass` (2026-10-11; [Pre-ready-Review](../reviews/PF-BG-001-pre-ready-review-2026-10-11.md))
- `arch_review.pre_ready_final`: `pass` (2026-10-11; Review umfasst ADR/IP/Scope und geplante Tests)
- `dor_check`: `pass` (2026-10-11; siehe Pre-ready-Review)
- `implementation_gate_report`: `pass` ([Scope- und WIP-Gate](../reviews/PF-BG-001-implementation-gate-2026-10-11.md))
- `review_report`: `pending`
- `done`: `not_authorized`

## 10. Verbindlicher Implementierungs-Kontrakt nach Pre-ready-Review

- Ownership: Pro `PcRaster*` maximal ein erfolgreiches Owning-Append, auch ueber Dokumentgrenzen. Ein zweiter Claim muss sicher abgewiesen werden.
- Bei Erfolg darf der externe Caller das Raster nicht selbst freigeben; bei Fehler verbleibt es beim Caller. Raster-Clones sind eigenstaendige Owner-Objekte mit ggf. geteilten COW-Tiles.
- Lokal abgeleitete Architektur: privater Owner-/Claim-Zustand des opaken Rasterobjekts oder nachweislich aequivalenter Mechanismus; keine neue stabile oeffentliche ABI und keine volle Raster-Umschreibung.
- Testplan: gleiches Raster zweimal im selben Dokument und in zwei Dokumenten, Clone/Destroy, fehlgeschlagene Claims, Allokationsfehler, ASan/UBSan. **Noch nicht ausgefuehrt**.
- Provenienz: `reference_only`, vorhandenes Portregister; keine Software, Assets oder SDK aus Drittquellen kopieren.
- Offene Gates: `implementation_gate_report` und der abschliessende PR-/Test-/Reviewer-Gate. Der jetzige PASS gilt ausschliesslich fuer DoR.

## Priorisierung 2026-10-11

- Prioritaet: `P0` — Speicherkorruptionsrisiko; vor Integration weiterer Layer-Implementierungen.
- Quelle: [Prioritaets-/Ready-Audit](../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine bestehenden Implementation-/CI-/Reviewer-Gates; dokumentierte Story-Statuswerte bleiben massgeblich.

## 11. Implementierungsstand 2026-10-11

- Separater Branch: `fix/pf-bg-001-raster-ownership` (Stack-Basis `docs/pf-open-findings-2026-10-11`).
- C99-Änderung: internes Object-Ownership-Claim in `pc_raster.c`, expliziter `pc_document_append_raster`-Transfer, Regression fuer gleichen und fremden Document-Owner, COW-Clone.
- `PF-TN-012`: Implementierung in `tests/pc_document_test.c`; Host-C99/ASan/UBSan am 2026-10-11 im Run #38125707879 PASS. GCC13/vamos und neuer finaler Branch-Head laufen separat.
- PR-/Final-Review und etwaige 68k/vamos-Regressionsnachweise weiterhin offen.
