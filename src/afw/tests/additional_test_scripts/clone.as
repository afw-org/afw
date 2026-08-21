#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: clone.as
//? customPurpose: Part of custom tests
//? description: Test the clone function.
//? sourceType: script
//?
//? test: clone_object
//? description: Clone an object
//? expect: 0
//? source: ...

let obj1: object = {
    "a": 1,
    "b": "abc",
    "c": true
};

let obj2: object = clone(obj1);

assert(obj1 === obj2);

obj2.a = 2;

assert(obj1.a === 1);
assert(obj2.a === 2);
assert(obj1 !== obj2);

return 0;

//?
//? test: clone_object_nested
//? description: Nested object properties are independent after clone
//? expect: 0
//? source: ...

let orig = {
    child: { x: 1 }
};
let copy = clone(orig);

copy.child.x = 2;
orig.child.y = 3;

assert(orig.child.x === 1, "orig nested not changed by copy");
assert(copy.child.x === 2, "copy nested changed");
assert(is_nullish(copy.child.y), "copy nested not changed by orig");

return 0;

//?
//? test: clone_array_of_objects
//? description: Objects inside a cloned array are independent
//? expect: 0
//? source: ...

let orig = [{ x: 1 }];
let copy = clone(orig);

copy[0].x = 2;

assert(orig[0].x === 1, "orig element not changed");
assert(copy[0].x === 2, "copy element changed");

return 0;

//?
//? test: clone_list
//? description: Clone an array
//? expect: 0
//? source: ...

let list1: array = [0, 1, 2];
let list2: array = clone(list1);

assert(list1 === list2);

list2[1] = 3;

assert(list1[1] === 1);
assert(list2[1] === 3);
assert(list1 !== list2);

return 0;