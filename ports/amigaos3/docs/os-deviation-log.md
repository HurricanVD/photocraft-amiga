# Port-local OS and upstream deviation log (draft)

| Area | Original rule or baseline | Proposed scoped deviation | Reason / evidence status |
|---|---|---|---|
| PhotoCraft AGENTS.md Rust-only | Rust-only application | allow C99 in `ports/amigaos3/` only | Native Intuition/ReAction/MiniGL ABI; accepted scoped exception, no main-crate changes |
| VD process canonical `docs/` | root process-overlay files | port-specific `ports/amigaos3/docs/` | retain upstream PhotoCraft docs for easy synchronization; accepted scoped exception, canonical bootstrap still pending |
| High-end CPU baseline | 68030+ typical | 68040+ with FPU for first MiniGL spike | target QuarkTex NG and PiStorm3D; physical CPU support and performance NOT validated |
| Display baseline | fallback RTG | optional MiniGL acceleration plus mandatory future CPU/RTG fallback | no unsupported claim that MiniGL outperforms RTG M2 |
| GUI automation | may exist in projects | do not run centrally disabled WinUAE automation | documented `disabled_by_process` status in VD process |

ADR-0001's architecture direction and the narrow fork-specific exceptions were accepted by the owner on 2026-10-08. These entries retain **unverified runtime/NDK details** and require implementation/pre-ready reviews. No WinUAE, PiStorm3D or m68k toolchain validation is implied.
