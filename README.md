# zenlz4

<div align="center">

**A lightweight CLI tool for encoding and decoding Mozilla-style `.jsonlz4` files**

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-23-blue.svg)](https://isocpp.org/)

</div>

---

## Overview

`zenlz4` provides seamless conversion between JSON and Mozilla's LZ4-compressed JSON container format (`.jsonlz4`). Perfect for working with Zen Browser boost data, bookmarks, and other compressed Mozilla files.

### Features

- **Decode** `.jsonlz4` → `.json`
- **Encode** `.json` → `.jsonlz4`
- Lightweight and fast
- Easy editor integration (Neovim, VS Code, etc.)

---

## Installation

### Prerequisites

This project requires the [LZ4 library](https://github.com/lz4/lz4). The easiest way to install it is via [vcpkg](https://vcpkg.io/):

```bash
vcpkg install lz4:x64-windows-static
```

### Building from Source

```bash
# Configure (ensure vcpkg path is set in init.bat)
.\init.bat --msvc --release

# Build
cmake --build build
```

> **Note:** Update the vcpkg toolchain path (the -DCMAKE_TOOLCHAIN_FILE=) near the bottom of `init.bat` to match your local installation.

---

## Usage

```bash
# Decode: .jsonlz4 → .json
zenlz4 d input.jsonlz4 output.json

# Encode: .json → .jsonlz4
zenlz4 e input.json output.jsonlz4
```

---

## Editor Integration

### Neovim

Add the following to your Neovim configuration for transparent `.jsonlz4` editing:

> **Prerequisite:** Ensure `zenlz4` is available in your system `PATH`.

<details>
<summary><b>Click to expand Lua configuration</b></summary>

```lua
-- Decode .jsonlz4 files on open
vim.api.nvim_create_autocmd("BufReadCmd", {
    pattern = { "*.jsonlz4" },
    callback = function()
        local file = vim.fn.expand("%:p")
        local tmp = vim.fn.tempname()
        local res = vim.system({ "zenlz4", "d", file, tmp }, {}):wait()
        
        if res.code ~= 0 then
            error("zenlz4 decode failed: " .. (res.stderr or ""))
        end

        local lines = vim.fn.readfile(tmp)
        vim.api.nvim_buf_set_lines(0, 0, -1, false, lines)
        vim.bo.filetype = "json"
        vim.bo.modified = false
    end,
})

-- Encode back to .jsonlz4 on save
vim.api.nvim_create_autocmd("BufWriteCmd", {
    pattern = { "*.jsonlz4" },
    callback = function()
        local file = vim.fn.expand("%:p")
        local lines = vim.api.nvim_buf_get_lines(0, 0, -1, false)
        local text = table.concat(lines, "\n")

        local tmp = vim.fn.tempname()
        local f = assert(io.open(tmp, "wb"))
        f:write(text)
        f:close()

        local res = vim.system({ "zenlz4", "e", tmp, file }, {}):wait()
        
        if res.code ~= 0 then
            error("zenlz4 encode failed: " .. (res.stderr or ""))
        end

        vim.notify("zenlz4: compressed file updated", vim.log.levels.INFO)
        vim.bo.modified = false
    end,
})
```

</details>

---

## License

This project is open source. See [LICENSE](LICENSE) for details.

---

<div align="center">

**[Report Bug](../../issues) · [Request Feature](../../issues)**

</div>

