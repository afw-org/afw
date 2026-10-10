#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: template_string_escapes.as
//? customPurpose: Part of language/script tests
//? description: ...
Escapes in template strings (`...`) work as in string literals. An
escaped character is text: \` does not end the template, and an
escaped $ or # does not open a substitution.
//? sourceType: script
//?
//? test: template-escaped-grave
//? description: \` is a grave accent in the text (it ended the template)
//? expect: 0
//? source: ...

assert(`a\`b` === "a`b", "a\\`b");
assert(`\`` === "`", "\\`");
assert(`x\`${1}` === "x`1", "before a substitution");
return 0;

//?
//? test: template-unicode-brace-escape
//? description: \u{...} works in a template as in a string ("Invalid hex digit")
//? expect: 0
//? source: ...

assert(`\u{41}` === "A", "\\u{41}");
assert(`\u{1F600}` === "\u{1F600}", "astral");
assert(`\u0041` === "A", "\\u0041 still works");
return 0;

//?
//? test: template-escaped-dollar-is-text
//? description: an escaped dollar sign (\$ or a \u escape) does not open a substitution
//? expect: 0
//? source: ...

assert(`\${1}` === "$" + "{1}", "\\$");
assert(`\u0024{1}` === "$" + "{1}", "\\u0024");
assert(`a${"b"}c` === "abc", "substitution");
return 0;

//?
//? test: same-escapes-as-strings
//? description: strings and templates share one escape reader: \xHH is U+0000-U+00FF in a string too (it was a raw octet, "Not valid UTF-8"), a line continuation is nothing in a template too (it kept the newline), and an identity escape of a multi-octet character is that character
//? expect: 0
//? source: ...

assert("\xE9" === "é", "string \xE9");
assert(`\xE9` === "é", "template \xE9");
assert("\x41" === "A", "string \x41");
assert(`a\
b` === "ab", "template LF continuation");
assert("a\
b" === "ab", "string LF continuation");
assert(`a\ b` === "ab", "template LS continuation");
assert("\é" === "é", "string identity escape");
assert(`\é` === "é", "template identity escape");
assert(`\${1}` === "${1}", "template escaped $");
return 0;
