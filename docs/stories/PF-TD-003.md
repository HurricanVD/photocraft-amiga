# PF-TD-003 — ReAction/RTG-Ressourcenbilanz reproduzierbar pruefen

- Story-ID: `PF-TD-003`
- Story-Art: `TD`
- Typ: `tech_debt`
- Status: `draft` (Neuanlage 2026-10-12; Refinement/DoR/Implementation nicht autorisiert)
- Prioritaet: `P2`
- Backlog: `docs/backlog.md`
- Bezug: PF-SP-011 (Fenster-Lifecycle), PF-SP-010 (RTG-Presenter), PF-SP-020 (MVP-Abnahme)

## Problem / Beobachtung
Der WinUAE-v0.0.6-Preview startet aus Workbench ohne verwaistes CON:-Fenster und schliesst dreimal aus der Shell mit RC 0. Fast-Memory-Messungen zeigen zwischen den ersten und dritten Nachlaufwerten -1144 Byte; dies ist weder ein bewiesener Leak noch ein Leakfreiheitsnachweis. Die aufeinanderfolgenden Prozess-internen -4,4-KiB-Deltas duerfen nicht als linear kumulierender Leak eingeordnet werden. Quelle: `ports/amigaos3/docs/manual-winuae-document-preview.md`.

## Eine Aenderungsachse
Reproduzierbare **Ressourcen-Attribution und Dokumentation** fuer den bestehenden Preview. Kein neuer RTG-Presenter und keine zweite GUI-Shell. Korrekturen gefundener Fehler werden nach Ursache in PF-SP-010/011 oder einem belegten P0-Bug umgesetzt.

## Scope
- Referenzierte Objekt- und Lifetime-Matrix: StackSwap, ReAction-Gadgets, Window, Libraries, Offscreen-Bitmaps, RastPorts und PcDocument.
- Warm-up und mindestens drei vergleichbare normale Shell-Start/Close-Zyklen, separate Nachlauf- und Prozess-internen AvailMem-Werte, Version/SHA/WinUAE-Konfiguration.
- Beobachtungen, Reproduktionsschritte, vorhandene Log-/Screenshot-Evidenz, OS-Cache-/Allokator-Unsicherheiten und begrenzte Abweichungsgrenzen dokumentieren.
- Fehlerpfade nach fehlenden Libraries/RTG nachvollziehbar erfassen, soweit Testkonfiguration vorhanden.

## Nicht-Ziele / Abhaengigkeiten
- Kein verpflichtender Low-Fast-RAM-Stresstest fuer den Preview: `skipped_by_owner_preview_only`; PF-TD-001 behandelt deterministische OOM-Injektion im portablen Kern, PF-SP-020 spaetere MVP-Freigabe.
- Keine automatische WinUAE-Steuerung, solange `disabled_by_process`.
- Kein produktiver Build/Shipping-Gate oder pauschales `leak_free`-Label ohne Beweis.
- Benutzt PF-SP-010/011-Preview-Code und das manuelle WinUAE-Testprotokoll.

## Akzeptanzkriterien
1. Alle relevanten allokierten OS-/C-Ressourcen sind mit Create/Free-Pfad und Failure-Exit bilanziert; unbekannte Besitzer explizit markiert.
2. Reproduzierbares Warm-up-Protokoll trennt OS-Caching von beobachtetem wachsendem Restbestand, ohne unbelegte Ursachenbehauptung.
3. Jeder bestaetigte reproduzierbare Ressourcenfehler erhaelt konkrete Ursache, Reproduktionsfall und Regressionstest/zugeordnetes Korrektur-Ticket.
4. Manuelle GUI-Evidenz erhaelt Tested-SHA, WinUAE-/RTG-Setup und dokumentierte Einschraenkungen.
5. PF-SP-011/PF-SP-020 koennen auf die Untersuchung verweisen; sie werden dadurch nicht automatisch `done`.

## Gates
- `refinement_triage: pending`; `arch_review.initial: pending`; `dor_check: pending`.
- `implementation_gate_report: pending`; `test_evidence: not_run`; `review_report: pending`; `done: not_authorized`.
- Prioritaet P2, solange kein reproduzierbarer daten- oder absturzrelevanter Defekt bestaetigt ist. Bei solcher Evidenz separat P0/P1 triagieren.
