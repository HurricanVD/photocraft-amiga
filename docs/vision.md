# PhotoCraft AmigaOS 3.2 — Port-Vision

Die originale PhotoCraft-Vision und [Architektur](architecture.md) bleiben
fachlich maßgeblich. Das 256×256-Sparse-/COW-, Ebenen- und Dokumentmodell
wird schrittweise auf AmigaOS 3.2 (68040+, 32 MiB Fast RAM, ReAction)
portiert. C99 ist ausschließlich unter [ports/amigaos3](../ports/amigaos3/)
als eigenständige CPU-/Plattform-Implementierung zulässig.
Das ursprüngliche Rust-/Cargo-Projekt bleibt unverändert.

Rust/C99-Differentialtests und GCC/Bebbo 13.3/vamos beweisen einen
Teilumfang, keinen fertig lauffähigen Editor. ReAction, MiniGL/QuarkTex NG,
PiStorm3D, RTG und PSD-/ICC-/Compositing-Integration brauchen gesonderte
Implementierung, Rechte- und Zielsystemnachweise. Proprietärer
Dunkelkammer-Code wird nicht ohne Rechtefreigabe kopiert.

Stand 2026-10-09: ADR-0001..0003 akzeptiert; PF-SP-003 abgeschlossen,
PF-SP-002 in Arbeit, PF-SP-001 MiniGL blockiert. Die
[Original-Roadmap](roadmap.md) wird nicht überschrieben.
