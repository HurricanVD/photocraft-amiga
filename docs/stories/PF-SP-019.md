# PF-SP-019 — Erstes natives Brush-Werkzeug mit History-Commit

- Story-ID: `PF-SP-019`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `ready` (2026-10-12 DoR, keine Implementation)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-12
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-023` (geplant / `not_run`)
- Review: `docs/reviews/PF-all-open-ready-review-2026-10-12.md`

## 1. Problem / Nutzerwert

Als Nutzer/Maintainer moechte ich mit der Maus einen sichtbaren Pinselstrich auf einer RGBA8-Ebene ausfuehren und rueckgaengig machen, damit eine vollstaendig autarke PhotoCraft-Amiga-Version entsteht, die den originalen Rust-Upstream nachweisbar respektiert.

## 2. In Scope

- Ein einfacher harter Rundpinsel, konstante Groesse/Opacity, stabile Dokumentkoordinaten aus PF-SP-014; Stroke in aktive Rasterebene, kein DkCore.
- Pointer down/move/up liefert eine atomare Command-History-Einheit, die nur Dirty-Bands repaintet; mehrfach gleiche Move-Events nicht als extra Stroke.
- Ungültige Layer, OOM, focus loss und Abbruch ohne teilweise offene History oder inkorrekte COW-Owner behandeln.

## 3. Nicht-Ziele

- Keine volle PhotoCraft-Paint-Paritaet, kein Tablet/Pressure, keine Smart Tools und keine Dunkelkammer-Rasterersetzung.
- Keine Erweiterung der originalen Rust-Crates und keine Remote-/Cloud-Pflicht.
- `ready` ist **keine** Implementierungs-, Release- oder WinUAE-Freigabe.

## 4. Akzeptanzkriterien

1. Golden RGBA8 stroke/pixel vectors gegen originalen Rust-Algorithmus wo mathematisch gleich; Abweichungen exakt markieren.
2. Eine Undo/Redo-Transaktion pro Stroke; Pointer-Abbruch und 256er Grenzen.
3. C99+ASan, HUNK/vamos and manual WinUAE brush/edit/undo test.
4. Ungueltige Daten/OOM/Fehlerpfade liefern kontrollierte Ergebnisse; alle genutzten Datenpuffer und OS-Libraries werden trotz Fehler vollstaendig freigegeben. 32 MiB Fast RAM bleibt gepruefte Testbaseline, keine unbelegte Minimumspec.
5. Eigene C99-Algorithmen bleiben durch originale Rust-Semantik und benannte Oracles differenziert; WinUAE/GUI-Status wird nur nach echter manueller Zielausfuehrung PASS.

## 5. Architektur / Betroffene Module

- ports/amigaos3/src pc_paint_commands + UI pointer adapter, PhotoCraft Rust paint as semantic oracle.
- Gate-Quellen: akzeptierte ADR-0001/0002/0003, [Port-Architektur](../../ports/amigaos3/docs/architecture.md) und [Pre-ready-Review](../reviews/PF-all-open-ready-review-2026-10-12.md).
- Capability: VD-CAP-WIN-001; AmigaOS/NDK 3.2 und GCC/Bebbo Testprofil gem. ADR-0003; reale Produktcompilerfreigabe getrennt.
- IP-Provenienz: `reference_only`. Dunkelkammer-Eigenanteile sind MIT (PR #9, Lizenzhead `70897e91`), fremde Quellen/Assets unter eigener Lizenz. Alle direkt importierten Dateien benoetigen SHA/Copyright/Notice im Portregister.
- Implementierungsabhaengigkeiten: PF-SP-008 (History), PF-SP-014 (Pointer), PF-SP-007/010 (Render), PF-TD-001 (OOM).

## 6. Testplanung

- [PF-TN-023](../tests/PF-TN-023.md): Testfaelle und Negativpfade vor Umsetzung, `not_run`.
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
