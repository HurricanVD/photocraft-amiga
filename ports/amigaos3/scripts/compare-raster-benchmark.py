#!/usr/bin/env python3
"""Measure the exact same fixture on pinned linear and indexed PcRaster sources."""
import csv
import os
import statistics
import subprocess
import sys
from pathlib import Path

SIZES = (64, 256, 1024)
REPETITIONS = 3
FIELDS = ("tiles", "reads", "writes", "insert_seconds", "read_seconds",
          "overwrite_seconds", "checksum")

def sample(executable, size):
    proc = subprocess.run([executable, str(size)], check=True,
                          capture_output=True, text=True, timeout=120)
    parts = proc.stdout.strip().split(",")
    if len(parts) != len(FIELDS) or len(proc.stdout.strip().splitlines()) != 1:
        raise RuntimeError("unexpected benchmark row: " + repr(proc.stdout))
    ints = [int(v) for v in parts[:3]]
    times = [float(v) for v in parts[3:6]]
    checksum = int(parts[6])
    if ints != [size, 150000, 30000] or checksum != 1050000:
        raise RuntimeError("fixture/digest mismatch: " + repr(proc.stdout))
    if not all(t >= 0 for t in times):
        raise RuntimeError("negative CPU clock")
    return times

def main():
    if len(sys.argv) != 6:
        raise SystemExit("usage: compare-raster-benchmark.py baseline_binary indexed_binary csv_path markdown_path baseline_commit")
    linear, indexed, csv_file, md_file, commit = sys.argv[1:]
    if len(commit) != 40 or any(ch not in "0123456789abcdef" for ch in commit):
        raise SystemExit("baseline must be a pinned full commit SHA")
    rows = []
    for size in SIZES:
        runs = {"linear": [], "indexed": []}
        for n in range(REPETITIONS):
            order = (("linear", linear), ("indexed", indexed)) if n % 2 == 0 else (("indexed", indexed), ("linear", linear))
            for label, executable in order:
                runs[label].append(sample(executable, size))
        for label in ("linear", "indexed"):
            medians = [statistics.median(v[i] for v in runs[label]) for i in range(3)]
            rows.append([size, label, REPETITIONS, *[f"{v:.9f}" for v in medians]])

    Path(csv_file).parent.mkdir(parents=True, exist_ok=True)
    with open(csv_file, "w", encoding="utf-8", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(("tiles", "variant", "repetitions", "insert_s", "read_s", "overwrite_s"))
        writer.writerows(rows)

    header = ("# PF-TD-001 – linear vs. indexed tile benchmark\n\n"
              f"- Historical baseline commit: `{commit}`, original `pc_raster.c` fetched via `git show`.\n"
              "- Identical benchmark source, compiler flags, tile coordinates and lookup/write requests.\n"
              "- U8 Gray, 256x256 tiles, reverse-order insertion, 150,000 reads and 30,000 existing-tile writes.\n"
              f"- {REPETITIONS} repetitions per variant and tile count; reported median CPU seconds, not wall time.\n"
              "- Ratios are historical linear time divided by indexed time; >1 means the indexed variant was faster.\n"
              "- Diagnostic only: runner CPU variability, memory profile and insertion order limit generalization.\n\n"
              "| Tiles | Linear insert (s) | Indexed insert (s) | Insert ratio | Linear read (s) | Indexed read (s) | Read ratio | Linear overwrite (s) | Indexed overwrite (s) | Overwrite ratio |\n"
              "|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|\n")
    lines=[header]
    for size in SIZES:
        a=next(r for r in rows if r[0]==size and r[1]=="linear")
        b=next(r for r in rows if r[0]==size and r[1]=="indexed")
        def ratio(l, i):
            return "n/a" if float(i)<=0 else f"{float(l)/float(i):.2f}x"
        lines.append(f"| {size} | {float(a[3]):.6f} | {float(b[3]):.6f} | {ratio(a[3],b[3])} | "
                     f"{float(a[4]):.6f} | {float(b[4]):.6f} | {ratio(a[4],b[4])} | "
                     f"{float(a[5]):.6f} | {float(b[5]):.6f} | {ratio(a[5],b[5])} |\n")
    lines.append("\nThis host-only comparison is not a 32 MiB WinUAE acceptance or an editor-performance guarantee.\n")
    Path(md_file).write_text("".join(lines), encoding="utf-8")
    print("".join(lines))

if __name__ == "__main__":
    main()
