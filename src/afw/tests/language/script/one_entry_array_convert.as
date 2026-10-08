#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: one_entry_array_convert.as
//? customPurpose: Part of language/script tests
//? description: ...
A single value passed where a built-in wants an array is converted to a
one entry array holding that value (#496 Example 2). It used to rebuild a
new value from the value's internal: a function value has none, so the
rebuilt value belonged to nothing and was read after its frame was
gone (ASan use-after-poison on the function's return).
//? sourceType: script
//?
//? test: function-literal-returned
//? description: A function literal through one_and_only, returned from a function.
//? expect: true
//? source: ...
#!/usr/bin/env afw

const pick = function () {
    return one_and_only<string>(function (x) { return x; });
};
const f = pick();
return f(5) === 5;
//?
//? test: function-literal-in-try
//? description: The fuzz shape: inside try in a function.
//? expect: true
//? source: ...
#!/usr/bin/env afw

const fuzz = function () {
    try { let r0 = one_and_only<string>(function (x) { return x; }); return r0; } catch (e) { }
};
return fuzz()(7) === 7;
//?
//? test: scalar-still-converts
//? description: A scalar still becomes a one entry array.
//? expect: 5
//? source: ...
#!/usr/bin/env afw

const pick = function () {
    return one_and_only<integer>(5);
};
return pick();
//?
//? test: function-in-variable
//? description: ...
A function value read from a variable evaluates to itself; convert used
to keep evaluating it and threw "value required > 20 evaluations".
//? expect: 9
//? source: ...
#!/usr/bin/env afw

const f = function (x) { return x; };
const pick = function () {
    return one_and_only<string>(f);
};
return pick()(9);
