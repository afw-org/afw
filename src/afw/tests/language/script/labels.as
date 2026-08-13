#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: labels.as
//? customPurpose: Part of language/script tests
//? description: ...
Loop labels (issue #62). A label may precede for, while, or do.
break / continue may name that label. Labels are not allowed on
blocks or if. One label per loop.
//? sourceType: script
//?
//? test: labeled-for-break
//? description: break label leaves the labeled for
//? expect: 0
//? source: ...

let s = 0;
outer: for (let i = 0; i < 5; i = i + 1) {
    s = s + 1;
    if (i === 1) {
        break outer;
    }
}
assert(s === 2);
return 0;

//?
//? test: labeled-while-break
//? description: break label leaves the labeled while
//? expect: 0
//? source: ...

let i = 0;
let s = 0;
outer: while (i < 5) {
    s = s + 1;
    i = i + 1;
    if (i === 2) {
        break outer;
    }
}
assert(s === 2);
assert(i === 2);
return 0;

//?
//? test: labeled-do-break
//? description: break label leaves the labeled do
//? expect: 0
//? source: ...

let i = 0;
let s = 0;
outer: do {
    s = s + 1;
    i = i + 1;
    if (i === 2) {
        break outer;
    }
} while (i < 5);
assert(s === 2);
return 0;

//?
//? test: labeled-for-of-break
//? description: break label leaves the labeled for-of
//? expect: 0
//? source: ...

let s = 0;
outer: for (let x of [1, 2, 3, 4]) {
    s = s + x;
    if (x === 2) {
        break outer;
    }
}
assert(s === 3);
return 0;

//?
//? test: nested-break-outer
//? description: break outer leaves both loops
//? expect: 0
//? source: ...

let s = 0;
outer: for (let i = 0; i < 3; i = i + 1) {
    for (let j = 0; j < 3; j = j + 1) {
        s = s + 1;
        if (j === 1) {
            break outer;
        }
    }
}
assert(s === 2);
return 0;

//?
//? test: nested-break-inner
//? description: unlabeled break leaves only the inner loop
//? expect: 0
//? source: ...

let s = 0;
outer: for (let i = 0; i < 3; i = i + 1) {
    for (let j = 0; j < 3; j = j + 1) {
        s = s + 1;
        if (j === 0) {
            break;
        }
    }
}
assert(s === 3);
return 0;

//?
//? test: nested-continue-outer
//? description: continue outer skips the rest of the outer body
//? expect: 0
//? source: ...

let s = 0;
let extra = 0;
outer: for (let i = 0; i < 3; i = i + 1) {
    for (let j = 0; j < 2; j = j + 1) {
        s = s + 1;
        continue outer;
    }
    extra = extra + 1;
}
assert(s === 3);
assert(extra === 0);
return 0;

//?
//? test: continue-outer-skips-inner-increment
//? description: continue outer does not run the inner for increment
//? expect: 0
//? source: ...

let inner = 0;
outer: for (let i = 0; i < 2; i = i + 1) {
    for (let j = 0; j < 10; j = j + 1) {
        inner = inner + 1;
        continue outer;
    }
}
assert(inner === 2);
return 0;

//?
//? test: labeled-continue-this
//? description: continue this label skips the rest of the body
//? expect: 0
//? source: ...

let s = 0;
let skipped = 0;
loop: for (let i = 0; i < 3; i = i + 1) {
    s = s + 1;
    continue loop;
    skipped = skipped + 1;
}
assert(s === 3);
assert(skipped === 0);
return 0;

//?
//? test: switch-unlabeled-break
//? description: unlabeled break in switch stays in the loop
//? expect: 0
//? source: ...

let s = 0;
for (let i = 0; i < 3; i = i + 1) {
    switch (i) {
        case 1:
            break;
        default:
            s = s + 1;
    }
    s = s + 10;
}
assert(s === 32);
return 0;

//?
//? test: switch-labeled-break
//? description: labeled break from switch leaves the loop
//? expect: 0
//? source: ...

let s = 0;
outer: for (let i = 0; i < 5; i = i + 1) {
    switch (i) {
        case 2:
            break outer;
        default:
            s = s + 1;
    }
}
assert(s === 2);
return 0;

//?
//? test: switch-continue
//? description: continue from switch continues the enclosing loop
//? expect: 0
//? source: ...

let s = 0;
let after = 0;
for (let i = 0; i < 3; i = i + 1) {
    switch (i) {
        case 1:
            continue;
        default:
            s = s + 1;
    }
    after = after + 1;
}
assert(s === 2);
assert(after === 2);
return 0;

//?
//? test: unlabeled-still-works
//? description: unlabeled break and continue are unchanged
//? expect: 0
//? source: ...

let s = 0;
for (let i = 0; i < 10; i = i + 1) {
    if (i === 1) {
        continue;
    }
    s = s + 1;
    if (i === 3) {
        break;
    }
}
assert(s === 3);
return 0;

//?
//? test: label-on-if-rejected
//? description: labels are not allowed on if
//? expect: error
//? source: ...

here: if (true) {
    break here;
}

//?
//? test: label-on-block-rejected
//? description: labels are not allowed on blocks
//? expect: error
//? source: ...

here: {
    break here;
}

//?
//? test: unknown-label-rejected
//? description: break of an unknown label is a compile error
//? expect: error
//? source: ...

for (let i = 0; i < 1; i = i + 1) {
    break missing;
}

//?
//? test: duplicate-label-rejected
//? description: the same label cannot nest
//? expect: error
//? source: ...

outer: for (let i = 0; i < 1; i = i + 1) {
    outer: for (let j = 0; j < 1; j = j + 1) {
    }
}

//?
//? test: two-labels-rejected
//? description: only one label per loop
//? expect: error
//? source: ...

a: b: for (let i = 0; i < 1; i = i + 1) {
}

//?
//? test: function-cannot-see-label
//? description: a nested function cannot break an outer label
//? expect: error
//? source: ...

outer: for (let i = 0; i < 1; i = i + 1) {
    function f() {
        break outer;
    }
}

//?
//? test: break-label-outside-loop
//? description: labeled break is not allowed outside a matching loop
//? expect: error
//? source: ...

break outer;
