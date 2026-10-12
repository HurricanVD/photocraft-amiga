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
