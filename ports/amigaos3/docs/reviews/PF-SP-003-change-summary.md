# PF-SP-003 — change_summary

- Datum: 2026-10-09
- Story-ID: `PF-SP-003`; type `SP`; entry profile `standard`
- Status: `reviewed/closed` after final review, closure technical scope only
- Scope: `amigaos32` port-local GCC13/vamos test lane and memory-bounded test harness
- Original source change: C99 test under `ports/amigaos3/tests/pc_core_test.c`; core implementation unchanged
- Initial implementation commits: `8bc3637` (toolchain lane), `c6ea8f2` (8-MiB setup), `622cfba` (bounded boundary matrix)
- Evidence docs: `PF-TN-003.md`, `PF-SP-003.md`, architecture review and port-local README
- Toolchain setup: GCC/Bebbo 13.3.0 with exact provider-root checks, `stefanreinauer/amiga-gcc@sha256:f9d09422a89f317a2f59d5db46227ad9bd8d753d5c9fe41553fcfdc6ecf60ce8`
- Build: 3 C99 programs × `-O0`/`-O2` → 6 genuine HUNK executables; `-m68020 -msoft-float -noixemul`
- Runtime: `vamos -S -C 20 -m 8192 -s 128` for all six; results PASS and Rust-original oracle outputs identical for O0/O2.
- Executed full test workflow: [#37915607402](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37915607402) — three jobs `success` on code commit `622cfba`.
- Product change: **none**, only portability/infrastructure evidence. GCC13 compatibility lane is opt-in, not an app release policy.
- Smallest sufficient root-cause fix: retain all 169 boundary coordinate pairs, reduce live tile working set to one six-tile row. No increase in emulator memory, no production code logic changed.
- Regression evidence: host GCC C99/ASan+UBSan, original Rust library tests and differential C oracle, GCC13 HUNK + vamos O0/O2.
- Manual application test: `n/a` for noninteractive CLI-only smoke (no user-facing app); accepted substitute = real emulated target regressions plus Rust oracle.
- IP/provenance impact: `reference_only`; only external toolchain/documentation references, no copied proprietary code. Licensing for optional MiniGL and Dunkelkammer remains separate release gate.
- Architecture: accepted ADR-0001 unchanged materially; metadata-only verification update, local `docs/architecture.md` and port README synchronized.
- OS deviation: unchanged; gcc13 used opt-in for test only, no new platform target decision.
- Changelog: port-local `CHANGELOG.md` summarizes bounded technical milestone, no product version bump.
- Check command: [AmigaOS host GitHub Actions](https://github.com/HurricanVD/photocraft-amiga/actions/workflows/amigaos3-host.yml) (CI includes Bash/Make, Rust and m68k/vamos; no PowerShell logic added).
- Central WinUAE automation: `disabled_by_process` (not used).
- Future concerns: process bootstrap/prefix activation, normal pre-ready gate on future app features, MiniGL/QuarkTex NG and PiStorm3D target evidence, format-generic raster and document engine.
- Candidate commit closure: isolate story/review/test/archive/docs changes to `ports/amigaos3/` and ADR verification metadata; no upstream/`main` mutation.
- Reference: `HurricanVD/vd-amiga-dev-process/docs/templates/change-summary.md` v0.2.1.

## Closure evidence

Final reviewer: [PF-SP-003-final-review.md](PF-SP-003-final-review.md) (recommendation approve, no open High/Medium findings). Archived story/test links: [PF-SP-003](../stories/done/PF-SP-003.md), [PF-TN-003](../tests/done/PF-TN-003.md). Central bootstrap and PF prefix `active` migration remain separate follow-up work. No product release/marketing approval.
