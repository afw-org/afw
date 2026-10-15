#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: utf8_nfc.as
//? customPurpose: Part of miscellaneous tests
//? description: Strings are NFC normalized when created (#615) and never hold U+FEFF; the bytes are compared with encode_as_base64Binary
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
//? test: byte-order-mark-anywhere
//? description: A string never holds U+FEFF (byte order mark), at the start or anywhere else, so equal text is equal bytes. ECMAScript keeps it; AFW does not.
//? expect: [0, true, 1, 2, true, "YWI="base64Binary]
//? source: ...

[
    length("\uFEFF"),
    "\uFEFF" === "",
    length("\uFEFFa"),
    length("a\uFEFFb"),
    "a\uFEFFb\uFEFF" === "ab",
    encode_as_base64Binary("a\uFEFFb")
]

//?
//? test: byte-order-mark-made-at-runtime
//? description: U+FEFF made at runtime is dropped too, and text on either side of one joins. Before, a slice could start with one: substring("a\uFEFFb", 1) had length 2 and was not equal to "b".
//? expect: [0, true, true, true, 1, []]
//? source: ...

[
    length(from_code_point(65279)),
    "x" + from_code_point(65279) + "y" === "xy",
    join(["a", from_code_point(65279), "b"], from_code_point(65279)) === "ab",
    substring("a" + from_code_point(65279) + "b", 1) === "b",
    length("a" + from_code_point(65279)),
    split(from_code_point(65279), "")
]

//?
//? test: byte-order-mark-between-base-and-mark
//? description: U+FEFF is dropped before normalizing, so e, U+FEFF, U+0301 composes to U+00E9.
//? expect: [true, 1]
//? source: ...

[
    "e\uFEFF\u0301" === "\u00E9",
    length("e" + from_code_point(65279) + "\u0301")
]

//?
//? test: decode-to-string-is-nfc
//? description: decode_to_string of base64Binary and hexBinary made a string of the bytes as they were, not NFC and keeping U+FEFF, so it was not equal to the same text written in a string.
//? expect: [true, 1, true, 1]
//? source: ...

[
    decode_to_string<base64Binary>("ZcyB"base64Binary) === "\u00E9",
    length(decode_to_string<base64Binary>("ZcyB"base64Binary)),
    decode_to_string<hexBinary>("efbbbf61efbbbf"hexBinary) === "a",
    length(decode_to_string<hexBinary>("efbbbf61efbbbf"hexBinary))
]
