#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: add_entries.as
//? customPurpose: Part of core function tests
//? description: Tests for add_entries function.
//? sourceType: script
//?
//? test: add_entries-1-not-array
//? description: a single value is not an array of one (#583); use push or [value]
//? expect: 0
//? source: ...

let l1 = [];

for (const v of [1, true, null, { "x": "y", "z": 1 }]) {
    let threw = false;
    try { add_entries(l1, v); } catch (e) { threw = true; }
    assert(threw, "add_entries of " + stringify(v) + " did not throw");
}
assert(length(l1) === 0, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-string-code-points
//? description: a string is a sequence of its code points (#153), so each one is added
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, "x😀");

assert(length(l1) === 2, "l1 is " + string(length(l1)));
assert(l1[0] === "x" && l1[1] === "😀", "code points");

return 0;

//? test: add_entries-array-1-int
//? description: Add a single integer entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ 1 ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-array-1-string
//? description: Add a single string entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ "x" ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-array-1-boolean
//? description: Add a single boolean entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ true ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-array-1-null
//? description: Add a single null entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ null ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-array-1-double
//? description: Add a single double entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ 1.0 ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-array-1-object
//? description: Add a single object entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ { "x": "y", "z": 1 } ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-2
//? description: add_entries from another array
//? expect: 0
//? source: ...

let l1 = [    
    { "x": "y", "z": 1 },    
    { "x": "y", "z": 1 }
];

let l2 = [];
for (const item of l1) {
    add_entries(l2, [ item ]);
}

assert(length(l2) === 2, "l2 is " + string(length(l1)));

return 0;

//? test: add_entries-3
//? description: add_entries from another array, plus one
//? expect: 0
//? source: ...

let l1 = [    
    { "x": "y", "z": 1 },    
    { "x": "y", "z": 1 }
];

let l2 = [];
for (const item of l1) {
    const x = 5;

    add_entries(l2, [ item ]);
    push(l2, x);
}

assert(length(l2) === 4, "l2 is " + string(length(l1)));

return 0;