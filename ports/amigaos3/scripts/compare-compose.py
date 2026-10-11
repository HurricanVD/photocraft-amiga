#!/usr/bin/env python3
"""PhotoCraft original Rust Normal-blend vs C99 subset: <=1 U8/channel."""
import sys
def read(path):
    rows=[]
    with open(path,encoding="utf-8") as f:
        for line in f:
            t=line.strip().split()
            if len(t)!=5: raise ValueError(f"invalid compositor row: {t}")
            v=tuple(int(x) for x in t[1:])
            if any(x<0 or x>255 for x in v):
                raise ValueError(f"invalid U8: {v}")
            rows.append((t[0],v))
    return rows
if len(sys.argv)!=3: sys.exit("usage: compare-compose.py rust.txt native.txt")
rust,native=read(sys.argv[1]),read(sys.argv[2])
if len(rust)!=3 or len(native)!=3:
    sys.exit("FAIL: expected three compositor cases")
for (rn,rv),(cn,cv) in zip(rust,native):
    if rn!=cn or any(abs(a-b)>1 for a,b in zip(rv,cv)):
        sys.exit(f"FAIL: {rn} original={rv} native={cv} label={cn}")
print("PASS: original PhotoCraft Rust Normal blend vs C99 within 1 U8")
