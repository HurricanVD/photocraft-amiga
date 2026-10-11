# PF Native Offline Ready-Gates — 2026-10-12

- Reviewtyp: nachvollziehbarer *AI-assisted static* Scope/IP/Architektur-/DoR-Review. Kein unabhaengiges menschliches GitHub-Approve.
- `review_stage: initial` — bestehende akzeptierte ADR-0001/0002/0003, VD process 0.2.1 Story-/ADR-Lifecycle, Capability-Katalog und PhotoCraft-original Rust-/C99-Core-Vertrag geprueft.
- `review_stage: pre_ready_final` — **separater Abschlussabgleich** der konkret definierten Scopes, Akzeptanz-/Testvektoren, Betroffene Module, IP-Rechte, Dokumentationen, Abhaengigkeiten und RAM/Toolchain/C99-Port-Grenzen gegen den Port-Runtime-Plan; Empfehlung `ready` fuer unten benannte Stories. Keine laufende Implementation oder Tests bewertet.
- Quelle PhotoCraft: `main` @ `1cf9b1a8`; UI-Reuse Planungs-PR #13. Dunkelkammer UI-Quellstand `26295506` und Besitzer-MIT-Lizenzstand `70897e91` durch PR #9. VirtualCommander/vxplatform jeweils MIT, Original-Copyright bleibt.
- Explizite Eigentuemervorgaben aus der laufenden Projektkonversation: **vollstaendig autarkes AmigaOS 3.2/m68k PhotoCraft**, Wiederverwendung Dunkelkammer erlaubt, Dunkelkammer auf MIT umgestellt. Keine Remote-Engine und kein Cloud-Backend.
- Architektur: ADR-0001/0002/0003 behalten `accepted` und definieren C99-Port-Seam, 256x256/32MiB-COW/RAM und getrennte Compiler-Testprofile. M2 RTG ReAction ist der erste *software* Sichtbarkeitspfad, MiniGL bleibt optional und `blocked`. Der neue Plan aendert **keine** akzeptierte stabile public ABI, Produkteinzugsgrenze oder Production Toolchain.
- IP: `rights_status: approved_owner_MIT` nur fuer Rechteinhaber-eigene Quellteile; `per_file_third_party_notice_check: required_at_import`. Das Zielverzeichnis enthaelt bislang *keine* Dunkelkammer-Codekopie. Fallback `reference_only` bei ungeklaerten Fremdassets.
- NDK/AmigaOS-first: Statische Referenz aus Dunkelkammer `src/platform/dk_present.c/.h`, `src/ui/dk_ui_layer_list.c/.h`, `src/ui/dk_ui_event.c`, `src/ui/dk_ctx_menu.c`, `src/app/main.c`; Capability-Katalog `VD-CAP-WIN-001`, `VD-CAP-LAYOUT-001`, `VD-CAP-ASL-001`, `VD-CAP-DT-001`, `VD-CAP-MENU-001`, `VD-CAP-REQ-001`, `VD-CAP-LB-001`, `VD-CAP-WB-001`, `VD-CAP-BUILD-001`, `VD-CAP-MEM-001`; unverified PhotoCraft HUNK GUI/Test bleibt Implementation-/Done-Gate, **nicht** hier PASS.
- Atomisierung: bestehende PF-SP-007/008/009/010/011/012/013/014/015/016/PF-TD-001 und neue PF-SP-017/018/019/020/021/PF-PR-001 sind jeweils einzelne, trennbare Bereiche. `keep_single_story` pro Kandidat, historischer PF-SP-002 bleibt `split_required`.
- `refinement_triage: pass`; `arch_review.initial: pass`; `arch_review.pre_ready_final: pass`; `dor_check: pass` **nur** fuer die vorab beschriebene Implementierungsplanung. Risiken und Abhaengigkeiten bei Erteilung eines konkreten `implementation_gate_report` noch einmal pruefen.

## Einzelvotum (Planung, keine Laufzeitfreigabe)

