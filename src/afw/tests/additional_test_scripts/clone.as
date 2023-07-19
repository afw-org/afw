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
#!/usr/bin/env afw

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

//? test: clone_list
//? description: Clone a list
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let list1: list = [0, 1, 2];
let list2: list = clone(list1);

assert(list1 === list2);

list2[1] = 3;

assert(list1[1] === 1);
assert(list2[1] === 3);
assert(list1 !== list2);

return 0;