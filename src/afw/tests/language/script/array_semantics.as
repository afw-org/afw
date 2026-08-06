#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: array_semantics.as
//? customPurpose: Part of language/script tests
//? description: ...
Issue #39 and related Adaptive array product semantics:
- Literal elision (holes) → dense undefined elements (script / non-strict)
- Strict JSON forbids elision (covered lightly via compile/json path where practical)
- Assign replace in range; append when index === length; no gap fill
- empty_array(n) pre-size with undefined slots and length caps
- for-of / list helpers visit undefined slots (no ES sparse skip model)
- Bracket OOB get throws; at() OOB returns undefined
//? sourceType: script
//?
//? test: elision-middle-basic
//? description: ['a', ,'b'] has length 3 and middle undefined
//? expect: 0
//? source: ...

let x = ['a', , 'b'];
assert(length(x) === 3, "length");
assert(x[0] === "a", "0");
assert(x[1] === undefined, "hole is undefined");
assert(x[2] === "b", "2");
assert(x[1] === undefined, "explicit compare");
return 0;

//?
//? test: elision-leading
//? description: [,1] leading hole
//? expect: 0
//? source: ...

let x = [, 1];
assert(length(x) === 2);
assert(x[0] === undefined);
assert(x[1] === 1);
return 0;

//?
//? test: elision-double-middle
//? description: [1,,2] and [1,,,2] dense holes
//? expect: 0
//? source: ...

let a = [1, , 2];
assert(length(a) === 3);
assert(a[0] === 1 && a[1] === undefined && a[2] === 2);

let b = [1, , , 2];
assert(length(b) === 4);
assert(b[0] === 1 && b[1] === undefined && b[2] === undefined && b[3] === 2);
return 0;

//?
//? test: elision-trailing-holes-before-bracket
//? description: [,,] is two undefined elements (not empty)
//? expect: 0
//? source: ...

let x = [, ,];
assert(length(x) === 2);
assert(x[0] === undefined && x[1] === undefined);
return 0;

//?
//? test: trailing-comma-not-a-hole
//? description: [1,] length 1 — trailing comma does not add a slot
//? expect: 0
//? source: ...

let x = [1,];
assert(length(x) === 1);
assert(x[0] === 1);
return 0;

//?
//? test: elision-same-as-explicit-undefined
//? description: hole and explicit undefined compare equal as values
//? expect: 0
//? source: ...

let a = ['a', , 'b'];
let b = ['a', undefined, 'b'];
assert(length(a) === length(b));
assert(a[1] === b[1]);
assert(a[1] === undefined);
return 0;

//?
//? test: elision-for-of-visits-undefined
//? description: for-of visits every index including holes as undefined
//? expect: 0
//? source: ...

let seen = [];
for (let v of ['a', , 'b']) {
    push(seen, v === undefined ? "U" : v);
}
assert(length(seen) === 3);
assert(seen[0] === "a" && seen[1] === "U" && seen[2] === "b");
return 0;

//?
//? test: elision-index-walk-visits-undefined
//? description: dense index walk visits every slot including holes
//? expect: 0
//? source: ...

let a = ['a', , 'b'];
let seen = [];
let i = 0;
while (i < length(a)) {
    push(seen, a[i] === undefined ? "U" : a[i]);
    i = i + 1;
}
assert(length(seen) === 3);
assert(seen[0] === "a" && seen[1] === "U" && seen[2] === "b");
return 0;

//?
//? test: elision-nested-in-object
//? description: hole in array nested in object literal
//? expect: 0
//? source: ...

let o = { items: [1, , 3] };
assert(length(o.items) === 3);
assert(o.items[1] === undefined);
return 0;

//?
//? test: elision-with-spread-and-hole
//? description: spread plus elision in one literal
//? expect: 0
//? source: ...

let base = [10, 20];
let x = [...base, , 30];
assert(length(x) === 4);
assert(x[0] === 10 && x[1] === 20 && x[2] === undefined && x[3] === 30);
return 0;

//?
//? test: assign-replace-in-range
//? description: a[i]= replaces existing element
//? expect: 0
//? source: ...

let a = [10, 20, 30];
a[1] = 99;
assert(a[1] === 99);
assert(length(a) === 3);
return 0;

