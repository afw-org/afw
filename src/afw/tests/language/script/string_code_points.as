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
//? description: map over a spread string walks its code points
//? expect: 0
//? source: ...

let out = map([..."ab"], function (c) { return c; });
assert(length(out) === 2);
assert(out[0] === "a");
assert(out[1] === "b");
return 0;

//?
//? test: map-string-multibyte
//? description: map over a spread string with a multi-byte code point
//? expect: 0
//? source: ...

let out = map([...("\u20ac" + "z")], function (c) { return c; });
assert(length(out) === 2);
assert(out[0] === "\u20ac");
assert(out[1] === "z");
return 0;

//?
//? test: filter-string
//? description: filter over a spread string keeps selected code points
//? expect: 0
//? source: ...

let out = filter([..."abc"], function (c) { return c !== "b"; });
assert(length(out) === 2);
assert(out[0] === "a");
assert(out[1] === "c");
return 0;

//?
//? test: reduce-string
//? description: reduce over a spread string concatenates code points
//? expect: 0
//? source: ...

let s = reduce([..."xy"], function (acc, c) { return acc + c; }, "");
assert(s === "xy");
return 0;

//?
//? test: map-empty-string
//? description: map of an empty spread string is an empty array
//? expect: 0
//? source: ...

let out = map([...""], function (c) { return c; });
assert(length(out) === 0);
return 0;

//?
//? test: script-formal-array-string
//? description: a string is not an array for a typed script parameter (#583)
//? expect: error:Type error in parameter: expected array but got string
//? source: ...

const join_cps = function (parts: array) {
    let s = "";
    for (let c of parts) {
        s = s + c;
    }
    return s;
};
return join_cps("ab");

//?
//? test: script-formal-array-spread
//? description: spread a string to pass its code points to a typed array parameter
//? expect: 0
//? source: ...

const join_cps = function (parts: array) {
    let s = "";
    for (let c of parts) {
        s = s + c;
    }
    return s;
};
assert(join_cps([..."ab"]) === "ab");
assert(join_cps([..."\u20ac" + "z"]) === "\u20ac" + "z");
return 0;

//?
//? test: script-formal-string-array-annotation
//? description: a string is not a string[] for a typed script parameter (#583)
//? expect: error:Type error in parameter: expected array type string[] but got string
//? source: ...

const first = function (parts: string[]) {
    return parts[0];
};
return first("xy");

//?
//? test: script-formal-string-array-spread
//? description: spread a string to pass it as string[]
//? expect: 0
//? source: ...

const first = function (parts: string[]) {
    return parts[0];
};
assert(first([..."xy"]) === "x");
assert(first([..."\u20ac" + "a"]) === "\u20ac");
return 0;

//?
//? test: includes-code-point-position
//? description: includes position is code-point index not byte (#153)
//? expect: 0
//? source: ...

/* Euro is one CP (3 bytes); position 1 is 'z', not mid-euro */
const s = "\u20ac" + "z";
assert(includes(s, "z") === true);
assert(includes(s, "z", 1) === true);
assert(includes(s, "z", 0) === true);
assert(includes(s, "\u20ac", 0) === true);
assert(includes(s, "\u20ac", 1) === false);
return 0;

//?
//? test: index-of-code-point
//? description: index_of returns code-point index for multi-byte
//? expect: 0
//? source: ...

const s = "\u20ac" + "z";
assert(index_of(s, "z") === 1);
assert(index_of(s, "\u20ac") === 0);
return 0;

//?
//? test: replace-code-point-boundaries
//? description: replace finds match at CP boundaries not mid-sequence
//? expect: 0
//? source: ...

/* Replace euro (one CP) with X; rest is z */
const s = replace("\u20ac" + "z", "\u20ac", "X");
assert(s === "Xz");
assert(replace("a\u20ac" + "b", "\u20ac", "") === "ab");
return 0;

//?
//? test: replace-empty-match-default
//? description: empty match default limit inserts once at the start
//? differences: Same as ES String.replace("", x). Adaptive default limit is 1.
//? expect: 0
//? source: ...

assert(replace("abc", "", "x") === "xabc");
assert(replace("abc", "", "x", 1) === "xabc");
assert(replace("abc", "", "x", 0) === "abc");
return 0;

//?
//? test: replace-empty-match-all
//? description: empty match limit -1 inserts at every code-point boundary
//? differences: Same shape as ES replaceAll("", x), but Adaptive walks code points (not UTF-16 units).
//? expect: 0
//? source: ...

assert(replace("abc", "", "x", -1) === "xaxbxcx");
assert(replace("abc", "", "x", 2) === "xaxbc");
assert(replace("", "", "x") === "x");
assert(replace("", "", "x", -1) === "x");
assert(replace("abc", "", "", -1) === "abc");
return 0;

