# PF-BG-001 — Implementation-Gate (2026-10-11)

- `story_id: PF-BG-001`
- `from_status: ready`
- `to_status: in_progress`
- `story_status_check: pass` — auf diesem Stack-Basisbranch in `docs/stories/PF-BG-001.md` als `ready` dokumentiert
- `backlog_status_check: pass` — `docs/backlog.md` auf diesem Basisbranch ordnet die Story `Bereit` zu
- `wip_limit_check: pass` — kanonischer Backlog hat aktuell keine `in_progress`-Eintraege; diese eine Story bleibt unter Limit 2
- `dor_check: pass` — [Pre-ready-Pruefung](PF-BG-001-pre-ready-review-2026-10-11.md)
- `ip_provenance_gate: pass` — ausschliesslich selbst erstellter C99-Portcode, keine Drittsource/Assets; `reference_only` fuer Rust/Verhaltenssemantik
- `implementation_allowed: yes` — **nur** die begrenzte Ownership-Ursache
- `repo_root_verified:` HurricanVD/photocraft-amiga, Basis `docs/pf-open-findings-2026-10-11`, Umsetzung auf `fix/pf-bg-001-raster-ownership`
- `write_scope:` `ports/amigaos3/{src/pc_raster.c,src/pc_raster_owner_internal.h,src/pc_document.c,include/pc_document.h,tests/pc_document_test.c,Makefile}` und die Story-/Test-/Review-/Backlogakte zu PF-BG-001
- `risk:` Owner-Flag muss eindeutige Document-Ownership auch dokumentuebergreifend wahren, Clone-Raster darf eigenstaendiger neuer Owner sein
- `scope_limit:` keine PF-SP-004/005/006-Implementierung, keine Rust-Crates, keine GUI, keine Produkt-ABI oder Formatentscheidung
- `test_plan:` lokaler Host-Test, ASan/UBSan wo ausfuehrbar, anschliessend opt-in GCC13/vamos O0/O2/CI im vorhandenen Testprofil
- `test_evidence_at_entry:` not_run fuer neue Regression; fruehere Basis-Tests gehoeren zum alten Code
- `review_report:` pending; keine Merge-, Done- oder Release-Freigabe
- `source_claim:` statischer Double-Free-Pfad in `pc_document_append_raster` und `pc_document_destroy` auf `main`
- `process:` VD Amiga 0.2.1; Dokumentationsnachweis fuer diese **neue** Transition, nicht rueckdatiert
