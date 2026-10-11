# PF-SP-017 — Bildimport PNG/IFF in ein natives PhotoCraft-Dokument

- Story-ID: `PF-SP-017`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `ready` (2026-10-12 DoR, keine Implementation)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-12
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-021` (geplant / `not_run`)
- Review: `docs/reviews/PF-all-open-ready-review-2026-10-12.md`

## 1. Problem / Nutzerwert

Als Nutzer/Maintainer moechte ich einen lokal gespeicherten PNG- oder IFF-Bildinhalt in ein eigenes PhotoCraft-Rasterdokument importieren, damit eine vollstaendig autarke PhotoCraft-Amiga-Version entsteht, die den originalen Rust-Upstream nachweisbar respektiert.

## 2. In Scope

- Datei via ASL oder Shell-Pfad laden; vor dem Lesen Magic/Dimensionen/RAM-Grenzen validieren, nicht nur Dateiendung.
- PNG8 RGBA/RGB als erste reale Eingabe; IFF ILBM/PBM separat als pruefbarer Decoderpfad; optionaler datatypes.library-Fallback nur als read-only und capability-geprueft.
- Dekodierte Pixel in PhotoCraft-256x256-Tiles und stabile Layer-IDs schreiben; Fehler darf keinen halben Dokumentwechsel freigeben.

## 3. Nicht-Ziele

- Kein PSD/RAW, kein vollstaendiges ICC oder Photoshop-Format und kein Netz-Decoder.
- Keine Erweiterung der originalen Rust-Crates und keine Remote-/Cloud-Pflicht.
- `ready` ist **keine** Implementierungs-, Release- oder WinUAE-Freigabe.

## 4. Akzeptanzkriterien

1. Kleines RGB/RGBA-PNG, IFF-Sample, Alpha-/Stride-/Endianness-Oracles und Null-Bilder.
2. Truncated/Malformed/huge dimensions, Dateifehler und deterministischer Allocator-Fault; kein Leak, altes Dokument bleibt gueltig.
3. Host C99/ASan, HUNK/vamos und GUI-Pfad per manuellem WinUAE/ASL-Roundtrip.
4. Ungueltige Daten/OOM/Fehlerpfade liefern kontrollierte Ergebnisse; alle genutzten Datenpuffer und OS-Libraries werden trotz Fehler vollstaendig freigegeben. 32 MiB Fast RAM bleibt gepruefte Testbaseline, keine unbelegte Minimumspec.
5. Eigene C99-Algorithmen bleiben durch originale Rust-Semantik und benannte Oracles differenziert; WinUAE/GUI-Status wird nur nach echter manueller Zielausfuehrung PASS.

## 5. Architektur / Betroffene Module

- ports/amigaos3/src: eigene pc_io_import und pc_document-Adapter; Dunkelkammer dk_png/dk_iff/dk_datatype nur mit MIT+Third-Party-Notice.
- Gate-Quellen: akzeptierte ADR-0001/0002/0003, [Port-Architektur](../../ports/amigaos3/docs/architecture.md) und [Pre-ready-Review](../reviews/PF-all-open-ready-review-2026-10-12.md).
- Capability: VD-CAP-ASL-001 / VD-CAP-DT-001; AmigaOS/NDK 3.2 und GCC/Bebbo Testprofil gem. ADR-0003; reale Produktcompilerfreigabe getrennt.
- IP-Provenienz: `code_dependency`. Dunkelkammer-Eigenanteile sind MIT (PR #9, Lizenzhead `70897e91`), fremde Quellen/Assets unter eigener Lizenz. Alle direkt importierten Dateien benoetigen SHA/Copyright/Notice im Portregister.
- Implementierungsabhaengigkeiten: MIT-Quellinventar PF-SP-012, dokumentierte C99-Raster-/Layer-Basis, Memory PF-TD-001; PF-SP-011 fuer UI-Requester.

## 6. Testplanung

- [PF-TN-021](../tests/PF-TN-021.md): Testfaelle und Negativpfade vor Umsetzung, `not_run`.
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
