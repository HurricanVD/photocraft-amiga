# PhotoCraft AmigaOS 3.2 — Canonical VD bootstrap evidence report

- Projekt: `HurricanVD/photocraft-amiga`
- Datum: 2026-10-09
- Prozess-Repo: `HurricanVD/vd-amiga-dev-process`
- Prozessversion: `0.2.1` (exact pinned shared baseline, main VERSION previously verified)
- Bootstrap-Profil: `process-overlay`
- Ausfuehrender: Repository integration via connected GitHub; **official local PowerShell/Bash bootstrap script not executed**
- Bootstrap status: **materialized_with_review_followups**, not claimed as a successfully executed official tool run.
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
- Registry-Status nach Materialisierung: `reserved` (unchanged)
- Registry-Transition: `n/a`
- Prefix-Aktivierung: `follow_up_required`
- Aktivierungs-Evidenz: project-metadata, bootstrap-report, canonical paths without prefix placeholders, reserved id-range; **formal process review outstanding**
- Registry-Update-Owner: `HurricanVD/vd-amiga-dev-process` maintainers/owner
- Prefix-Aktivierung-Naechster-Schritt: official bootstrap smoke, drift checks and evidence review; only then separately activate `PF`.
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

- Materialisierung: `canonical_materialized` for existing PF-SP-001/002/003; future PO stories remain seed-only.
- Naechste Rolle: `Product Owner/Refinement`

| Story | Titel | Typ | Status | Quelle |
|---|---|---|---|---|
| PF-SP-001 | MiniGL/QuarkTex NG validation | SP | blocked | `docs/stories/PF-SP-001.md` |
| PF-SP-002 | PhotoCraft native core parity | SP | in_progress | `docs/stories/PF-SP-002.md` |
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

- Prozessversion-Drift-Check PowerShell: `not_run`
- Prozessversion-Drift-Check POSIX: `not_run`
- Anwendbare lokale Checks: canonical file presence, PF ID/registry match, source-preservation, CI host/Rust and HUNK/vamos tests
- Check-Runtime-Blocker: local sibling process checkout and PowerShell/WSL2 workspace not accessible through GitHub connector runtime
- Check-Failure: `not_assessed` for official drift/full check (must not infer PASS)
- Check-Rerun-Plan: check out exact VD `0.2.1` version into sibling workspace `tools/vd-amiga-dev-process`; run official `tools/bootstrap-repo.sh` without `--force` or `--activate-prefix` and run POSIX/PowerShell drift checks; examine report changes before further commit.
- Available CI evidence: [AmigaOS core green run](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37920223129); main repository CI/Linux/Windows separate.

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

- Official bootstrap script run + generated output comparison, POSIX/PowerShell drift checks, final bootstrap architecture/process review before PF activation.
- Real AmigaOS/ReAction/RTG/MiniGL/WinUAE/QuarkTex/PiStorm3D tests and product compiler choice are **not** implied by this repository bootstrap.
- Canonical stories/tests are the primary status records; port-local originals remain historical copies until a later link-drift cleanup.
