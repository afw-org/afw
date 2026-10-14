#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/find_last.as
//? customPurpose: Part of core function tests
//? description: Tests for find_last and find_last_index: from the last entry to the first.
//? sourceType: script
//?
//? test: find_last-value
//? description: The last entry that passes.
//? expect: 3
//? source: ...

[1, 2, 3, 4]->find_last(function(v) { return v % 2 == 1; })

//?
//? test: find_last-none
//? description: None passes: undefined.
//? expect: true
//? source: ...

[1, 2]->find_last(function(v) { return v > 5; }) === undefined

//?
//? test: find_last-index
//? description: find_last_index.
//? expect: 2
//? source: ...

[1, 2, 3, 4]->find_last_index(function(v) { return v < 4; })

//?
//? test: find_last-index-none
//? description: None passes: -1.
//? expect: -1
//? source: ...

[1]->find_last_index(function(v) { return false; })

//?
//? test: find_last-order
//? description: Entries are visited last to first, with their indexes.
//? expect: [3, 2, 1]
//? source: ...

(function() { let seen = []; [1, 2, 3]->find_last(function(v, i) { seen->push(v); return false; }); return seen; })()
//?
//? test: find_last-callback-changes-array
//? description: from the end, the indexes there at the start are visited, each read as it is now, as ECMAScript findLast
//? expect: 0
//? source: ...

let a = [1, 2, 3, 4];
let seen = [];
a->find_last_index(function(v, i) {
    seen->push(v);
    if (i === 3) { a->pop(); a->pop(); a->push(9); }
    return false;
});
assert(eq(seen, [4, 9, 2, 1]), "got: " + stringify(seen));
return 0;
//?
//? test: find_last-callback-removes
//? description: an index a callback removed reads as undefined, as ECMAScript findLast
//? expect: 0
//? source: ...

let a = [1, 2, 3, 4];
let seen = [];
a->find_last(function(v, i) {
    seen->push(v === undefined ? "_" : v);
    if (i === 3) { a->pop(); a->pop(); }
    return false;
});
assert(eq(seen, [4, "_", 2, 1]), "got: " + stringify(seen));
return 0;
