# PF-TD-001 – manual WinUAE 32 MiB Fast-RAM acceptance

- Status: `not_run` (instructions only; no claimed target PASS).
- Tester records exact Git commit, GCC/Bebbo 13.3 compiler details, SHA256 of the built HUNK, WinUAE version, CPU/FPU, installed AmigaOS 3.2 version, Kickstart, RTG/P96 settings, graphics memory, and active Fast/Chip RAM.
- Use exactly **32 MiB configured Fast RAM** for this test, not an arbitrary 32 MiB of free application heap. Record `Avail` before and after OS warm-up, before app launch and after each run.
- Use standard manual WinUAE execution only; automated UI broker/capture is still `disabled_by_process`.

## RAM-06 (target system) checklist

1. Warm up the AmigaOS/ReAction/CGX environment with a normal preview launch/close. Capture an AmigaShell `Avail` baseline and largest free Fast allocation if instrumentation supports it.
2. Build an Amiga HUNK with GCC13 using `gcc13-build-raster-atomic`. Confirm both O0 and O2 HUNK signatures with `scripts/check-hunk.py`, and record artifact SHA256.
3. Run `pc_raster_atomic_test` for both variants in the 32-MiB WinUAE target (not only `vamos`). Record return codes and terminal output, including tracked raster heap high-water and current bytes at exit.
4. For full application integration, launch the native document-preview from Workbench and Shell with the same configuration; capture screen, normal close, and before/after `Avail` readings. The preview test does **not** automatically prove an OOM during a large raster transaction.
5. Force each supported allocator failure under the deterministic test-only allocation hook. Confirm unchanged pixel contents/tile counts and snapshot isolation; no partially committed region or crash. Use a specific recorded bounded fixture and log its requested sizes.
6. Measure full-process Fast-RAM consumption and its transient peak, including allocator/OS/stack/RTG components. If only before/after `Avail` is available, record `peak_unknown` and **do not** mark the 32-MiB peak gate passed.
7. Repeat normal startup/close several times after warm-up; do not infer a leak solely from transient OS caches. Escalate reproducible monotonic residual allocations to PF-TD-003.

## Acceptance

- All valid reference operations either commit fully or return controlled OOM with identical original/snapshot bytes; no invalid pointers, double frees, or leaks in scoped core.
- Measured target peak fits the configured environment, with documented remaining Fast RAM and observed largest allocation; `Avail` alone cannot prove a peak.
- Report `PASS` only with actual manual target logs and SHA/configuration. Otherwise `not_run` or `blocked`, not `pass`.
- This gate does not replace PF-SP-020 end-to-end editor acceptance and does not authorize a product minimum-RAM specification.

## Ready-to-run test package (PR #20; 2026-10-12)

The `AmigaOS 3.2 Host Core` GitHub Actions workflow cross-compiles two
**standalone Shell-only Amiga HUNK probes** and publishes the artifact
`pf-td-001-winuae-ram06-hunk` with:

- `pc_ram_winuae_probe_O0`, `pc_ram_winuae_probe_O2` – binaries from GCC13.3;
- `SHA256SUMS.txt` and `SOURCE-COMMIT.txt` – reproducibility markers;
- `RAM-06-MANUAL.md` – this procedure.

The probe executes the **same** C99 raster fault and COW regression suite
as the existing `pc_raster_atomic_test`. During tracked raster allocations
and frees, its read-only observer samples `AvailMem(MEMF_FAST)` and
`AvailMem(MEMF_FAST|MEMF_LARGEST)`. The report provides initial, minimum
and final values for both plus internal raster-heap peak and outstanding
bytes. This improves visibility into transient pressure, but is **not**
whole-application instrumentation: it does not sample unrelated
ReAction/RTG/OS allocations occurring independently of raster events.

### Preparation (host)

1. Download the `pf-td-001-winuae-ram06-hunk` artifact from a fully
   successful **current-head** PR #20 Actions run.
2. Verify `SHA256SUMS.txt` using `sha256sum -c SHA256SUMS.txt`.
   Record full `SOURCE-COMMIT.txt`, workflow run ID, artifact SHA256 and
   compiler container image digest (available in CI logs).
3. Copy both executables and the manual to an Amiga-accessible local volume.
   Do not transfer any SDK, ROM, font or proprietary AmigaOS binary.

### Execution (manual WinUAE, AmigaShell)

- Configure **exactly 32 MiB Fast RAM**, AmigaOS 3.2, 68040, and the
  specified RTG/P96 setup; document Chip RAM, screen mode, WinUAE version
  and ROM separately. Disable network for the offline requirement.
- Warm up the OS/runtime, record `Avail`, and ensure enough Shell stack.
  These probe HUNKs do **not** use the preview application's StackSwap
  helper; set `Stack 200000` in Shell before running.
- Execute `pc_ram_winuae_probe_O0`, record full console output and
  return code; repeat for `pc_ram_winuae_probe_O2`.
- For each, require the exact `PASS: PF-TD-001 sorted lookup and
  fault-atomic COW region` line, `return_code=0`,
  `raster_heap_end=0`, and `observations>0`. Capture both minimum
  free-Fast and minimum-largest-block values, not just the final `Avail`.
- Run 3 warmed-up launch/close cycles (both variants where feasible)
  and record before/after `Avail` and any persistent drift. Capture the
  generated in-memory preview separately; it is not the same workload as
  the fault-injected RAM probe.
- For a failed test, preserve the exact program output, code and WinUAE
  configuration. Mark `FAIL` or `BLOCKED` according to evidence;
  do not reduce RAM or silently skip cases.

### Acceptance record (fill from actual manual execution)

| Field | O0 | O2 |
|---|---|---|
| Tested commit / binary SHA256 | not_run | not_run |
| Return code | not_run | not_run |
| Fast start / minimum / end (bytes) | not_run | not_run |
| Largest block start / minimum / end (bytes) | not_run | not_run |
| Raster heap peak / end (bytes) | not_run | not_run |
| Allocation observations | not_run | not_run |
| Failure-injection + COW integrity | not_run | not_run |

**Gate rule:** the standalone probe can establish bounded native raster
behaviour, not peak memory usage of the whole PhotoCraft editor. A true
32-MiB **application** target acceptance additionally requires instrumented
presenter/ReAction/stack/document peak data with explicit workload and
normal+error-path cleanup evidence. If that data is unavailable, set the
application-level RAM peak gate to `not_proven`; a healthy probe alone
must not close PF-TD-001 or PF-SP-020.
