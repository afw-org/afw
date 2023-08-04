#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: add_entries.as
//? customPurpose: Part of core function tests
//? description: Tests for add_entries function.
//? sourceType: script
//?
//? test: add_entries-1-int
//? description: Add an integer
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, 1);

assert(length(l1) === 1, "l1 is " + string(length(l1)));
assert(l1[0] === 1, "l1[0] is " + string(l1[0]));

return 0;

//? test: add_entries-1-string
//? description: Add a string
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, "x");

assert(length(l1) === 1, "l1 is " + string(length(l1)));
assert(l1[0] === "x", "l1[0] is " + string(l1[0]));

return 0;

//? test: add_entries-1-boolean
//? description: Add a boolean
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, true);

assert(length(l1) === 1, "l1 is " + string(length(l1)));
assert(l1[0] === true, "l1[0] is " + string(l1[0]));

return 0;

//? test: add_entries-1-null
//? description: Add a null
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, null);

assert(length(l1) === 1, "l1 is " + string(length(l1)));
assert(l1[0] === null, "l1[0] is " + string(l1[0]));

return 0;

//? test: add_entries-1-object
//? description: Add a object
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, { "x": "y", "z": 1 });

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-list-1-int
//? description: Add a single integer entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ 1 ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-list-1-string
//? description: Add a single string entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ "x" ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-list-1-boolean
//? description: Add a single boolean entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ true ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-list-1-null
//? description: Add a single null entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ null ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-list-1-double
//? description: Add a single double entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ 1.0 ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-list-1-object
//? description: Add a single object entry
//? expect: 0
//? source: ...

let l1 = [];

add_entries(l1, [ { "x": "y", "z": 1 } ]);

assert(length(l1) === 1, "l1 is " + string(length(l1)));

return 0;

//? test: add_entries-2
//? description: add_entries from another list
//? expect: 0
//? source: ...

let l1 = [    
    { "x": "y", "z": 1 },    
    { "x": "y", "z": 1 }
];

let l2 = [];
foreach const item of l1 {
    add_entries(l2, [ item ]);
}

assert(length(l2) === 2, "l2 is " + string(length(l1)));

return 0;

//? test: add_entries-3
//? description: add_entries from another list, plus one
//? expect: 0
//? source: ...

let l1 = [    
    { "x": "y", "z": 1 },    
    { "x": "y", "z": 1 }
];

let l2 = [];
foreach const item of l1 {
    const x = 5;

    add_entries(l2, item);
    add_entries(l2, x);
}

assert(length(l2) === 4, "l2 is " + string(length(l1)));

return 0;