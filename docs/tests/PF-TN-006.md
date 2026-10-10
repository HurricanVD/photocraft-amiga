# PF-TN-006 — Nested raster groups and deep-COW document tree

- Date: 2026-10-10
- Story: `PF-SP-002` (`in_progress`)
- Branch: `feature/pf-sp-002-group-tree`, **stacked on PR #3** (flat-layer/opacity increment)
- Scoped verification: **PASS** against source commit `c67bbca4325668daace0cc8cfddd060ef0495469` in [CI #38042365174](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38042365174); overall multi-platform CI is recorded separately. Subsequent documentation commits do not inherit this source SHA automatically.
- Accepted scope: ADR-0002 PhotoCraft bottom-first group semantics,
  `MAX_GROUP_DEPTH=100`, stable global layer IDs, bounded C99 allocation.
- Classification: incremental host/target **spike**, not native product API.

## Contracts to test

| Area | Case | Expected |
|---|---|---|
| Nested groups | Root group containing raster + subgroup with raster | Stable bottom-first children, lookup by group ID |
| Global IDs | Existing descendant ID reused at root or elsewhere | Reject without tree mutation |
| Parent ownership | Missing parent, raster parent, failed append | Reject, input raster remains caller-owned |
| Reordering | Shift only within a group | Same parent, IDs/names/surface retained |
| Copy semantics | Clone group tree, mutate/shift descendants | Independent arrays/names/order and COW raster contents |
| Lifetime | Destroy original before copy | Cloned descendants remain valid |
| Depth | 100 group levels, child raster inside deepest | Succeeds and globally findable |
| Depth guard | 101st nested group | Reject without mutation |
| Rust original | `Document::shift`, `Layer::group` output | Byte-identical C99 group oracle |
| m68k | GCC13.3 `-O0/-O2` HUNK + pinned vamos | Extended `pc_document_test` passes |

## Execution

- `make -C ports/amigaos3 host-test`
- `make -C ports/amigaos3 host-core-sanitize`
- `cargo run --manifest-path ports/amigaos3/tests/rust_oracle/Cargo.toml -- group`
- `ports/amigaos3/build/group_oracle`
- Diff oracle output byte-for-byte in the main CI `amiga-document-parity` job.
- The main CI native `amiga-document-m68k` job builds and executes the
  extended C99 document test under pinned GCC/Bebbo 13.3 and vamos.

Do **not** interpret a passing group-tree contract as rendering/Photoshop
group parity. Masks, ICC/PSD, blend/pass-through rules, effect stacks, group
state, cross-parent moves, command history and native AmigaOS UI are excluded.

## Recorded test evidence — 2026-10-10

- Source/fixture commit: `c67bbca4325668daace0cc8cfddd060ef0495469`
  (all group implementation code and CI comparison present).
- [CI #38042365174](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38042365174)
  scoped job `PF-SP-002 flat Document C99 vs original Rust`:
  **completed/success**. Executed C99 host and ASan+UBSan tests, 51
  unchanged original PhotoCraft `photocraft-doc` tests and byte-identical
  Rust `LayerContent::Group` vs C99 nested group oracle.
- Observed markers: `PASS: PhotoCraft bounded group hierarchy/deep COW/ID semantics`
  and `PASS: original Rust Group order/clone vs C99`.
- Same run, native job `PF-SP-002 GCC13 m68k Document O0/O2 vamos`:
  **completed/success**. Validated six real m68k HUNK binaries; the
  extended group-aware `pc_document_test` passed in pinned `vamos`
  at both `-O0` and `-O2`.
- Compiler baseline: opt-in GCC/Bebbo 13.3.0 test image
  `stefanreinauer/amiga-gcc:gcc-v13.3-20260622`; resolved
  digest `sha256:f9d09422a89f317a2f59d5db46227ad9bd8d753d5c9fe41553fcfdc6ecf60ce8`.
  The test profile is **not** an approved product compiler/ABI.
- The full application CI `corpus tests`, Windows, macOS, Linux,
  documentation and bootstrap checks are separate verification jobs.
  Do not claim a final merge-ready status until all required final-head
  checks have completed.
- Test status: **scoped PASS; no WinUAE/QuarkTex/MiniGL/PiStorm3D
  graphical or hardware tests**.
