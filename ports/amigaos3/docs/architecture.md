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

## Core and compiler ADRs / experimental implementation (updated 2026-10-09)

- [ADR-0002](../../../docs/adr/ADR-0002-photocraft-core-and-memory-contract.md)
  is **accepted** for PhotoCraft 256x256 sparse COW semantics, typed storage
  and constrained target memory; a stable/complete document ABI still needs implementation review.
- [ADR-0003](../../../docs/adr/ADR-0003-amigaos-toolchain-and-verification.md)
  is **accepted** for test-profile separation and evidence gates. GCC13 remains
  an opt-in tested profile; a full PhotoCraft production compiler is **not selected**.
- `pc_raster.h/.c` and `pc_document.h/.c` are isolated experimental
  format-generic byte storage and *flat* layer-ownership probes. Neither
  changes accepted ADR-0001 nor represents the full original doc model.
- Exact Rust-original `Surface::write_interleaved` /
  `to_interleaved` output is the test oracle across format/strides; 68k
  GCC13/vamos and host sanitizer jobs are mandatory validation.

## New format-generic storage spike (2026-10-09)

`pc_raster.h/.c` is a **separate experimental C99 encoded raster API** supporting U8/U16/F32 and PhotoCraft mode/channel metadata, 256×256 sparse COW tiles, default pixels and rectangular IO. It is not a colour-management or compositing conversion layer; U16/F32 bytes are native-endian as in Rust `to_ne_bytes`. Multi-tile write failure is not yet atomic. `pc_document.h/.c` is a *flat raster-only ownership scaffold* and does not replace PhotoCraft's grouped document model.

[PF-TN-004](tests/PF-TN-004.md) shows host, original Rust differential and GCC13/vamos O0/O2 passing on CI #37919197110. ADR-0002 and ADR-0003 were **accepted by the project owner on 2026-10-09** for their stated architectural direction and separated test profiles. Further specific core/memory/ABI, graphics and release decisions need implementation-level review before a stable public interface.

## Aktueller begrenzter Gruppen-Prototyp (PF-SP-005, 2026-10-11)

Die obigen Aussagen `flat raster-only` gelten fuer den **historischen
2026-10-09-Baseline-Stand**. Auf dem PF-SP-005-Integrationsbranch wird der
Port um eine **experimentelle** Gruppe/Raster-Baumstruktur erweitert:
Root-/Child-Vektoren in Bottom-first-Reihenfolge, globale Layer-IDs,
maximal 100 Gruppenebenen, sibling-only Shift und tiefe Clone-Kopien
der Metadaten mit unabhaengigen PcRaster-Wrappern. Pixel-Tiles bleiben
256x256 sparse COW gemaess ADR-0002. Ein globaler (auch zwischen
Dokumenten wirksamer) PF-BG-001-Objekt-Claim schuetzt Raster-Appends
und rekursiv geklonte Rasterobjekte vor Doppelbesitz.

Ein Wert von `pc_document_layer_count` zaehlt jetzt Root- und alle
Kind-Layer, waehrend `pc_document_root_layer_count` die Root-Siblings
bezeichnet. `pc_document_shift_layer` findet verschachtelte Siblings;
`pc_document_group_shift_child` ist ein zusaetzlicher gezielter Helfer.
Die neue Semantik wird per original Rust Group-/Document-Oracle, Host
ASan/UBSan und GCC13-HUNK/vamos O0/O2 geprueft.

Dies **ersetzt keinen vollstaendigen PhotoCraft-Document-Kern** und
bestaetigt weder Masken, Compositor, Persistenz, ReAction/MiniGL,
Produktcompiler noch stabile ABI. Historische Tests bleiben auf ihren
alten Commit beschraenkt; neues CI/Review auf PR4 erforderlich.

## Erweiterung Gray8 LayerMask (PF-SP-006, 2026-10-11)

Die experimentellen Layer-/Gruppen-Knoten besitzen jetzt optional eine
**separate Gray/U8 NoAlpha-Maske** mit sparse default 255 (reveal) bzw.
0 (hide), enabled/linked Flags und vordefiniertem density=1/feather=0.
Der gesamte CPU-Compositor und Photoshop PSD-Roundtrip sind davon
ausdruecklich **nicht** abgedeckt.

Mask-**Attach** beansprucht nach erfolgreicher Format-, ID- und
Aliasing-Pruefung den dokumentuebergreifenden PcRaster-Objekt-Owner.
Das Detach gibt vor Rueckgabe des Masken-Pointers die Owner-Claim
kontrolliert frei. Deep-Clones besitzen eigene Raster- und Masken-
Objektansprueche; nur die Tile-Bytes teilen sich ueber COW.

Original-Rust LayerMask-Oracle, Host ASan/UBSan und GCC13-HUNK/vamos
O0/O2 sind eigenstaendige PF-SP-006-Gates. Kein Produkt-ABI,
MiniGL/WinUAE-/PiStorm3D-Erfolg oder Release aus diesen Tests ableiten.

