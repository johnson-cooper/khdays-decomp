import { inspectRom, buildPak } from "./packer.js";

const ROMCHECK_WASM_B64 = "AGFzbQEAAAABCAFgA39/fwF/AwIBAAUEAQECAgYIAX8BQYCIBAsHHwIGbWVtb3J5AgASdmFsaWRhdGVfZXVfaGVhZGVyAAAKlQEBkgEBAX9BASEDAkAgAUGABEkNAEECIQMgAC0ADEHZAEcNACAALQANQcsARw0AIAAtAA5BxwBHDQAgAC0AD0HQAEcNAEEDIQMgAiAAKABAIgFJDQAgACgARCACIAFrSw0AQQQhAyACIAAoAEgiAUkNACAAKABMIgAgAiABa0sNAEEFQQAgAEEHcRtBBSAAGyEDCyADCwBABG5hbWUADg1yb21jaGVjay53YXNtARUBABJ2YWxpZGF0ZV9ldV9oZWFkZXIHEgEAD19fc3RhY2tfcG9pbnRlcgB/CXByb2R1Y2VycwEMcHJvY2Vzc2VkLWJ5AQVjbGFuZ18xNy4wLjAgKGh0dHBzOi8vZ2l0aHViLmNvbS9zd2lmdGxhbmcvbGx2bS1wcm9qZWN0LmdpdCAxMDk5OWI2ZDAzNGZlMzE4ZjNkNTZjODNiZGRiNjU3MjU5M2E4YmIwKQBJD3RhcmdldF9mZWF0dXJlcwQrCm11bHRpdmFsdWUrD211dGFibGUtZ2xvYmFscysPcmVmZXJlbmNlLXR5cGVzKwhzaWduLWV4dA==";

const els = {
  drop: document.querySelector("#drop-zone"),
  picker: document.querySelector("#rom-picker"),
  choose: document.querySelector("#choose-rom"),
  build: document.querySelector("#build-pak"),
  reset: document.querySelector("#reset"),
  status: document.querySelector("#status"),
  romInfo: document.querySelector("#rom-info"),
  progress: document.querySelector("#progress"),
  progressBar: document.querySelector("#progress-bar"),
  progressLabel: document.querySelector("#progress-label"),
  download: document.querySelector("#download-pak"),
  outputInfo: document.querySelector("#output-info")
};

let selectedFile = null;
let romInfo = null;
let outputUrl = null;
let wasmValidator = null;

function formatBytes(bytes) {
  const units = ["B", "KiB", "MiB", "GiB"];
  let value = bytes;
  let unit = 0;
  while (value >= 1024 && unit < units.length - 1) {
    value /= 1024;
    unit++;
  }
  return `${value.toFixed(unit === 0 ? 0 : 1)} ${units[unit]}`;
}

function setStatus(kind, title, body = "") {
  els.status.className = `status ${kind}`;
  els.status.innerHTML = `<strong>${title}</strong>${body ? `<span>${body}</span>` : ""}`;
  els.status.hidden = false;
}

function clearOutput() {
  if (outputUrl) URL.revokeObjectURL(outputUrl);
  outputUrl = null;
  els.download.hidden = true;
  els.outputInfo.hidden = true;
}

async function loadWasm() {
  try {
    const raw = atob(ROMCHECK_WASM_B64);
    const bytes = new Uint8Array(raw.length);
    for (let i = 0; i < raw.length; i++) bytes[i] = raw.charCodeAt(i);
    const { instance } = await WebAssembly.instantiate(bytes);
    const { memory, validate_eu_header } = instance.exports;
    wasmValidator = (header, romSize) => {
      new Uint8Array(memory.buffer, 0, header.byteLength).set(header);
      return validate_eu_header(0, header.byteLength, romSize >>> 0);
    };
    document.documentElement.dataset.wasm = "ready";
  } catch (err) {
    console.warn("WASM preflight unavailable; JavaScript validation remains active.", err);
    document.documentElement.dataset.wasm = "fallback";
  }
}

