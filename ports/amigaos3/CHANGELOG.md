# PhotoCraft AmigaOS port changelog

This is a **port-local** engineering changelog. Upstream PhotoCraft's version
and top-level changelog remain unchanged.

## 2026-10-09 — CPU-core test evidence / PF-SP-003

- Added an opt-in GCC/Bebbo 13.3.0 target-build lane patterned after
  vxplatform's GCC13 provider profile, with explicit compiler/root validation.
- Verified six m68k Amiga HUNK binaries at O0 and O2 using pinned vamos;
  C99 core/oracle fixtures match the original Rust photoCraft crates.
- Diagnosed an excessive-memory regression fixture and bounded the live
  256×256 RGBA8 tiles without removing any of 169 coordinate combinations.
- Host C99/sanitizer and original Rust-versus-C differential tests pass.
- Scope is infrastructure/core fixture testing; no editor, GPU backend, GUI,
  full document model or distribution artifact is released.
