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

## Implementierungs-/Testnachweis 2026-10-12 (begrenzter Scope)

- Code und CI-Hauptnachweis: `f62c0c27604cba33829c7ae928899f5c688137eb` /
  [AmigaOS Host Core #38183070642](https://github.com/HurricanVD/photocraft-amiga/actions/runs/38183070642).
- **PASS** — host C99 `pc_compose_test` sowie bestehender C99-Raster/Document-Tests,
  GCC `-O2`, ASan/UBSan.
- **PASS** — Original-Rust-`photocraft_color::blend::composite(BlendMode::Normal,...)`
  gegenüber C99 `compose_oracle` in den Fixtures `half`, `mask128`,
  `hidden`, Toleranz <=1 U8/Kanal; Log:
  `PASS: original PhotoCraft Rust Normal blend vs C99 within 1 U8`.
  Das Original-`photocraft-color`-Crate wird nur als Oracle auf dem Host gebaut,
  nicht als m68k-Laufzeit.
- **PASS** — gepinnter GCC/Bebbo 13.3 m68k-HUNK und `vamos`
  `-O0`/`-O2` im selben Workflow (Step:
  `Execute twelve m68k tests and compare with original Rust`,
  auch mit dem neuen `gcc13-vamos-compose` inklusive CPU-Test + Rust-Band/Pixeloracle).
- **Begrenzung:** nur *flache* RGBA8 Normal-Layer, layer
  `opacity * fill_opacity`, Gray8-Maske, Region-Ausgabe;
  Groups sind fail-closed `unsupported`. Keine Blend-Modes ausser Normal,
  keine HDR-/ICC-Paritaet, keine ReAction/RTG-Grafikevidenz,
  kein Produkt-Release. Der integerbasierte Alpha-Pfad ist durch diese drei
  Proben bewertet, nicht durch exhaustive Random-/Cross-Format-Oracles.
- `manual_winuae: n/a` fuer diesen portablen CPU-Spike;
  der native *GUI-/Presenter*-Integrationslauf (PF-SP-010/011) bleibt
  `required/not_run`.
- `scope_result: pass` fuer beschriebene Fixtures; `story_done: not_authorized`
  bis separates Implementation-/Final-Review und Abschlussgates.
