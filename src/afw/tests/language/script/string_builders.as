#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: string_builders.as
//? customPurpose: Part of language/script tests
//? description: Functions that build a string from pieces (join, replace, replace_all, pad_start, pad_end, repeat)
//? sourceType: script
//?
//? test: join-empty-first-entries
//? description: A separator goes before every entry but the first, even after empty entries
//? expect: true
//? source: ...

assert(join(["", "a"]) === ",a", join(["", "a"]));
assert(join(["", "", "a"], "-") === "--a", join(["", "", "a"], "-"));
assert(join(["", "a", "b"], ", ") === ", a, b", join(["", "a", "b"], ", "));
assert(join(["", ""]) === ",", join(["", ""]));
assert(length(join(["", "a"])) === 2);
return true;

//?
//? test: empty-results
//? description: A result with no characters is the empty string, not an error
//? expect: true
//? source: ...

assert(replace("b", "b", "") === "");
assert(replace_all("bb", "b", "") === "");
assert(replace_all("", "", "") === "");
assert(repeat("", 3) === "");
return true;

//?
//? test: results-are-nfc
//? description: Where two pieces meet, a base and a combining mark compose (every string is NFC)
//? expect: true
//? source: ...

assert(join(["e", "\u0301"], "") === "\u00e9");
assert(join(["a", "b"], "\u0301") === "a\u0301b");
assert(length(join(["e", ""], "\u0301")) === 1);
assert(replace("eX\u0301", "X", "") === "\u00e9");
assert(replace_all("eX\u0301X", "X", "") === "\u00e9");
assert(replace_all("e", "", "\u0301") === "\u0301\u00e9");
assert(pad_start("\u0301", 2, "e") === "\u00e9");
assert(pad_end("e", 2, "\u0301") === "\u00e9");
assert(repeat("\u0301e", 2) === "\u0301\u00e9e");
assert(length(repeat("\u0301e", 2)) === 3);
return true;
