# First Phase Backlog Seed

- Projekt:
- Phase:
- Quelle README:
- Quelle Vision:
- Quelle Roadmap:
- Quelle Architektur:

## Ableitungsregeln

- Nur erste Roadmap-Phase in konkrete Story-Kandidaten zerlegen.
- Spaetere Phasen in `docs/roadmap.md` belassen.
- Unsichere OS-/NDK-/ReAction-Annahmen als Spike markieren.
- Status initial `draft|refining`, nicht `ready`.
- Finale IDs folgen `PF-(PO|BG|SP|LL|PR|TD)-NNN`, z. B. `VC-PO-001`.
- `NNN` ist pro Prefix und Story-Art fortlaufend dreistellig.
- Ohne reservierten Prefix nur Platzhalter wie `PF-PO-001` verwenden.
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
| PF-SP-002 | in_progress | PF-TN-002/004 |
| PF-SP-003 | done | PF-TN-003 |

Noch keine neue Produktstory ohne freigegebenes pre_ready_final-Gate.
