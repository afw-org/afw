#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: expression_starts_with_bracket.as
//? customPurpose: Part of language/script tests
//? description: ...
Where an expression is allowed (a script that is one expression, an expect
value, a substitution), '[' starts an array literal unless an assignment
operator, '++', or '--' follows its ']'; then it is a destructuring target
(#572).
//? sourceType: script
//?
//? test: script-is-array-literal
//? description: a script that is only [1, 2] is that array (the expect line is one too)
//? expect: [1, 2]
//? source: ...

[1, 2]

//?
//? test: script-is-array-of-mixed
//? description: an array of mixed values is one value, not a list of statements
//? expect: ["a", 4, [true], {"b": null}]
//? source: ...

["a", 4, [true], {b: null}]

//?
//? test: script-is-empty-array
//? description: []
//? expect: []
//? source: ...

[]

//?
//? test: array-literal-then-operator
//? description: an array literal can go on with operators
//? expect: true
//? source: ...

[1, 2] == [1, 2]

//?
//? test: substitution-starts-with-bracket
//? description: ${[1, 2] == x} and ${[...a, 3]}
//? expect: 0
//? source: ...

let x = [1, 2];
let a = [1, 2];
assert(`${[1, 2] == x}` === true, "== in substitution");
assert(length(`${[...a, 3]}`) > 0, "spread in substitution");
return 0;

//?
//? test: destructuring-still-a-target
//? description: [a, b] = ... still destructures, with nested patterns and a template holding ']'
//? expect: 0
//? source: ...

let a = 0;
let b = 0;
[a, b] = [1, 2];
assert(a === 1 && b === 2, "plain");
[a, b] = [b, a];
assert(a === 2 && b === 1, "swap");
[[a], b] = [[5], `x${a}]`];
assert(a === 5 && b === "x2]", "nested and template");
return 0;

//?
//? test: compound-assign-to-destructure
//? description: [a] += x has no meaning: compile error
//? expect: true
//? source: ...

try {
    compile<script>(script("let a = 1; [a] += [3];"));
}
catch (e) {
    return e.id === "syntax" && includes<string>(e.message,
        "A destructuring target can only be assigned with '='");
}
return false;

//?
//? test: increment-destructure
//? description: [a]++ is a compile error, not a runtime one
//? expect: true
//? source: ...

try {
    compile<script>(script("let a = 1; [a]++;"));
}
catch (e) {
    return e.id === "syntax" && includes<string>(e.message,
        "A destructuring target can only be assigned with '='");
}
return false;
