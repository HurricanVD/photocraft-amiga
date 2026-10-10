# PhotoCraft AmigaOS 3.2 — Canonical VD bootstrap evidence report

- Projekt: `HurricanVD/photocraft-amiga`
- Datum: 2026-10-09
- Prozess-Repo: `HurricanVD/vd-amiga-dev-process`
- Prozessversion: `0.2.1` (exact pinned shared baseline, main VERSION previously verified)
- Bootstrap-Profil: `process-overlay`
- Ausfuehrender: Repository integration via connected GitHub. Official bootstrap smoke on a disposable four-input seed and a non-destructive official bootstrap on a complete temporary consumer Git snapshot in private process CI. No official in-place bootstrap run over the existing PhotoCraft worktree.
- Bootstrap status: **materialized / isolated official smoke + full consumer snapshot and POSIX/PowerShell gates PASS; PF registry active via separate owner-authorized process PR #19**. The two tailored `process-overlay` exceptions have been explicitly accepted by the owner; no independent native reviewer subagent was used.
- Workstream and IP boundary: public PhotoCraft fork, no proprietary Dunkelkammer or private VD process sources vendored.

## Eingangsdokumente

| Pfad | Rolle | Status | Notiz |
|---|---|---|---|
| `README.md` | Produktuebersicht | present | Original PhotoCraft plus isolated Amiga port note, unchanged by bootstrap |
| `docs/vision.md` | Projekt-/Port-Vision | present | additive fork-local vision |
| `docs/roadmap.md` | Original-Roadmap | present | upstream authoritative, not overwritten |
| `docs/architecture.md` | Original-Architektur | present | upstream authoritative, not overwritten |
| `ports/amigaos3/docs/architecture.md` | native Port-Architektur | present | ADR-0001..0003 / PF-SP-002 evidence |

## Erzeugte und gepruefte Pfade

| Pfad | Quelle | Notiz |
|---|---|---|
| `docs/project-metadata.env` | VD-v0.2.1 template + explicit port metadata | GCC_BEBBO, 68040+, 32 MiB, PF/PF |
| `docs/development-process.md` | VD-v0.2.1 template + port overlay | original PhotoCraft architecture preserved |
| `docs/implementation-status.md` | VD template + scoped evidence | no full engine / GUI claim |
| `docs/backlog.md`, `docs/backlog-done.md` | VD templates | current and archived stories |
| `docs/stories/`, `docs/tests/` and `done/` | canonical copies of PF records | no duplicates of story/test IDs allocated |
| `docs/bootstrap/first-phase-backlog.md`, `first-phase-story-drafts.md` | VD templates + fork first phase | seed only, no new ready product stories |
| `tools/check-process-version.sh`, `tools/check-process-version.ps1` | VD-v0.2.1 templates | execution not yet verified in target workspace |
| `docs/os-deviation-log.md`, `docs/style.md`, `CHANGELOG.md` | VD templates with fork-specific overlay | original root Rust/PhotoCraft sources retained |

## Prefixe und ID-Bereiche