//?
//? test: replace-empty-match-multibyte
//? description: empty-match replace-all steps by code point, not UTF-16 unit
//? differences: ES replaceAll("", x) on a supplementary character splits the surrogate pair; Adaptive inserts around the one code point.
//? expect: 0
//? source: ...

assert(replace("\u20ac" + "z", "", "x", -1) === "x\u20ac" + "xzx");
assert(replace("\u{1F600}", "", "x", -1) === "x\u{1F600}x");
return 0;

//?
//? test: replace-empty-match-edges
//? description: method form, anyURI, limit past last boundary, longer replace-all
//? expect: 0
//? source: ...

const s = "abc";
assert(s->replace("", "x") === "xabc");
assert(s->replace("", "x", -1) === "xaxbxcx");
assert(replace("abc"anyURI, "", "x", -1) === "xaxbxcx");
/* limit larger than n+1 boundaries must stop, not spin */
assert(replace("ab", "", "x", 100) === "xaxbx");
const src = "abcdefghij";
const got = replace(src, "", "|", -1);
assert(got === "|a|b|c|d|e|f|g|h|i|j|");
assert(length(got) === 21);
return 0;

//?
//? test: empty-needle-search-siblings
//? description: empty substring in includes / index_of / last_index_of / starts_with / ends_with terminates (related to empty-match replace)
//? differences: Same results as ES includes/indexOf/lastIndexOf/startsWith/endsWith with "".
//? expect: 0
//? source: ...

assert(includes("abc", "") === true);
assert(index_of("abc", "") === 0);
assert(last_index_of("abc", "") === 3);
assert(starts_with("abc", "") === true);
assert(ends_with("abc", "") === true);
assert(includes("", "") === true);
assert(index_of("", "") === 0);
assert(last_index_of("", "") === 0);
return 0;

//?
//? test: split-separator-code-point
//? description: split on separator only at CP boundaries
//? expect: 0
//? source: ...

const parts = split("a\u20ac" + "b", "\u20ac");
assert(length(parts) === 2);
assert(parts[0] === "a");
assert(parts[1] === "b");
return 0;

//?
//? test: Deferred-produce-type-script-call-return
//? description: script call produce type for typed return (compile soft probes)
//? skip: true
//? skipReason: ...
FIXME: skipped because this body would pass at runtime and be a false
positive. The real check is compile-time produce-type on the call, not
eval of f().
//? expect: 0
//? source: ...

// Fixture only. Do not unskip until a compile/listing (or similar) probe
// can assert, without evaluating f(), that the call IR for f() produces
// string. Today get_data_type / inf->data_type on script-call is NULL
// (@fixme Get right data type). When that lands: type-check and soft
// iterator step type can use the call's produce type. See
// designs/compile-optimize-notes.md.
const f = function (): string {
    return "hi";
};
const x: string = f();
assert(x === "hi");
return 0;

//?
//? test: lower-case-changes-utf8-length
//? description: Lower case of a code point whose lower case has another UTF-8 length (U+0130 2->1 bytes, U+023A 2->3 bytes)
//? expect: 0
//? source: ...

const a = normalize_to_lower_case("İİ");
assert(a === "ii", "U+0130: " + stringify(a));
assert(length(a) === 2, "U+0130 length");
const b = normalize_to_lower_case("ȺB");
assert(b === "ⱥb", "U+023A: " + stringify(b));
return 0;

//?
//? test: iterable-as-array-walkers
//? description: functions whose parameter is marked iterableAsArray read a string as its code points (#583)
//? expect: 0
//? source: ...

assert(any_of(eq<string>, "b", ["a", "b"]), "a lone string next to an array is a value");
assert(all_of(function (c) { return c !== "z"; }, "abc"));
assert(join("abc", "-") === "a-b-c");
assert(join("x€y", "") === "x€y");
return 0;

//?
//? test: es-walkers-reject-string
//? description: map, filter, find, find_index, some, every and reduce don't walk a string letter by letter (#599)
//? expect: 0
//? source: ...

const c = function (v) { return true; };
for (const f of [
    function () { return map("ab", c); },
    function () { return filter("ab", c); },
    function () { return find("ab", c); },
    function () { return find_index("ab", c); },
    function () { return some("ab", c); },
    function () { return every("ab", c); },
    function () { return reduce("ab", function (a, v) { return a + v; }, ""); }
]) {
    let message = "";
    try { f(); } catch (e) { message = e.message; }
    assert(index_of(message, "write [...value]") >= 0, "got: " + message);
}
assert(find_index([..."abc"], function (v) { return v === "b"; }) === 1);
assert(reduce([..."abc"], function (a, v) { return v + a; }, "") === "cba");
return 0;

//?
//? test: iterable-as-array-ambiguous
//? description: with no array and two strings, a walker can't tell which to walk
//? expect: error:No value passed to any_of is an array and more than one could be read as one; write [...value] for the one to walk
//? source: ...

