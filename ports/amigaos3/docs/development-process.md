# VD Amiga development-process overlay (port-local)

Source of process rules: private `HurricanVD/vd-amiga-dev-process`, pinned to **0.2.1**. For a conventional workspace checkout, resolve the path separately; do not vendor that private repository into this public fork.

This fork keeps upstream `docs/`, `Cargo.toml` and root `AGENTS.md` primarily governed by PhotoCraft's Rust development process. The port-specific overlay is scoped to `ports/amigaos3/` and its accepted architecture ADR. This deliberate namespace exception preserves upstream synchronization.

- Compiler lane: GCC/Bebbo `m68k-amigaos-gcc`, NDK 3.2. Avoid assuming the Rust workspace builds for m68k.
- Host build: `make -C ports/amigaos3 host-test`.
- m68k smoke: `make -C ports/amigaos3 amiga-smoke ...` when independently supplied MiniGL headers/import lib and toolchain are present.
- Test ladder: host deterministic test -> HUNK/vamos (once supported) -> manual WinUAE QuarkTex NG -> PiStorm3D real hardware.
- **Do not run or claim the process's automated WinUAE broker/capture workflow** while it is centrally `disabled_by_process`.
- `PF` is now `reserved` in the shared `workspace-id-prefixes.md` registry. Project metadata pins `PF` for story/test IDs; no canonical IDs have been allocated. Formal bootstrap and `active` registration are still pending.
- Use process ADR/review/version-check and IP-provenance procedures before code imports or release decisions.

State: architecture direction accepted by the owner on 2026-10-08; isolated port skeleton/overlay only. No full process bootstrap gate, compiler validation, final pre-ready review or runtime acceptance has been completed.

## 2026-10-09 scope note

This fork uses the process's story/test-ID and review conventions inside the isolated port overlay, not the complete canonical root-level bootstrap layout. For PF-SP-003, the meaningful test ladder is host tests -> real GCC13 HUNK -> headless `vamos` -> Rust oracle, while manual WinUAE/Workbench is **n/a for the CLI-only spike**; MiniGL/GUI tests remain mandatory later under PF-SP-001. No central automated WinUAE test is activated. Separate full-bootstrap/registry activation and process-gate parity work remain follow-ups rather than evidence of a shipped product.
