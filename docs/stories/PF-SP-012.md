# PF-SP-012 — Dunkelkammer-UI-Reuse: Lizenz-, Provenienz- und Modulgrenze

- Story-ID: `PF-SP-012`
- Story-Art: `SP`
- Typ: `documentation/spike`
- Status: `ready` (scoped static pre-ready review 2026-10-12)
- Prioritaet: `P1`
- Entry-Profil: `standard`
- Datum: 2026-10-12
- Backlog: `docs/backlog.md`
- Test-Traceability: `PF-TN-016` (geplant, `not_run`)
- Dunkelkammer Quell-Pin: `26295506ba363b1bd52bfca1852b0230ffb2bad9` (`main`, 2026-08-20)

## 1. Problem / Nutzerwert

Die Dunkelkammer-ReAction- und RTG-Implementierungen liegen in einem privaten Repository, dessen eigene Rechteinhaber-Anteile mit Merge PR #9 vom 2026-10-12 unter MIT gestellt wurden. Drittanteile und Notices sind vor Quellimport gesondert zu prüfen.

## 2. In Scope

- Eine konkrete Quellinventur fuer die UI-Kandidaten mit Ausgangs-Commit und Modulabhängigkeiten: src/platform/dk_present.*, src/ui/dk_ui_layer_list.*, dk_ui_layer_panel.*, dk_ui_draw.*, dk_ctx_menu.*, dk_ui_dialogs.c, dk_ui_event.c, canvas/dk_canvas.* sowie minimal benoetigte Fenster-Lifecycle-Ausschnitte aus dk_ui.c und app/main.c.
- Rechtekette getrennt pruefen: Virtual Dimension / Dunkelkammer; aus virtualcommander abgeleitete UI-Helfer; bereits in libamiga2d/vxplatform unter MIT bereitgestellte RTG-Teile; SDK und andere Third-Party-Teile.
- Eine schriftlich nachvollziehbare Weiterverwendungs-/Relicensing-Entscheidung pro Datei fuer MIT OR Apache-2.0 des PhotoCraft-Forks oder ersatzweise eine ausdruecklich kompatible Erlaubnis; Copyright- und Attribution-Texte.
- IP-Provenienzregister und Release-IP-Manifest als geplante Nachweise benennen; uebertragene Dateien erst in getrennten spaeteren Implementierungs-PRs zulassen.

## 3. Nicht-Ziele

- Keine Codekopie oder Lizenzbehauptung durch diese reine Planungsstory.
- Kein pauschales Umlizenzieren von Drittquellen, Bibliotheken, Fonts, Logos oder proprietaeren Produktassets.
- Keine UI-, CPU- oder Release-Implementierung.

## 4. User Story

Als Maintainer des oeffentlichen PhotoCraft-Amiga-Forks moechte ich die konkrete Freigabe und Herkunft jedes aus Dunkelkammer uebernommenen UI-Bausteins nachweisen, damit wir den nativen Port sicher veroeffentlichen koennen.

## 5. Akzeptanzkriterien

1. Fuer alle geplanten Kopien liegt eine Datei-/Commit-Matrix mit Eigentuemer, urspruenglicher Lizenz, erteilter oeffentlicher Nutzungslizenz, Anpassungsscope und Notice-Pflichten vor.
2. Die MIT-Freigabe der Eigenanteile ist mit Dunkelkammer Merge PR #9 nachgewiesen; fremde Copyright-Anteile sind separat nachzuweisen und Notices muessen erhalten bleiben.
3. Virtualcommander-abgeleitete Module und eventuell separate Shared-Library-Fassungen werden auf ihre eigene Rechtekette geprueft; ungeklaerte Dateien bleiben reference_only.
4. Keine privaten Dateien, SDKs, Fonts, ROMs, Asset-Binaries oder Lizenztexte ohne Berechtigung werden in das oeffentliche Repo kopiert; statischer Diff-/Provenienzcheck ist dokumentiert.

## 6. Architektur, Abhaengigkeiten und Grenzen

