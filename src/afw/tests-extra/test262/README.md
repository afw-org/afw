# test262 extra tests (opt-in)

Copies of the BMP comment sweeps. The same files under
`src/afw/tests/test262/comments-bmp-*.as` run in default `afwdev test -j`.

```bash
afwdev test -T src/afw/tests-extra/test262 --show-all
```

| File | What |
|------|------|
| `comments-bmp-slash-0.as` / `-1.as` | S7.4_A5 BMP `\\uXXXX` in `//` comments |
| `comments-bmp-block-0.as` / `-1.as` | S7.4_A6 BMP `\\uXXXX` in `/* */` comments |

Split so valgrind can finish. Origin notes: `src/afw/tests/test262/README.md`.