| Story | Prioritaet | Test | Readiness-Check |
|---|---|---|---|
| PF-TD-001 | P1 | PF-TN-011 (not_run) | Allocator/transaction COW bounds; 32MiB, separate atomic scope; `ready` |
| PF-SP-007 | P1 | PF-TN-008 (not_run) | RGBA8 CPU compositor parity, separate atomic scope; `ready` |
| PF-SP-008 | P1 | PF-TN-009 (not_run) | one reversible command/rollback, separate atomic scope; `ready` |
| PF-SP-009 | P2 | PF-TN-010 (not_run) | experimental limited format spike only, separate atomic scope; `ready` |
| PF-SP-010 | P1 | PF-TN-013 (not_run) | window-neutral M2 RTG and channel bridge, separate atomic scope; `ready` |
| PF-SP-011 | P1 | PF-TN-014 (not_run) | ReAction sole window owner, lifecycle, separate atomic scope; `ready` |
| PF-SP-012 | P1 | PF-TN-016 (not_run) | MIT copyright and third-party provenance, separate atomic scope; `ready` |
| PF-SP-013 | P1 | PF-TN-017 (not_run) | BOOPSI layer/group stable IDs and list >64 rows, separate atomic scope; `ready` |
| PF-SP-014 | P1 | PF-TN-018 (not_run) | zoom/pan bounded pointer queue, separate atomic scope; `ready` |
| PF-SP-015 | P1 | PF-TN-019 (not_run) | PhotoCraft command/menu adapter, separate atomic scope; `ready` |
| PF-SP-016 | P2 | PF-TN-020 (not_run) | native popup/dialog cancel semantics, separate atomic scope; `ready` |
| PF-SP-017 | P1 | PF-TN-021 (not_run) | Bildimport PNG/IFF in ein natives PhotoCraft-Dokument, separate atomic scope; `ready` |
| PF-SP-018 | P1 | PF-TN-022 (not_run) | RGBA8-CPU-Komposit als lokales PNG exportieren, separate atomic scope; `ready` |
| PF-SP-019 | P1 | PF-TN-023 (not_run) | Erstes natives Brush-Werkzeug mit History-Commit, separate atomic scope; `ready` |
| PF-SP-020 | P1 | PF-TN-024 (not_run) | Autarke native MVP-End-to-End-Abnahme, separate atomic scope; `ready` |
| PF-SP-021 | P2 | PF-TN-025 (not_run) | Geschichtetes Projektformat fuer Amiga-Spike, separate atomic scope; `ready` |
| PF-PR-001 | P1 | PF-TN-026 (not_run) | Upstream-Delta und Original-Rust-Oracle fortlaufend pruefen, separate atomic scope; `ready` |

## Entscheider/Blocker und Geltungsgrenze

- **Kein** `implementation_gate_report` ist bereits bestanden. Abhaengige Stories, insbesondere PF-SP-020, koennen nicht in `in_progress`, bevor die vorangehenden abgenommenen technischen Module tatsaechlich vorliegen. WIP-Limit 2.
- `PF-SP-001` bleibt **blocked/P3** (QuarkTex/MiniGL Runtime- und SDK-Evidenz fehlt); `PF-SP-002` bleibt **blocked/P3** (historisches split_required-Parent, keine neue Implementierung).
- GCC13 ist nur getestetes, abgesondertes m68k/vamos-Profil, Produktcompiler ist nach ADR-0003 noch zu bestimmen. Produkt-Build/-ABI/-Distribution brauchen einen eigenen Entscheidungs- und Laufzeitnachweis.
- PF-SP-009/021 koennen als klar begrenzte Format-*Spikes* ready sein, ohne zu behaupten, ein finales nativeres `.pcraft`-/PSD-Format gewaehlt zu haben. Eine spaetere weitreichende Dateiformat-ADR braucht explizite Eigentuemergenehmigung.
- Alle Zieltests `not_run`; manuelle WinUAE-Tests bei GUI erforderlich. Automatisierte WinUAE-UI/Broker/Capture weiter `disabled_by_process`.
- Root `README.md` und `docs/architecture.md` bleiben Rust-Upstream-kompatibel und verweisen ausschliesslich auf das isolierte Port-Overlay; Port-`README.md` und `docs/architecture.md` tragen detailliertes lokales MVP-Design.
