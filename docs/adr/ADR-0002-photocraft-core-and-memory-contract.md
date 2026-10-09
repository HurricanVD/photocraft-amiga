# ADR-0002: Native PhotoCraft core, tile storage and memory contract

- ADR-ID: `ADR-0002`
- Status: `proposed`
- Created / updated: 2026-10-09
- Lifecycle action: `propose`
- Decision depth: `far_reaching` — core ABI, data model, storage and memory budget across future stories
- Decision authority: user / project owner; approval `pending`
- Scope: `ports/amigaos3` (PhotoCraft fork); upstream Rust crates are not modified
- Referenced accepted ADR: [ADR-0001](ADR-0001-amigaos3-port-seam.md)
- Architectural test baseline: `PF-SP-002`, `PF-TN-002`; `PF-SP-003` and archived `PF-TN-003`
- Baseline standards: VD Amiga process 0.2.1, Amiga architecture baseline, IP/provenance and C99 coding style

## Context

PhotoCraft's original `crates/raster` stores sparse **256×256** tiles with
copy-on-write sharing, a pixel-format-dependent byte stride and per-surface
default pixels. `crates/doc` defines the document/layer/group model, with
children ordered bottom-first. A classic m68k application cannot assume a
large workstation memory budget. A 256×256 RGBA8 tile uses 256 KiB before
allocator overhead; RGBA16 requires 512 KiB and RGBA32F 1 MiB.

The accepted ADR-0001 requires a genuine PhotoCraft port, not replacing it
with Dunkelkammer's 128×128 document/tile model. The first RGBA8 C99
COW prototype and its six GCC13/vamos tests have passing evidence.

## Proposed technical contract

1. Preserve **PhotoCraft's 256×256 tile grid** and negative-coordinate floor
   division. Missing tiles return their format-specific default pixel.
2. Represent `PixelFormat` as (colour mode, U8/U16/F32 sample, alpha). The
   experimental C99 raster supports raw *encoded* interleaved sample bytes
   for all the relevant strides; no hidden quantization, ICC conversion or
   endianness normalization inside raw read/write APIs. U16 and F32 bytes
   follow the native architecture's sample encoding, as with Rust
   `read_sample`/`write_sample` native bytes.
3. Retain reference-counted, sparse COW tiles and immutable snapshots.
   Adopt bounded tile allocations and avoid a full float RGBA framebuffer
   merely to display small dirty rectangles. Mark bulk region edits
   non-atomic on allocation failure until a transactional COW strategy is
   accepted and validated.
4. Keep platform-free raster and math separate from AmigaOS/NDK, ReAction,
   RTG and MiniGL. Later define a stable C command/render bridge without
   copying Dunkelkammer's `DkDocument` structures.
5. Preserve PhotoCraft's *bottom-first* layer ordering and stable IDs,
   visibility, opacity and group semantics in the eventual document engine.
   A **flat raster-layer experiment** may be used for host tests, but is not
   the full `Document` or an accepted public ABI.
6. Preserve non-destructive PhotoCraft operations, history and blend
   semantics by differential Rust-vs-C fixtures before claiming parity.
   Memory optimization must not silently change 16-bit, float/HDR or model
   semantics; no fixed 16-layer product limit.
7. Define memory/allocator fault-injection and rollback semantics, tile
   pool/lookup complexity, region row/band sizes, max document geometry and
   cache eviction in implementation follow-ups before product acceptance.
   Avoid global permanent single-pixel allocations in tight loops.

## Alternatives considered

- Copy Dunkelkammer 128×128 DkDocument engine — rejected by accepted ADR-0001.
- Standardize on RGBA8 and discard 16-bit/HDR — rejected as a semantic loss.
- Full Rust std-based cross compilation now — not yet established on m68k;
  keep a Rust-reference-first C99 parity port.
- Always materialize a full float RGBA canvas — rejected for Amiga RAM.
- Store format samples in host-order normalized floats only — rejected for
  raw pixel IO; floats remain relevant to reference compositing later.

## Consequences and evidence still needed

+ Portable C99 testing works without MiniGL or NDK.
+ Original PhotoCraft Rust can serve as executable differential oracle.
- A pixel-format-generic COW store consumes up to 1 MiB per tile at 32F RGBA.
- Bulk writes and clone failures need precise low-memory safety tests.
- Persistence, multi-layer compositor, masks, groups, ICC, effects, history
  and target performance remain unimplemented or unverified.

## Approval and revisit

**This ADR is NOT accepted.** PF-SP-002 may run reversible, explicitly
experimental C99 spikes while the permanent storage/API decision is pending.
Before making it a stable engine or first implementation-driven PO/TD/BG
story, obtain a separate explicit user approval and complete the applicable
pre-ready architecture review and target memory tests.