async function acceptFile(file) {
  clearOutput();
  selectedFile = null;
  romInfo = null;
  els.build.disabled = true;
  els.romInfo.hidden = true;
  els.progress.hidden = true;

  if (!file) return;
  if (!/\.nds$/i.test(file.name)) {
    setStatus("error", "Unsupported file type", "Choose an unmodified Nintendo DS .nds ROM.");
    return;
  }

  setStatus("working", "Checking ROM…", "Reading the Nintendo DS header and NitroFS tables locally.");
  try {
    const info = await inspectRom(file, wasmValidator);
    selectedFile = file;
    romInfo = info;
    els.romInfo.innerHTML = `
      <div><span>Game</span><strong>Kingdom Hearts 358/2 Days</strong></div>
      <div><span>Region</span><strong>${info.region}</strong></div>
      <div><span>Game code</span><strong>${info.gameCode}</strong></div>
      <div><span>ROM size</span><strong>${formatBytes(info.romBytes)}</strong></div>
      <div><span>NitroFS files</span><strong>${info.namedFileCount.toLocaleString()}</strong></div>
    `;
    els.romInfo.hidden = false;
    els.build.disabled = false;
    setStatus("success", "Compatible European ROM detected", "Ready to build the PS2 game-data pack. Nothing has been uploaded.");
  } catch (err) {
    setStatus("error", "ROM validation failed", err.message || String(err));
  }
}

els.choose.addEventListener("click", () => els.picker.click());
els.picker.addEventListener("change", () => acceptFile(els.picker.files?.[0]));

["dragenter", "dragover"].forEach(type => els.drop.addEventListener(type, event => {
  event.preventDefault();
  els.drop.classList.add("dragging");
}));
["dragleave", "drop"].forEach(type => els.drop.addEventListener(type, event => {
  event.preventDefault();
  els.drop.classList.remove("dragging");
}));
els.drop.addEventListener("drop", event => acceptFile(event.dataTransfer?.files?.[0]));
els.drop.addEventListener("click", event => {
  if (event.target.closest("button")) return;
  els.picker.click();
});
els.drop.addEventListener("keydown", event => {
  if (event.key === "Enter" || event.key === " ") {
    event.preventDefault();
    els.picker.click();
  }
});

els.build.addEventListener("click", async () => {
  if (!selectedFile || !romInfo) return;
  clearOutput();
  els.build.disabled = true;
  els.progress.hidden = false;
  els.progressBar.style.width = "0%";
  els.progressLabel.textContent = "Preparing pack layout…";
  setStatus("working", "Building khdays.pak…", "The ROM remains on this device.");

  try {
    await new Promise(resolve => requestAnimationFrame(resolve));
    const result = buildPak(selectedFile, romInfo, value => {
      const pct = Math.max(0, Math.min(100, Math.round(value * 100)));
      els.progressBar.style.width = `${pct}%`;
      els.progressLabel.textContent = `Preparing pack layout… ${pct}%`;
    });

    outputUrl = URL.createObjectURL(result.blob);
    els.download.href = outputUrl;
    els.download.download = "khdays.pak";
    els.download.hidden = false;
    els.outputInfo.innerHTML = `
      <strong>khdays.pak</strong>
      <span>${formatBytes(result.totalBytes)} · ${result.fileCount.toLocaleString()} game files</span>
      <code>KHDAYS/ps2data/khdays.pak</code>
    `;
    els.outputInfo.hidden = false;
    els.progressLabel.textContent = "Pack ready.";
    els.progressBar.style.width = "100%";
    setStatus("success", "khdays.pak is ready", "Download it and place it in the ps2data folder next to the PS2 port.");
  } catch (err) {
    els.progress.hidden = true;
    setStatus("error", "Could not build the pack", err.message || String(err));
  } finally {
    els.build.disabled = !selectedFile;
  }
});

els.reset.addEventListener("click", () => {
  clearOutput();
  selectedFile = null;
  romInfo = null;
  els.picker.value = "";
  els.romInfo.hidden = true;
  els.progress.hidden = true;
  els.status.hidden = true;
  els.build.disabled = true;
});

loadWasm();
