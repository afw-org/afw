#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/normalize.as
//? customPurpose: Part of core function tests
//? description: Tests for normalize: every string is already NFC.
//? sourceType: script
//?
//? test: normalize-nfkc
//? description: NFKC replaces compatibility characters.
//? expect: "fi1"
//? source: ...

"ﬁ①"->normalize("NFKC")

//?
//? test: normalize-nfc
//? description: NFC, the default, is the string itself.
//? expect: ["é", "é"]
//? source: ...

["é"->normalize(), "é"->normalize("NFC")]

//?
//? test: normalize-nfd
//? description: NFD can not be a string.
//? expect: error:normalize form "NFD": a string in Adaptive Script is always NFC, so a decomposed string can not be made; use "NFC" or "NFKC"
//? source: ...

"é"->normalize("NFD")

//?
//? test: normalize-bad
//? description: Another form is an error.
//? expect: error:normalize form must be "NFC" or "NFKC", not "nfc"
//? source: ...

"é"->normalize("nfc")
