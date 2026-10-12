# Integration review — PR #18 (PhotoCraft-Amiga native document preview)

- Datum: 2026-10-12
- `review_stage: scoped_pr_merge_review` — **kein** `story_review_final` / keine Release-Freigabe.
- `review_report: approve_for_main_integration` nach Code-/Diff-/Lizenz-/Test-Evidenzpruefung, **unter Vorbehalt der abschliessenden relevanten CI-Gates**.
- `reviewer: AI-assisted consultant`; **keine** unabhaengige GitHub-APPROVE/Owner-Selbstfreigabe vorgetaeuscht.
- Integrationshead bei Reviewbeginn: `abb535939b635940659360e18e616bbbece4ba53`.
- Letzte Aenderung im Reviewer-Fix: `ports/amigaos3/Makefile` `$opt` -> `$$opt` im selbststaendigen `gcc13-build-compose-oracle`-Target; neuer Codehead vor diesem Reviewbericht: `3d3563c0dc92ae16d2fe05cd1dcf203dd12b42e7`.
- Ziel: `main`, nicht veroeffentlichter Produktrelease oder archivierter `done`-Status.
- Abhaengigkeiten: Feature-Branches aus PR #14 M2-RTG, #15 ReAction-Shell, #16 CPU-/Rust-Oracle, zusammen als Elternhistorie in #18; MIT-Quellregister/Importaudit aus [PR #17](https://github.com/HurricanVD/photocraft-amiga/pull/17) **bereits in main** bei `f53c7e679944741494a66dedaf02e4ad3ccb2888`.

## 1. Scope, Architektur und Source-Review

| Gegenstand | Pruefung | Ergebnis |
|---|---|---|
| Upstream-Grenze | Urspruengliche Rust-Workspace-`crates/` unberuehrt, einzig Rust-Oracle-Test-Entrypoint in `ports/amigaos3/tests/rust_oracle/src/main.rs` ergaenzt | `pass` |
| PhotoCraft-Kern | Native `PcDocument`, 256x256-Sparse-COW-Tiles, zwei lokale RGBA8-Ebenen, Alpha/Opacity + Gray8-Maske; kein Dunkelkammer-`DkDocument` | `pass_scoped` |
| CPU-Referenz | Normalblend, Visibility, Opacity, Mask und begrenztes 8-Zeilen-RGBA8-Band mit numerischem Rust-Original-Oracle; andere Blend-Modes/Groups explizit nicht Teil dieser Preview | `pass_scoped` |
| ReAction / CyberGraphX | `window.class` einziger Window-Owner, `gadgets/layout.gadget` und `gadgets/space.gadget`; M2 Offscreen-Friend-Bitmap, `WritePixelArray`, `BltBitMapRastPort`, kein MiniGL oder Remote-Backend | `pass_scoped` |
| Stack | AmigaOS Exec `StackSwap()` auf 200 KiB aus `AllocMem`, statisches Swap-Objekt und `noinline pc_main_body`; Rueckswap vor `FreeMem` | `pass_scoped` |
| Workbench | `argc==0` vermeidet stdout-CON:-Fenster; native `EasyRequestArgs` fuer relevante Startfehler; Shell behaelt Diagnose/MEMF_FAST/CHIP-Messung | `pass_scoped` |
| Canvas resize | Ein gemeinsames Preview-Limit 1920x1200, kein 1024x768 Grey-Fallback mehr innerhalb getesteter Groessen; Region-/Stride- und Band-Bounds, grosse/small-Regression | `pass_scoped` |
| RAM- und Ressourcengrenze | `pc_shutdown` raeumt ReAction, Presenter, Dokument, Klassen/Bibliotheken, Band-Puffer auf; echte vollstaendige Heap-/Bitmap-Leakfreiheit nicht instrumentiert; Worst-case-1920x1200-M2-Staging+Offscreen braucht rund 17.6 MiB zzgl. weiterer RAM-Verbraucher | `nonblocking_risk_for_preview` |
| IP/Third-Party | `ports/amigaos3/NOTICE-DUNKELKAMMER-MIT.txt`, Quell-SHA `26295506`, Eigentuemerlizenz `70897e91` (MIT), packaging mit eigenen Lizenzdateien und originalen Notices; keine Fremd-Fonts/NDK/ROMs/Icons kopiert | `pass_for_source_review` |

## 2. CI und Testnachweise

**Reproduzierbarer Build/Teststand vor dem reinen Review-Fix:**

