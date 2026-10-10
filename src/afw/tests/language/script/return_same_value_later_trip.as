#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: return_same_value_later_trip.as
//? customPurpose: Part of core language tests
//? description: return from a nested { } on a later loop trip, of the value an earlier trip left as the result
//? sourceType: script
//?
//? test: while
//? description: return i in a nested { } returned the body's earlier statement value (7) when trip 1's i = 1 left the same value as the result
//? expect: 0
//? source: ...

function g() {
    let i = 3;
    while (true) {
        abs(-7);
        {
            if (i == 1) return i;
        }
        i = 1;
    }
}
assert(g() === 1, "while");
return 0;

//? test: closure-pushed
//? description: the same with a closure pushed first (returned the closure)
//? expect: 0
//? source: ...

let fns = [];
function g() {
    let i = 3;
    while (true) {
        push(fns, function () { return 1; });
        {
            if (i == 1) return i;
        }
        i = 1;
    }
}
assert(g() === 1, "closure pushed");
return 0;

//? test: for-of-and-for
//? description: for-of and for (let ...) trips
//? expect: 0
//? source: ...

function f1() {
    let i = 3;
    for (const x of [1, 2]) {
        abs(-7);
        {
            if (i == 1) return i;
        }
        i = 1;
    }
    return -1;
}
function f2() {
    let i = 3;
    for (let k = 0; k < 2; k = k + 1) {
        abs(-7);
        {
            if (i == 1) return i;
        }
        i = 1;
    }
    return -1;
}
assert(f1() === 1, "for-of");
assert(f2() === 1, "for");
return 0;
