# PF-SP-020 — Autarke native MVP-End-to-End-Abnahme

- Story-ID: `PF-SP-020`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `ready` (2026-10-12 DoR, keine Implementation)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-12
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-024` (geplant / `not_run`)
- Review: `docs/reviews/PF-all-open-ready-review-2026-10-12.md`

## 1. Problem / Nutzerwert

Als Nutzer/Maintainer moechte ich PhotoCraft ohne Host und Netzwerk auf AmigaOS 3.2 starten, bearbeiten und exportieren, damit eine vollstaendig autarke PhotoCraft-Amiga-Version entsteht, die den originalen Rust-Upstream nachweisbar respektiert.

## 2. In Scope

- Ein selbststaendiges m68k-HUNK-Programm per Workbench und Shell starten; ReAction-Fenster zeigt lokale PNG-Datei und Ebenen an.
- Maus-Stroke -> genau ein History-Commit -> Undo/Redo -> lokaler Flatten-PNG-Export; Neustart testet Export-Datei.
- Produktintegration ohne MiniGL- oder Remote-Rust-Abhaengigkeit; Versionsstring, Ziel-OS, Kernel/RTG, RTG-Ausfallmeldungen, RAM-Messung und reproduzierbarer manueller WinUAE-Test.

## 3. Nicht-Ziele

- Keine vollständige Photoshop-Funktionsparitaet, kein finales Release, keine Version mit verpflichtendem QuarkTex/MiniGL.
- Keine Erweiterung der originalen Rust-Crates und keine Remote-/Cloud-Pflicht.
- `ready` ist **keine** Implementierungs-, Release- oder WinUAE-Freigabe.

## 4. Akzeptanzkriterien

1. HUNK binary/Compiler/SHA/target CPU/Fast-RAM aufgenommen und Screenshot + Output-PNG pixelvalidiert.
2. Start/close/reopen, fehlende NDK-Gadgets/RTG, OOM, Fehler nach Quell-PNG ohne Leak.
3. Ausdruecklicher Nachweis, dass nur die AmigaOS-Umgebung aktiv und kein Host/Netz erforderlich ist.
4. Ungueltige Daten/OOM/Fehlerpfade liefern kontrollierte Ergebnisse; alle genutzten Datenpuffer und OS-Libraries werden trotz Fehler vollstaendig freigegeben. 32 MiB Fast RAM bleibt gepruefte Testbaseline, keine unbelegte Minimumspec.
5. Eigene C99-Algorithmen bleiben durch originale Rust-Semantik und benannte Oracles differenziert; WinUAE/GUI-Status wird nur nach echter manueller Zielausfuehrung PASS.

## 5. Architektur / Betroffene Module

- ports/amigaos3/ Makefile, app entry, native UI/core facade, packaging manifest; existing approved core and UI modules.
- Gate-Quellen: akzeptierte ADR-0001/0002/0003, [Port-Architektur](../../ports/amigaos3/docs/architecture.md) und [Pre-ready-Review](../reviews/PF-all-open-ready-review-2026-10-12.md).
- Capability: VD-CAP-WB-001 / VD-CAP-WIN-001 / VD-CAP-BUILD-001; AmigaOS/NDK 3.2 und GCC/Bebbo Testprofil gem. ADR-0003; reale Produktcompilerfreigabe getrennt.
- IP-Provenienz: `mixed`. Dunkelkammer-Eigenanteile sind MIT (PR #9, Lizenzhead `70897e91`), fremde Quellen/Assets unter eigener Lizenz. Alle direkt importierten Dateien benoetigen SHA/Copyright/Notice im Portregister.
- Implementierungsabhaengigkeiten: PF-SP-007/008/010/011/013/014/015/017/018/019 sowie PF-TD-001 durch nachgewiesene Ergebnisse; wenn unvollstaendig: Implementation-Gate blockiert.

## 6. Testplanung

- [PF-TN-024](../tests/PF-TN-024.md): Testfaelle und Negativpfade vor Umsetzung, `not_run`.
- Reproduzierbare Host-/ASan-/UBSan- und original Rust-Differential-Tests bei CPU/Modell, genuine m68k HUNK/vamos O0/O2; zusaetzlich manuelle WinUAE-ReAction-/RTG-Abnahme bei UI.
- Private automatisierte WinUAE-Capture/Broker-Flows bleiben `disabled_by_process`.

## 7. Refinement / atomization_report

- `atomization_report.decision`: `keep_single_story` (eine abgegrenzte Veraenderungsachse, keine versteckte Zweitimplementierung).
- `refinement_triage`: `pass`
- `arch_review.initial`: `pass`
- `arch_review.pre_ready_final`: `pass`
- `dor_check`: `pass`
- Quelle: [PF-all-open-ready-review 2026-10-12](../reviews/PF-all-open-ready-review-2026-10-12.md), statische Planungs-/DoR-Analyse, kein Test-PASS.

## 8. Implementation-/Review-Gates

- `implementation_gate_report`: `pending` (Vorbedingungen direkt vor Umsetzung erneut pruefen)
- `review_report`: `pending`
- `test_evidence`: `not_run`
- `done`: `not_authorized`

## Review-Priorisierung / Bestandsabgleich 2026-10-12

- **Prioritaet bleibt P1 (Reihenfolge C, End-to-End nach Abhaengigkeiten).** Es gibt keinen bestaetigten P0-Defekt aus der Review-Beobachtung allein.
- Der Open/Edit/Undo/Export-MVP bleibt P1 nach PF-TD-001, den nativen Integrations-Gates und PF-SP-017/018/019. Keine Vorziehung der Abnahme vor belegte Full-Story-Gates.
- Review 2026-10-12 ist Planungs-/Evidence-Abgleich, kein neuer Implementierungs-, DoR-, Runtime-PASS- oder Release-Beschluss.
