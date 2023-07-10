#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: nullish_coalescing.as
//? customPurpose: Part of core function tests
//? description: Tests to exercise adaptive script nullish coalescing operator.
//? sourceType: script
//?
//? test: nullish_coalescing-1
//? description: simple tests.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const foo: string = null ?? "default string";
assert(foo === "default string", "nullish coalescing operator did not work against null.");

const bar: integer = 0 ?? 42;
assert(bar === 0, "nullish coalescing operator did not work against 0.");

return 0;

//? test: chainable
//? description: If the coalescing head is undefined/null, follow return the right-side value. Otherwise, return the left-side value.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc x: integer;

x = null ?? undefined ?? 42;
assert(x === 42, "null ?? undefined ?? 42");

x = undefined ?? null ?? 42;
assert(x === 42, "undefined ?? null ?? 42");

x = null ?? null ?? 42;
assert(x === 42, "null ?? null ?? 42");

x = undefined ?? undefined ?? 42;
assert(x === 42, "undefined ?? undefined ?? 42");

return 0;

//? test: short-circuit-number-0
//? description: short circuit if the coalescing head is not undefined or null (0)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc x: integer;

x = undefined;
x = 0 ?? 1;
assert(x === 0, "0 ?? 1");

x = undefined;
x = 0 ?? null;
assert(x === 0, "0 ?? null");

x = undefined;
x = 0 ?? undefined;
assert(x === 0, "0 ?? undefined");

x = undefined;
x = 0 ?? null ?? undefined;
assert(x === 0, "0 ?? null ?? undefined");

x = undefined;
x = 0 ?? undefined ?? null;
assert(x === 0, "0 ?? undefined ?? null");

x = undefined;
x = 0 ?? null ?? null;
assert(x === 0, "0 ?? null ?? null");

x = undefined;
x = 0 ?? undefined ?? undefined;
assert(x === 0, "0 ?? undefined ?? undefined");

x = undefined;
x = null ?? 0 ?? null;
assert(x === 0, "null ?? 0 ?? null");

x = undefined;
x = null ?? 0 ?? undefined;
assert(x === 0, "null ?? 0 ?? undefined");

x = undefined;
x = undefined ?? 0 ?? null;
assert(x === 0, "undefined ?? 0 ?? null");

x = undefined;
x = undefined ?? 0 ?? undefined;
assert(x === 0, "undefined ?? 0 ?? undefined");

return 0;

//? test: short-circuit-number-42
//? description: short circuit if the coalescing head is not undefined or null (42)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc x: integer;

x = undefined;
x = 42 ?? 1;
assert(x === 42, "42 ?? 1");

x = undefined;
x = 42 ?? null;
assert(x === 42, "42 ?? null");

x = undefined;
x = 42 ?? undefined;
assert(x === 42, "42 ?? undefined");

x = undefined;
x = 42 ?? null ?? undefined;
assert(x === 42, "42 ?? null ?? undefined");

x = undefined;
x = 42 ?? undefined ?? null;
assert(x === 42, "42 ?? undefined ?? null");

x = undefined;
x = 42 ?? null ?? null;
assert(x === 42, "42 ?? null ?? null");

x = undefined;
x = 42 ?? undefined ?? undefined;
assert(x === 42, "42 ?? undefined ?? undefined");

x = undefined;
x = null ?? 42 ?? null;
assert(x === 42, "null ?? 42 ?? null");

x = undefined;
x = null ?? 42 ?? undefined;
assert(x === 42, "null ?? 42 ?? undefined");

x = undefined;
x = undefined ?? 42 ?? null;
assert(x === 42, "undefined ?? 42 ?? null");

x = undefined;
x = undefined ?? 42 ?? undefined;
assert(x === 42, "undefined ?? 42 ?? undefined");

return 0;

//? test: short-circuit-number-empty-string
//? description: short circuit if the coalescing head is not undefined or null (the empty string)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc x: string;
loc str: string = '';

x = undefined;
x = str ?? 1;
assert(x === str, "str ?? 1");

x = undefined;
x = str ?? null;
assert(x === str, "str ?? null");

