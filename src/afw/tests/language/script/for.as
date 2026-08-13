#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: for.as
//? customPurpose: Part of language/script tests
//? description: ...
C-style for initializer is one let, one const, or assignment (issue #62).
for (let i = 0, j = 1; …) is a multi let. for (let i = 0, let j = 1; …)
is not accepted. for-of is unchanged.
//? sourceType: script
//?
//? test: for-let-one
//? description: classic for (let i = 0; …)
//? expect: 0
//? source: ...

let s = 0;
for (let i = 0; i < 3; i = i + 1) {
    s = s + i;
}
assert(s === 3);
return 0;

//?
//? test: for-let-two
//? description: for (let i = 0, j = 1; …) binds both
//? expect: 0
//? source: ...

let s = 0;
for (let i = 0, j = 1; i < 3; i = i + 1) {
    s = s + i + j;
}
assert(s === 6);
return 0;

//?
//? test: for-let-second-sees-first
//? description: later init binding can use an earlier name
//? expect: 0
//? source: ...

let seen = 0;
for (let i = 2, j = i + 1; i < 3; i = i + 1) {
    seen = j;
}
assert(seen === 3);
return 0;

//?
//? test: for-let-does-not-leak
//? description: for-let shadows an outer name
//? expect: 0
//? source: ...

let i = 9;
for (let i = 0; i < 1; i = i + 1) {
    assert(i === 0);
}
assert(i === 9);
return 0;

//?
//? test: for-let-two-does-not-leak
//? description: multi for-let does not change outer names
//? expect: 0
//? source: ...

let i = 9;
let j = 8;
for (let i = 0, j = 1; i < 1; i = i + 1) {
    assert(i === 0);
    assert(j === 1);
}
assert(i === 9);
assert(j === 8);
return 0;

//?
//? test: for-let-let-rejected
//? description: second let keyword in for init is not TS/JS
//? expect: error
//? source: ...

for (let i = 0, let j = 1; i < 1; i = i + 1) {
}

//?
//? test: for-assign-two
//? description: for (i = 0, j = 1; …) assigns existing names
//? expect: 0
//? source: ...

let i = 0;
let j = 0;
let s = 0;
for (i = 1, j = 2; i < 3; i = i + 1) {
    s = s + i + j;
}
assert(s === 7);
assert(i === 3);
assert(j === 2);
return 0;

//?
//? test: for-empty-init
//? description: for (; cond; ) with no initializer
//? expect: 0
//? source: ...

let i = 0;
let s = 0;
for (; i < 3; ) {
    s = s + i;
    i = i + 1;
}
assert(s === 3);
assert(i === 3);
return 0;

//?
//? test: for-const-one
//? description: for (const n = 1; …) with no increment assign
//? expect: 0
//? source: ...

let seen = 0;
for (const n = 7; seen < 1; ) {
    seen = n;
}
assert(seen === 7);
return 0;

//?
//? test: for-const-two
//? description: for (const a = 1, b = 2; …)
//? expect: 0
//? source: ...

let s = 0;
for (const a = 1, b = 2; s < 1; ) {
    s = a + b;
}
assert(s === 3);
return 0;

//?
//? test: for-let-uninitialized
//? description: for (let i; …) starts undefined
//? expect: 0
//? source: ...

let saw_undefined = false;
let n = 0;
for (let i; n < 2; n = n + 1) {
    if (i === undefined) {
        saw_undefined = true;
        i = 0;
    }
    else {
        i = i + 1;
    }
}
assert(saw_undefined);
return 0;

//?
//? test: for-of-let-still-works
//? description: for-of is unchanged
//? expect: 0
//? source: ...

let s = 0;
for (let v of [1, 2, 3]) {
    s = s + v;
}
assert(s === 6);
return 0;

//?
//? test: for-let-destructure
//? description: destructure in for let init
//? expect: 0
//? source: ...

let s = 0;
for (let [x] = [4]; s < 1; ) {
    s = x;
}
assert(s === 4);
return 0;
