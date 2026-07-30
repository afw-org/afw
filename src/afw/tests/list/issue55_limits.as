#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: issue55_limits.as
//? customPurpose: Part of core function tests
//? description: Boundary and limit tests for issue #55 array/object helpers.
//? sourceType: script
//?
//? test: at-index-equals-length
//? description: at(length) is out of range
//? expect: 0
//? source: ...

let a = [1, 2, 3];
assert(is_nullish(at(a, 3)), "at length");
assert(is_nullish(at(a, 100)), "at far positive");
assert(is_nullish(at(a, -4)), "at past negative");
assert(is_nullish(at(a, -100)), "at far negative");

return 0;

//?
//? test: at-single-element
//? description: at on one-element array endpoints
//? expect: 0
//? source: ...

let a = ["only"];
assert(at(a, 0) === "only", "0");
assert(at(a, -1) === "only", "-1");
assert(is_nullish(at(a, 1)), "1 oob");
assert(is_nullish(at(a, -2)), "-2 oob");

return 0;

//?
//? test: push-empty-then-pop
//? description: push onto empty then pop back to empty
//? expect: 0
//? source: ...

let a = [];
push(a, "x");
assert(length(a) === 1 && a[0] === "x", "after push");
assert(pop(a) === "x", "pop");
assert(length(a) === 0, "empty again");
assert(is_nullish(pop(a)), "pop empty");
assert(is_nullish(shift(a)), "shift empty");

return 0;

//?
//? test: unshift-empty-and-no-args
//? description: unshift empty array and unshift with no values
//? expect: 0
//? source: ...

let a = [];
unshift(a, 1);
assert(length(a) === 1 && a[0] === 1, "unshift empty");

let n = length(a);
unshift(a);
assert(length(a) === n, "unshift no args");

let b = [9];
unshift(b, 7, 8);
assert(b[0] === 7 && b[1] === 8 && b[2] === 9, "multi unshift order");

return 0;

//?
//? test: shift-single-element
//? description: shift last remaining element
//? expect: 0
//? source: ...

let a = [42];
assert(shift(a) === 42, "shift only");
assert(length(a) === 0, "empty");
assert(is_nullish(shift(a)), "shift empty");

return 0;

//?
//? test: pop-single-element
//? description: pop last remaining element
//? expect: 0
//? source: ...

let a = [7];
assert(pop(a) === 7, "pop only");
assert(length(a) === 0, "empty");

return 0;

//?
//? test: splice-start-past-end
//? description: startIndex past end acts as append point
//? expect: 0
//? source: ...

let a = [1, 2];
let r = splice(a, 10, 0, 3);
assert(length(r) === 0, "nothing removed");
assert(length(a) === 3 && a[2] === 3, "appended");

return 0;

//?
//? test: splice-deleteCount-too-large
//? description: deleteCount larger than remaining is clamped
//? expect: 0
//? source: ...

let a = [0, 1, 2];
let r = splice(a, 1, 100);
assert(length(r) === 2 && r[0] === 1 && r[1] === 2, "removed rest");
assert(length(a) === 1 && a[0] === 0, "kept head");

return 0;

//?
//? test: splice-deleteCount-negative
//? description: negative deleteCount treated as zero
//? expect: 0
//? source: ...

let a = [0, 1, 2];
let r = splice(a, 1, -5, 9);
assert(length(r) === 0, "none removed");
assert(a[0] === 0 && a[1] === 9 && a[2] === 1 && a[3] === 2, "inserted only");

return 0;

//?
//? test: splice-start-zero-delete-all
//? description: splice(0) removes everything when deleteCount omitted
//? expect: 0
//? source: ...

let a = [1, 2, 3];
let r = splice(a, 0);
assert(length(r) === 3, "all removed");
assert(length(a) === 0, "empty target");

return 0;

//?
//? test: splice-empty-array
//? description: splice on empty array
//? expect: 0
//? source: ...

let a = [];
let r = splice(a, 0, 0, "z");
assert(length(r) === 0, "no remove");
assert(length(a) === 1 && a[0] === "z", "insert into empty");

let b = [];
let r2 = splice(b, 0, 5);
assert(length(r2) === 0 && length(b) === 0, "delete on empty");

return 0;

//?
//? test: splice-negative-start-beyond
//? description: large negative startIndex clamps to 0
//? expect: 0
//? source: ...

let a = [1, 2, 3];
let r = splice(a, -100, 1);
assert(r[0] === 1, "removed first");
assert(a[0] === 2 && a[1] === 3, "rest");

return 0;

//?
//? test: splice-start-at-length-insert
//? description: startIndex === length inserts at end
//? expect: 0
//? source: ...

let a = [1, 2];
splice(a, length(a), 0, 3, 4);
assert(a[0] === 1 && a[1] === 2 && a[2] === 3 && a[3] === 4, "end insert");

return 0;

//?
//? test: keys-values-entries-single-prop
//? description: single property object limits
//? expect: 0
//? source: ...

