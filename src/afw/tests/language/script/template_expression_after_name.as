#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: template_expression_after_name.as
//? customPurpose: Part of language/script tests
//? description: ...
A substitution that starts with a name, call, or member access can go on
with operators (#542). As a statement, `x + 1;` is still not allowed.
//? sourceType: script
//?
//? test: substitution-operator-after-name
//? description: ${x + 1}, ${o.a * 3}, ${length(s) > 1}, ${f(2) - f(1)}
//? expect: 0
//? source: ...

let x = 1;
let o = {a: 2};
let s = "ab";
let a = [1];
function f(n) { return n; }
assert(`${x + 1}` === 2, "x + 1");
assert(`${o.a * 3}` === 6, "o.a * 3");
assert(`${length(s) > 1}` === true, "call > 1");
assert(`${f(2) - f(1)}` === 1, "call - call");
assert(`v=${x * x + 1}!` === "v=2!", "inside text");
assert(`${a[0] == 1 && true}` === true, "element ==");
assert(`${x > 0 ? "p" : "n"}` === "p", "conditional");
assert(`${x}` === 1, "name alone");
return 0;

//?
//? test: expression-statement-still-invalid
//? description: x + 1; as a statement is still an error
//? expect: error
//? source: ...

let x = 1;
x + 1;
return 0;
