#!/usr/bin/env -S afw --syntax test_script
//? testScript: call_builtin_value.as
//? customPurpose: Part of language/script tests
//? description: Built-in assigned to a variable (or other callee) is callable
//? sourceType: script
//?
//? test: let-builtin-call
//? description: let add1 = add; add1(2, 3)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let add1 = add;
assert(add1(2, 3) === 5);
return 0;
//?
//? test: const-builtin-call
//? description: const add1 = add; add1(2, 3)
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const add1 = add;
assert(add1(2, 3) === 5);
return 0;
//?
//? test: assign-builtin-call
//? description: Later assign of a built-in, then call
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let add1;
add1 = add;
assert(add1(2, 3) === 5);
return 0;
//?
//? test: object-property-builtin-call
//? description: o.fn(2, 3) when fn is a built-in
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const o = { fn: add };
assert(o.fn(2, 3) === 5);
return 0;
//?
//? test: array-slot-builtin-call
//? description: a[0](2, 3) when the slot is a built-in
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const a = [add];
assert(a[0](2, 3) === 5);
return 0;
//?
//? test: apply-builtin
//? description: Pass a built-in into a script function and call it
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function apply(fn, x, y) {
    return fn(x, y);
}

assert(apply(add, 2, 3) === 5);
return 0;
//?
//? test: getter-returns-builtin
//? description: get()(2, 3) when get returns a built-in
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function get() {
    return add;
}

assert(get()(2, 3) === 5);
return 0;
//?
//? test: paren-builtin-name
//? description: (add)(2, 3) — compile may already specialize
//? expect: 0
//? source: ...
#!/usr/bin/env afw

assert((add)(2, 3) === 5);
return 0;
//?
//? test: polymorphic-eq-via-variable
//? description: let eq1 = eq; eq1(1, 1) resolves the data-type method
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let eq1 = eq;
assert(eq1(1, 1) === true);
assert(eq1("a", "a") === true);
return 0;
//?
//? test: print-via-variable
//? description: Non-polymorphic built-in called through a variable
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let p = print;
p("ok");
return 0;
//?
