#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: first_arg_evaluated_once.as
//? customPurpose: Part of language/script tests
//? description: ...
Issue #507: a polymorphic call or a <dataType> call evaluates its first
argument once. The dispatch evaluates it to pick the data type; the
function then uses that value instead of evaluating the argument again.
//? sourceType: script
//?
//? test: polymorphic-first-arg-once
//? description: Standard polymorphic dispatch (length, normalize_space, includes).
//? skip: false
//? expect: 0
//? source: ...

let n = 0;
function f(): string { n = n + 1; return "ab"; }
function g(): array { n = n + 1; return [1, 2]; }

assert(length(f()) === 2, "length value");
assert(n === 1, "length evaluates f() once");

n = 0;
assert(normalize_space(f()) === "ab", "normalize_space value");
assert(n === 1, "normalize_space evaluates f() once");

n = 0;
assert(includes<array>(g(), 1) === true, "includes value");
assert(n === 1, "includes<array> evaluates g() once");

n = 0;
assert(add(1, length(f())) === 3, "nested value");
assert(n === 1, "nested length evaluates f() once");

return 0;

//?
//? test: data-type-first-arg-once
//? description: A <dataType> function called directly.
//? skip: false
//? expect: 0
//? source: ...

let n = 0;
function f(): string { n = n + 1; return "ab"; }

assert(length<string>(f()) === 2, "length<string> value");
assert(n === 1, "length<string> evaluates f() once");

n = 0;
assert(to_string<string>(f()) === "ab", "to_string<string> value");
assert(n === 1, "to_string<string> evaluates f() once");

return 0;

//?
//? test: first-arg-once-controls
//? description: Functions that already evaluated once stay at one.
//? skip: false
//? expect: 0
//? source: ...

let n = 0;
function f(): string { n = n + 1; return "ab"; }

assert(eq(f(), "ab") === true, "eq value");
assert(n === 1, "eq evaluates f() once");

n = 0;
assert(concat(f(), "c") === "abc", "concat value");
assert(n === 1, "concat evaluates f() once");

return 0;