## Native offline delivery architecture (2026-10-12, scoped addendum)

The user explicitly requires PhotoCraft to run **entirely self-contained
on AmigaOS 3.2/m68k**. The C99 port remains the immediate executable
path, with no host, remote Rust command server, network or cloud at runtime.
PhotoCraft Rust is the **build-time semantic oracle** and unchanged
upstream merge source; an optional future Rust/m68k module is allowed only
after verified local m68k compiler, allocator, ABI and conformance gates.

```text
Original Rust PhotoCraft (unchanged; test-time semantic oracle)
          | host/Rust-vs-C99 differential fixtures
          v
Native PhotoCraft C99 core, 256x256 tiles, groups/masks
  +-- CPU compositor (PF-SP-007) -> RGBA8 visible bands
  +-- command/history (PF-SP-008) -> one edit transaction
  +-- local PNG/IFF import / PNG export (PF-SP-017/018)
          | bounded C facade, no AmigaOS symbols in portable core
          v
ReAction/window.class single display owner (PF-SP-011)
  +-- BOOPSI Layer/Group tree (PF-SP-013)
  +-- canvas navigation & pointer queue (PF-SP-014)
  +-- PhotoCraft command toolbar/menus (PF-SP-015)
  +-- native popup/requester widgets (PF-SP-016)
          | bounded RGBA8 -> ARGB32 staging (explicit, never cast)
          v
M2 software-RTG presenter (PF-SP-010)
  WritePixelArray -> offscreen Friend BitMap -> BltBitMapRastPort
          |
          +-- MiniGL/QuarkTex/PiStorm3D later optional, PF-SP-001 blocked
```

### Reuse and ownership

- Dunkelkammer UI source snapshot `26295506` and MIT licensing merge
  `70897e91` ([PR #9](https://github.com/HurricanVD/dunkelkammer/pull/9)).
  Import only self-authored/authorized UI/RTG components with original
  MIT copyright/permission notices and separate upstream third-party
  notices. Review `src/ui/dk_ui_layer_list.*`, `dk_ui_draw.*`,
  `dk_ctx_menu.*`, `dk_ui_event.c`, targeted ReAction lifecycle
  parts, `src/platform/dk_present.*`. No whole `dk_ui.c` monolith,
  `DkDocument`, 128x128 Dunkelkammer tiles or product artwork.
- ReAction owns the window, gadgets, Main Task and event loop. Do **not**
  use `vxplatform` process-global `vxp_display_open()` at the same
  time. If consuming vxplatform, use only demonstrated neutral
  geometry/helpers or isolate its M2-equivalent code.
- Raster/pixel model remains PhotoCraft 256x256 sparse COW, typed data
  and bounded groups/masks; RGBA8 presentation is a *conversion
  edge*, not a replacement of U8/U16/F32 storage or ICC promises.
- Dirty bands/tile row streams are preferred to mandatory FullFrame
  buffers, including on an approximately 32 MiB Fast RAM test target.
  Atomic OOM handling, lookup indexing and peak accounting are
  separate PF-TD-001 scope. All target limits must be measured.

### Offline file flow and acceptance

- PF-SP-017: small PNG/IFF import via C99 decoder + optional installed
  datatypes.library read fallback. File selection uses ASL requester.
- PF-SP-018: flat composited RGBA8 PNG export; never claim
  layered-project write semantics for a flattened file.
- PF-SP-019: one native brush with a single undoable command.
- PF-SP-020: Shell/Workbench native HUNK launch, local image open,
  layer view, brush edit, undo/redo, local PNG export. No host/network/
  cloud, and no required MiniGL.
- PF-SP-009/021: *experimental* layered format evaluations only.
  A future stable product file-format/compatibility decision needs
  separate owner-accepted ADR.

### Build, CI and status

- PF-PR-001 tracks upstream source/API and Rust semantic-oracle deltas.
  Upstream refresh may generate a reviewable PR; never auto-merge C
  reimplementations or silently patch Rust workspace code.
- Test ladder is host C99/ASan+UBSan and actual Rust references ->
  GCC/Bebbo 13.3 compatibility HUNK/vamos O0/O2 -> manual native
  WinUAE ReAction/RTG target evidence -> later supported hardware.
  The production/compiler ABI decision remains deferred by ADR-0003.
- The accepted ADR-0001/0002/0003 are consistent with CPU-first
  RTG-fallback and optional MiniGL; this is a delivery refinement,
  **not** an unrecorded superseding of an accepted target or API.
- [Ready gate review](../../../docs/reviews/PF-all-open-ready-review-2026-10-12.md)
  concerns scope, not implementation. `ready` stories' GUI,
  file I/O, compositor and brush tests remain `not_run`.
  Previously dated flat-only 2026-10-09 passages above are historical.