x = undefined;
x = str ?? undefined;
assert(x === str, "str ?? undefined");

x = undefined;
x = str ?? null ?? undefined;
assert(x === str, "str ?? null ?? undefined");

x = undefined;
x = str ?? undefined ?? null;
assert(x === str, "str ?? undefined ?? null");

x = undefined;
x = str ?? null ?? null;
assert(x === str, "str ?? null ?? null");

x = undefined;
x = str ?? undefined ?? undefined;
assert(x === str, "str ?? undefined ?? undefined");

x = undefined;
x = null ?? str ?? null;
assert(x === str, "null ?? str ?? null");

x = undefined;
x = null ?? str ?? undefined;
assert(x === str, "null ?? str ?? undefined");

x = undefined;
x = undefined ?? str ?? null;
assert(x === str, "undefined ?? str ?? null");

x = undefined;
x = undefined ?? str ?? undefined;
assert(x === str, "undefined ?? str ?? undefined");

return 0;

//? test: short-circuit-number-false
//? description: short circuit if the coalescing head is not undefined or null (false)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc x: boolean;

x = undefined;
x = false ?? 1;
assert(x === false, "false ?? 1");

x = undefined;
x = false ?? null;
assert(x === false, "false ?? null");

x = undefined;
x = false ?? undefined;
assert(x === false, "false ?? undefined");

x = undefined;
x = false ?? null ?? undefined;
assert(x === false, "false ?? null ?? undefined");

x = undefined;
x = false ?? undefined ?? null;
assert(x === false, "false ?? undefined ?? null");

x = undefined;
x = false ?? null ?? null;
assert(x === false, "false ?? null ?? null");

x = undefined;
x = false ?? undefined ?? undefined;
assert(x === false, "false ?? undefined ?? undefined");

x = undefined;
x = null ?? false ?? null;
assert(x === false, "null ?? false ?? null");

x = undefined;
x = null ?? false ?? undefined;
assert(x === false, "null ?? false ?? undefined");

x = undefined;
x = undefined ?? false ?? null;
assert(x === false, "undefined ?? false ?? null");

x = undefined;
x = undefined ?? false ?? undefined;
assert(x === false, "undefined ?? false ?? undefined");

return 0;


//? test: short-circuit-number-true
//? description: short circuit if the coalescing head is not undefined or null (true)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc x: boolean;

x = undefined;
x = true ?? 1;
assert(x === true, "true ?? 1");

x = undefined;
x = true ?? null;
assert(x === true, "true ?? null");

x = undefined;
x = true ?? undefined;
assert(x === true, "true ?? undefined");

x = undefined;
x = true ?? null ?? undefined;
assert(x === true, "true ?? null ?? undefined");

x = undefined;
x = true ?? undefined ?? null;
assert(x === true, "true ?? undefined ?? null");

x = undefined;
x = true ?? null ?? null;
assert(x === true, "true ?? null ?? null");

x = undefined;
x = true ?? undefined ?? undefined;
assert(x === true, "true ?? undefined ?? undefined");

x = undefined;
x = null ?? true ?? null;
assert(x === true, "null ?? true ?? null");

x = undefined;
x = null ?? true ?? undefined;
assert(x === true, "null ?? true ?? undefined");

x = undefined;
x = undefined ?? true ?? null;
assert(x === true, "undefined ?? true ?? null");

x = undefined;
x = undefined ?? true ?? undefined;
assert(x === true, "undefined ?? true ?? undefined");

return 0;


//? test: short-circuit-prevents-evaluation
//? description: short circuit can prevent evaluation of the right-side expressions
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function poison(): any{
    throw "should not evaluate poison";
}

loc x: integer;

x = undefined;
x = undefined ?? 42 ?? undefined ?? poison();
assert(x == 42, "undefined ?? 42 ?? undefined ?? poison()");

x = undefined;
x = 42 ?? undefined ?? poison();
assert(x == 42, "42 ?? undefined ?? poison()");

x = undefined;
x = undefined ?? 42 ?? poison();
assert(x == 42, "undefined ?? 42 ?? poison()");

x = undefined;
x = 42 ?? poison();
assert(x == 42, "42 ?? poison()");

return 0;