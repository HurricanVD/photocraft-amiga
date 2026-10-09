# AmigaOS port architecture (accepted direction, 2026-10-08; runtime unverified)

Upstream architecture: [PhotoCraft docs/architecture.md](../../../docs/architecture.md).
Fork decision: [ADR-0001](../../../docs/adr/ADR-0001-amigaos3-port-seam.md).

## Ownership boundaries

```text
PhotoCraft Rust source (untouched reference)
  geom / color / raster / doc / ops / compose / paint / psd / engine
                               |
                    conformance fixtures
                               v
  Target photo core (Rust/m68k IF viable; otherwise explicit C ports)
                               |
                        portable C ABI
                   +-----------+-----------+
                   |                       |
            ReAction shell           Canvas/present
         (Dunkelkammer-derived       MiniGL + RTG fallback
          patterns only)
```

The C bridge API is NOT yet finalized. A renderer must consume *PhotoCraft* RGBA8 composite tiles (or convert at explicit staging edges), not reinterpret `DkArgb32` as RGBA8. The documented `pc_argb32_to_rgba8` helper is a narrow legacy transfer utility.

## Graphics decisions still requiring evidence

- QuarkTex NG's `minigl.library` and PiStorm3D expose matching MiniGL entrypoints, but renderer behavior varies.
- The real ReAction integration must not draw on top of side-panel gadgets. Evaluate `mglCreateContextFromBitMap` using supported four-byte-per-pixel RTG bitmaps, only after validating context ownership and off-screen present semantics.
- Keep presentation cached; do not upload every tile on every frame. Dirty tiles, zoom and pan require an explicit invalidation policy.
- Compare MiniGL texture upload/present against the existing off-screen `WritePixelArray` + `BltBitMapRastPort` M2 baseline from Dunkelkammer.
- Alpha and ICC/display-transform semantics remain owned by PhotoCraft CPU output. GPU interpolation must not change exported pixel values.

## Gate order

Host algorithm tests -> optional m68k HUNK/vamos tests -> manual WinUAE/QuarkTex NG visual tests -> supported PiStorm3D real hardware tests. A m68k Rust toolchain with required std/alloc is not assumed to exist.

## GCC13.3 + vamos as a separate compatibility gate

PF-SP-003 adds an opt-in C99 build/runtime probe against GCC/Bebbo 13.3.0,
following vxplatform's `TOOLCHAIN_PROFILE=gcc13` *provider* guard pattern.
Unlike vxplatform, PhotoCraft builds no VXP archive in this probe. Amiga HUNK
runs under vamos at O0/O2 and should match the original Rust oracle bytes.
This is independent of the unverified MiniGL UI/backend compatibility.

## Current CPU core target evidence (2026-10-09)

The **isolated C99 subset** (`pc_core` geometry, RGBA8 sparse COW surface, colour-format metadata) has host ASan/UBSan evidence and has been compiled as 68020-compatible Amiga HUNK by GCC/Bebbo 13.3 at `-O0` and `-O2`. All six headless test/fixture HUNK binaries run under pinned `vamos`; the two m68k C oracles match the original Rust crate oracle byte-for-byte for the enumerated fixtures ([PF-TN-003](tests/done/PF-TN-003.md), [CI #37915607402](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37915607402)). GCC13 remains an opt-in test lane consistent with vxplatform ADR-0014; it does not replace a product compiler policy or the accepted ADR-0001 direction. The native PhotoCraft document model, formats, UI, bitmap MiniGL and RTG present paths remain unimplemented or unverified.

## Proposed core and compiler ADRs / exploratory implementation (2026-10-09)

- [ADR-0002](../../../docs/adr/ADR-0002-photocraft-core-and-memory-contract.md)
  scopes typed, sparse COW raster formats and constrained memory; remains
  `proposed`, requiring explicit owner acceptance.
- [ADR-0003](../../../docs/adr/ADR-0003-amigaos-toolchain-and-verification.md)
  scopes opt-in GCC13 versus future compiler product choice; remains
  `proposed`. GCC13 is validated only as a test profile, not as native
  PhotoCraft product toolchain.
- `pc_raster.h/.c` and `pc_document.h/.c` are isolated experimental
  format-generic byte storage and *flat* layer-ownership probes. Neither
  changes accepted ADR-0001 nor represents the full original doc model.
- Exact Rust-original `Surface::write_interleaved` /
  `to_interleaved` output is the test oracle across format/strides; 68k
  GCC13/vamos and host sanitizer jobs are mandatory validation.

## New format-generic storage spike (2026-10-09)

`pc_raster.h/.c` is a **separate experimental C99 encoded raster API** supporting U8/U16/F32 and PhotoCraft mode/channel metadata, 256×256 sparse COW tiles, default pixels and rectangular IO. It is not a colour-management or compositing conversion layer; U16/F32 bytes are native-endian as in Rust `to_ne_bytes`. Multi-tile write failure is not yet atomic. `pc_document.h/.c` is a *flat raster-only ownership scaffold* and does not replace PhotoCraft's grouped document model.

[PF-TN-004](tests/PF-TN-004.md) shows host, original Rust differential and GCC13/vamos O0/O2 passing on CI #37919197110. ADR-0002 and ADR-0003 have been submitted only as **proposals**, not accepted product architecture. Further core/memory/ABI, graphics and release decisions need explicit review before implementing a stable public interface.
