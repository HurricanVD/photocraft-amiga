# ADR-0003: PhotoCraft AmigaOS compiler, target and verification profiles

- ADR-ID: `ADR-0003`
- Status: `accepted`
- Created: 2026-10-09
- Accepted: 2026-10-09
- Last updated: 2026-10-09
- Lifecycle action: `accept`
- Decision depth: `far_reaching` — production compiler choice, ABI, target and build/release gate
- Decision authority: project owner; approval `approved` by explicit instruction "adr freigeben, commit und merge nach main. push für vd-amiga-dev-process" on 2026-10-09
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

## Accepted profile separation (product-compiler selection deliberately deferred)

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
- Exact eventual production compiler and SDK selection **still requires a separate owner decision** before a release or application target is approved. Approval of ADR-0003 does NOT choose GCC13 as the default, and it does NOT inherit vxplatform's GCC16 default. Experimental spikes may evaluate both without claiming product acceptance.
- The defined `host`, `gcc13-vamos`, `gcc16-evaluation` and `minigl-winuae` profiles, regression ladder and isolation rules are now the **accepted architecture**. Compiler selection for the full native product is an explicitly deferred decision, not an unspecified silent default.

**Accepted** on 2026-10-09 by explicit project owner authorization. Future changes to production compiler policy, toolchain version, ABI boundaries or release gates require a separate recorded decision/review.

Review record: [ADR-0002/0003 joint approval audit](../../ports/amigaos3/docs/reviews/ADR-0002-0003-approval-2026-10-09.md).
