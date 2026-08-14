#!/usr/bin/env -S afw --syntax test_script
//? testScript: unicode_escape_sequence.as
//? customPurpose: Part of miscellaneous category tests
//? description: Test unicode escape sequences
//? sourceType: script
//?
//? test: UnicodeEscapeSequence1
//? description: Test unicode escape sequence for emotes
//? expect: "😀😁😂😃😄 == 😀😁😂😃😄"
//? source: ...

return'\uD83D\uDE00\uD83D\uDE01\uD83D\uDE02\uD83D\uDE03\uD83D\uDE04 == \u{1F600}\u{1F601}\u{1F602}\u{1F603}\u{1F604}';

//?
//? test: UnicodeEscapeSequence1
//? description: Test unicode escape sequence out of bound
//? expect: error
//? source: ...

return'\u{11FFFF}';


//?
//? test: HexEscapeSequence-xHH
//? description: \xHH hex escapes
//? expect: true
//? source: ...

return "\x41" === "A" && "\x00" === "\0" && "\x61" === "a";

//?
//? test: NonEscapeSequence-identity
//? description: Identity NonEscapeSequence \A === A
//? expect: true
//? source: ...

return "\A" === "A" && "\z" === "z" && "\Q" === "Q";

//?
//? test: NullEscape-backslash-zero
//? description: \0 is U+0000 when not followed by a digit
//? expect: true
//? source: ...

return "\0" === "\u0000" && length("\0") === 1;
