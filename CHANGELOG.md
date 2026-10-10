# Changelog

Alle nennenswerten Produkt-, Prozess-Overlay- und Dokumentationsaenderungen
fuer dieses App-Repository werden hier gepflegt.

## [Unreleased]

- Kanonische PF-TN-002/004-Baseline-Tests historisch markiert; Live-Statusquellen widerspruchsfrei. Das Bootstrap-Gate validiert explizit aktuelle Backlog-Spalte/Sektion und verlangt vorhandene DoR-/Implementation-/Review-Gate-Marker vor Story-Promotion; die Marker werden durch diese Korrektur nicht selbst erteilt.
- ID-Vergabe beim Refinement (`draft|refining`) von DoR-Promotion nach `ready` getrennt; PF-Statusquellen konsolidiert. Die Statuspruefungen erlauben nach jeweiligen Gates den regulären Child-Lifecycle, ohne ein Gate selbst freizugeben.
- PF-SP-002 nach VD-v0.2.1-Atomisierung `split_required` als historischer Sammelspike eingefroren (`blocked`). Eigenstaendige PF-SP-004..009 und PF-TD-001 unter `refining|draft` mit Akzeptanzkriterien, Nicht-Zielen, Dependencies und Traceability auf PF-TN-005..007 bzw. kuenftige Tests angelegt. Die bereits offenen PRs #3–#5 gelten nicht als genehmigte Child-Implementierung; kein Produktcode, kein Release, kein rückwirkendes DoR/Implementation-Gate.

- Kanonisches VD-Amiga-Prozess-Overlay v0.2.1 für PhotoCraft-Fork initialisiert; Rust-Originaldokumentation und Quellen bewahrt. PF-Stories/Tests bleiben ihrer Evidenz nach klassifiziert.

