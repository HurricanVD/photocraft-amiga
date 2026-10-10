# PF-TN-006 — Nested raster groups and deep-COW document tree

- Date: 2026-10-10
- Story: `PF-SP-002` (`in_progress`)
- Branch: `feature/pf-sp-002-group-tree`, **stacked on PR #3** (flat-layer/opacity increment)
- Status: `pending` until CI on exact group branch commit completes.
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

## Recorded evidence

Pending: exact source commit, GitHub Actions run IDs, host/original Rust
results, m68k HUNK/vamos run and reviewer conclusion. Never infer PASS from
earlier PF-TN-005 CI.