return any_of(eq<string>, "b", "abc");

//?
//? test: set-functions-reject-string
//? description: XACML-derived set functions don't read a string as an array (#583)
//? expect: 0
//? source: ...

for (const f of [
    function () { return is_in<string>("admin", "admin"); },
    function () { return at_least_one_member_of<string>("ab", ["a"]); },
    function () { return intersection<string>("ab", ["a"]); },
    function () { return union<string>("ab", ["c"]); },
    function () { return subset<string>("ab", ["a", "b"]); },
    function () { return set_equals<string>("ab", ["a", "b"]); },
    function () { return one_and_only<string>("a"); }
]) {
    let message = "";
    try { f(); } catch (e) { message = e.message; }
    assert(index_of(message, "write [...value]") >= 0, "got: " + message);
}
assert(is_in<string>("admin", ["admin"]));
assert(is_in<string>("b", [..."abc"]), "spread to mean code points");
return 0;

//?
//? test: array-only-functions-reject-string
//? description: reverse, sort, add_entries and push don't read a string as an array (#583)
//? expect: 0
//? source: ...

for (const f of [
    function () { return reverse("abc"); },
    function () { return sort("cab", function (a, b) { return a < b; }); },
    function () { return add_entries([], "ab"); },
    function () { return push("ab", "c"); }
]) {
    let threw = false;
    try { f(); } catch (e) { threw = true; }
    assert(threw);
}
assert(eq(reverse([..."abc"]), ["c", "b", "a"]));
assert(eq(sort([..."cab"], function (a, b) { return a < b; }), ["a", "b", "c"]));
return 0;

//?
//? test: iterable-as-array-type-check
//? description: compile-time type checking accepts a string exactly where iterableAsArray does, and a spread string for map and reduce
//? expect: 0
//? source: ...

#compile typeCheck;
assert(join("abc", "-") === "a-b-c");
assert(eq(map([..."ab"], function (c: string): string { return c; }), ["a", "b"]));
assert(reduce([..."ab"], function (a, c) { return a + c; }, "") === "ab");
return 0;

//?
//? test: iterable-as-array-type-check-rejects
//? description: compile-time type checking rejects a string for an unmarked array parameter, and accepts it for a marked one
//? expect: 0
//? source: ...

let message = "";
try {
    compile<script>(script("#compile typeCheck;\nreturn is_in<string>(\"a\", \"abc\");"));
} catch (e) {
    message = e.message;
}
assert(index_of(message, "Type error in parameter") >= 0, "got: " + message);
message = "";
try {
    compile<script>(script("#compile typeCheck;\nreturn map(\"abc\", function (c) { return c; });"));
} catch (e) {
    message = e.message;
}
assert(index_of(message, "Type error in parameter") >= 0, "map: got: " + message);
compile<script>(script("#compile typeCheck;\nreturn join(\"abc\", \"-\");"));
return 0;

//?
//? test: slice-string
//? description: slice of a string is a string of code points, as in ECMAScript (#583)
//? expect: 0
//? source: ...

assert(slice("abc", 1) === "bc");
assert(slice("abc", -2) === "bc");
assert(slice("abc", 0, -1) === "ab");
assert(slice("abc") === "abc");
assert(slice("abc", -5) === "abc");
assert(slice("abc", 2, 1) === "");
assert(slice("abc", 5) === "");
assert(slice("", 0) === "");
assert(slice("x€y😀z", 1, 4) === "€y😀");
assert(slice("ab"anyURI, 1) === "b");
assert(meta(slice("ab"anyURI, 1)).dataType === "string");
assert(eq(slice([..."abc"], 1), ["b", "c"]), "spread for the code points as an array");
assert(eq(slice([1, 2, 3], -2), [2, 3]), "arrays unchanged");
return 0;

//?
//? test: at-string
//? description: at of a string is the code point at an index, as in ECMAScript and s[i] (#583)
//? expect: 0
//? source: ...

const s = "x€y";
assert(at("abc", 1) === "b");
assert(at("abc", -1) === "c");
assert(at("abc", 3) === undefined);
assert(at("abc", -4) === undefined);
assert(at(s, 1) === "€");
assert(at(s, 1) === s[1], "same as s[i]");
assert(s->at(-1) === "y", "method form");
assert(at([1, 2], -1) === 2, "arrays unchanged");
assert(at([1, 2], 5) === undefined);
return 0;

//?
//? test: slice-at-reject-other
//? description: slice and at take an array or a string
//? expect: 0
//? source: ...

for (const f of [
    function () { return slice(5, 1); },
    function () { return at(true, 0); }
]) {
    let message = "";
    try { f(); } catch (e) { message = e.message; }
    assert(index_of(message, "must be an array or a string") >= 0, "got: " + message);
}
return 0;
