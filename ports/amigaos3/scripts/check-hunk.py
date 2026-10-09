#!/usr/bin/env python3
"""Validate Amiga HUNK_HEADER magic, not just that GCC returned exit code 0."""
from pathlib import Path
import sys

if len(sys.argv) < 2:
    raise SystemExit("usage: check-hunk.py executable ...")
for filename in sys.argv[1:]:
    data = Path(filename).read_bytes()
    magic = int.from_bytes(data[:4], "big") if len(data) >= 4 else 0
    if magic != 0x000003F3:
        raise SystemExit(f"FAIL: not an Amiga HUNK executable: {filename} "
                         f"(magic 0x{magic:08x})")
    print(f"PASS: Amiga HUNK executable: {filename} ({len(data)} bytes)")
