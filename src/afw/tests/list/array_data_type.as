#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: array_data_type.as
//? customPurpose: Part of core function tests
//? description: ...
An array's data type as values are added and removed (#583).
An array created without a data type has the data type of every value it
holds, kept up to date as values are added and removed. An array created
with a data type throws when a value of another data type is added. A
copy keeps whichever it was. Checked here through a T[] declaration under
#compile typeCheck, which reads the array's data type.
//? sourceType: script
//?
//? test: generic-push-other-type
//? description: an array literal is not typed by what it holds
//? expect: 0
//? source: ...

let a = [1, 2];
push(a, "x");
assert(a[2] === "x", "pushed string");
let b = clone([1, 2]);
push(b, "x");
assert(b[2] === "x", "clone of a literal is not typed");
return 0;

//?
//? test: typed-push-other-type
//? description: a typed array rejects a value of another data type
//? expect: error:An array of integer can not hold a value of data type string
//? source: ...

let a = array<integer>(1, 2);
push(a, "x");
return 0;

//?
//? test: typed-set-other-type
//? description: a typed array rejects a set of another data type
//? expect: error:An array of integer can not hold a value of data type string
//? source: ...

let a = array<integer>(1, 2);
a[0] = "x";
return 0;

//?
//? test: typed-clone-stays-typed
//? description: a copy of a typed array is typed
//? expect: error:An array of integer can not hold a value of data type string
//? source: ...

let a = clone(array<integer>(1, 2));
push(a, "x");
return 0;

//?
//? test: mixed-is-not-integers
//? description: a mixed array is not integer[]
//? expect: error
//? source: ...

#compile typeCheck;
function ints(v: any): integer[] { const k: integer[] = v; return k; }
ints([1, 2, "x"]);
return 0;

//?
//? test: remove-other-type
//? description: removing the only value of another data type
//? expect: 0
//? source: ...

#compile typeCheck;
function ints(v: any): integer[] { const k: integer[] = v; return k; }
let a = [1, 2, "x"];
pop(a);
ints(a);
let b = [1, "x", 2];
splice(b, 1, 1);
ints(b);
let c = [1, 2];
c[1] = "x";
c[1] = 3;
ints(c);
return 0;

//?
//? test: remove-first-type
//? description: removing every value of the first data type counts again
//? expect: 0
//? source: ...

#compile typeCheck;
function ints(v: any): integer[] { const k: integer[] = v; return k; }
let a = ["x", 1, 2];
shift(a);
ints(a);
push(a, 3);
ints(a);
let b = ["x", "y", 1];
shift(b);
shift(b);
ints(b);
return 0;

//?
//? test: emptied-is-untyped
//? description: an emptied array takes any data type again
//? expect: 0
//? source: ...

#compile typeCheck;
function strs(v: any): string[] { const k: string[] = v; return k; }
let a = [1];
pop(a);
push(a, "x");
strs(a);
return 0;

//?
//? test: array-of-data-type
//? description: array<T> makes a typed array from values and spreads
//? expect: 0
//? source: ...

#compile typeCheck;
const a: integer[] = array<integer>(1, ...[2, 3]);
assert(length(a) === 3, "three entries");
const b = array<string>();
push(b, "x");
assert(b[0] === "x", "empty typed array takes its data type");
const c = array_integer(4);
assert(c[0] === 4, "plain function name");
return 0;

//?
//? test: array-of-data-type-argument
//? description: array<T> rejects a value of another data type
//? expect: error:Argument 2 of array<integer>: expected integer but got string; write to_integer(v)
//? source: ...

array<integer>(1, "2");
return 0;

//?
//? test: array-of-data-type-spread
//? description: array<T> rejects a spread entry of another data type
//? expect: error:Argument 1 of array<date>: expected date but got string; write "2026-10-12"date or to_date(v)
//? source: ...

array<date>(...["2026-10-12"]);
return 0;

//?
//? test: one-and-only
//? description: one_and_only<T> takes an array of exactly one T (#583)
//? expect: 0
//? source: ...

assert(one_and_only<integer>([5]) === 5);
assert(one_and_only<integer>(array<integer>(6)) === 6);
for (const bad of [5, [], [1, 2], ["5"]]) {
    let threw = false;
    try { one_and_only<integer>(bad); } catch (e) { threw = true; }
    assert(threw, "one_and_only<integer>(" + stringify(bad) + ") did not throw");
}
return 0;
