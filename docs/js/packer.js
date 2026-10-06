const ALIGN_DATA = 2048;
const KHPK_MAGIC = 0x4b50484b;

export function align(value, boundary) {
  return Math.ceil(value / boundary) * boundary;
}

function readAscii(bytes, start, length) {
  let out = "";
  for (let i = 0; i < length; i++) {
    const c = bytes[start + i];
    if (!c) break;
    out += String.fromCharCode(c);
  }
  return out;
}

function ensureRange(offset, size, total, label) {
  if (!Number.isInteger(offset) || !Number.isInteger(size) || offset < 0 || size < 0 || offset > total || size > total - offset) {
    throw new Error(`${label} points outside the ROM.`);
  }
}

function parseNamedFileIds(fntBytes, fatEntries) {
  const view = new DataView(fntBytes.buffer, fntBytes.byteOffset, fntBytes.byteLength);
  const named = new Set();
  const visiting = new Set();
  const visited = new Set();

  function walk(dirId) {
    const idx = dirId & 0x0fff;
    const ent = idx * 8;
    if (ent + 8 > fntBytes.byteLength) throw new Error("NitroFS directory table is truncated.");
    if (visiting.has(dirId)) throw new Error("NitroFS directory table contains a cycle.");
    if (visited.has(dirId)) return;

    visiting.add(dirId);
    const subOff = view.getUint32(ent, true);
    let fid = view.getUint16(ent + 4, true);
    let pos = subOff;
    if (pos >= fntBytes.byteLength) throw new Error("NitroFS directory entry points outside the FNT.");

    while (true) {
      if (pos >= fntBytes.byteLength) throw new Error("NitroFS directory listing is truncated.");
      const marker = fntBytes[pos++];
      if (marker === 0) break;
      const nameLen = marker & 0x7f;
      if (nameLen === 0 || pos + nameLen > fntBytes.byteLength) throw new Error("NitroFS contains an invalid file name entry.");
      pos += nameLen;

      if (marker & 0x80) {
        if (pos + 2 > fntBytes.byteLength) throw new Error("NitroFS subdirectory entry is truncated.");
        const subId = view.getUint16(pos, true);
        pos += 2;
        walk(subId);
      } else {
        if (fid >= fatEntries) throw new Error("NitroFS references a file id beyond the FAT.");
        named.add(fid++);
      }
    }

    visiting.delete(dirId);
    visited.add(dirId);
  }

  walk(0xf000);
  return named;
}

export async function inspectRom(file, wasmValidate) {
  if (!(file instanceof Blob)) throw new Error("No ROM file was selected.");
  if (file.size < 0x200) throw new Error("The selected file is too small to be a Nintendo DS ROM.");
  if (file.size > 0xffffffff) throw new Error("ROM files larger than 4 GiB are not supported by the PS2 pack format.");

  const headerBuffer = await file.slice(0, 0x200).arrayBuffer();
  const header = new Uint8Array(headerBuffer);
  const headerView = new DataView(headerBuffer);
  const gameCode = readAscii(header, 12, 4);
  const title = readAscii(header, 0, 12);

  if (wasmValidate) {
    const code = wasmValidate(header, file.size);
    const messages = {
      1: "The Nintendo DS header is incomplete.",
      2: `Unsupported ROM (${gameCode || "unknown game code"}). This builder currently requires the European release (YKGP).`,
      3: "The ROM has an invalid NitroFS file-name-table range.",
      4: "The ROM has an invalid NitroFS file-allocation-table range.",
      5: "The ROM has an invalid NitroFS FAT size."
    };
    if (code !== 0) throw new Error(messages[code] || `ROM validation failed (code ${code}).`);
  } else if (gameCode !== "YKGP") {
    throw new Error(`Unsupported ROM (${gameCode || "unknown game code"}). This builder currently requires the European release (YKGP).`);
  }

  const fntOffset = headerView.getUint32(0x40, true);
  const fntSize = headerView.getUint32(0x44, true);
  const fatOffset = headerView.getUint32(0x48, true);
  const fatSize = headerView.getUint32(0x4c, true);

  ensureRange(fntOffset, fntSize, file.size, "FNT");
  ensureRange(fatOffset, fatSize, file.size, "FAT");
  if (fatSize === 0 || fatSize % 8 !== 0) throw new Error("The NitroFS FAT has an invalid size.");

  const [fntBuffer, fatBuffer] = await Promise.all([
    file.slice(fntOffset, fntOffset + fntSize).arrayBuffer(),
    file.slice(fatOffset, fatOffset + fatSize).arrayBuffer()
  ]);
  const fntBytes = new Uint8Array(fntBuffer);
  const fatView = new DataView(fatBuffer);
  const fatEntries = fatSize / 8;
  const namedFileIds = parseNamedFileIds(fntBytes, fatEntries);

  let namedBytes = 0;
  for (const fid of namedFileIds) {
    const start = fatView.getUint32(fid * 8, true);
    const end = fatView.getUint32(fid * 8 + 4, true);
    if (end < start) throw new Error(`FAT entry ${fid} has a negative length.`);
    ensureRange(start, end - start, file.size, `FAT entry ${fid}`);
    namedBytes += end - start;
  }

  return {
    title,
    gameCode,
    region: "Europe",
    romBytes: file.size,
    fntOffset,
    fntSize,
    fatOffset,
    fatSize,
    fatEntries,
    namedFileIds,
    namedFileCount: namedFileIds.size,
    namedBytes,
    fntBytes,
    sourceFat: new DataView(fatBuffer)
  };
}

