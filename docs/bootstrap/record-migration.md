# Migration of PhotoCraft PF records to canonical VD directories

Date: 2026-10-09. Source records under `ports/amigaos3/docs/stories/`
and `ports/amigaos3/docs/tests/` remain present for backwards-compatible
documentation. These records were copied without changing the tracked
implementation state, into canonical VD locations:

| Story | Canonical path | Status |
|---|---|---|
| PF-SP-001 | `docs/stories/PF-SP-001.md` | blocked, MiniGL/WinUAE |
| PF-SP-002 | `docs/stories/PF-SP-002.md` | `blocked` (seit Atomisierung 2026-10-11); bei urspruenglicher Migration 2026-10-09 historisch `in_progress` |
| PF-SP-003 | `docs/stories/done/PF-SP-003.md` | done, GCC13/vamos |

Test references are `docs/tests/PF-TN-001.md`,
`docs/tests/PF-TN-002.md`, `docs/tests/PF-TN-004.md`, and
`docs/tests/done/PF-TN-003.md`.

Reserved prefix `PF` is already registered. No additional IDs
were allocated during this migration. Old source paths remain secondary
mirrors; new lifecycle mutations must update canonical records first.
The original PhotoCraft Rust `README.md`, `AGENTS.md`,
`docs/roadmap.md`, and `docs/architecture.md` are not changed.

## Lifecycle-Statusaktualisierung 2026-10-11

Die kanonische Story PF-SP-002 ist nach `split_required` `blocked`, der vorherige Migrationsstand ist nur historische Evidenz. PF-SP-004..009 und PF-TD-001 sind getrennte Refinement-Stories ohne DoR-Freigabe. Quelle: `docs/reviews/PF-SP-002-atomization-report-2026-10-11.md`.