- [PR #18](https://github.com/HurricanVD/photocraft-amiga/pull/18) Head `abb535939b635940659360e18e616bbbece4ba53`.
- Host C99, ASan/UBSan, normaler PhotoCraft Rust/C99-Differentialvergleich, isolierter `pc_canvas_doc_test`, echte GCC/Bebbo-13.3 HUNK + `vamos` O0/O2, PNG-loses nativer ReAction-Preview-HUNK: **CI success** fuer relevante Port-/Dokumentationsjobs. Zum Zeitpunkt dieses Reports laeuft ein allgemeiner, nicht port-spezifischer Ubuntu-Rust-Workspace-Test noch; dieser wird nicht als `success` vorweggenommen.
- Die Nachkorrektur des eigenstaendigen GCC13-Oracles ist auf neuem Head gepusht; `final_head_ci` **wird vor GitHub-Merge nochmals abgefragt**. Keine neue WinUAE-Testausfuehrung durch den Reviewer.

**WinUAE manuelle Anwendungsevidenz (vom Owner bereitgestellte Chat-Screenshots):**

- v0.0.4: `GUI-01` Fenster startet, `GUI-02` echtes lokales Dokument sichtbar, sauberer Exit `RC 0`; harte 1024×768-Grenze verursachte bei vergroessertem Fenster vorher sichtbare graue Fallbackflaeche.
- v0.0.5: `GUI-04` Vergroesserung ueber die vorherige Fehlgroesse funktioniert, Dokument/Schachbrett/Maskenbild sichtbar und zentriert.
- v0.0.5: vier aufeinanderfolgende Starts und normales Schliessen `RC 0` ohne sichtbaren Guru.
- v0.0.6: Workbench-Icon startet ReAction-Fenster ohne unerwuenschtes Shell/CON:-Fenster; mehrere normale Shell-Laeufe `RC 0`. Besitzerfotos liegen **nur in der Konversation**, nicht als oeffentliche GitHub-Anlagen.
- `available_memory_observation`: erster v0.0.6-Lauf Fast `-128408 B`, spaetere Fast innerhalb der Laeufe `-4464/-4384 B`, aber zwischen den Post-Exit-Messungen nur `-1144 B` insgesamt; Chip nach erstem Lauf stabil. Eine sich je Prozessstart um 4.4 KiB aufaddierende Leckage ist nicht gezeigt, aber **keine Leckfreiheitszertifizierung**.
- `low_fast_ram_manual: skipped_by_owner_preview_only`: ausdrueckliche, nur auf die aktuelle Preview begrenzte Owner-Ausnahme, **kein PASS**.
- `resource_heap_tracking: not_run`, `full_PF-SP-020_editor: not_implemented`, `Workbench_no_RTG_or_startup_failure: not_run`.

## 3. Code Findings und Bewertung

- `High: none` im begrenzten native-Preview-Merge-Scope.
- `Medium: none` in der ueberprueften Betriebsart mit dokumentiertem Pixel-/GUI-/Stack-Pfad.
- `Low/Fix resolved`: Im allein aufrufbaren Makefile-Oracletarget wurde `$opt` bei `mkdir` durch GNU Make zu frueh expandiert (shell `$$opt` notwendig). Vor Merge behoben; eigenstaendiger Cross-Build braucht trotz bisher gruener Sammeljobs weiterhin Verifikationsgate im naechsten CI-Lauf.
- `Low/accepted for preview`: offener RAM-/Bitmap-Restverbrauch kann nicht auf einen konkreten PhotoCraft-Leak zurueckgefuehrt werden; expliziter Revisit bei monotonem Verlust nach Warmlauf bzw. vor Produktfreigabe.
- `Low/accepted for preview`: kein Produkt-PSD/PNG-Import/Export, kein Layer-Tool, keine Gruppen-/ICC/16/32f-Kompositfaehigkeit, keine reale 68040-Hardware-Leistungsfreigabe. Diese sind **separate Ready-Stories** (PF-SP-013/017/018/019/020) und keine verdeckten Merge-Blocker fuer die nativen Basisbausteine.
- `open_auto_fix: none` nach dem Makefile-Patch; `final_story_review_report: pending` fuer vollstaendiges `done` der PF-SP-007/010/011/012.

## 4. Schlussentscheidung

- `integration_review: approve_for_main` **wenn** GitHub den neuen PR18 Head sauber gegen `main` mergen kann und relevante neue GitHub-Portchecks nicht fehlschlagen.
- Merge **schliesst NICHT** die Stories als `done`, setzt keinen Produktcompiler, keine stabile ABI, keinen Release und behauptet keine ungetesteten Low-RAM-/Leakfreiheit-Gates.
- Akzeptierte ADR-0001/0002/0003 und isolierter C99-AmigaOS-Port bleiben massgeblich; Produkt ist **vollstaendig offline**, Rust bleibt Upstream-Referenz.
- `agent_model_plan:` Quell-/Diff-/Lizenz-/CI-/WinUAE-Evidenz statisch pruefen, Scope-Limit dokumentieren, kleinen Reviewer-Fix korrigieren, neue CI und Main-Merge verifizieren; keine eigenmaechtige Story-Archivierung.
- `skill_usage_evidence:` VD Amiga process `docs/process/story-lifecycle.md` und vorhandene PhotoCraft Reviewberichtsvorlagen gelesen; externes unabhaengiges GitHub-Codereview `not_submitted`.