//?
//? test: assign-append-from-empty
//? description: [] then a[0]= appends (grow by one at length)
//? expect: 0
//? source: ...

let a = [];
a[0] = "x";
assert(length(a) === 1);
assert(a[0] === "x");
a[1] = "y";
assert(length(a) === 2);
assert(a[1] === "y");
return 0;

//?
//? test: assign-append-at-length
//? description: a[length]= appends; a[length+1] throws
//? expect: 0
//? source: ...

let a = [1, 2];
a[2] = 3;
assert(length(a) === 3 && a[2] === 3);

let threw = false;
try {
    a[5] = 9;
} catch (e) {
    threw = true;
}
assert(threw, "gap assign must throw");
assert(length(a) === 3, "length unchanged after failed gap assign");
return 0;

//?
//? test: assign-gap-throws-on-empty
//? description: a[1]= on empty array throws (not fill)
//? expect: 0
//? source: ...

let a = [];
let threw = false;
try {
    a[1] = "no";
} catch (e) {
    threw = true;
}
assert(threw);
assert(length(a) === 0);
return 0;

//?
//? test: assign-negative-no-grow
//? description: negative index assign only hits existing range
//? expect: 0
//? source: ...

let a = [1, 2, 3];
a[-1] = 9;
assert(a[2] === 9 && length(a) === 3);

let b = [];
let threw = false;
try {
    b[-1] = 1;
} catch (e) {
    threw = true;
}
assert(threw, "negative on empty throws");
return 0;

//?
//? test: bracket-get-oob-throws
//? description: a[i] past end throws; at() returns undefined
//? expect: 0
//? source: ...

let a = [1, 2, 3];
let threw = false;
try {
    let _ = a[10];
} catch (e) {
    threw = true;
    assert(e.id === "evaluate" || is_defined(e.message), "error shape");
}
assert(threw, "bracket OOB throws");
assert(at(a, 10) === undefined, "at OOB undefined");
assert(at(a, -10) === undefined, "at far negative undefined");
return 0;

//?
//? test: empty-array-zero
//? description: empty_array(0) is empty mutable array
//? expect: 0
//? source: ...

let a = empty_array(0);
assert(length(a) === 0);
a[0] = 1;
assert(length(a) === 1 && a[0] === 1);
return 0;

//?
//? test: empty-array-prefill-undefined
//? description: empty_array(n) length n all undefined then assign
//? expect: 0
//? source: ...

let a = empty_array(4);
assert(length(a) === 4);
assert(a[0] === undefined && a[3] === undefined);
a[2] = "z";
assert(a[2] === "z");
assert(a[1] === undefined);
return 0;

//?
//? test: empty-array-negative-throws
//? description: empty_array(-1) throws arg_error
//? expect: error
//? source: ...

empty_array(-1);
return 0;

//?
//? test: empty-array-over-max-throws
//? description: empty_array beyond max length throws
//? expect: error
//? source: ...

empty_array(1000001);
return 0;

//?
//? test: empty-array-independent-instances
//? description: two empty_array calls do not share storage
//? expect: 0
//? source: ...

let a = empty_array(2);
let b = empty_array(2);
a[0] = 1;
assert(b[0] === undefined, "independent");
return 0;

//?
//? test: push-still-grows
//? description: push remains idiomatic grow path
//? expect: 0
//? source: ...

let a = [];
push(a, 1);
push(a, 2);
assert(length(a) === 2 && a[0] === 1 && a[1] === 2);
return 0;

//?
//? test: hole-then-assign-replace
//? description: can replace a hole slot by index assign
//? expect: 0
//? source: ...

let a = [1, , 3];
assert(a[1] === undefined);
a[1] = 2;
assert(a[1] === 2);
assert(length(a) === 3);
return 0;

//?
//? test: json-strict-elision-rejected
//? description: compile as json rejects mid-list elision
//? expect: error
//? source: ...

compile(json("[1,,2]"));
return 0;

//?
//? test: relaxed-json-elision-allowed
//? description: relaxed_json accepts elision as undefined
//? expect: 0
//? source: ...

let v = compile(relaxed_json("[1,,2]"));
assert(length(v) === 3);
assert(v[0] === 1 && v[1] === undefined && v[2] === 2);
return 0;