- Ziel: `ports/amigaos3/` (C99-Ausnahme), AmigaOS 3.2, 68040+, RTG/P96, 32 MiB Fast-RAM-Baseline.
- Abhaengigkeiten: Keine technische Abhaengigkeit; *IP-Voraussetzung* vor jeder direkten Dunkelkammer-Quellkopie in PF-SP-010/011/013/014/015/016. Die bereits akzeptierte ADR-0001 wird nicht stillschweigend ersetzt.
- PhotoCraft Rust bleibt semantische Referenz; 256x256-Sparse-Tiles, Gruppen, Masken und Commands gehoeren dem PhotoCraft-Kern. Kein Ersatz durch Dunkelkammer-Dokument-/Rastermodell.
- Provenienz: MIT-Freigabe der Dunkelkammer-Eigenanteile durch PR #9 nachgewiesen; Dateiabgleich und Fremdanteils-Notices bleiben offen.
- Explizite Eigentuemeraussage zur Wiederverwendung liegt vor; bei direkter Veroeffentlichung kopierter proprietaerer Quelldateien ist der dateibezogene Lizenz- und Notice-Nachweis aus PF-SP-012 vor der Codeuebernahme erforderlich.
- Der Code-Import ist kein Scope dieses Dokumentations-PRs. Kein MiniGL als harte Abhaengigkeit; keine Produkt-/ABI-/Release-Freigabe.

## 7. Tests und Evidenz

- PF-TN-016: geplanter Dokumenten-/Provenienzabgleich (nicht OS-Laufzeittest); Rechteinhaber-Freigabe und Lizenztexte sind gesonderte manuelle Gates.
- Host-/ASan/UBSan-Tests fuer portablen Teil, echte m68k-HUNK/vamos-O0/O2-Smokes fuer anwendbaren Teil; GUI/Presenter erfordert zusaetzlich manuellen WinUAE-AmigaOS-3.2-Nachweis.
- Prozessgesteuerte automatisierte WinUAE-UI-/Broker-/Capture-Skripte bleiben `disabled_by_process` und werden nicht ausgefuehrt.
- Alle neuen Tests und Runtime-Checks: `not_run`; keine PASS-Evidenz aus Dunkelkammer wird als PhotoCraft-Port-PASS ausgegeben.

## 8. Refinement / atomization_report

- `refinement_triage`: `pass`
- `atomization_report.decision: keep_single_story` (vorlaeufig, formaler Scope-/IP-/Architekturreview offen).
- Aenderungsachse: Dunkelkammer-UI-Reuse: Lizenz-, Provenienz- und Modulgrenze.
- `rights_status: owner_MIT_confirmed; third_party_notices_pending`.
- `arch_review.initial`: `pass`
- Vor `ready`: separate `arch_review.pre_ready_final: pass` und `dor_check: pass` mit echten Belegen erforderlich; WIP-Limit bleibt unveraendert.

## 9. Review-/Implementierungsgates

- `arch_review.pre_ready_final`: `pass`
- `dor_check`: `pass`
- `implementation_gate_report`: `pending`
- `review_report: pending`
- `test_evidence: not_run`
- `done: not_authorized`

## MIT-Nachweis 2026-10-12

- Die Lizenz der Dunkelkammer-Eigenanteile ist seit [Merge PR #9](https://github.com/HurricanVD/dunkelkammer/pull/9) am Commit `70897e91c94214421b0595b3e858446d6b3586ea` **MIT** (Copyright 2026 Virtual Dimension). UI-Quellenstand: `26295506ba363b1bd52bfca1852b0230ffb2bad9`.
- Kopien lizenzierter Eigenanteile sind unter den MIT-Bedingungen moeglich. Vor Quellimport: Herkunft je Datei, MIT-Copyright-/Permission-Notice, VirtualCommander-/vxplatform-Anteile und unlizenzierte Fremdassets kontrollieren; [Quellmatrix](../../ports/amigaos3/docs/dunkelkammer-ui-source-inventory.md).
- Keine kopierten Quellen oder Tests in diesem Dokumentations-PR. Story weiterhin `draft`, Umsetzungsgates und WinUAE-Evidenz `not_run`.

## Pre-ready delta 2026-10-12

- Separate, bounded delivery: MIT notice / per-file provenances; VirtualCommander, vxplatform and third-party ancestry audit; no source import yet.
- Architecture and DoR: [final pre-ready report](../reviews/PF-all-open-ready-review-2026-10-12.md); native self-contained C99 PhotoCraft AmigaOS 3.2, no host/server, accepted ADR-0001/0002/0003.
- License: Dunkelkammer owner code is MIT (merge PR #9), VirtualCommander/vxplatform notices and third-party rights retained. The [source inventory](../../ports/amigaos3/docs/dunkelkammer-ui-source-inventory.md) is an *input to*, not a substitute for, the specific import diff review.
- No implemented changes, no runtime/GUI tests, `implementation_gate_report: pending` and WIP limit unchanged. Any dependency gap blocks the later implementation gate, not a fabricated PASS.
