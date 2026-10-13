#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: utf8_validation.as
//? customPurpose: Part of miscellaneous tests
//? description: UTF-8 is validated to the end of the value, also after a code point whose NFC quick check is not YES
//? sourceType: script
//?
//? test: invalid-byte-alone
//? description: An invalid byte is not valid UTF-8.
//? expect: error:value does not decode to valid utf-8
//? source: ...

decode_to_string("Zf8="base64Binary)

//?
//? test: invalid-byte-after-combining-mark
//? description: e, U+0301, then 0xFF. The NFC quick check of U+0301 is MAYBE; the scan used to stop there, and the lenient conversion turned 0xFF into U+FFFD.
//? expect: error:value does not decode to valid utf-8
//? source: ...

decode_to_string("ZcyB/w=="base64Binary)

//?
//? test: truncated-sequence-later-after-combining-mark
//? description: e, U+0301, abc, then a lead byte with no continuation at the end.
//? expect: error:value does not decode to valid utf-8
//? source: ...

decode_to_string("ZcyBYWJjww=="base64Binary)

//?
//? test: surrogate-after-combining-marks
//? description: a, U+0301, U+0302, then an encoded surrogate (ED A0 80), which UTF-8 does not allow.
//? expect: error:value does not decode to valid utf-8
//? source: ...

decode_to_string(to_hexBinary("61cc81cc82eda080"))

//?
//? test: invalid-byte-after-hangul-jamo
//? description: Conjoining jamo (NFC quick check MAYBE / NO), then 0xFF.
//? expect: error:value does not decode to valid utf-8
//? source: ...

decode_to_string("4YSA4YWh/w=="base64Binary)

//?
//? test: valid-combining-mark
//? description: e followed by U+0301 is valid UTF-8.
//? expect: 2
//? source: ...

length(decode_to_string("ZcyB"base64Binary))
