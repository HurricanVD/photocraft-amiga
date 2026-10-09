# Development Process Overlay

Dieses Dokument beschreibt nur lokale Projektdetails. Gemeinsame Regeln stehen
in der gepinnten Version von `VD_PROCESS_REPO`.

## Prozess-Pin

- Prozess-Repo: `vd-amiga-dev-process`
- Prozessversion:
- Adoption: `overlay`
- Story-ID-Prefix:
- Test-ID-Prefix:
- Story-ID-Format: `PF-(PO|BG|SP|LL|PR|TD)-NNN`
- Test-ID-Format: `PF-TN-NNN`
- vxlibs-Policy: `use_ready_components`
- vxlibs-Baseline-Status: `ready_components_available`

## Lokale Quellenreihenfolge

1. `AGENTS.md`
2. `docs/project-metadata.env`
3. `docs/development-process.md`
4. `README.md`, `docs/vision.md`, `docs/roadmap.md` und
   `docs/architecture.md`
5. `docs/implementation-status.md`, `docs/adr/` und
   `docs/os-deviation-log.md`
6. gepinnte Prozessbasis `VD_PROCESS_REPO`
7. `docs/backlog.md`, `docs/stories/`, `docs/tests/` und `docs/target/`
8. reale Implementierung

## Lokale Kommandos

- Build:
- Windows-WSL2-Produktbuild:
- WSL-Distribution:
- Starter-App-Build:
- Starter-App-Build-Evidenz:
- Build-Artefakt:
- Full Check PowerShell:
- Full Check Bash/Make:
- Hosttest fuer OS-freie Logik:
- m68k-Test-Harness-Build:
- Vamos-Test:
- automatisierter WinUAE-Test: `disabled_by_process` (nicht ausfuehren)
- manueller WinUAE-/Hardwaretest:
- Test-/Build-Log-Verzeichnisse:
- Release-Build:
- Skill-/Tool-Freshness:
- Story-Commit-Konvention: `<STORY-ID>: <knappe imperative Zusammenfassung>`

## Runtime-Blocker

| Pfad | Blocker | Diagnose | Rerun-Plan |
|---|---|---|---|

## Windows/WSL2-Build-Regel

Unter Windows laufen AmigaOS/m68k-Produktbuilds in WSL2/Ubuntu. PowerShell darf
nur als Wrapper fuer `wsl.exe ... bash -lc '...'` dienen. Toolchain-Variablen
wie `VBCC` oder `GCC_BEBBO` muessen in derselben WSL-Sitzung gesetzt werden,
bevor ein Make-Ziel startet. Prozesschecks bleiben PowerShell-first, wenn
`pwsh` verfuegbar ist.

## Story- und Testprozess

- Backlog: `docs/backlog.md`
- ADRs: `docs/adr/`
- Architekturbeschreibung: `docs/architecture.md`
- README-Abschlussquelle: `README.md`
- Aktive Storys: `docs/stories/`
- Abgeschlossene Storys:
- Story-Archiv: `docs/stories/done/`
- Testnachweise: `docs/tests/`
- Test-Archiv: `docs/tests/done/`
- Regressionstest-Fallback: bei manuellem Anwendungstest `n/a|exception`
  erforderlich, automatisiert zulaessig, sofern fachlich sinnvoll
- WIP-Limit:
- Story-ID-Beispiel:
- Fastpath: `enabled|disabled|stricter_local_gate`
- Fastpath-Template: `VD_PROCESS_REPO/docs/templates/fast-lane-fix.md`
- Lokale Fastpath-Verschaerfungen:
- Architektur-Schreibscope: ADRs, Architekturbeschreibung und `arch_review`
- Dokuabschluss-Schreibscope: Architekturbeschreibung, `README.md` und weitere
  story-relevante Doku

## Dokumentationsregeln

- `implemented`, `planned` und `proposed` getrennt halten.
- Gemeinsame Regeln aus dem Prozess-Repo referenzieren, nicht duplizieren.
- Im Standardprofil vor `ready` initiales und finales `pre_ready_final`-
  Architekturreview nach dem zentralen ADR-Lifecycle; weitreichende ADRs nur
  mit Nutzerzustimmung. Ein bestandener Fastpath folgt stattdessen
  `VD_PROCESS_REPO/docs/process/fast-lane.md`.
- Vor dem finalen Freeze Architekturbeschreibung und `README.md` aktualisieren
  oder als aktuell beziehungsweise begruendet nicht anwendbar belegen. Beim
  Fastpath gilt Architektur nur nach finalem Klassifikationsaudit als
  `not_impacted`; README folgt der tatsaechlichen Wirkung.
- Wiederverwendbare Regeln als Prozess-Aenderung zurueck in
  `VD_PROCESS_REPO` fuehren.

## Lokale Abweichungen

Keine, bis hier explizit dokumentiert.

## PhotoCraft-AmigaOS-Overlay (kanonischer Bootstrap)

- VD-Prozessbasis `0.2.1` unter `../../tools/vd-amiga-dev-process`;
  exakt gepinnt, gemeinsame Regeln nicht vendored.
- `README.md`, `AGENTS.md`, `docs/roadmap.md`, `docs/architecture.md`:
  Original-PhotoCraft-Autorität. Die bestehenden Dateien bleiben erhalten.
- C99-Port unter `ports/amigaos3/`, kanonische PF-Stories/Tests
  unter `docs/stories/` bzw. `docs/tests/`; historische Portpfade
  bleiben als Nachweis erhalten.
- `PF-SP-001`: blocked (MiniGL). `PF-SP-002`: in_progress (Core).
  `PF-SP-003`: done (GCC13/vamos).
- `make -C ports/amigaos3 host-test` und `host-core-sanitize`
  sowie GCC13 HUNK/vamos und Rust-Originalvergleich in CI.
- Drift-Checks: `sh tools/check-process-version.sh .` bzw.
  `pwsh -File tools/check-process-version.ps1`.
- WinUAE-Automatisierung: `disabled_by_process`; MiniGL manuell.
- Noch kein produktiver Compiler-/API-/GUI-Release-Nachweis.
