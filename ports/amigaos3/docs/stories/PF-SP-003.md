# PF-SP-003 — GCC/Bebbo 13.3 + vamos compatibility lane

- ID: PF-SP-003
- Status: in_progress; acceptance requires green m68k/vamos CI evidence
- Date: 2026-10-09
- Scope: port-local toolchain and test infrastructure only; no production-toolchain change
- ADR: ADR-0001 accepted; scoped reference: HurricanVD/vxplatform ADR-0014 (gcc13 opt-in)
- Test: PF-TN-003
- Owner request: "können wir die tests mit der bebbo-Toolchain mit gcc 13 und vamos erweitern?"
- Clarification: vxplatform already has an explicit GCC13.3 provider profile, but GCC16 remains its default.

## Acceptance

1. GCC_BEBBO_ROOT must be an absolute explicit path, GCC + G++ exactly 13.3.0 for m68k-amigaos, libgcc in the same root. Version or target mismatch must fail before compile.
2. Build isolated genuine Amiga HUNK executables at -O0 and -O2 with -m68020 -msoft-float -fno-omit-frame-pointer -noixemul: core test, tile conversion test, and C oracle.
3. Verify HUNK_HEADER (0x3F3) on each binary.
4. Run all six test binaries under pinned amitools/vamos and machine68k; exit status and expected PASS lines checked.
5. Compare the m68k C oracle from *both* optimization levels byte-for-byte with the output of original PhotoCraft Rust crates from the previous CI job.
6. Record image digest, toolchain version, commands and log artifacts; do not claim success for unexecuted runs.
7. Keep host C99+sanitizers and original Rust comparison unchanged. Do not require MiniGL SDK or WinUAE for this lane.

## Source references and constraints

- vxplatform ADR-0014: dedicated gcc13.3 opt-in root and isolated variant; no production adoption.
- VD process toolchain codegen registry: GCC13 CG-006/CG-007 are C++ caveats, not assumed fixes for C99.
- Docker GCC13 tag is `stefanreinauer/amiga-gcc:gcc-v13.3-20260622` (CI logs image digest); exact upstream version gate is independent of image tag.
- Native vamos machine68k and amitools are pinned to the same revisions as the central VD toolchain's gcc6 lane. Greenlet is resolved from PyPI and its installed version must be visible in the CI log for release-grade repeatability.
- Original Rust, C99 core, and MiniGL experiments remain separate layers.
- No code copied from private vxplatform or Dunkelkammer; only documented build patterns.

## Pending

Until GitHub Actions job `GCC13.3 m68k HUNK + vamos vs Rust` is confirmed green and logs reviewed, status stays `in_progress`.
Runtime comparison in vamos does not verify real PiStorm3D, QuarkTex NG, FPU or RTG.

## First CI test finding (2026-10-09)

Run 37910690849 built all 6 HUNK binaries successfully and installed pinned vamos, but stopped before the first test due to its own 16 MiB machine68k memory-map configuration (`Too much RAM allocated with hw access enabled`). This is an emulator setup defect, not an observed application failure. The runtime runner now requests 8 MiB; target evidence remains pending.
