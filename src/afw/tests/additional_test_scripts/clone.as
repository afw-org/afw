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

//?
//? test: clone_nested_to_variable
//? description: let v = copy.child shares with copy, not orig
//? expect: 0
//? source: ...

let orig = { child: { x: 1 } };
let copy = clone(orig);
let fromCopy = copy.child;
fromCopy.x = 30;
fromCopy.z = 4;
assert(copy.child.x === 30, "variable from clone.child shares with copy");
assert(copy.child.z === 4, "new prop via variable visible on copy.child");
assert(orig.child.x === 1, "orig not changed by clone.child variable");
assert(is_nullish(orig.child.z), "orig has no z");
return 0;

//?
//? test: clone_array_element_to_variable
//? description: let e = copy[0] shares with copy, not orig
//? expect: 0
//? source: ...

let orig = [{ n: 1 }];
let copy = clone(orig);
let fromCopyEl = copy[0];
fromCopyEl.n = 99;
fromCopyEl.extra = true;
assert(copy[0].n === 99, "variable from clone element shares with copy");
assert(copy[0].extra === true, "new prop via variable visible on copy element");
assert(orig[0].n === 1, "orig element not changed");
assert(is_nullish(orig[0].extra), "orig element has no extra");
return 0;

//?
//? test: clone_nested_array_property_to_variable
//? description: let a = copy.arr shares with copy.arr, not orig.arr
//? expect: 0
//? source: ...

let orig = { arr: [0, 1] };
let copy = clone(orig);
let fromCopyArr = copy.arr;
fromCopyArr[0] = 7;
push(fromCopyArr, 2);
assert(copy.arr[0] === 7, "index set via variable visible on copy.arr");
assert(length(copy.arr) === 3, "push via variable visible on copy.arr");
assert(orig.arr[0] === 0, "orig.arr index not changed");
assert(length(orig.arr) === 2, "orig.arr length not changed");
return 0;

//?
//? test: clone_deep_to_variable
//? description: let b = copy.a.b shares with copy, not orig
//? expect: 0
//? source: ...

let deep = { a: { b: { c: 1 } } };
let copy = clone(deep);
let b2 = copy.a.b;
b2.c = 3;
b2.d = 4;
assert(copy.a.b.c === 3, "deep clone nested variable writes copy");
assert(copy.a.b.d === 4, "new prop via deep variable visible on copy");
assert(deep.a.b.c === 1, "deep clone nested assign independent of orig");
assert(is_nullish(deep.a.b.d), "orig deep has no d");
return 0;

//?
//? test: clone_temp_nested_to_variable
//? description: let v = clone(orig).child; clone root is a temp
//? expect: 0
//? source: ...

let orig = { child: { x: 1 } };
let fromTemp = clone(orig).child;
fromTemp.x = 5;
fromTemp.z = 6;
assert(fromTemp.x === 5, "nested from clone temp mutated");
assert(fromTemp.z === 6, "new prop on nested from clone temp");
assert(orig.child.x === 1, "orig not changed by clone-temp child");
assert(is_nullish(orig.child.z), "orig.child has no z");
return 0;

//?
//? test: clone_of_nested_property
//? description: clone(o.child) is independent of o.child
//? expect: 0
//? source: ...

let orig = { child: { x: 1 } };
let childClone = clone(orig.child);
childClone.x = 8;
childClone.z = 9;
assert(orig.child.x === 1, "clone of nested property does not write orig");
assert(is_nullish(orig.child.z), "orig.child has no z");
assert(childClone.x === 8, "nested property clone mutated");
let fromCopy = clone(orig).child;
let fromCopyClone = clone(fromCopy);
fromCopyClone.x = 99;
assert(fromCopy.x === 1, "clone of clone.child does not write that child");
assert(orig.child.x === 1, "clone of clone.child does not write orig");
return 0;