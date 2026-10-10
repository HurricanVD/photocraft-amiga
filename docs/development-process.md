# Development Process Overlay

Dieses Dokument beschreibt nur lokale Projektdetails. Gemeinsame Regeln stehen
in der gepinnten Version von `VD_PROCESS_REPO`.

## Prozess-Pin

- Prozess-Repo: `vd-amiga-dev-process`
- Prozessversion: `0.2.1` (Pin in `docs/project-metadata.env`; kein automatisches Update)
- Adoption: `overlay`
- Story-ID-Prefix: `PF` (zentral `active` seit Prozess-Registry-Merge PR #19 am 2026-10-10)
- Test-ID-Prefix: `PF`
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

- Build: `make -C ports/amigaos3 host-test` (C99-Test-Build, kein natives Editor-Artefakt)
- Windows-WSL2-Produktbuild: `n/a` (kein freigegebener Produktbuild; m68k-Make nur in WSL2/Ubuntu mit Toolchain-Variablen derselben Session)
- WSL-Distribution: `Ubuntu` (VD-Standard; lokale Installation nicht verifiziert)
- Starter-App-Build: `not_run` (`STARTER_APP_PROFILE=process-overlay`; kein generierter Starter)
- Starter-App-Build-Evidenz: `docs/bootstrap-report.md` (Starter-App-Build-Ergebnis `not_run`)
- Build-Artefakt: `n/a` (keine produktive AmigaOS-Binärdatei; C99-Testausgaben unter `ports/amigaos3/build/`)
- Full Check PowerShell: `pwsh -NoProfile -File tools/check-process-version.ps1 -RepoRoot .` (Prozess-Pin-Drift-Check; kein App-Full-Check definiert)
- Full Check Bash/Make: `sh tools/check-process-version.sh .` (Pin); `make -C ../../tools/vd-amiga-dev-process check` (separater Full Check der Prozessbasis)
- Hosttest fuer OS-freie Logik: `make -C ports/amigaos3 host-test` und `make -C ports/amigaos3 host-core-sanitize`
- m68k-Test-Harness-Build: `make -C ports/amigaos3 gcc13-build-tests GCC_BEBBO_ROOT=<lokaler-GCC13.3-Pfad>` (optionales Testprofil; kein Produktcompiler)
- Vamos-Test: `make -C ports/amigaos3 gcc13-vamos-tests RUST_ORACLE=<Original-Rust-Oracle-Datei>` (GCC13-Testprofil, siehe PF-TN-003)
- automatisierter WinUAE-Test: `disabled_by_process` (nicht ausfuehren)
- manueller WinUAE-/Hardwaretest: `make -C ports/amigaos3 amiga-preflight` und `amiga-smoke` mit externem MiniGL-SDK, danach manuelle GL-01..04 unter WinUAE; `PF-TN-001` noch blocked
- Test-/Build-Log-Verzeichnisse: `ports/amigaos3/build/` (lokal), `docs/tests/` und `docs/tests/done/` (kanonische Evidenz), GitHub Actions (CI-Logs)
- Release-Build: `n/a` (kein nativer PhotoCraft-AmigaOS-Release freigegeben)
- Skill-/Tool-Freshness: `sh ../../tools/vd-amiga-dev-process/tools/check-skill-freshness.sh` (nur mit Prozesscheckout; Ausführung separat nachweisen)
- Story-Commit-Konvention: `<STORY-ID>: <knappe imperative Zusammenfassung>`

## Runtime-Blocker

| Pfad | Blocker | Diagnose | Rerun-Plan |
|---|---|---|
| Prozess-Pin (lokal) | Checkout `../../tools/vd-amiga-dev-process` nicht nachgewiesen | Remote-VERSION `0.2.1` stimmt mit Metadata-Pin überein; kein lokaler Drift-PASS | Checkout herstellen und POSIX-/PowerShell-Drift-Checks ausführen |
| AmigaOS-Grafiktest | Offizielles MiniGL-SDK und WinUAE-/QuarkTex-Runtime nicht nachgewiesen | PF-SP-001 `blocked` | SDK/QuarkTex installieren; WinUAE GL-01..04 manuell belegen |
| Produktcompiler/ABI | Keine Freigabe als Produktcompiler/ABI | GCC13.3-/vamos-Erfolg belegt ausschließlich Testprofil | Compiler/ABI getrennt im Architektur-/Release-Gate entscheiden |

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
- Abgeschlossene Storys: `PF-SP-003` (Nachweis `docs/stories/done/PF-SP-003.md`)
- Story-Archiv: `docs/stories/done/`
- Testnachweise: `docs/tests/`
- Test-Archiv: `docs/tests/done/`
- Regressionstest-Fallback: bei manuellem Anwendungstest `n/a|exception`
  erforderlich, automatisiert zulaessig, sofern fachlich sinnvoll
- WIP-Limit: `n/a` (kein lokaler Grenzwert beschlossen; zentraler Prozess bleibt maßgeblich)
- Story-ID-Beispiel: `PF-SP-001` (existiert); `PF-PO-001` nur Formatbeispiel, keine vergebene Story
- Fastpath: `enabled` (ausschließlich nach vollständiger VD-Fast-Lane-Klassifikation; nicht für Architektur-/IP-Arbeit)
- Fastpath-Template: `VD_PROCESS_REPO/docs/templates/fast-lane-fix.md`
- Lokale Fastpath-Verschaerfungen: `keine` (VD-Baseline gilt unverändert)
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

Dokumentiert in `docs/os-deviation-log.md` als `PF-OS-001` (Host/HUNK vor
ReAction-Integration) und `PF-OS-002` (GCC13 nur Testprofil);
portlokales Register: `ports/amigaos3/docs/os-deviation-log.md`.

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
