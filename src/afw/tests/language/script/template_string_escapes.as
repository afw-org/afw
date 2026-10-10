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
