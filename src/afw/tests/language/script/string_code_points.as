#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: string_code_points.as
//? customPurpose: Part of language/script tests
//? description: ...
Issue #153 — UTF-8 values as code-point sequences:
- for-of walks code points (one-code-point string elements)
- Bracket s[i] is a code-point index (not octet); OOR soft undefined
- Negative index counts from end (like array)
- Assignment into s[i] is rejected (immutable sequence)
//? sourceType: script
//?
//? test: for-of-ascii
//? description: for-of visits each ASCII code point as a one-char string
//? expect: 0
//? source: ...

let seen = [];
for (let c of "ab") {
    push(seen, c);
}
assert(length(seen) === 2);
assert(seen[0] === "a");
assert(seen[1] === "b");
return 0;

//?
//? test: for-of-multibyte
//? description: for-of counts code points, not UTF-8 bytes (euro + a)
//? expect: 0
//? source: ...

/* U+20AC EURO SIGN is three UTF-8 bytes, one code point */
let s = "\u20ac" + "a";
let seen = [];
for (let c of s) {
    push(seen, c);
}
assert(length(seen) === 2, "two code points");
assert(length(seen[0]) === 1, "euro is one code point string");
assert(seen[1] === "a");
assert(length(s) === 2, "string length is code points");
return 0;

//?
//? test: for-of-empty
//? description: for-of empty string does not run body
//? expect: 0
//? source: ...

let n = 0;
for (let c of "") {
    n = n + 1;
}
assert(n === 0);
return 0;

//?
//? test: for-of-array-still-works
//? description: for-of array path still works via shared iterator façade
//? expect: 0
//? source: ...

let seen = [];
for (let v of [1, 2]) {
    push(seen, v);
}
assert(length(seen) === 2);
assert(seen[0] === 1);
assert(seen[1] === 2);
return 0;

//?
//? test: index-ascii
//? description: s[i] returns one-code-point string
//? expect: 0
//? source: ...

const s = "abc";
assert(s[0] === "a");
assert(s[1] === "b");
assert(s[2] === "c");
return 0;

//?
//? test: index-multibyte
//? description: s[0] is full euro code point, not a single octet
//? expect: 0
//? source: ...

const s = "\u20ac" + "z";
assert(s[0] === "\u20ac");
assert(s[1] === "z");
assert(length(s[0]) === 1);
return 0;

//?
//? test: index-out-of-range
//? description: s[i] out of range is undefined (soft like array)
//? expect: 0
//? source: ...

const s = "ab";
assert(s[2] === undefined);
assert(s[99] === undefined);
return 0;

//?
//? test: index-negative
//? description: negative index counts from end
//? expect: 0
//? source: ...

const s = "abc";
assert(s[-1] === "c");
assert(s[-2] === "b");
assert(s[-3] === "a");
assert(s[-4] === undefined);
return 0;

//?
//? test: index-empty
//? description: any index on empty string is undefined
//? expect: 0
//? source: ...

const s = "";
assert(s[0] === undefined);
assert(s[-1] === undefined);
return 0;

//?
//? test: assign-rejected
//? description: cannot assign into string index (immutable sequence)
//? expect: error
//? source: ...

let s = "ab";
s[0] = "x";
return 0;

//?
//? test: for-of-break
//? description: break leaves for-of over string
//? expect: 0
//? source: ...

let seen = [];
for (let c of "abcd") {
    push(seen, c);
    if (c === "b") {
        break;
    }
}
assert(length(seen) === 2);
assert(seen[0] === "a");
assert(seen[1] === "b");
return 0;

//?
//? test: map-string-identity
//? description: map over string walks code points (array formal / HOF choke)
//? expect: 0
//? source: ...

let out = map(function (c) { return c; }, "ab");
assert(length(out) === 2);
assert(out[0] === "a");
assert(out[1] === "b");
return 0;

//?
//? test: map-string-multibyte
//? description: map over string with multi-byte code point
//? expect: 0
//? source: ...

let out = map(function (c) { return c; }, "\u20ac" + "z");
assert(length(out) === 2);
assert(out[0] === "\u20ac");
assert(out[1] === "z");
return 0;

//?
//? test: filter-string
//? description: filter over string keeps selected code points
//? expect: 0
//? source: ...

let out = filter(function (c) { return c !== "b"; }, "abc");
assert(length(out) === 2);
assert(out[0] === "a");
assert(out[1] === "c");
return 0;

//?
//? test: reduce-string
//? description: reduce over string concatenates code points
//? expect: 0
//? source: ...

let s = reduce(function (acc, c) { return acc + c; }, "", "xy");
assert(s === "xy");
return 0;

//?
//? test: map-empty-string
//? description: map of empty string is empty array
//? expect: 0
//? source: ...

let out = map(function (c) { return c; }, "");
assert(length(out) === 0);
return 0;

//?
//? test: Deferred-produce-type-script-call-return
//? description: script call produce type for typed return (compile soft probes)
//? skip: true
//? skipReason: ...
Deferred: produce-type percolation — script call IR does not yet report
return type on get_data_type / quick inf->data_type. When that lands
(see designs/compile-optimize-notes.md and #28 comment), type-check /
soft iterator step type on call expressions can lock this without eval.
//? expect: 0
//? source: ...

/* Placeholder body: unskip when produce type is filled on script calls. */
const f = function (): string {
    return "hi";
};
const x: string = f();
assert(x === "hi");
return 0;
