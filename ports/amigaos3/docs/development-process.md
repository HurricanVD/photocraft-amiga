# VD Amiga development-process overlay (draft)

Source of process rules: private `HurricanVD/vd-amiga-dev-process`, pinned to **0.2.1**. For a conventional workspace checkout, resolve the path separately; do not vendor that private repository into this public fork.

This fork keeps upstream `docs/`, `Cargo.toml` and root `AGENTS.md` primarily governed by PhotoCraft's Rust development process. The port-specific overlay is scoped to `ports/amigaos3/` and its draft ADR. This deliberate namespace exception preserves upstream synchronization.

- Compiler lane: GCC/Bebbo `m68k-amigaos-gcc`, NDK 3.2. Avoid assuming the Rust workspace builds for m68k.
- Host build: `make -C ports/amigaos3 host-test`.
- m68k smoke: `make -C ports/amigaos3 amiga-smoke ...` when independently supplied MiniGL headers/import lib and toolchain are present.
- Test ladder: host deterministic test -> HUNK/vamos (once supported) -> manual WinUAE QuarkTex NG -> PiStorm3D real hardware.
- **Do not run or claim the process's automated WinUAE broker/capture workflow** while it is centrally `disabled_by_process`.
- Before canonical PF-* story/test IDs, reserve `PF` in the shared `workspace-id-prefixes.md` registry; none are allocated in this bootstrap.
- Use process ADR/review/version-check and IP-provenance procedures before code imports or release decisions.

State: skeleton/overlay only; no full process bootstrap gate, compiler validation or runtime acceptance has been completed.
