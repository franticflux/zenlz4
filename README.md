# jsonlz4 / zenlz4

A small CLI tool for encoding and decoding Mozilla-style `.jsonlz4` files.

It converts between:
- JSON ↔ LZ4-compressed JSON container format
- Designed for integration with editors like Neovim

---

## What it does

jsonlz4 handles:

[ "mozLz40" header ][ original size (4 bytes) ][ LZ4 compressed JSON ]

It supports:
- decode: .jsonlz4 → JSON
- encode: JSON → .jsonlz4
