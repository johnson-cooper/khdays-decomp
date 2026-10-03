"""Minimal PCSX2 PINE client (development helper): read EE memory from scripts.

    python ps2/tools/win/pine.py r32 0x74e6d4          # one word
    python ps2/tools/win/pine.py dump 0xacb21c 12      # hex words

PINE listens on 127.0.0.1:28011 (Settings > Advanced > PINE).  Protocol: request =
u32 total size, u8 opcode, args; reply = u32 total size, u8 status (0 = ok), payload.
"""
import socket
import struct
import sys

OP_READ8, OP_READ16, OP_READ32, OP_READ64 = 0, 1, 2, 3


class Pine:
    def __init__(self, port=28011):
        self.s = socket.create_connection(("127.0.0.1", port), timeout=5)

    def _call(self, op, payload, reply_len):
        msg = struct.pack("<IB", 5 + len(payload), op) + payload
        self.s.sendall(msg)
        want = 5 + reply_len
        buf = b""
        while len(buf) < want:
            chunk = self.s.recv(want - len(buf))
            if not chunk:
                raise IOError("PINE connection closed")
            buf += chunk
        size, status = struct.unpack_from("<IB", buf)
        if status != 0:
            raise IOError("PINE error %d" % status)
        return buf[5:]

    def r32(self, addr):
        return struct.unpack("<I", self._call(OP_READ32, struct.pack("<I", addr), 4))[0]

    def read_bytes(self, addr, nbytes):
        """Read an aligned EE-memory range with batched PINE Read32 commands."""
        if (addr & 3) or (nbytes & 3):
            raise ValueError("read_bytes address and size must be 4-byte aligned")
        out = bytearray()
        words_left = nbytes // 4
        while words_left:
            count = min(words_left, 100000)  # 400005-byte reply, below PINE's 450000 limit
            payload = bytearray(count * 5)
            for i in range(count):
                struct.pack_into("<BI", payload, i * 5, OP_READ32, addr + (len(out) // 4 + i) * 4)
            self.s.sendall(struct.pack("<I", 4 + len(payload)) + payload)
            want = 5 + count * 4
            buf = b""
            while len(buf) < want:
                chunk = self.s.recv(want - len(buf))
                if not chunk:
                    raise IOError("PINE connection closed")
                buf += chunk
            size, status = struct.unpack_from("<IB", buf)
            if size != want or status != 0:
                raise IOError("PINE batch read failed (size %d, status %d)" % (size, status))
            out.extend(buf[5:])
            words_left -= count
        return bytes(out)

    def r16(self, addr):
        return struct.unpack("<H", self._call(OP_READ16, struct.pack("<I", addr), 2))[0]

    def r8(self, addr):
        return self._call(OP_READ8, struct.pack("<I", addr), 1)[0]

    def s32(self, addr):
        v = self.r32(addr)
        return v - (1 << 32) if v & 0x80000000 else v


def fmt(v):
    return "(%.1f, %.1f, %.1f)" % v if v else "None"


def main():
    p = Pine()
    cmd = sys.argv[1]
    addr = int(sys.argv[2], 0) if len(sys.argv) > 2 and sys.argv[2][:1].isdigit() else 0
    if cmd == "pos":
        print(player_pos(p))
        return
    if cmd == "walk":                      # walk <Key> <ms>: hold a key, report the displacement
        import subprocess
        a = player_pos(p)
        subprocess.run(["powershell", "-NoProfile", "-ExecutionPolicy", "Bypass", "-File",
                        "ps2/tools/win/pcsx2_key.ps1", "-KeyList", sys.argv[2], "-HoldMs", sys.argv[3]],
                       capture_output=True)
        b = player_pos(p)
        print("from %s to %s" % (fmt(a), fmt(b)))
        return
    if cmd == "r32":
        print("0x%08x" % p.r32(addr))
    elif cmd == "dump":
        n = int(sys.argv[3], 0)
        print(" ".join("%08x" % p.r32(addr + 4 * i) for i in range((n + 3) // 4)))
    elif cmd == "dumpbin":
        n = int(sys.argv[3], 0)
        path = sys.argv[4]
        with open(path, "wb") as f:
            f.write(p.read_bytes(addr, n))
        print("wrote %d bytes to %s" % (n, path))



def player_pos(p, nm="build/obj/khdays-ps2/khdays-ps2.unstripped.elf"):
    """Field player position (fx32 x, y, z) or None; resolves data_ov022_020b2e78 from the ELF."""
    import subprocess
    out = subprocess.run([r"C:\Users\yvnem\AppData\Local\ps2build\toolchain\ee\bin\mips64r5900el-ps2-elf-nm.exe", nm],
                         capture_output=True, text=True).stdout
    g = int([l.split()[0] for l in out.splitlines() if l.endswith(" data_ov022_020b2e78")][0], 16)
    base = p.r32(g + 4)
    if not base:
        return None
    entry = p.r32(base + 4)
    if not entry:
        return None
    actor = p.r32(entry + 0x20)
    return tuple(p.s32(actor + 0x48c + 4 * i) / 4096.0 for i in range(3))


if __name__ == "__main__":
    main()
