# PF-SP-003 — GCC/Bebbo 13.3 + vamos compatibility lane

- ID: PF-SP-003
- Status: **done** — bounded toolchain/test spike formally reviewed and archived 2026-10-09, all scoped runtime/CI gates passed
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

GitHub Actions #37915607402 confirms a successful GCC13.3 m68k/vamos run at O0 and O2 and byte-identical Rust reference fixtures. Technical evidence is complete for this spike; formal VD review/closure was completed on 2026-10-09.
Runtime comparison in vamos does not verify real PiStorm3D, QuarkTex NG, FPU or RTG.

## First CI test finding (2026-10-09)

Run 37910690849 built all 6 HUNK binaries successfully and installed pinned vamos, but stopped before the first test due to its own 16 MiB machine68k memory-map configuration (`Too much RAM allocated with hw access enabled`). This is an emulator setup defect, not an observed application failure. The runtime runner now requests 8 MiB; after a bounded boundary-matrix test change, target evidence passed on run 37915607402.

## Runtime-Befund und gezielte Testreparatur (2026-10-09)

Bei 8 MiB `vamos`-Speicher beanspruchte der monolithische Boundary-Test mit
13 x 13 Koordinaten 36 verschiedene Tiles, d.h. 9 MiB reine Pixelpuffer.
Die Reparatur ändert **nur die Lebensdauer der Test-Surfaces**, nicht die
PhotoCraft-Kernimplementierung: 13 Testzeilen mit je 13 Schreib-/Lesepaaren,
sechs X-Tiles pro Zeile, Freigabe nach jeder Zeile. Alle ursprünglichen 169
Koordinatenpaare werden weiterhin geprüft. Das ist ein Memory-Budget-Fix der
Regressionstests, keine Anpassung des Produktcodes an Testfehler.

## Validation accepted for technical spike; formal closure pending (2026-10-09)

[CI #37915607402](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37915607402) on code commit `622cfba` shows all three jobs green. GCC/Bebbo 13.3.0 generated six genuine HUNK files; all six executed under pinned `vamos` successfully, and both C core oracles (`-O0`, `-O2`) matched the original PhotoCraft Rust outputs byte-for-byte. Complete compiler and image provenance, sizes and logs: [PF-TN-003](../../tests/done/PF-TN-003.md).

At the 2026-10-09 test-evidence snapshot this was in `review`; see the later documented final review and closure record. No runtime performance or MiniGL/WinUAE capability follows from this spike.

## Closure audit preparation (2026-10-09)

An independent closure scope review and a `change_summary` are recorded under `../reviews/`. This is a **toolchain/test-only spike** with automated native target smoke, no ReAction/MiniGL integration, no shipping artefact, and no proprietary code import. Manual application test is `n/a` for this isolated headless executable, justified by the six automated vamos regressions and original Rust differential checks. The fork's canonical full bootstrap and PF prefix activation remain separate process follow-ups; the accepted architectural direction is unchanged.

## Final closure — 2026-10-09

- Status: `done`; technical test-only acceptance **approved** (see [final review](../../reviews/PF-SP-003-final-review.md) and [change summary](../../reviews/PF-SP-003-change-summary.md)).
- Test report: [PF-TN-003](../../tests/done/PF-TN-003.md), archived with the Story.
- Code acceptance: [CI #37915607402](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37915607402) fully green on `622cfbae`; documentation CI [#37916110300](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37916110300) fully green on `9474e7f`.
- All six HUNK executables and all six vamos invocations passed (core, staging, original-Rust oracle in O0/O2), including 169 boundary combinations under bounded RAM.
- Manual application test: `n/a` — no graphical application or OS UI was modified; actual 68k HUNK runtime under `vamos` and differential Rust regression replace it within this limited spike, **not** for MiniGL.
- Final review deliberately records a historical deviation: centralized repo bootstrap and the original pre-ready chronological gate were not completed before the first code commit; the closure-time architectural audit is not backdated.
- No release/production toolchain migration authorized. GCC13 stays an opt-in test profile; `vxplatform` GCC16 default unaffected.
- The Story and its PF-TN test report moved into the port-local `done/` archive. Subsequent work belongs to PF-SP-001/PF-SP-002 or new formal Stories.
