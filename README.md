# zenlz4

A small CLI tool for encoding and decoding Mozilla-style `.jsonlz4` files.

It converts between:
- JSON ↔ LZ4-compressed JSON container format
- Designed for integration with editors like Neovim

---

## What it does

zenlz4 handles:

[ "mozLz40" header ][ original size (4 bytes) ][ LZ4 compressed JSON ]

It supports:
- decode: .jsonlz4 → JSON
- encode: JSON → .jsonlz4

Example of usage:

vim.api.nvim_create_autocmd("BufReadCmd", {
  pattern = { "*.jsonlz4" },
  callback = function()
    local file = vim.fn.expand "%:p"
    local tmp = vim.fn.tempname()

    local res = vim
      .system({
        "zenlz4",
        "d",
        file,
        tmp,
      }, {})
      :wait()

    if res.code ~= 0 then
      error("zenlz4 decode failed: " .. (res.stderr or ""))
    end

    local lines = vim.fn.readfile(tmp)

    vim.api.nvim_buf_set_lines(0, 0, -1, false, lines)

    vim.bo.filetype = "json"
    vim.bo.modified = false
  end,
})

vim.api.nvim_create_autocmd("BufWriteCmd", {
  pattern = { "*.jsonlz4" },
  callback = function()
    local file = vim.fn.expand "%:p"

    local lines = vim.api.nvim_buf_get_lines(0, 0, -1, false)
    local text = table.concat(lines, "\n")

    local tmp = vim.fn.tempname()
    local f = assert(io.open(tmp, "wb"))
    f:write(text)
    f:close()

    local res = vim
      .system({
        "zenlz4",
        "e",
        tmp,
        file,
      }, {})
      :wait()

    if res.code ~= 0 then
      error("zenlz4 encode failed: " .. (res.stderr or ""))
    end
    vim.notify("zenlz4: compressed file updated", vim.log.levels.INFO)
    vim.bo.modified = false
  end,
})