function makeKhpkHeader({ fatOffset, fatSize, fntOffset, fntSize, fileCount, gameCode, dataOffset }) {
  const buffer = new ArrayBuffer(64);
  const view = new DataView(buffer);
  view.setUint32(0x00, KHPK_MAGIC, true);
  view.setUint32(0x04, 1, true);
  view.setUint32(0x08, fatOffset, true);
  view.setUint32(0x0c, fatSize, true);
  view.setUint32(0x10, fntOffset, true);
  view.setUint32(0x14, fntSize, true);
  view.setUint32(0x18, fileCount, true);
  for (let i = 0; i < 4; i++) view.setUint8(0x1c + i, gameCode.charCodeAt(i));
  view.setUint32(0x20, dataOffset, true);
  return new Uint8Array(buffer);
}

const ZEROES = new Uint8Array(ALIGN_DATA);
function padPart(length) {
  return length > 0 ? ZEROES.subarray(0, length) : null;
}

export function buildPak(file, info, onProgress = () => {}) {
  const khHeaderSize = 64;
  const khFntOffset = khHeaderSize;
  const khFatOffset = align(khFntOffset + info.fntSize, 16);
  const khFatSize = info.fatEntries * 8;
  const khDataOffset = align(khFatOffset + khFatSize, ALIGN_DATA);

  const rewrittenFat = new ArrayBuffer(khFatSize);
  const fatOut = new DataView(rewrittenFat);
  const positions = new Array(info.fatEntries);
  let pos = khDataOffset;

  for (let fid = 0; fid < info.fatEntries; fid++) {
    if (!info.namedFileIds.has(fid)) continue;
    const srcStart = info.sourceFat.getUint32(fid * 8, true);
    const srcEnd = info.sourceFat.getUint32(fid * 8 + 4, true);
    const length = srcEnd - srcStart;
    const outStart = pos;
    const outEnd = outStart + length;
    if (outEnd > 0xffffffff) throw new Error("The generated pack would exceed the 32-bit KHPK v1 size limit.");
    fatOut.setUint32(fid * 8, outStart, true);
    fatOut.setUint32(fid * 8 + 4, outEnd, true);
    positions[fid] = { srcStart, srcEnd, outStart, outEnd };
    pos = align(outEnd, ALIGN_DATA);
    if ((fid & 0xff) === 0) onProgress(fid / info.fatEntries);
  }

  const totalBytes = pos;
  const header = makeKhpkHeader({
    fatOffset: khFatOffset,
    fatSize: khFatSize,
    fntOffset: khFntOffset,
    fntSize: info.fntSize,
    fileCount: info.namedFileCount,
    gameCode: info.gameCode,
    dataOffset: khDataOffset
  });

  const parts = [header, info.fntBytes];
  let cursor = khFntOffset + info.fntSize;
  let padding = khFatOffset - cursor;
  if (padding) parts.push(padPart(padding));
  parts.push(new Uint8Array(rewrittenFat));
  cursor = khFatOffset + khFatSize;
  padding = khDataOffset - cursor;
  if (padding) parts.push(padPart(padding));
  cursor = khDataOffset;

  for (let fid = 0; fid < positions.length; fid++) {
    const p = positions[fid];
    if (!p) continue;
    if (cursor !== p.outStart) throw new Error("Internal pack layout mismatch.");
    parts.push(file.slice(p.srcStart, p.srcEnd));
    cursor = p.outEnd;
    const next = align(cursor, ALIGN_DATA);
    if (next > cursor) parts.push(padPart(next - cursor));
    cursor = next;
  }

  if (cursor !== totalBytes) throw new Error("Internal pack size mismatch.");
  onProgress(1);
  const blob = new Blob(parts, { type: "application/octet-stream" });
  if (blob.size !== totalBytes) throw new Error("Browser produced an unexpected pack size.");
  return { blob, totalBytes, fileCount: info.namedFileCount };
}
