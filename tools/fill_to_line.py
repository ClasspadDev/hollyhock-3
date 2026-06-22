#!/usr/bin/env python
import sys

file_name = "sdk/os/functions/nolto/symbols.def"

input_line = int(sys.argv[1], base=0)

if input_line < 0xBEEF0001:
    print("Invalid input line. Must be greater than 0xBEEF0001")
    sys.exit(1)
    
if input_line < 0xC0FFE001:
    line = (input_line - 0xBEEF0001) // 2
    print(f"libc source: impl.c:{line}")
    sys.exit(0)

if input_line < 0xDEAD0001:
    line = (input_line - 0xC0FFE001) // 2
    print(f"gint source: syscalls.S:{line}")
    sys.exit(0)

line = (input_line - 0xDEAD0001) // 2

print(f"{file_name}:{line}: ", end="")
with open(file_name, "r") as f:
    print(f.readlines()[line - 1])