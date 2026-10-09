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
