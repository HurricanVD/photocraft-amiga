# First Phase Backlog Seed

- Projekt: `HurricanVD/photocraft-amiga` (AmigaOS-3.2-Port)
- Phase: technische Portgrundlagen vor GUI-Integration
- Quelle README: `README.md` und `ports/amigaos3/README.md`
- Quelle Vision: `docs/vision.md`
- Quelle Roadmap: `docs/roadmap.md` (Original-PhotoCraft)
- Quelle Architektur: `docs/architecture.md` und `ports/amigaos3/docs/architecture.md`

## Ableitungsregeln

- Nur erste Roadmap-Phase in konkrete Story-Kandidaten zerlegen.
- Spaetere Phasen in `docs/roadmap.md` belassen.
- Unsichere OS-/NDK-/ReAction-Annahmen als Spike markieren.
- Status initial `draft|refining`, nicht `ready`.
- Finale IDs folgen `PF-(PO|BG|SP|LL|PR|TD)-NNN`; reines Formatbeispiel (nicht vergeben): `PF-PO-001`.
- `NNN` ist pro Prefix und Story-Art fortlaufend dreistellig.
- Ohne registrierten Prefix nur Platzhalter wie `<PP>-PO-001` verwenden; `PF` ist laut **immutablem zentralem Registry-Merge** [VD main Commit `1c32299`](https://github.com/HurricanVD/vd-amiga-dev-process/blob/1c32299deeb1b120703769f71ee0ae427370cc48/docs/process/workspace-id-prefixes.md) seit PR #19 (`2026-10-10`) `active`. Story-IDs koennen beim Intake/Refinement als `draft|refining` zugeteilt werden, sobald der PF-Prefix zentral registriert ist. Das DoR und `pre_ready_final` sind erst fuer die Promotion nach `ready` erforderlich; ID-Vergabe bedeutet keine Implementierungsfreigabe.
- Beim Materialisieren nach `docs/backlog.md` gilt:
  `draft|refining` kommt nach `Offen`, `blocked` nach `Blockiert`,
  `ready` nur mit erfuellter DoR-/Gate-Evidenz nach `Bereit`.

## Backlog-Kandidaten

| Vorgeschlagene ID | Titel | Typ | Prioritaet | Status | Quelle | Notiz |
|---|---|---|---|---|---|---|

## Blocker

| Thema | Blocker | Naechster Schritt |
|---|---|---|

## PhotoCraft-Port-Seed
- Phase: Erst CPU-Core-/HUNK-/Rust-Parität, dann echte GUI/Rendering.
- Quellen: `docs/vision.md`, Root-`README.md`, Original-Roadmap
  und `docs/architecture.md`.

| Story | Stand | Nachweis |
|---|---|---|
| PF-SP-001 | blocked | MiniGL/WinUAE offen |
| PF-SP-002 | blocked (split_required, 2026-10-11) | PF-TN-002/004 historisch; Child-Stories PF-SP-004..009, PF-TD-001 unter docs/stories/ |
| PF-SP-003 | done | PF-TN-003 |

Noch keine neue Produktstory ohne freigegebenes pre_ready_final-Gate.

## Nachtraegliche Atomisierung (2026-10-11)

PF-SP-002 war zu gross und ist kanonisch `blocked`. Die formalen Child-Storys stehen im aktiven Backlog als `refining|draft`. Der Bootstrap-Seed vergibt selbst keine IDs; PF-SP-004..009 und PF-TD-001 wurden in einem eigenen Refinement bei aktivem PF-Prefix erfasst. Massgeblich sind `docs/backlog.md` und [atomization_report](../reviews/PF-SP-002-atomization-report-2026-10-11.md).

Die private Bootstrap-Vollsnapshot-CI benutzt separat die ältere Prozessbaseline `0566a39` (VD 0.2.1), deren historische PF-Zeile `reserved` ist. Deren PASS-Marker **belegen keine aktuelle Registry-Aktivierung**; der reale Status `active` folgt ausschließlich aus dem separaten, oben exakt gepinnten Registry-Merge `1c32299`.
