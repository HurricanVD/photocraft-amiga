# ADR-0003: PhotoCraft AmigaOS compiler, target and verification profiles

- ADR-ID: `ADR-0003`
- Status: `proposed`
- Created / updated: 2026-10-09
- Lifecycle action: `propose`
- Decision depth: `far_reaching` — production compiler choice, ABI, target and build/release gate
- Decision authority: user / project owner; approval `pending`
- Scope: `ports/amigaos3` (fork only)
- Existing accepted basis: ADR-0001, VD Amiga process 0.2.1
- Related: HurricanVD/vxplatform ADR-0014 `gcc13` opt-in provider profile; PF-SP-003 / PF-TN-003

## Context

PF-SP-003 demonstrated GCC/Bebbo **13.3.0**, producing six HUNK programs
(O0/O2, 68020 soft-float) which executed under pinned vamos; defined C99
core fixtures were byte-identical to original PhotoCraft Rust outputs.
This is a **test/compatibility lane**, not an approved product compiler or
an assertion that MiniGL/ReAction builds and behaves on real hardware.

Separately, vxplatform currently uses GCC/Bebbo 16.1.1b as product default,
with GCC13 as **opt-in**, explicit `GCC_BEBBO_ROOT` and distinct provider
artifacts. The central VD process documents a staged 16.1 reference
migration. We must not silently force another project's compiler policy
onto the PhotoCraft fork.

## Proposed profiles (decision not final)

| Profile | Toolchain | Purpose | Required evidence |
|---|---|---|---|
| `host` | native C99 + ASan/UBSan; original Rust | fast deterministic fixtures | host tests and Rust/C differential |
| `gcc13-vamos` | Bebbo 13.3.0 opt-in | executable 68k regression baseline | exact compiler/root, HUNK, O0/O2, vamos, Rust C oracle |
| `gcc16-evaluation` | exact pinned GCC/Bebbo 16.1.x bundle | candidate future product compiler and vxplatform consumer integration | independent ABI/size/performance/target evidence |
| `minigl-winuae` | chosen compatible C compiler and separately obtained MiniGL SDK | ReAction/RTG/MiniGL UI + runtime | manual WinUAE/QuarkTex NG and hardware tests |

1. Never infer a product compiler from a passing standalone C99 test.
   Keep GCC13 and GCC16 artifacts/archives segregated, including libgcc,
   libstdc++ and other runtime libraries when C++ becomes relevant.
2. Default native CPU baseline remains `68040+` and documented RAM
   baseline `32 MiB Fast RAM` from port metadata. 68020/soft-float HUNK
   tests are a *compatibility* subset, not a product-target downgrade.
3. Keep explicit `GCC_BEBBO_ROOT` and fail-closed compiler/target/version
   validation; pin image digest, archive provenance and flags in CI reports.
4. Run the same algorithm fixtures under `-O0` and `-O2`. Validate actual
   m68k HUNK and process exit codes; a host test alone is insufficient.
5. `vamos` is a headless CPU/runtime test and does **not** prove the
   miniGL.library, Workbench, Picasso96/CGX, QuarkTex NG or PiStorm3D.
6. WinUAE automation remains `disabled_by_process` until centrally
   re-enabled. Manual target testing is required at the graphics stage.
7. For actual product and release builds, a separate scope decision must
   identify exact compiler profile, NDK 3.2, CPU/FPU flags, library ABI,
   runtime, distribution/IP constraints and rollback. No implicit promotion.

## Alternatives

- Adopt GCC13 as product default immediately — not justified by current
  isolated test evidence.
- Adopt vxplatform GCC16 automatically — not yet validated for PhotoCraft's
  linked product and renderer.
- Use unfixed `latest` compiler or mixed object archives — rejected.

## Current evidence and open review

- [CI #37915607402](https://github.com/HurricanVD/photocraft-amiga/actions/runs/37915607402):
  C99/Sanitizers, real Rust fixtures and six GCC13/vamos HUNK runs PASS.
- Prefix `PF` remains `reserved`, full central bootstrap pending.
- Exact eventual production compiler and SDK selection **requires user
  sign-off**. Until then PF-SP-002/other experimental spikes may validate
  additional compiler profiles, but must not claim product acceptance.

**Status remains proposed** pending explicit decision and final pre-ready review.
