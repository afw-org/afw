#!/usr/bin/env -S afw --syntax test_script
//? testScript: regexp_literal.as
//? customPurpose: Part of language/script tests
//? description: Regular expression literals /.../flags (#585)
//? sourceType: script
//?
//? test: regexp-literal-value
//? description: /.../ is a compiled regexp, the same value as the typed literal.
//? expect: ["regexp", true, true]
//? source: ...

const r = /a+b/;
return [meta(r).dataType, r == "a+b"re, test(r, "xaab")];

//?
//? test: regexp-literal-raw-body
//? description: The body goes to ICU as written, so a backslash is not doubled; a / is written \/ or inside a class.
//? expect: [true, true, true, "13/10/2024"]
//? source: ...

return [
    test(/^a\d$/, "a1"),
    test(/^a\/b$/, "a/b"),
    test(/^a[/]b$/, "a/b"),
    replace("2024-10-13", /(\d+)-(\d+)-(\d+)/, "$3/$2/$1")
];

//?
//? test: regexp-literal-flags
//? description: Flags i, m, s, and x are ICU inline flags in the source, in a fixed order; u changes nothing.
//? expect: [true, true, true, true, true, true, true]
//? source: ...

return [
    test(/a+b/i, "xAAB"),
    test(/^ab$/m, "x\nab"),
    test(/a.b/s, "a\nb"),
    test(/a b/x, "ab"),
    test(/^\p{L}$/u, "é"),
    /a+b/i == "(?i)a+b"re,
    /a/mi == /a/im
];

//?
//? test: regexp-literal-and-divide
//? description: Where an operand can start, / begins a regexp; after an operand it divides.
//? expect: [4, 4, 4, 4, 2, -4, 4, true]
//? source: ...

const o = {x: 8};
const a = [8];
let y: integer = 8;
y /= 2;
return [
    8 / 2, o.x / 2, a[0] / 2, (8) / 2, 8/2/2, -8 / 2, y,
    (true ? /a/ : /b/) == "a"re
];

//?
//? test: regexp-literal-flag-g
//? description: Flag g is not supported; replace takes a limit.
//? expect: error:Parse error at offset 1 around line 2 column 1: Regular expression flag g is not supported; replace replaces every match with limit -1
//? source: ...

/a/g

//?
//? test: regexp-literal-unknown-flag
//? description: An unknown flag is a parse error.
//? expect: error:Parse error at offset 1 around line 2 column 1: Regular expression flag q is not supported (supported: i, m, s, x, u)
//? source: ...

/a/q

//?
//? test: regexp-literal-repeated-flag
//? description: A repeated flag is a parse error.
//? expect: error:Parse error at offset 1 around line 2 column 1: Regular expression flag i is repeated
//? source: ...

/a/ii

//?
//? test: regexp-literal-bad-pattern
//? description: A pattern ICU does not accept is a parse error at the literal.
//? expect: error:Parse error at offset 1 around line 2 column 1: Invalid regular expression literal: U_REGEX_MISMATCHED_PAREN at line 1 offset 2
//? source: ...

/(a/

//?
//? test: regexp-literal-unterminated
//? description: A literal must end with / on the same line.
//? expect: error:Parse error at offset 1 around line 2 column 1: Regular expression literal missing ending /
//? source: ...

/a
