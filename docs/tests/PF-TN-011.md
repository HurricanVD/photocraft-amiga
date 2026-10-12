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
