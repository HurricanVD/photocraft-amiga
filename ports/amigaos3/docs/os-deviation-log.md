# Port-local OS and upstream deviation log (draft)

| Area | Original rule or baseline | Proposed scoped deviation | Reason / evidence status |
|---|---|---|---|
| PhotoCraft AGENTS.md Rust-only | Rust-only application | allow C99 in `ports/amigaos3/` only | Native Intuition/ReAction/MiniGL ABI; proposed for this fork, no main-crate changes |
| VD process canonical `docs/` | root process-overlay files | port-specific `ports/amigaos3/docs/` | retain upstream PhotoCraft docs for easy synchronization; initial pre-bootstrap exception |
| High-end CPU baseline | 68030+ typical | 68040+ with FPU for first MiniGL spike | target QuarkTex NG and PiStorm3D; physical CPU support and performance NOT validated |
| Display baseline | fallback RTG | optional MiniGL acceleration plus mandatory future CPU/RTG fallback | no unsupported claim that MiniGL outperforms RTG M2 |
| GUI automation | may exist in projects | do not run centrally disabled WinUAE automation | documented `disabled_by_process` status in VD process |

All deviations are provisional pending architecture and process review.
