# Session-Abschluss: PF-SP-002 Atomisierung / PhotoCraft Port

- Stand: 2026-10-11 (GitHub-Snapshot; kein Produktrelease)
- Konsumierendes Repository: `HurricanVD/photocraft-amiga`
- Zentraler Prozess: `HurricanVD/vd-amiga-dev-process`, Version `0.2.1`, PF-Prefix `active` gemaess Registry-Merge `1c32299`
- Story: `PF-SP-002` als zu breiter Parent `blocked` nach `atomization_report: split_required`
- Massgebliche Quellen: `docs/reviews/PF-SP-002-atomization-report-2026-10-11.md`, `docs/backlog.md`, `docs/stories/PF-SP-002.md`, `docs/development-process.md`

## Eingecheckte Governance-Korrektur

[PhotoCraft PR #6](https://github.com/HurricanVD/photocraft-amiga/pull/6) trennt die urspruengliche Sammelstory in einzeln nachverfolgbare Scopes, dokumentiert die historische Implementierungsabweichung und konsolidiert Bootstrap-Status, Backlog und Canonical-TNs. PF-SP-004/005/006 sind `refining`, PF-SP-007/008/009 und PF-TD-001 `draft`. Keine dieser Teilstories ist durch die Atomisierung selbst auf `ready`/`in_progress`/`done` angehoben.

Das private VD-Gegenstueck ist [PR #20](https://github.com/HurricanVD/vd-amiga-dev-process/pull/20): Registry-Pin-/Consumer-Snapshot, Bash/PowerShell-/Fixture-/Full-Snapshot-Verifikation, Status-/Archiv-Paritaet und separate Gate-Evidenz. Diese Implementierung fuehrt den Consumer-Code nicht als private Validator-Autoritaet aus. Die alte VD-Prozessbaseline `0566a39` zeigt historisch `PF reserved`; **der aktuelle `active`-Status stammt ausschliesslich aus Registry-Merge `1c32299`**.

Die statischen Checks gestatten spaetere regulaere Lifecycle-Statuswechsel, fordern aber fuer `ready` dokumentiertes Refinement, **initiales und finales** Architekturreview und DoR, fuer `in_progress` ein Implementation-Gate sowie fuer `done` das finale Review-Approval. Widersprechende doppelte Gate-Marker sollen abgewiesen werden. Dies **ersetzt keinen eigenstaendigen** fachlichen Review des Nachweises. Archive-/Backlog-Synchronitaet und Schutz vor doppelten IDs werden getestet.

## Offene Implementierungs-PRs: nicht mergefreigegeben

| PR | Child-Story | Technischer Scope | Status |
|---|---|---|---|
| [#3](https://github.com/HurricanVD/photocraft-amiga/pull/3) | PF-SP-004 | Flat Layer: Order, Opacity, Rust/C99-Diff | Draft; getrennte DoR-/Review-Gates noch offen |
| [#4](https://github.com/HurricanVD/photocraft-amiga/pull/4) | PF-SP-005 | Nested Groups, globale IDs, Clone/COW | Draft; Codex: `Document::shift` fuer Nested-Siblings und rekursiver `layer_count` entsprechen noch nicht vollstaendig Rust, Test-/Artefaktzuordnung pruefen |
| [#5](https://github.com/HurricanVD/photocraft-amiga/pull/5) | PF-SP-006 | Gray8 LayerMask, Attach/Detach, Sparse-COW | Draft; Codex: Rust/C99-Masken-Differential fehlt im dedizierten `main`-Workflow, Testjob/Artefakt noch historisch PF-SP-002 benannt |

Alle drei PR-Branches sind gestapelt und zuletzt ohne GitHub-Text-Mergekonflikt abgleichbar; *mergeable* allein bedeutet aber keine fachliche Freigabe. Testakten PF-TN-005/006/007 sind den jeweiligen Child-IDs zugeordnet. Aeltere Rust-Originaltests, C99/Sanitizer und m68k GCC13/vamos O0/O2 PASS betreffen **nur** die damals eingefrorenen Testcommits, keine automatisch neu freigegebenen Endheads. Ein PR, fuer den final-head CI noch laeuft, hat keinen behaupteten Gesamterfolg.

## Naechste Session — Pflichtfolge

1. Nur Governance-PR #6 und privaten Prozess-PR #20 nach gruenen Checks, separatem Delta-Review und aktueller SHA-Pruefung mergen. Alte PR-Findings duerfen nicht als stillschweigend genehmigt gelten.
2. PF-SP-004 aus `refining` durch initialen + finalen `pre_ready_final`-Architektur-/DoR- und Implementation-Gate begleiten. Bereits implementierten Scope kritisch reviewen und genaue heutige Tests auf finalem SHA pruefen.
3. Erst dann PR #3 (PF-SP-004); danach PR #4 (PF-SP-005) inklusive rekursiver Rust-Gruppensemantik und eigener Tests; danach PR #5 (PF-SP-006) mit konsistentem Main-Mask-Orakel und eigenem Artefaktnamen abnehmen.
4. Weitere Childs PF-SP-007..009 / PF-TD-001 in `draft` belassen. WIP-Limit zentral hoechstens zwei aktive Stories.
5. Offene AmigaOS-Laufzeit-/MiniGL-/WinUAE-/Produktcompiler-/IP-/Release-Gates separat nachweisen. Kein Produktbuild oder Editor freigegeben.

## Governance- und Abnahmegrenzen

- Keine eigenmaechtige `ready`-/`done`-Promotion, kein erfundenes finales Approval.
- Ein automatischer Review-Finding-Loop oder gruenes Snapshot-CI ist keine Lizenz-/IP-/Produktfreigabe.
- Falls Checks, Codex-Review oder Branch-Integration noch offen sind, bleiben die zugehoerigen PRs offen. Status bei erneuter Arbeit anhand exakter PR-SHA und GitHub Actions aktualisieren.
