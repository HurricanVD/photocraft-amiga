# PF-TN-011 — PF-TD-001 planned acceptance

- Story: [PF-TD-001](../stories/PF-TD-001.md)
- Date: 2026-10-12
- Status: `planned / not_run`. No implementation, build, runtime or manual native test PASS asserted.
- Scope: Tile index complexity, allocator fault injection (each allocation site), transactional region write and 32 MiB Fast RAM.

## Proposed verification

1. Fixture-positive and negative/boundary cases and repeatability.
2. Host C99 smoke/ASan/UBSan and unchanged original PhotoCraft Rust oracle for OS-free semantics; m68k/GCC13 -O0 and -O2 HUNK/vamos for target-portable code.
3. If GUI/RTG, direct manual AmigaOS 3.2 + WinUAE window/gadget visual and interaction test, exact binary SHA, hardware/RTG settings, screenshots and logs.
4. Invalid input, allocator fault, cleanup, rollback and RAM peak; no floating-point/byte-order assumptions outside documented contract.
5. No automated WinUAE UI/broker/capture flows while `disabled_by_process`.

- `test_evidence: not_run`
- `runtime_evidence: none`
- `release_authorization: no`

## PF-TD-001 implementation evidence checkpoint (2026-10-12)

- Implementation branch: `implementation/pf-td-001-raster-atomic-index`.
- Code: sorted entries / binary-search tile lookup; COW clone-and-swap region write; compile-time-only deterministic allocator fault injection `PC_RASTER_TEST_ALLOC`.
- Test added: `ports/amigaos3/tests/pc_raster_atomic_test.c` (unordered tile coordinates, readback, COW isolation, allocation failure positions, all-or-nothing pixel content and tile count).
- Build targets added: `host-raster-atomic-test`, `host-raster-atomic-sanitize`, `gcc13-build-raster-atomic`, `gcc13-vamos-raster-atomic`.
- Test status: `not_run` in this review session. Source has been submitted, but CI, host compiler, sanitizers and m68k/vamos have not been executed or attested here.
- Performance: indexed lookup is O(log N), insertion remains O(N) from ordered vector shifts. No wall-clock benchmark yet; cannot assert throughput improvement for write-heavy workloads.
- Peak memory: clone-and-swap retains old COW tiles and a second entry vector; copying changed tiles raises transient peak. No verified <=32 MiB RAM budget / low-memory target pass.
- Scope status: `partial_implementation`, not `done`; pending independent implementation/review gates and measured PF-TN-011 acceptance.

## Verifizierte CI-Evidenz nach Compile-Fix (2026-10-12)

- Commit `6b5583192e64a790a52eaace9fe4cc444022fbf3`.
- [AmigaOS 3.2 Host Core run #38255580495](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38255580495): `success` fuer Portable C99 + ASan/UBSan, Rust originals vs C99 port und GCC13.3 HUNK/vamos.
- Im Host-Joblog explizit nachgewiesen: `PASS: PF-TD-001 sorted lookup and fault-atomic COW region` fuer `host-test` und `host-core-sanitize` (neuer Regressionstest mit Fault-Injection).
- Die **neuen** dedizierten `gcc13-build-raster-atomic` / `gcc13-vamos-raster-atomic` Targets sind nicht in den CI-Standardjob eingebunden: deren Ausfuehrung bleibt `not_run`. Die bestehenden GCC13/vamos-Jobs sind `success`, aber kein Ersatz fuer das dedizierte Target.
- Ein Index-/Write-Benchmark und ein gemessener <=32 MiB Fast-RAM-Peak liegen noch nicht vor; Gesamtstory bleibt offen. Diese Evidenz ersetzt keine Implementation-/Final-Review-Gates.

## Zusatz-Gates im PR #20 (2026-10-12)

- Die bestehende Workflow-Datei `.github/workflows/amigaos3-host.yml` baut jetzt `gcc13-build-raster-atomic`, prueft beide HUNK-Dateien und fuehrt `gcc13-vamos-raster-atomic` unter O0/O2 aus. PASS gilt erst nach neuer CI-Evidenz, nicht aufgrund der Konfigurationsaenderung.
- `make -C ports/amigaos3 host-raster-benchmark` fuehrt 256 Reverse-Order-Tile-Inserts und 120000 deterministische Pixel-Lookups aus. Ausgabe sind CPU-Zeiten ohne voreiliges Performanz-Gate; noch kein Vergleich mit einem identischen Baseline-Binary.
- Hinweis fuer die RAM-Analyse: bei 256x256 U8-RGBA benoetigt ein vollstaendig allokiertes Tile 262144 Byte Pixel-Bytes, plus Referenzzaehler/Allocator-Overhead. Der clone-and-swap-Rollback kann betroffene Tiles duplizieren; ein 32-MiB-Peak muss mit Workload und Target-Allocator separat nachgewiesen werden.
- `RAM_budget_32MiB: not_measured`; `dedicated_m68k_fault_test: pending_CI`; `benchmark: added_not_run`.

## CI-Nachweis PF-TD-001 (Nachtrag 2026-10-12)

- [Run #38257133288](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38257133288): **PASS** fuer Host C99+ASan/UBSan, Rust/C99 und GCC13.3 m68k/vamos. Das Joblog bestaetigt ausdruecklich `PASS: PF-TD-001 sorted lookup and fault-atomic COW region` bei **O0 und O2** und validiert beide nativen HUNK-Executables. Die frueheren `pending_CI`-Stellen oben sind historische Stichtage.
- Host-Mess-Workflow unter PR #20 erweitert: 256 Gray8-Tiles (16 MiB Pixeldaten), 120000 Lookups, getrennte `/usr/bin/time -v`-Peak-RSS-Messung des Benchmark-Prozesses, reproduzierbares Artefakt `pf-td-001-host-benchmark-and-rss`. Werte sind erst nach erfolgreichem neuen Workflow und heruntergeladenem Artefakt bestaetigt.
- Selbst ein gemessener Host-RSS-Wert unter 32 MiB kann **keine** AmigaOS-3.2-32-MiB-Fast-RAM-Freigabe ersetzen: anderer Allocator, OS-Overhead und die Peak-Belastung durch grosse multi-Tile COW-Schreibtransaktionen bleiben separat offen.
- `dedicated_m68k_fault_test: PASS_2026-10-12`; `host_benchmark_measurement: pending`; `target_32MiB_budget: not_proven`; `story_done: no`.

## Host-Messung aus CI (2026-10-12)

- GitHub Actions [AmigaOS Host Core #38262358382](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38262358382), Job `Portable C99 + ASan/UBSan`: PASS.
- Host-Benchmark `pc_raster_bench` (256 Gray8-Tiles / 16 MiB Pixelbytes, 120000 Lookups): Inserttime `0.032013` CPU-s, Lookuptime `0.001426` CPU-s, Checksum `840000`.
- `/usr/bin/time -v` fuer den **Benchmark-Prozess**: maximaler residenter Host-Speicher `18156 KiB` (Linux, nicht AmigaOS Fast RAM). Die Compiler-RSS ist nicht Teil dieser Messung.
- Keine Vergleichsmessung mit gleicher Fixture auf linearem Vorgaenger-Index; kein belastbarer relativer Speedup.
- 32-MiB-Fast-RAM-Target incl. transaktionaler grosser Region-Writes bleibt **nicht nachgewiesen** und ist kein PASS.
