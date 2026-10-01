#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: nested_occupant_share.as
//? customPurpose: Part of language/script tests
//? description: ...
Assigning a nested object or array occupant to a variable shares that
occupant. Mutation through the variable is mutation of the source.
clone() is the isolate (see additional_test_scripts/clone.as).
//? sourceType: script
//?
//? test: object-property-to-variable
//? description: let v = o.child; mutate v; o.child sees it
//? skip: false
//? expect: 0
//? source: ...

let orig = { child: { x: 1 } };
let sharedChild = orig.child;
sharedChild.x = 2;
sharedChild.y = 3;
assert(orig.child.x === 2, "uncloned child assign shares x");
assert(orig.child.y === 3, "uncloned child assign shares y");
return 0;

//?
//? test: array-element-to-variable
//? description: let e = a[0]; mutate e; a[0] sees it
//? skip: false
//? expect: 0
//? source: ...

let orig = [{ n: 1 }];
let sharedEl = orig[0];
sharedEl.n = 9;
sharedEl.extra = true;
assert(orig[0].n === 9, "uncloned array element assign shares");
assert(orig[0].extra === true, "new prop via variable visible on element");
return 0;

//?
//? test: nested-array-property-to-variable
//? description: let a = o.arr; index set and push mutate o.arr
//? skip: false
//? expect: 0
//? source: ...

let orig = { arr: [0, 1] };
let sharedArr = orig.arr;
sharedArr[0] = 7;
assert(orig.arr[0] === 7, "array property assign shares index set");
push(sharedArr, 2);
assert(length(orig.arr) === 3, "array property assign shares push");
assert(orig.arr[2] === 2, "pushed value visible on source array");
return 0;

//?
//? test: deep-property-to-variable
//? description: let b = deep.a.b; mutate b; deep.a.b sees it
//? skip: false
//? expect: 0
//? source: ...

let deep = { a: { b: { c: 1 } } };
let b = deep.a.b;
b.c = 2;
b.d = 3;
assert(deep.a.b.c === 2, "deep property assign shares");
assert(deep.a.b.d === 3, "new prop via deep variable visible on source");
return 0;
