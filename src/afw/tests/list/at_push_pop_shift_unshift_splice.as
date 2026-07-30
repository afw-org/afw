#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: at_push_pop_shift_unshift_splice.as
//? customPurpose: Part of core function tests
//? description: Tests for at, push, pop, shift, unshift, splice (issue #55).
//? sourceType: script
//?
//? test: at-basic
//? description: at returns value at index
//? expect: 0
//? source: ...

let a = [10, 20, 30];
assert(at(a, 0) === 10, "at 0");
assert(at(a, 1) === 20, "at 1");
assert(at(a, 2) === 30, "at 2");
assert(a->at(1) === 20, "method at");

return 0;

//?
//? test: at-negative
//? description: at supports negative indexes from end
//? expect: 0
//? source: ...

let a = [10, 20, 30];
assert(at(a, -1) === 30, "at -1");
assert(at(a, -2) === 20, "at -2");
assert(at(a, -3) === 10, "at -3");

return 0;

//?
//? test: at-oob
//? description: at out of range is undefined
//? expect: 0
//? source: ...

let a = [1];
assert(is_nullish(at(a, 1)), "oob positive");
assert(is_nullish(at(a, -2)), "oob negative");
assert(is_nullish(at([], 0)), "empty");

return 0;

//?
//? test: push-basic
//? description: push appends and returns array
//? expect: 0
//? source: ...

let a = [1];
let r = push(a, 2, 3);
assert(r === a, "returns same array");
assert(length(a) === 3, "length");
assert(a[0] === 1 && a[1] === 2 && a[2] === 3, "contents");

a->push(4);
assert(a[3] === 4, "method push");

push(a);
assert(length(a) === 4, "push no args");

return 0;

//?
//? test: pop-basic
//? description: pop removes last and returns it
//? expect: 0
//? source: ...

let a = [1, 2, 3];
assert(pop(a) === 3, "pop 3");
assert(length(a) === 2, "length after pop");
assert(a->pop() === 2, "method pop");
assert(pop(a) === 1, "pop last");
assert(length(a) === 0, "empty");
assert(is_nullish(pop(a)), "pop empty undefined");
assert(is_nullish(a->pop()), "method pop empty");

return 0;

//?
//? test: shift-basic
//? description: shift removes first and returns it
//? expect: 0
//? source: ...

let a = [1, 2, 3];
assert(shift(a) === 1, "shift 1");
assert(a[0] === 2, "new first");
assert(length(a) === 2, "length");
assert(a->shift() === 2, "method shift");
assert(shift(a) === 3, "shift last");
assert(is_nullish(shift(a)), "shift empty");

return 0;

//?
//? test: unshift-basic
//? description: unshift inserts at front preserving order
//? expect: 0
//? source: ...

let a = [3];
unshift(a, 1, 2);
assert(length(a) === 3, "length");
assert(a[0] === 1 && a[1] === 2 && a[2] === 3, "order");

a->unshift(0);
assert(a[0] === 0, "method unshift");
assert(a[1] === 1, "rest shifted");

return 0;

//?
//? test: stack-lifo
//? description: push/pop as LIFO stack
//? expect: 0
//? source: ...

let s = [];
push(s, "a");
push(s, "b");
push(s, "c");
assert(pop(s) === "c", "lifo c");
assert(pop(s) === "b", "lifo b");
assert(pop(s) === "a", "lifo a");

return 0;

//?
//? test: queue-fifo
//? description: push/shift as FIFO queue
//? expect: 0
//? source: ...

let q = [];
push(q, "a");
push(q, "b");
push(q, "c");
assert(shift(q) === "a", "fifo a");
assert(shift(q) === "b", "fifo b");
assert(shift(q) === "c", "fifo c");

return 0;

//?
//? test: splice-remove-only
//? description: splice removes and returns deleted values
//? expect: 0
//? source: ...

let a = [0, 1, 2, 3, 4];
let r = splice(a, 1, 2);
assert(length(r) === 2, "removed len");
assert(r[0] === 1 && r[1] === 2, "removed values");
assert(length(a) === 3, "remaining");
assert(a[0] === 0 && a[1] === 3 && a[2] === 4, "remaining values");

return 0;

//?
//? test: splice-insert-only
//? description: splice with deleteCount 0 inserts
//? expect: 0
//? source: ...

let a = [1, 4];
let r = splice(a, 1, 0, 2, 3);
assert(length(r) === 0, "nothing removed");
assert(a[0] === 1 && a[1] === 2 && a[2] === 3 && a[3] === 4, "inserted");

return 0;

//?
//? test: splice-replace
//? description: splice removes and inserts
//? expect: 0
//? source: ...

let a = ["a", "b", "c", "d"];
let r = a->splice(1, 2, "X", "Y");
assert(r[0] === "b" && r[1] === "c", "removed");
assert(a[0] === "a" && a[1] === "X" && a[2] === "Y" && a[3] === "d", "result");

return 0;

//?
//? test: splice-negative-start
//? description: splice startIndex from end
//? expect: 0
//? source: ...

let a = [0, 1, 2, 3];
let r = splice(a, -2, 1);
assert(r[0] === 2, "removed 2");
assert(a[0] === 0 && a[1] === 1 && a[2] === 3, "rest");

return 0;

//?
//? test: splice-omit-deleteCount
//? description: omitted deleteCount removes to end
//? expect: 0
//? source: ...

let a = [0, 1, 2, 3];
let r = splice(a, 2);
assert(length(r) === 2 && r[0] === 2 && r[1] === 3, "tail removed");
assert(length(a) === 2 && a[0] === 0 && a[1] === 1, "head kept");

return 0;

//?
//? test: push-immutable-error
//? description: push on immutable array throws
//? expect: error
//? source: ...

let a = freeze([1, 2]);
push(a, 3);

//?
//? test: pop-immutable-error
//? description: pop on immutable array throws
//? expect: error
//? source: ...

let a = freeze([1]);
pop(a);

//?
//? test: splice-immutable-error
//? description: splice on immutable array throws
//? expect: error
//? source: ...

let a = freeze([1, 2, 3]);
splice(a, 0, 1);
