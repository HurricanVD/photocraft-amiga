# PF-SP-018 — RGBA8-CPU-Komposit als lokales PNG exportieren

- Story-ID: `PF-SP-018`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `ready` (2026-10-12 DoR, keine Implementation)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-12
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-022` (geplant / `not_run`)
- Review: `docs/reviews/PF-all-open-ready-review-2026-10-12.md`

## 1. Problem / Nutzerwert

Als Nutzer/Maintainer moechte ich das lokale PhotoCraft-Dokument als flaches PNG exportieren, damit eine vollstaendig autarke PhotoCraft-Amiga-Version entsteht, die den originalen Rust-Upstream nachweisbar respektiert.

## 2. In Scope

- RGBA8-CPU-Compositor liefert zeilen-/bandweise RGBA8 mit definierter Alpha-Semantik, PNG-Writer schreibt lokal ohne Pflicht-Fullframe-Kopie.
- Fehler- und Overwrite-Vertrag: Output zuerst in separate Datei, Quelle unveraendert, atomare Ersetzung nur wo OS-Dateisemantik dies beweisbar hergibt; sonst kontrollierter Backup-/Recovery-Pfad.
- Kein Verwechseln von flatten PNG export mit layered document Save.

## 3. Nicht-Ziele

- Kein PSD, kein .pcraft oder verlustfreies geschichtetes Speichern; kein Remote-Export.
- Keine Erweiterung der originalen Rust-Crates und keine Remote-/Cloud-Pflicht.
- `ready` ist **keine** Implementierungs-, Release- oder WinUAE-Freigabe.

## 4. Akzeptanzkriterien

1. Original-Rust-CPU-Referenz gegen PNG-dekodierte Pixel fuer kleine Multilayer-Fixtures.
2. Disk-full, Invalid path, OOM, kurze Schreiboperation, Cleanup temporärer Datei, kein unbemerkter Verlust vorhandener Datei.
3. Host encoder/raster tests, genuine HUNK/vamos and manual WinUAE open/export/reimport.
4. Ungueltige Daten/OOM/Fehlerpfade liefern kontrollierte Ergebnisse; alle genutzten Datenpuffer und OS-Libraries werden trotz Fehler vollstaendig freigegeben. 32 MiB Fast RAM bleibt gepruefte Testbaseline, keine unbelegte Minimumspec.
5. Eigene C99-Algorithmen bleiben durch originale Rust-Semantik und benannte Oracles differenziert; WinUAE/GUI-Status wird nur nach echter manueller Zielausfuehrung PASS.

## 5. Architektur / Betroffene Module

- ports/amigaos3/src pc_io_export, pc_compositor, Dunkelkammer-PNG-Encoder hinter schmalem Adapter.
- Gate-Quellen: akzeptierte ADR-0001/0002/0003, [Port-Architektur](../../ports/amigaos3/docs/architecture.md) und [Pre-ready-Review](../reviews/PF-all-open-ready-review-2026-10-12.md).
- Capability: VD-CAP-ASL-001 / VD-CAP-DOS-002; AmigaOS/NDK 3.2 und GCC/Bebbo Testprofil gem. ADR-0003; reale Produktcompilerfreigabe getrennt.
- IP-Provenienz: `code_dependency`. Dunkelkammer-Eigenanteile sind MIT (PR #9, Lizenzhead `70897e91`), fremde Quellen/Assets unter eigener Lizenz. Alle direkt importierten Dateien benoetigen SHA/Copyright/Notice im Portregister.
- Implementierungsabhaengigkeiten: PF-SP-007 (Kompositor), PF-SP-012 (PNG-Writer-Lizenz), PF-TD-001 (RAM), PF-SP-011 fuer Save-Requester.

## 6. Testplanung

- [PF-TN-022](../tests/PF-TN-022.md): Testfaelle und Negativpfade vor Umsetzung, `not_run`.
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