let o = { "only": 99 };
assert(length(keys(o)) === 1 && keys(o)[0] === "only", "keys");
assert(length(values(o)) === 1 && values(o)[0] === 99, "values");
assert(entries(o)[0][0] === "only" && entries(o)[0][1] === 99, "entries");

return 0;

//?
//? test: freeze-empty-array-object
//? description: freeze empty containers then mutate fails
//? expect: error
//? source: ...

let a = freeze([]);
push(a, 1);

//?
//? test: freeze-empty-object-mutate
//? description: freeze empty object then set fails
//? expect: error
//? source: ...

let o = freeze({});
o.x = 1;

//?
//? test: shift-immutable-error
//? description: shift on immutable throws
//? expect: error
//? source: ...

let a = freeze([1, 2]);
shift(a);

//?
//? test: unshift-immutable-error
//? description: unshift on immutable throws
//? expect: error
//? source: ...

let a = freeze([1]);
unshift(a, 0);

//?
//? test: count-after-many-ops
//? description: length stays correct through mixed mutators
//? expect: 0
//? source: ...

let a = [];
push(a, 1, 2, 3, 4, 5);
assert(length(a) === 5, "push5");
pop(a);
pop(a);
assert(length(a) === 3, "after pops");
shift(a);
assert(length(a) === 2, "after shift");
unshift(a, 0);
assert(length(a) === 3 && a[0] === 0, "after unshift");
splice(a, 1, 1);
assert(length(a) === 2, "after splice");
assert(at(a, 0) === 0 && at(a, -1) === 3, "ends");

return 0;

//?
//? test: at-after-mutations
//? description: at tracks ends after push/shift
//? expect: 0
//? source: ...

let a = [10, 20, 30];
push(a, 40);
assert(at(a, -1) === 40, "after push");
shift(a);
assert(at(a, 0) === 20, "after shift");
assert(at(a, -1) === 40, "last still 40");

return 0;

//?
//? test: at-long-array-nearer-end-walk
//? description: at on longer array hits head/mid/tail (memory entry_at nearer-end)
//? expect: 0
//? source: ...

/* Residual #55 polish: impl_entry_at walks from nearer end for mid indexes. */
let a = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
assert(length(a) === 10, "len");
assert(at(a, 0) === 0, "head");
assert(at(a, 1) === 1, "near head");
assert(at(a, 4) === 4, "mid low");
assert(at(a, 5) === 5, "mid high");
assert(at(a, 8) === 8, "near tail");
assert(at(a, 9) === 9, "tail");
assert(at(a, -1) === 9, "neg last");
assert(at(a, -2) === 8, "neg near last");
assert(at(a, -10) === 0, "neg first");

return 0;

//?
//? test: index-assign-mid-and-near-end
//? description: a[i]= replaces mid and near-end slots (set_value + entry_at)
//? expect: 0
//? source: ...

let a = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
a[3] = 30;
a[8] = 80;
a[-1] = 90;
assert(at(a, 3) === 30, "mid replace");
assert(at(a, 8) === 80, "near-end replace");
assert(at(a, 9) === 90, "last via -1 assign");
assert(a[0] === 0 && a[4] === 4, "neighbors unchanged");
/* replace same slot again (store-as-is; no hold/release yet #2) */
a[3] = 31;
assert(at(a, 3) === 31, "second replace");

return 0;

//?
//? test: unshift-splice-at-and-for-of
//? description: insert front + mid splice then at/for-of twice (iterator end clear)
//? expect: 0
//? source: ...

let a = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
/* insert_value(0) path used by unshift */
unshift(a, -1);
assert(at(a, 0) === -1 && at(a, 1) === 0, "unshift front");
assert(length(a) === 11, "len after unshift");

/* mid remove/insert exercises entry_at from either end */
splice(a, 5, 1, 50, 51);
assert(at(a, 5) === 50 && at(a, 6) === 51, "mid splice insert");
assert(at(a, -1) === 9, "tail still 9");

let n1 = 0;
for (let x of a) {
    n1 = n1 + 1;
}
let n2 = 0;
for (let x of a) {
    n2 = n2 + 1;
}
assert(n1 === length(a) && n2 === length(a), "double for-of full scan");

/* empty for-of then reuse array */
let empty = [];
let z = 0;
for (let x of empty) {
    z = z + 1;
}
assert(z === 0, "empty for-of");
push(empty, 1);
assert(at(empty, 0) === 1, "reuse after empty iter");

return 0;

//?
//? test: mid-remove-then-at-ends
//? description: splice mid remove keeps ends and length consistent
//? expect: 0
//? source: ...

let a = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
let r = splice(a, 4, 2);
assert(length(r) === 2 && r[0] === 4 && r[1] === 5, "removed mid");
assert(length(a) === 8, "len");
assert(at(a, 0) === 0 && at(a, -1) === 9, "ends");
assert(at(a, 3) === 3 && at(a, 4) === 6, "joined");
assert(at(a, 7) === 9, "last by positive");

return 0;