- Story-ID-Prefix: `PF`
- Test-ID-Prefix: `PF`
- Registry-Status vor Bootstrap: `reserved`
- Registry-Status nach Materialisierung: `reserved` (historical initial bootstrap state)
- Aktueller Registry-Status: `active` (2026-10-10, process repo merge `1c32299`)
- Registry-Transition: `reserved -> active` (separately reviewed process PR [#19](https://github.com/HurricanVD/vd-amiga-dev-process/pull/19), merge [`1c32299`](https://github.com/HurricanVD/vd-amiga-dev-process/commit/1c32299deeb1b120703769f71ee0ae427370cc48))
- Prefix-Aktivierung: `done` (central PF registry entry now `active`)
- Aktivierungs-Evidenz: project-metadata, this bootstrap-report, canonical PF story/test paths without placeholders, reserved ID range, final-head private [Bootstrap gate #38007610665](https://github.com/HurricanVD/vd-amiga-dev-process/actions/runs/38007610665) (success on merged consumer PR head `394134731` and process 0.2.1 pin `0566a39`), [PhotoCraft main CI #38009180757](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38009180757) success, owner acceptance of tailored overlay exceptions, and central [PF activation PR #19](https://github.com/HurricanVD/vd-amiga-dev-process/pull/19) merged.
- Registry-Update-Owner: `HurricanVD/vd-amiga-dev-process` maintainers/owner
- Prefix-Aktivierung-Naechster-Schritt: `n/a` — registry transition is completed; revisit the two `process-overlay` exceptions on a future VD process pin upgrade or changed Bootstrap/ADR templates.
- Story-ID-Format: `PF-(PO|BG|SP|LL|PR|TD)-NNN`
- Story-ID-Beispiel: `PF-SP-001`
- Test-ID-Format: `PF-TN-NNN`
- Erster Story-ID-Bereich: `PF-*-001` bis `PF-*-899`
- Erster Test-ID-Bereich: `PF-TN-001` bis `PF-TN-899`

## OS-/Architektur-Abweichungen

- Abweichungsstatus: `dokumentiert`
- Abweichungslog: `docs/os-deviation-log.md` and `ports/amigaos3/docs/os-deviation-log.md`.
- Naechster Schritt: confirm platform APIs and full ReAction/MiniGL/RTG fallback under manual target test.

## First-Phase-Seed

- Materialisierung: Initial PF-SP-001/002/003 kanonisch. Seit 2026-10-11 PF-SP-002 `blocked` (split_required), PF-SP-004..009 und PF-TD-001 `refining|draft` im Backlog; keine DoR-Freigabe.
- Naechste Rolle: `Product Owner/Refinement`

| Story | Titel | Typ | Status | Quelle |
|---|---|---|---|---|
| PF-SP-001 | MiniGL/QuarkTex NG validation | SP | blocked | `docs/stories/PF-SP-001.md` |
| PF-SP-002 | PhotoCraft native core parity (historischer Parent) | SP | blocked (`split_required`, 2026-10-11) | `docs/stories/PF-SP-002.md` |
| PF-SP-003 | GCC13 HUNK/vamos + Rust oracle | SP | done | `docs/stories/done/PF-SP-003.md` |

## ADR-Entscheidungen

| ADR | Zielpfad | Thema | Status | Lifecycle-Aktion | Revisit-Trigger | Naechste Story |
|---|---|---|---|---|---|---|
| ADR-0001 | `docs/adr/ADR-0001-amigaos3-port-seam.md` | OS port boundary | accepted | accept | native UI integration | PF-SP-001 |
| ADR-0002 | `docs/adr/ADR-0002-photocraft-core-and-memory-contract.md` | sparse COW / memory architecture | accepted | accept | stable ABI / low-memory model | PF-SP-002 |
| ADR-0003 | `docs/adr/ADR-0003-amigaos-toolchain-and-verification.md` | compiler and verification profile separation | accepted | accept | native product compiler selection | next implementation story |

## Starterprofil

- Entscheidung: `n/a` — process-overlay instead of generated ReAction starter (pre-existing Rust application workspace).
- NDK-Pfad: `not_present_in_this_remote_tool_runtime`
- NDK-Include-H-Pfad: `not_present_in_this_remote_tool_runtime`
- PRIMARY_TOOLCHAIN: `GCC_BEBBO`
- VBCC-Pfad: `n/a`
- GCC_BEBBO-Pfad: `CI container profile, not a local NDK SDK install`
- Starter-App-Profil: `process-overlay`
- Starter-App-Template-Quelle: `VD_PROCESS_REPO/docs/templates/starter-app` (reference only)
- Starter-App-Zielartefakt: `n/a`
- `vxlibs`-Policy: `use_ready_components`
- `vxlibs`-Baseline-Status: `ready_components_available`
- `vxlibs`-Entscheidung: `spike_required` for a new shared helper dependency
- Blocker: target SDK/Workbench/QuarkTex runtime remains unverified

## Materialisierte Starter-App-Pfade

| Zielpfad | Quelle/Template | Status | Notiz |
|---|---|---|---|
| `ports/amigaos3` | fork port-local scaffold | existing | independent source/test code; no generated ReAction app |

## Lokale Check-Evidenz

- Prozessversion-Drift-Check PowerShell: `pass` (GitHub Actions Ubuntu/pwsh, pinned process baseline; not evidence for the user's WSL2 workstation)
- Prozessversion-Drift-Check POSIX: `pass` (GitHub Actions Ubuntu, pinned process baseline)
- Anwendbare lokale Checks: canonical PF documents/ID and reserved registry, POSIX and PowerShell process version drift, PowerShell bootstrap fixture, exhaustive POSIX bootstrap fixture, shared process `make check`, official disposable consumer-input bootstrap smoke. Existing Rust/C99/HUNK/vamos CI remains separate product evidence.
- Check-Runtime-Blocker: `n/a` for the validated private GitHub Actions environment; local workstation/WSL2, NDK and MiniGL remain unverified.
- Check-Failure: `n/a` for the merged consumer PR head `394134731` in private run #38007610665 (`completed/success`) and main consumer CI #38009180757 (success). New changes still require new head-specific evidence.
- Check-Rerun-Plan: in the *private* process repository, check out process baseline `0566a39c28ee73285463f33eb72b210b5d215db4` and exact public consumer review SHA; run the private process-owned `tools/check-photocraft-bootstrap.sh` rather than executing public validation scripts. Only pinned byte-matched VD drift templates are used. Never use `--force` or `--activate-prefix` on the real PhotoCraft worktree.
- Available process CI evidence: [private PF gate #38007610665](https://github.com/HurricanVD/vd-amiga-dev-process/actions/runs/38007610665) successful for merged consumer PR head `394134731` against pinned VD `0566a39` (0.2.1). `STATIC_BOOTSTRAP_DOCS`, `POSIX_DRIFT`, `POWERSHELL_DRIFT`, `POWERSHELL_BOOTSTRAP_FIXTURE`, `BOOTSTRAP_FIXTURE`, `PROCESS_FULL_CHECK`, `CONSUMER_INPUT_SMOKE`, `FULL_CONSUMER_SNAPSHOT`, and `PF_BOOTSTRAP_VALIDATION` all `PASS`. Consumer [main CI #38009180757](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38009180757) success. Registry PR #19 central [process check #38030964284](https://github.com/HurricanVD/vd-amiga-dev-process/actions/runs/38030964284) success. Product [AmigaOS core test #37920223129](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37920223129) is separate evidence.

## Starter-App-Build-Evidenz

- Build-Kommando: `n/a`
- Build-Artefakt: `n/a`
- Build-Ergebnis: `not_run`
- Build-Log: `n/a`
- Runtime-Blocker: `n/a — no starter source chosen`
- Rerun-Plan: target MiniGL/Workbench product separate
- Starter-Deaktivierung: `process-overlay`
- Not-Run-Begruendung: `process-overlay`

## Offene Annahmen und Nacharbeiten

- Official VD bootstrap was tested on the four-input fixture **and** on a complete temporary Git checkout of consumer `394134731`, with `--force`/`--activate-prefix` disabled. The full snapshot changed only `docs/bootstrap-report.md` and introduced `docs/adr/ADR-0001-bootstrap-baseline.md`; original README/architecture, accepted `ADR-0001..0003`, canonical PF records, overlays and backlog remained unchanged. These two differences were accepted as **scoped PhotoCraft process-overlay exceptions** in the owner-authorized PF activation: the templated report would replace tailored CI/acceptance evidence, and a second generic ADR-0001 would conflict with the accepted port ADR-0001. The actual checkout was not overwritten. The reviewer separation was a documented sequential fallback; no independent human or native subagent review is claimed. Revisit these exceptions upon migration of VD process version or changed bootstrap/ADR templates. New code revisions still require fresh validation; native AmigaOS/product claims remain excluded.
- Real AmigaOS/ReAction/RTG/MiniGL/WinUAE/QuarkTex/PiStorm3D tests and product compiler choice are **not** implied by this repository bootstrap.
- Canonical stories/tests are the primary status records; port-local originals remain historical copies until a later link-drift cleanup.
