"""Automatic exclusion of decomp library sources that cannot run on the EE.

A library file is judged by the text that would actually be compiled: its prepared copy
(ps2/gen/src, from prep_sources.py) when there is one.  It is excluded when

  * it contains inline assembly;
  * a DS memory/register address is still present in code after preparation (an address the
    prep rules could not map, or a DS-range constant they left alone as a non-address);
  * preparation mapped one of its accesses to a register with side effects other than the
    geometry engine (divider/sqrt, DMA/timers, IPC/card, IRQ -- build/ps2/hw_semantic.txt),
    unless the file was reviewed and listed in ps2/config/semantic_ok.txt.

ps2/config/lib_keep.txt forces a reviewed file in.
"""
import os
import re

import ps2cfg

DS_LITERAL = re.compile(r"\b0[xX]0*(4[0-9a-fA-F]{6}|5[0-9a-fA-F]{6}|6[0-9a-fA-F]{6}|7[0-9a-fA-F]{6}|27[ef][0-9a-fA-F]{4})[uUlL]*\b")
ASM = re.compile(r"\basm\b|__asm")
COMMENT = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)
STRING = re.compile(r'"(?:\\.|[^"\\])*"')


def _list(name):
    p = os.path.join(ps2cfg.PS2, "config", name)
    out = set()
    if os.path.exists(p):
        for line in open(p):
            line = line.split("#", 1)[0].split()
            if line:
                out.add(line[0])
    return out


_KEEP = None
_OK = None
_SEMANTIC = None


def _semantic():
    global _SEMANTIC
    if _SEMANTIC is None:
        _SEMANTIC = {}
        p = os.path.join(ps2cfg.ROOT, "build", "ps2", "hw_semantic.txt")
        if os.path.exists(p):
            for line in open(p):
                parts = line.rstrip("\n").split("\t")
                if len(parts) >= 3 and parts[2] != "geometry engine":
                    _SEMANTIC.setdefault(parts[0], parts[2])
    return _SEMANTIC


def hw_reason(relpath):
    """Why a library file must not be compiled for the EE, or None."""
    global _KEEP, _OK
    if not relpath.startswith("libs/"):
        return None
    if _KEEP is None:
        _KEEP = _list("lib_keep.txt")
        _OK = _list("semantic_ok.txt")
    if relpath in _KEEP:
        return None
    prepared = os.path.join(ps2cfg.PS2, "gen", "src", relpath)
    path = prepared if os.path.exists(prepared) else os.path.join(ps2cfg.ROOT, relpath)
    text = STRING.sub('""', COMMENT.sub(" ", open(path, encoding="utf-8", errors="replace").read()))
    if ASM.search(text):
        return "inline asm"
    m = DS_LITERAL.search(text)
    if m:
        return m.group(0)
    why = _semantic().get(relpath)
    if why and relpath not in _OK:
        return why
    return None
