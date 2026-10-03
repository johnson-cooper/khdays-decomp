"""Render a DS 3D texture from the running game's VRAM to a PNG (development helper, PINE).

    python ps2/tools/win/tex_dump.py TEXIMAGE_PARAM PLTT_BASE out.png

Formats 1 (A3I5), 2/3/4 (palettes), 6 (A5I3), 7 (direct).  Shown over a checkerboard so alpha
is visible.  Uses the texture / texture-palette bank mappings the game has set.
"""
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pine import Pine  # noqa: E402
from ds2d_dump import sym  # noqa: E402
from PIL import Image  # noqa: E402

ELF = "build/obj/khdays-ps2/khdays-ps2.unstripped.elf"
BANK_SIZE = [0x20000, 0x20000, 0x20000, 0x20000, 0x10000, 0x4000, 0x4000, 0x8000, 0x4000]


def main():
    teximage, pltt, out = int(sys.argv[1], 16), int(sys.argv[2], 16), sys.argv[3]
    p = Pine()
    vram = sym(ELF, "kh_ds_vram")
    banks = sym(ELF, "g_view_banks")
    vofs = sym(ELF, "g_view_ofs")
    bank_ofs = [p.r32(sym(ELF, "kh_ds_vram_bank_ofs") + b * 4) for b in range(9)]

    def view_addr(view, ofs):
        m = p.r32(banks + view * 4)
        for b in range(9):
            base = p.r32(vofs + (view * 9 + b) * 4)
            if (m >> b) & 1 and base <= ofs < base + BANK_SIZE[b]:
                return vram + bank_ofs[b] + ofs - base
        return None

    def read(view, ofs, n):
        data = b""
        for i in range(0, n, 4):
            a = view_addr(view, ofs + i)
            data += struct.pack("<I", p.r32(a) if a else 0)
        return data

    fmt = (teximage >> 26) & 7
    w, h = 8 << ((teximage >> 20) & 7), 8 << ((teximage >> 23) & 7)
    ofs = (teximage & 0xffff) << 3
    pofs = pltt << (3 if fmt == 2 else 4)
    bpp = {1: 8, 2: 2, 3: 4, 4: 8, 6: 8, 7: 16}[fmt]
    tex = read(4, ofs, w * h * bpp // 8)
    pal = struct.unpack("<256H", read(5, pofs, 512)) if fmt != 7 else None
    img = Image.new("RGBA", (w, h))
    for y in range(h):
        for x in range(w):
            i = y * w + x
            if bpp == 8:
                v = tex[i]
            elif bpp == 4:
                v = (tex[i >> 1] >> ((i & 1) * 4)) & 15
            elif bpp == 2:
                v = (tex[i >> 2] >> ((i & 3) * 2)) & 3
            else:
                v = struct.unpack_from("<H", tex, i * 2)[0]
            if fmt == 1:
                c, a = pal[v & 31], (v >> 5) * 255 // 7
            elif fmt == 6:
                c, a = pal[v & 7], (v >> 3) * 255 // 31
            elif fmt == 7:
                c, a = v, 255 if v & 0x8000 else 0
            else:
                c, a = pal[v], 0 if (v == 0 and (teximage >> 29) & 1) else 255
            img.putpixel((x, y), ((c & 31) * 255 // 31, ((c >> 5) & 31) * 255 // 31, ((c >> 10) & 31) * 255 // 31, a))
    bg = Image.new("RGBA", (w, h))
    for y in range(h):
        for x in range(w):
            g = 200 if ((x >> 2) + (y >> 2)) & 1 else 140
            bg.putpixel((x, y), (g, g, g, 255))
    bg.alpha_composite(img)
    bg.resize((w * 4, h * 4), Image.NEAREST).save(out)
    print(f"fmt {fmt} {w}x{h} vram {ofs:#x} pltt {pofs:#x} -> {out}")


if __name__ == "__main__":
    main()
