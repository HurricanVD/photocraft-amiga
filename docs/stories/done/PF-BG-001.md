# PF-BG-001 — PcRaster-Ownership beim Layer-Anhaengen absichern

- Story-ID: `PF-BG-001`
- Story-Art: `BG`
- Typ: `bug`
- Status: `done` (2026-10-11; begrenzter Ownership-Fix implementiert, getestet, reviewt und gemergt)
- Prioritaet: `P0`
- Entry-Profil: `standard`
- Datum: 2026-10-11
- Herkunft: statische Codepruefung von `main`, `ports/amigaos3/src/pc_document.c`
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-012` (Host, ASan/UBSan und GCC13/vamos O0/O2 auf Code-SHA `ae3102f` PASS; Endreview des PR-Heads separat)

## 1. Problem / Nutzerwert

Im urspruenglichen `main` vor dem Fix pruefte `pc_document_append_raster()` eindeutige Layer-IDs, aber nicht, ob der uebergebene `PcRaster *` bereits einem Layer gehoert. Wird derselbe Pointer unter zwei verschiedenen IDs uebernommen, ruft `pc_document_destroy()` zweimal `pc_raster_destroy()` auf demselben Objekt auf. Moegliche Folgen sind Double-Free, Heap-Korruption und Absturz. Das war der **statische Ausgangsbefund**. Der nun implementierte Claim und seine Tests werden in `PF-TN-012` mit ausgefuehrter CI-Evidenz nachgewiesen.

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
- Evidenz: Host/ASan/GCC13-vamos und Rust-Oracle PASS, [PR #8](https://github.com/HurricanVD/photocraft-amiga/pull/8) gemergt; Final-PR-Head CI #38140374511 und Host #38140374487 erfolgreich.

## 7. refinement_note / refinement_triage

- Eine Aenderungsachse: Raster-Ownership an der Append-Grenze.
- `refinement_triage`: `pass` (Pre-ready-Beurteilung vor der Umsetzung; der damals noch `not_run` markierte Test ist inzwischen fuer den begrenzten Code-Scope in CI #38125707879 ausgefuehrt).
- IP-Klassifikation: `reference_only`, keine Drittquellenuebernahme; `rights_status`: `not_required`.
- `atomization_report.decision`: `keep_single_story` (Pre-ready-Review 2026-10-11).
- Die damalige `ready`-Freigabe bezog sich ausschliesslich auf den beschriebenen Ownership-Bugfix; danach folgte das dokumentierte Implementation-Gate und die ausgefuehrte technische CI. Finaler Merge-/Release-Status wird gesondert beurteilt.

## 8. Abhaengigkeiten

- Bestehende flache Dokumentbaseline auf `main`.
- Vor Integration von PF-SP-004 bis PF-SP-006 gegen deren modifizierte Dokument-API erneut pruefen.
- PF-TD-001 behandelt Allokations-Rollback/Tile-Index separat; diese Bugstory behandelt ausschliesslich mehrfaches Raster-Eigentum.

## 9. Review und Gate-Status

- `arch_review.initial`: `pass` (2026-10-11; [Pre-ready-Review](../../reviews/PF-BG-001-pre-ready-review-2026-10-11.md))
- `arch_review.pre_ready_final`: `pass` (2026-10-11; Review umfasst ADR/IP/Scope und geplante Tests)
- `dor_check`: `pass` (2026-10-11; siehe Pre-ready-Review)
- `implementation_gate_report`: `pass` ([Scope- und WIP-Gate](../../reviews/PF-BG-001-implementation-gate-2026-10-11.md))
- `review_report`: `approve`
- `done`: `completed` (2026-10-11, Scope PF-BG-001; kein Produktrelease)

## 10. Verbindlicher Implementierungs-Kontrakt nach Pre-ready-Review

- Ownership: Pro `PcRaster*` maximal ein erfolgreiches Owning-Append, auch ueber Dokumentgrenzen. Ein zweiter Claim muss sicher abgewiesen werden.
- Bei Erfolg darf der externe Caller das Raster nicht selbst freigeben; bei Fehler verbleibt es beim Caller. Raster-Clones sind eigenstaendige Owner-Objekte mit ggf. geteilten COW-Tiles.
- Lokal abgeleitete Architektur: privater Owner-/Claim-Zustand des opaken Rasterobjekts oder nachweislich aequivalenter Mechanismus; keine neue stabile oeffentliche ABI und keine volle Raster-Umschreibung.
- Testmatrix: gleiches Raster in einem und in zwei Dokumenten, Clone/Destroy, abgelehnte Claims und ASan/UBSan ausgefuehrt (**PASS**, CI #38125707879). Deterministische Allokator-Fault-Injection ist separat unter PF-TD-001 offen.
- Provenienz: `reference_only`, vorhandenes Portregister; keine Software, Assets oder SDK aus Drittquellen kopieren.
- Das Implementation-Gate ist inzwischen dokumentiert und bestanden. Offen bleibt das finale PR-/Review-/Merge-Gate. Die zugehoerige DoR-Freigabe wird dadurch nicht rueckdatiert.

## Priorisierung 2026-10-11

- Prioritaet: `P0` — Speicherkorruptionsrisiko; vor Integration weiterer Layer-Implementierungen.
- Quelle: [Prioritaets-/Ready-Audit](../../reviews/PF-backlog-priority-ready-audit-2026-10-11.md).
- Prioritaetsentscheidung aendert keine bestehenden Implementation-/CI-/Reviewer-Gates; dokumentierte Story-Statuswerte bleiben massgeblich.

## 11. Implementierungsstand 2026-10-11

- Separater Branch: `fix/pf-bg-001-raster-ownership` (Stack-Basis `docs/pf-open-findings-2026-10-11`).
- C99-Änderung: internes Object-Ownership-Claim in `pc_raster.c`, expliziter `pc_document_append_raster`-Transfer, Regression fuer gleichen und fremden Document-Owner, COW-Clone.
- `PF-TN-012`: Host-C99/ASan+UBSan und GCC13/vamos O0/O2 am 2026-10-11 in [CI #38125707879](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38125707879) PASS auf Code-SHA `ae3102f`. Finaler PR-/Main-Integrations-Review ausstehend.
- 68k/vamos auf `ae3102f` sowie letztem PR-Head #38140374487 PASS; Reviewbefund vom externen Codex-Check behoben, eigenständiger Abschlussbericht abgelegt.

## 12. Review-Handoff (2026-10-11)

- [Change Summary](../../reviews/PF-BG-001-change-summary-2026-10-11.md) und [PF-TN-012](../../tests/done/PF-TN-012.md) mit Host-, Sanitizer- und m68k/vamos-Ergebnissen.
- Historischer Review-Handoff; nun abgeschlossen durch [finalen Reviewbericht](../../reviews/PF-BG-001-final-review-2026-10-11.md) und Merge #8 (`4fa6ca6`).
- PR #7 und #8 sind integriert; PF-SP-004/005/006 bleiben separat `refining` und nicht freigegeben.

## 13. Finaler Story-Abschluss — 2026-10-11

- `review_report: approve` — [finaler Review](../../reviews/PF-BG-001-final-review-2026-10-11.md) auf Basis des eingefrorenen Changesets und ausgefuehrter Checks; Reviewempfehlung dokumentiert, **keine selbst erteilte GitHub-APPROVE-Submission**.
- `test_evidence: pass` — [PF-TN-012](../../tests/done/PF-TN-012.md); [PR #8](https://github.com/HurricanVD/photocraft-amiga/pull/8), Commit `4fa6ca6a5b5a5b1c48257835fabacb7922c3b03e`.
- Final-PR-Head `5c862a3ca149b26af0eb9fd20b762f671f7b9217`: [CI #38140374511](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374511), [AmigaOS Host/GCC13/vamos #38140374487](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374487), [Docs #38140374504](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374504), [PF Preflight #38140374455](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374455), [Packaging #38140374462](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38140374462) — jeweils success.
- Manueller WinUAE-Test: `n/a` fuer plattformfreien C99-Ownership-Bugfix, ersetzt durch Host/Sanitizer und echten m68k-HUNK/vamos-Regressionstest; keine MiniGL-/UI-Abnahme.
- Der Integrierte Port und die bestehende API-/IP-/Architekturbeschreibung wurden fuer diesen Scope abgeglichen. Produktions-ABI, PSD, Masken/Groups, OOM-Fault-Injection und Release bleiben ausserhalb dieser Story.
- Archivpfad: `docs/stories/done/PF-BG-001.md`, Test: `docs/tests/done/PF-TN-012.md`. Die alleinigen kanonischen Statuszeilen liegen im `docs/backlog-done.md`.
