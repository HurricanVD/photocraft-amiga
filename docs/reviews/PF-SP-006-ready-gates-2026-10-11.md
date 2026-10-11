# PF-SP-006 — Initial-/Pre-ready-/DoR-Review (2026-10-11)
- refinement_triage: pass; atomization: keep_single_story fuer Gray8-Masken COW/default/attach/detach ohne Compositor/PSD.
- arch_review.initial: pass; Original-Rust LayerMask::reveal_all/hide_all und fixed density=1 feather=0; akzeptierte ADR-0001/0002/0003.
- arch_review.pre_ready_final: pass; im aktualisierten P0-Sicherheitsmodell muss jedes Mask-Add das globale PcRaster-Claim nutzen und Detach den Owner vor Pointer-Rueckgabe an Caller wieder freigeben.
- Separate Clone-Ansprueche fuer Raster und Masken, keine Zwei-Doc-Aliase und kein Clone-Pointer Re-Append; negative Koordinaten und Gray8-Format guard.
- dor_check: pass; Testmatrix Host/ASan/UBSan, Rust LayerMask-Oracle, echte GCC13/vamos O0/O2 auf NEUEM finalen PR5-Head.
- Umsetzung soll der Implementation-Gate nach aktueller WIP-Pruefung folgen; keine rueckdatierte Freigabe der historischen PR5-Code-Commits.
- IP/Third-Party: reference_only, kein Copy aus proprietaeren Dunkelkammer-Repos; OS/NDK/ReAction/MiniGL fuer OS-freien C99-Spike n/a.
- Review-Typ: sequenzieller Quellen-Audit, keine behauptete externe menschliche APPROVE-Submission.
