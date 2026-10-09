# PhotoCraft bootstrap remediation – F-01 to F-03

- Review date: 2026-10-10
- Consumer: `HurricanVD/photocraft-amiga`, base `main` / `2a06a51f4b1b152b3be2d46473ccd6a035fbae7c`
- Process source: `HurricanVD/vd-amiga-dev-process`, version `0.2.1`, pinned commit `0566a39c28ee73285463f33eb72b210b5d215db4`.
- Review type: remediation evidence / **not a final independent approval**.
- CI execution result: pinned private process [run #38004848964](https://github.com/HurricanVD/vd-amiga-dev-process/actions/runs/38004848964) `completed/success` on consumer `43cea705`, process baseline `0566a39`. Decision: **technical bootstrap checks PASS; final independent review pending**, PF prefix remains `reserved`.

## Findings addressed

| Finding | Remediation | Status |
|---|---|---|
| F-01: incomplete local process overlay | Fill in documented pin, PF prefixes, build/check commands, targets, story/archive status and runtime blockers in `docs/development-process.md`; do not claim unverified AmigaOS build/GUI/ABI. | Documentation corrected; independent review pending |
| F-02: stray `VC` ID examples | Replace examples in active and archived backlog and bootstrap seed with clearly non-allocated `PF` examples; preserve PF-SP-001 `blocked`, PF-SP-002 `in_progress`, PF-SP-003 `done`. | Documentation corrected |
| F-03: unexecuted official bootstrap and drift checks | Add `tools/check-bootstrap-evidence.sh`; public fork Actions performs `--static-only` preflight. The private `HurricanVD/vd-amiga-dev-process` Actions job checks out the exact public fork commit and runs Bash/PowerShell drift, exhaustive fixtures, process full check and disposable official bootstrap smoke. | **Private CI checks PASS; independent review pending** |

## Evidence requirements

- **Do not treat PhotoCraft product CI or the public static docs preflight as a process bootstrap pass.** The separate private process-repository Actions job #38004848964 completed successfully against consumer `43cea705` and exact VD baseline `0566a39`.
- Verified in the private job logs for run #38004848964: `STATIC_BOOTSTRAP_DOCS=PASS`, `POSIX_DRIFT=PASS`, `POWERSHELL_DRIFT=PASS`, `POWERSHELL_BOOTSTRAP_FIXTURE=PASS`, `BOOTSTRAP_FIXTURE=PASS`, `PROCESS_FULL_CHECK=PASS`, `CONSUMER_INPUT_SMOKE=PASS`, `PF_BOOTSTRAP_VALIDATION=PASS`. Each marker was present.
- Private process sources, workflow logs and fixtures must not be published into the public fork.
- The official bootstrap script must **not** run with `--force` or `--activate-prefix` on the actual consumer. The smoke runs on a temporary input snapshot and does not overwrite the real PhotoCraft files.
- If a runtime/toolchain/service prevents execution, classify it as `runtime_blocked`, not PASS; include an exact rerun plan in `docs/bootstrap-report.md`.
- The recorded CI run and commands are now in `docs/bootstrap-report.md`. Next: re-check the final PR head, independent architecture/process evidence review, then submit the separate `PF reserved -> active` registry change. This remediation does not itself activate PF.

## Out of scope

The PF process gate does not release a native AmigaOS editor, MiniGL or WinUAE target runtime, production compiler, shipping ABI, or proprietary third-party code. Nothing here changes `storytold/photocraft`.

## Evidence note (2026-10-10)

- Exact private Actions workflow: [PF Consumer Bootstrap – Private VD Gates #38004848964](https://github.com/HurricanVD/vd-amiga-dev-process/actions/runs/38004848964).
- Result: `completed/success`, official disposable bootstrap smoke and both Bash/PowerShell drift / exhaustive fixtures PASS. This proves the process checks on a complete GitHub Actions checkout, not a local WSL2/NDK/QuarkTex target runtime.
- Public fork [PF Bootstrap Docs Preflight #38004817978](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38004817978): `completed/success`.
- No independent final approval was issued by this implementer. PF stays reserved until separate review/registry change.
