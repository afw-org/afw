#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: array_push_pop_lifetime.as
//? customPurpose: Part of language/script tests
//? description: ...
Managed array push slot_stores. pop/shift transfer the occupant and
register release on the current scope pool so it acts like a temp.
Assign still slot_stores.
//? sourceType: script
//?
//? test: push-pop-discard-loop
//? description: discarded pop extra-hold is released each trip
//? expect: 0
//? source: ...

let a = [0];
let i = 0;
let n = 0;
while (n < 200) {
    push(a, i);
    pop(a);
    i = i + 1;
    n = n + 1;
}
assert(i === 200, "i advanced");
assert(length(a) === 1 && a[0] === 0, "seed kept");
return 0;

//?
//? test: push-pop-assign-loop
//? description: assigned pop of a unique occupant each trip
//? expect: 0
//? source: ...

let a = [];
let i = 0;
let x = 0;
while (i < 200) {
    push(a, i + 1);
    x = pop(a);
    i = i + 1;
}
assert(x === 200, "last assigned pop");
assert(length(a) === 0, "empty");
return 0;

//?
//? test: shift-discard-loop
//? description: discarded shift extra-hold is released each trip
//? expect: 0
//? source: ...

let a = [];
let i = 0;
while (i < 200) {
    push(a, i);
    shift(a);
    i = i + 1;
}
assert(i === 200, "i advanced");
assert(length(a) === 0, "empty");
return 0;

//?
//? test: push-pop-in-use
//? description: finite discarded push/pop does not grow pool_bytes_in_use
//? expect: 0
//? source: ...

let a = [0];
let i = 0;
let n = 0;
while (n < 50) {
    push(a, i);
    pop(a);
    i = i + 1;
    n = n + 1;
}
let start = pool_bytes_in_use();
n = 0;
while (n < 20000) {
    push(a, i);
    pop(a);
    i = i + 1;
    n = n + 1;
}
let used = pool_bytes_in_use() - start;
assert(used < 1000000, "discarded pop extra-hold was released");
return 0;
