# PF-TN-005 — Flat Document shift and layer opacity parity

- Story: [PF-SP-002](../stories/PF-SP-002.md), **in_progress**.
- Started: 2026-10-10.
- Decision basis: [accepted ADR-0002](../adr/ADR-0002-photocraft-core-and-memory-contract.md) and [ADR-0003](../adr/ADR-0003-amigaos-toolchain-and-verification.md).
- Scope: experimental flat-raster `PcDocument` metadata. This is **not** the product `doc::Document`, an approved C ABI or a native GUI.
- Initial verification status: **pending** — do not mark PASS until the matching branch CI jobs are inspected.

## Verification matrix

| Gate | Command / fixture | Expected |
|---|---|---|
| C99 document host fixtures | `make -C ports/amigaos3 host-test` | Positive/negative `shift`, stable IDs, name/pointer ownership, rejection of out-of-range shifts, opacity and fill opacity defaults/bounds, NaN/INF rejection, clone metadata independence |
| Host sanitizer | `make -C ports/amigaos3 host-core-sanitize` | No ASan/UBSan diagnostics on the new document tests |
| Original Rust `doc` test suite | `cargo test -p photocraft-doc --locked --lib` | Rust original unchanged and passing |
| Flat-document Rust-vs-C oracle | Rust `tests/rust_oracle ... -- document` against C `tests/document_oracle.c` | Byte-for-byte identical order/shift/metadata snapshot fixture output |
| GCC/Bebbo 13.3 m68k | Existing `gcc13-build-typed`, `gcc13-check-typed-hunks` | Real HUNK for the extended `pc_document_test` at O0 and O2 |
| `vamos` runtime | Existing `gcc13-vamos-typed-tests` | Extended C document test exits successfully at O0 and O2 |

## Out-of-scope and acceptance limits

This test covers no group insertion, masks, blending, compositing, ICC,
PSD save/load, command routing, undo, WinUAE/MiniGL/QuarkTex NG or PiStorm3D.
No product compiler, shipping rights or full-port acceptance is implied.
`PF-SP-002` stays `in_progress`; full original Rust document semantics
require further implementation and verification.

## Evidence status

CI references, exact commit hashes, executed steps, results and failures will
be added only after the GitHub Actions job finishes. For a runtime blocker,
record the actual blocking environment and rerun plan, not a synthetic PASS.
