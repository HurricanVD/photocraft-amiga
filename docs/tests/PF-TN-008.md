# PF-TN-008 — PF-SP-007 planned acceptance

- Story: [PF-SP-007](../stories/PF-SP-007.md)
- Date: 2026-10-12
- Status: `planned / not_run`. No implementation, build, runtime or manual native test PASS asserted.
- Scope: CPU Normal blend RGBA8, opacity and Gray8-mask reference with per-band parity and m68k O0/O2.

## Proposed verification

1. Fixture-positive and negative/boundary cases and repeatability.
2. Host C99 smoke/ASan/UBSan and unchanged original PhotoCraft Rust oracle for OS-free semantics; m68k/GCC13 -O0 and -O2 HUNK/vamos for target-portable code.
3. If GUI/RTG, direct manual AmigaOS 3.2 + WinUAE window/gadget visual and interaction test, exact binary SHA, hardware/RTG settings, screenshots and logs.
4. Invalid input, allocator fault, cleanup, rollback and RAM peak; no floating-point/byte-order assumptions outside documented contract.
5. No automated WinUAE UI/broker/capture flows while `disabled_by_process`.

- `test_evidence: not_run`
- `runtime_evidence: none`
- `release_authorization: no`
