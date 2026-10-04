#!/usr/bin/env python3
"""Create the PS2 port's game data from YOUR OWN Kingdom Hearts 358/2 Days ROM.

    python ps2/tools/make_ps2data.py <rom.nds> [<output dir, default ps2data>]

Copy the resulting folder next to khdays-ps2.elf (e.g. mass:/KHDAYS/ps2data/).  Nothing here is
distributable: the output is derived from your ROM.

ps2data/khdays.pak  (version 1)
    0x000  header (64 bytes, little endian)
             u32 magic "KHPK", u32 version, u32 fat_offset, u32 fat_size, u32 fnt_offset,
             u32 fnt_size, u32 file_count, char gamecode[4], u32 data_offset, u32 reserved[7]
    ....   FNT, verbatim from the ROM (directory/file names; offsets inside it are FNT-relative)
    ....   FAT, rewritten: file i -> (start, end) of its data inside this pack
    ....   file data, each file 2 KiB aligned (sector aligned on USB/HDD/CD)
File ids are the ROM's, so the game's NitroFS code works unchanged.  ARM9/ARM7 program code and
the overlay binaries are NOT copied: the PS2 executable contains the game code.

Later format versions will add PS2-preprocessed resources (textures, audio) next to the pack;
the game logic keeps reading the original structures from here.
"""
import hashlib
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ndsfs import Rom  # noqa: E402

ALIGN = 2048
KNOWN = {
    "YKGE": "Kingdom Hearts 358/2 Days (USA)",
    "YKGP": "Kingdom Hearts 358/2 Days (Europe)",
    "YKGJ": "Kingdom Hearts 358/2 Days (Japan)",
}


def align(n, a=ALIGN):
    return (n + a - 1) // a * a


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 1
    rom = Rom(argv[1])
    out_dir = argv[2] if len(argv) > 2 else "ps2data"
    code = rom.gamecode
    if code not in KNOWN:
        print(f"error: {argv[1]} is not Kingdom Hearts 358/2 Days (game code {code})", file=sys.stderr)
        return 1
    print(f"ROM: {KNOWN[code]} [{code}], {len(rom.files)} files")
    if code != "YKGP":
        print("note: the decompilation is of the European release; other regions' data is used as-is. "
              "Report anything that fails to load.")

    fnt = rom.read(rom.fnt_off, rom.fnt_size)
    nfat = len(rom.fat)
    hdr_size = 64
    fnt_off = hdr_size
    fat_off = align(fnt_off + len(fnt), 16)
    fat_size = nfat * 8
    data_off = align(fat_off + fat_size)

    os.makedirs(out_dir, exist_ok=True)
    path = os.path.join(out_dir, "khdays.pak")
    tmp = path + ".tmp"
    fat = [(0, 0)] * nfat
    pos = data_off
    sha = hashlib.sha1()
    with open(tmp, "wb") as f:
        f.write(b"\0" * data_off)
        for fid in range(nfat):
            if fid not in rom.files:      # overlay binaries: code lives in the ELF
                continue
            data = rom.file_data(fid)
            f.seek(pos)
            f.write(data)
            sha.update(data)
            fat[fid] = (pos, pos + len(data))
            pos = align(pos + len(data))
        total = pos
        f.truncate(total)
        f.seek(0)
        f.write(struct.pack("<7I4sI7I", 0x4b50484b, 1, fat_off, fat_size, fnt_off, len(fnt), len(rom.files),
                            code.encode(), data_off, *([0] * 7)))
        f.seek(fnt_off)
        f.write(fnt)
        f.seek(fat_off)
        for s, e in fat:
            f.write(struct.pack("<II", s, e))
    os.replace(tmp, path)
    with open(os.path.join(out_dir, "files.tsv"), "w", encoding="utf-8", newline="\n") as f:
        f.write("file_id\tpack_start\tpack_end\tbytes\tpath\n")
        for fid in range(nfat):
            start, end = fat[fid]
            name = rom.files.get(fid, "<overlay in ELF>")
            f.write(f"{fid}\t0x{start:08x}\t0x{end:08x}\t{end - start}\t{name}\n")
    with open(os.path.join(out_dir, "info.txt"), "w") as f:
        f.write(f"source: {KNOWN[code]} [{code}]\nfiles: {len(rom.files)}\npack bytes: {total}\n"
                f"content sha1: {sha.hexdigest()}\nformat: khdays.pak version 1\n")
    print(f"wrote {path}: {total / 1048576:.1f} MiB")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
