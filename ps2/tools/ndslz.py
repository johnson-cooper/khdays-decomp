"""Nintendo DS LZ10 / LZ11 decompression (BIOS-compatible), pure Python."""


def decompress(data):
    kind = data[0]
    size = data[1] | (data[2] << 8) | (data[3] << 16)
    pos = 4
    if size == 0:
        size = int.from_bytes(data[4:8], "little")
        pos = 8
    out = bytearray()
    if kind == 0x10:
        while len(out) < size:
            flags = data[pos]
            pos += 1
            for bit in range(8):
                if len(out) >= size:
                    break
                if flags & (0x80 >> bit):
                    b0, b1 = data[pos], data[pos + 1]
                    pos += 2
                    n = (b0 >> 4) + 3
                    disp = (((b0 & 0xF) << 8) | b1) + 1
                    for _ in range(n):
                        out.append(out[-disp])
                else:
                    out.append(data[pos])
                    pos += 1
    elif kind == 0x11:
        while len(out) < size:
            flags = data[pos]
            pos += 1
            for bit in range(8):
                if len(out) >= size:
                    break
                if flags & (0x80 >> bit):
                    b0 = data[pos]
                    ind = b0 >> 4
                    if ind == 0:
                        b1, b2 = data[pos + 1], data[pos + 2]
                        n = (((b0 & 0xF) << 4) | (b1 >> 4)) + 0x11
                        disp = (((b1 & 0xF) << 8) | b2) + 1
                        pos += 3
                    elif ind == 1:
                        b1, b2, b3 = data[pos + 1], data[pos + 2], data[pos + 3]
                        n = (((b0 & 0xF) << 12) | (b1 << 4) | (b2 >> 4)) + 0x111
                        disp = (((b2 & 0xF) << 8) | b3) + 1
                        pos += 4
                    else:
                        b1 = data[pos + 1]
                        n = ind + 1
                        disp = (((b0 & 0xF) << 8) | b1) + 1
                        pos += 2
                    for _ in range(n):
                        out.append(out[-disp])
                else:
                    out.append(data[pos])
                    pos += 1
    else:
        raise ValueError("not LZ10/LZ11 data (0x%02x)" % kind)
    return bytes(out[:size])
