# PF-SP-021 — Geschichtetes Projektformat fuer Amiga-Spike

- Story-ID: `PF-SP-021`
- Story-Art: `SP`
- Typ: `enhancement/spike`
- Status: `ready` (2026-10-12 DoR, keine Implementation)
- Prioritaet: `P2`
- Entry-Profil: `standard`
- Datum: 2026-10-12
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-025` (geplant / `not_run`)
- Review: `docs/reviews/PF-all-open-ready-review-2026-10-12.md`

## 1. Problem / Nutzerwert

Als Nutzer/Maintainer moechte ich die Voraussetzungen fuer verlustfreies Speichern und Laden von PhotoCraft-Ebenen, Gruppen und Masken ermitteln, damit eine vollstaendig autarke PhotoCraft-Amiga-Version entsteht, die den originalen Rust-Upstream nachweisbar respektiert.

## 2. In Scope

- Synthetisches versioniertes Experimentformat fuer Layer/Group/Mask + Pixel-Tiles definieren und in einem begrenzten Roundtrip testen.
- Ownership/Endianness/Document-IDs und Crash/OOM/Truncation-Fehler abgleichen, Dateigrenzen definieren; originaler Rust-Dokumentvergleich.
- Untersuchen, ob kompatible .pcraft-Teilmengen implementierbar sind; explizite zukuenftige ADR zur Shipping-/Kompatibilitaetsentscheidung.

## 3. Nicht-Ziele

- Kein auszulieferndes garantiert .pcraft-kompatibles Format, keine PSD-/ICC-Semantik.
- Keine Erweiterung der originalen Rust-Crates und keine Remote-/Cloud-Pflicht.
- `ready` ist **keine** Implementierungs-, Release- oder WinUAE-Freigabe.

## 4. Akzeptanzkriterien

1. Rust/C typed layer fixture and negative-format bytes.
2. HUNK/vamos reader/writer bounds and corruption rollback, 32 MiB budget.
3. Kein Claim auf reales .pcraft/PSD-Produktaustauschformat ohne separate Entscheidungen.
4. Ungueltige Daten/OOM/Fehlerpfade liefern kontrollierte Ergebnisse; alle genutzten Datenpuffer und OS-Libraries werden trotz Fehler vollstaendig freigegeben. 32 MiB Fast RAM bleibt gepruefte Testbaseline, keine unbelegte Minimumspec.
5. Eigene C99-Algorithmen bleiben durch originale Rust-Semantik und benannte Oracles differenziert; WinUAE/GUI-Status wird nur nach echter manueller Zielausfuehrung PASS.

## 5. Architektur / Betroffene Module

- ports/amigaos3/tests + begrenzte pc_layered_io-Experimentalfunktionen, docs/adr/ proposed future product-format decision.
- Gate-Quellen: akzeptierte ADR-0001/0002/0003, [Port-Architektur](../../ports/amigaos3/docs/architecture.md) und [Pre-ready-Review](../reviews/PF-all-open-ready-review-2026-10-12.md).
- Capability: VD-CAP-DOS-002; AmigaOS/NDK 3.2 und GCC/Bebbo Testprofil gem. ADR-0003; reale Produktcompilerfreigabe getrennt.
- IP-Provenienz: `reference_only`. Dunkelkammer-Eigenanteile sind MIT (PR #9, Lizenzhead `70897e91`), fremde Quellen/Assets unter eigener Lizenz. Alle direkt importierten Dateien benoetigen SHA/Copyright/Notice im Portregister.
- Implementierungsabhaengigkeiten: PF-SP-009 experimenteller Serialisierungsvertrag, PF-SP-017 Import; stabiler Produktformatentscheid eigener vom Eigentuemer zu akzeptierender ADR.

## 6. Testplanung

- [PF-TN-025](../tests/PF-TN-025.md): Testfaelle und Negativpfade vor Umsetzung, `not_run`.
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
