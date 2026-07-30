#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: freeze_every_some.as
//? customPurpose: Part of core function tests
//? description: Tests for freeze, every, and some (issue #55).
//? sourceType: script
//?
//? test: freeze-array
//? description: freeze array then mutate fails
//? expect: error
//? source: ...

let a = [1, 2];
let r = freeze(a);
assert(r === a, "returns same");
push(a, 3);

//?
//? test: freeze-array-method
//? description: array->freeze then assign fails
//? expect: error
//? source: ...

let a = [1];
a->freeze();
a[0] = 9;

//?
//? test: freeze-object
//? description: freeze object then set property fails
//? expect: error
//? source: ...

let o = { "a": 1 };
freeze(o);
o.b = 2;

//?
//? test: freeze-object-method
//? description: object->freeze then set fails
//? expect: error
//? source: ...

let o = { "a": 1 };
o->freeze();
o.a = 2;

//?
//? test: freeze-already-immutable
//? description: freeze twice is fine
//? expect: 0
//? source: ...

let a = [1];
freeze(a);
freeze(a);
assert(length(a) === 1, "still readable");
assert(at(a, 0) === 1, "at works");

return 0;

//?
//? test: every-true
//? description: every true when all pass
//? expect: 0
//? source: ...

let a = [2, 4, 6];
let isEven = function (n) { return n % 2 === 0; };
assert(every(isEven, a) === true, "all even");
assert(all_of(isEven, a) === true, "all_of same");

return 0;

//?
//? test: every-false
//? description: every false when one fails
//? expect: 0
//? source: ...

let a = [2, 3, 4];
let isEven = function (n) { return n % 2 === 0; };
assert(every(isEven, a) === false, "not all even");

return 0;

//?
//? test: every-empty
//? description: every on empty is true
//? expect: 0
//? source: ...

let never = function (n) { return false; };
assert(every(never, []) === true, "empty every");

return 0;

//?
//? test: some-true
//? description: some true when one passes
//? expect: 0
//? source: ...

let a = [1, 2, 3];
let isTwo = function (n) { return n === 2; };
assert(some(isTwo, a) === true, "found 2");
assert(any_of(isTwo, a) === true, "any_of same");

return 0;

//?
//? test: some-false
//? description: some false when none pass
//? expect: 0
//? source: ...

let big = function (n) { return n > 10; };
assert(some(big, [1, 2, 3]) === false, "none");

return 0;

//?
//? test: some-empty
//? description: some on empty is false
//? expect: 0
//? source: ...

let always = function (n) { return true; };
assert(some(always, []) === false, "empty some");

return 0;

//?
//? test: every-some-named-function
//? description: every/some work with named function declarations
//? expect: 0
//? source: ...

function positive(n) {
    return n > 0;
}

assert(every(positive, [1, 2, 3]) === true, "every named");
assert(some(positive, [-1, 0, 2]) === true, "some named");
assert(every(positive, [-1, 2]) === false, "every named fail");

return 0;
