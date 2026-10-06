# Browser PS2 data builder

The GitHub Pages site in this `docs/` directory creates `khdays.pak` from a user's own European Kingdom Hearts 358/2 Days Nintendo DS ROM.

It mirrors `ps2/tools/make_ps2data.py`:

- requires game code `YKGP`;
- copies the NitroFS FNT verbatim;
- rewrites FAT entries to pack-relative offsets;
- omits unnamed overlay binaries (overlay code is already linked into the PS2 ELF);
- aligns game files to 2048-byte boundaries;
- writes KHPK format version 1.

The ROM never leaves the browser. The page has no backend and can be hosted as a static GitHub Pages site.

## GitHub Pages setup

In **Settings → Pages → Build and deployment**:

1. Set **Source** to **Deploy from a branch**.
2. Select **ps2-port**.
3. Select **/docs**.
4. Save.

After that, updates to `ps2-port/docs/` are published automatically. No repository-managed GitHub Actions workflow is needed.

## WebAssembly

The page includes a tiny WebAssembly preflight validator directly in `js/app.js`. It checks the European game code and the ROM's FNT/FAT ranges before the JavaScript packer reads NitroFS.

The full packer also performs its own bounds checks, so WebAssembly failure falls back safely to JavaScript validation.
