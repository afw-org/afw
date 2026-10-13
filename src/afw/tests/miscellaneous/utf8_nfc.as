#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: utf8_nfc.as
//? customPurpose: Part of miscellaneous tests
//? description: Strings are NFC normalized when created (#615); the bytes are compared with encode_as_base64Binary
//? sourceType: script
//?
//? test: marks-out-of-canonical-order
//? description: Two marks whose NFC quick check is YES (Hebrew points, combining class 11 then 10) are out of canonical order, so the string is not NFC and is reordered. Before #615 it was kept as written.
//? expect: ["15DWsNax"base64Binary, "15DWsNax"base64Binary]
//? source: ...

[
    encode_as_base64Binary("אְֱ"),
    encode_as_base64Binary("אְֱ")
]

//?
//? test: marks-reordered-and-composed
//? description: a, U+0307 (230), U+0323 (220) reorders and composes to U+1EA1 U+0307.
//? expect: "4bqhzIc="base64Binary
//? source: ...

encode_as_base64Binary("ạ̇")

//?
//? test: decomposed-composes
//? description: e followed by U+0301 composes to U+00E9.
//? expect: "w6k="base64Binary
//? source: ...

encode_as_base64Binary("é")

//?
//? test: mark-that-does-not-compose
//? description: x followed by U+0301 is already NFC and is kept.
//? expect: "eMyB"base64Binary
//? source: ...

encode_as_base64Binary("x́")

//?
//? test: singleton
//? description: U+212B (ANGSTROM SIGN) is never NFC; it becomes U+00C5.
//? expect: "w4U="base64Binary
//? source: ...

encode_as_base64Binary("Å")

//?
//? test: long-value-one-mark-at-end
//? description: Only the end of a long value needs work; the rest is kept as is.
//? expect: [201, "é"]
//? source: ...

const s = repeat("a", 200) + "é";
return [length(s), substring(s, 200)];

//?
//? test: mixed-runs
//? description: Several runs that need work, separated by text that does not.
//? expect: "Y2Fmw6kgYW5kINeQ1rDWsSBvaw=="base64Binary
//? source: ...

encode_as_base64Binary("café and אְֱ ok")

//?
//? test: byte-order-mark-only
//? description: A leading UTF-8 byte order mark is dropped, so a value that is only one is the empty string (as before #615).
//? expect: [0, true, 1]
//? source: ...

[length("\uFEFF"), "\uFEFF" === "", length("\uFEFFa")]
