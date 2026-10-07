#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: evaluate_with_retry.as
//? customPurpose: Part of compiler category tests
//? description: Test evaluate_with_retry function
//? sourceType: script
//?
//? test: evaluate_with_retry-no-params
//? description: Test evaluate_with_retry without any parameters
//? expect: error
//? source: ...

evaluate_with_retry();
//?
//? test: evaluate_with_retry-default-one-retry
//? description: Without limit, one retry: a value that fails once succeeds.
//? expect: 2
//? source: ...

let n: integer = 0;
function f(): integer {
    n = n + 1;
    if (n < 2) {
        throw "not yet";
    }
    return n;
}
return evaluate_with_retry(f());

//?
//? test: evaluate_with_retry-default-gives-up
//? description: Without limit, a value that fails twice is an error after 2 tries.
//? expect: 2
//? source: ...

let n: integer = 0;
function f(): integer {
    n = n + 1;
    throw "never";
}
try {
    evaluate_with_retry(f());
}
catch {
}
return n;

//?
//? test: evaluate_with_retry-limit-ten
//? description: limit 10 makes at most 11 tries.
//? expect: 11
//? source: ...

let n: integer = 0;
function f(): integer {
    n = n + 1;
    throw "never";
}
try {
    evaluate_with_retry(f(), 10);
}
catch {
}
return n;

//?
//? test: evaluate_with_retry-limit-range
//? description: limit outside 1 to 10 is an argument error, before any try.
//? expect: "argument_error argument_error argument_error 0"
//? source: ...

let n: integer = 0;
function f(): integer {
    n = n + 1;
    return n;
}
let ids: array = [];
for (const limit of [0, 11, #integerMax]) {
    try {
        evaluate_with_retry(f(), limit);
    }
    catch (e) {
        push(ids, e.id);
    }
}
return join(ids, " ") + " " + string(n);
