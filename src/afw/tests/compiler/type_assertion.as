#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: type_assertion.as
//? customPurpose: Type assertion
//? description: v as T and as_T check that v is T at run time and never convert; is<T> needs a data type (#582)
//? sourceType: script
//?
//? test: each-data-type
//? description: v as T and as_T(v) return v when v is T, and throw for a value of every other data type
//? expect: 0
//? source: ...

// One value of each data type that has as_T (unevaluated has no value).
const values = [
    ["anyURI", "https://example.com/"anyURI],
    ["array", [1, 2]],
    ["base64Binary", "AQID"base64Binary],
    ["boolean", true],
    ["date", "2026-10-11"date],
    ["dateTime", "2026-10-11T12:00:00Z"dateTime],
    ["dayTimeDuration", "P1DT2H"dayTimeDuration],
    ["dnsName", "example.com"dnsName],
    ["double", 1.5],
    ["function", function () { return 1; }],
    ["hexBinary", "0a1b"hexBinary],
    ["ia5String", "abc"ia5String],
    ["integer", 1],
    ["ipAddress", "10.0.0.1"ipAddress],
    ["null", null],
    ["object", {a: 1}],
    ["objectId", "abc"objectId],
    ["objectPath", "/afw/_AdaptiveFunction_/add"objectPath],
    ["password", "secret"password],
    ["rfc822Name", "a@example.com"rfc822Name],
    ["script", script("return 1;")],
    ["string", "text"],
    ["template", template("x")],
    ["time", "12:00:00"time],
    ["x500Name", "cn=a,o=b"x500Name],
    ["xpathExpression", "/a/b"xpathExpression],
    ["yearMonthDuration", "P1Y2M"yearMonthDuration]
];

for (const [t, v] of values) {
    const as_operator: function = evaluate(compile<script>(script(
        "return function (v) { return v as " + t + "; };")));
    const as_function: function = evaluate(compile<script>(script(
        "return function (v) { return as_" + t + "(v); };")));
    for (const [t2, v2] of values) {
        for (const assert_t of [as_operator, as_function]) {
            let result = "returned";
            try {
                assert(assert_t(v2) === v2, t + " " + t2 + " same value");
            }
            catch (e) {
                assert(e.id === "argument_error", t + " " + t2 + " " + e.id);
                assert(includes(e.message,
                    "Type assertion failed: expected " + t + " but got " + t2),
                    t + " " + t2 + " " + e.message);
                result = "threw";
            }
            assert(result === ((t === t2) ? "returned" : "threw"), t + " " + t2);
        }
    }
}
return 0;

//?
//? test: unevaluated
//? description: as_unevaluated exists like is_unevaluated, and an evaluated value is not unevaluated
//? expect: 0
//? source: ...

let message = "";
try {
    message = string(1 as unevaluated);
}
catch (e) {
    message = e.message;
}
assert(includes(message, "expected unevaluated but got integer"), message);
return 0;

//?
//? test: hints
//? description: the mismatch error says how to get a T instead (typed literal, to_T, string_to_boolean)
//? expect: 0
//? source: ...

function assertionError(f) {
    try {
        f();
    }
    catch (e) {
        assert(e.id === "argument_error", e.id);
        return e.message;
    }
    return "returned";
}

const s = "2026-10-11";
assert(assertionError(function () { return s as date; }) ===
    'Type assertion failed: expected date but got string; write "2026-10-11"date or to_date(v)');
assert(assertionError(function () { return "a\"b" as password; }) ===
    'Type assertion failed: expected password but got string; write "a\\"b"password or to_password(v)');
assert(assertionError(function () { return "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" as date; }) ===
    'Type assertion failed: expected date but got string; write "..."date or to_date(v)');
assert(assertionError(function () { return "1" as integer; }) ===
    "Type assertion failed: expected integer but got string; write to_integer(v)");
assert(assertionError(function () { return 5 as double; }) ===
    "Type assertion failed: expected double but got integer; write to_double(v)");
assert(assertionError(function () { return 5 as string; }) ===
    "Type assertion failed: expected string but got integer; write to_string(v)");
assert(assertionError(function () { return "false" as boolean; }) ===
    "Type assertion failed: expected boolean but got string; write string_to_boolean(v)");
assert(assertionError(function () { return [1] as object; }) ===
    "Type assertion failed: expected object but got array");
return 0;

//?
//? test: null-and-undefined
//? description: null is its own data type; undefined is not any data type
//? expect: 0
//? source: ...

function assertionError(f) {
    try {
        f();
    }
    catch (e) {
        assert(e.id === "argument_error", e.id);
        return e.message;
    }
    return "returned";
}

let n = null;
let u = undefined;
assert((n as null) === null);
assert(as_null(n) === null);
assert(assertionError(function () { return n as string; }) ===
    "Type assertion failed: expected string but got null");
assert(assertionError(function () { return n as object; }) ===
    "Type assertion failed: expected object but got null");
assert(assertionError(function () { return u as string; }) ===
    "Type assertion failed: expected string but got undefined");
assert(assertionError(function () { return u as null; }) ===
    "Type assertion failed: expected null but got undefined");
assert(assertionError(function () { return as_integer(u); }) ===
    "Type assertion failed: expected integer but got undefined");
return 0;

//?
//? test: precedence
//? description: as binds like a comparison, as in TypeScript; arithmetic after as needs parentheses
//? expect: 0
//? source: ...

function compileError(source) {
    try {
        compile<script>(script(source));
    }
    catch (e) {
        return e.message;
    }
    return "compiled";
}

const a = 1;
const b = 2;
assert(a + b as integer == 3, "asserts the sum");
assert(a as integer == 1, "(a as integer) == 1");
assert(a as integer < b, "(a as integer) < b");
assert(a < b as boolean, "(a < b) as boolean");
assert(a as integer as integer === 1, "left to right");
assert(true && a as integer == 1, "binds tighter than &&");
assert((a as integer) + 1 === 2, "parentheses");
assert(-a as integer === -1, "prefixed operand");
assert(((a > 0) ? a as integer : 0) === 1, "conditional");
assert(includes(compileError("return 1 as integer + 1;"),
    "Write (v as T) in parentheses"), "+");
assert(includes(compileError("return 1 as integer * 2;"),
    "Write (v as T) in parentheses"), "*");
assert(includes(compileError("return 1 as integer ** 2;"),
    "Write (v as T) in parentheses"), "**");
return 0;

//?
//? test: types-after-as
//? description: only a data type with as_T follows as for now; a type alias of one is fine
//? expect: 0
//? source: ...

function compileError(source) {
    try {
        compile<script>(script(source));
    }
    catch (e) {
        return e.message;
    }
    return "compiled";
}

assert(includes(compileError("return [1] as integer[];"),
    "'as' supports a data type only"), "array type");
assert(includes(compileError("return 1 as integer | null;"),
    "'as' supports a data type only"), "union");
assert(includes(compileError("return {a: 1} as {a: integer};"),
    "'as' supports a data type only"), "object type");
assert(includes(compileError("return 1 as undefined;"),
    "'as undefined' is not supported; test with v === undefined"), "undefined");
assert(includes(compileError("return 1 as any;"),
    "'as any' is not supported; there is no as_any"), "any");
assert(includes(compileError("return 1 as nope;"),
    "Unknown type 'nope'"), "unknown type");
assert(includes(compileError("return as<integer>(1);"),
    "Unknown built-in function 'as'"), "as<T> is not a spelling");
assert(compileError("return as_integer(1);") === "compiled", "as_T");

// A type alias of a data type.
type Day = date;
const d = "2026-10-11"date;
assert((d as Day) === d);
return 0;

//?
//? test: is-needs-data-type
//? description: is<T> and is_T work; is without a data type is a compile error, not false
//? expect: 0
//? source: ...

function compileError(source) {
    try {
        compile<script>(script(source));
    }
    catch (e) {
        return e.message;
    }
    return "compiled";
}

assert(is<integer>(1));
assert(!is<integer>("1"));
assert(is_integer(1));
assert(is<null>(null));
assert(fn::is<string>("a"));
assert(includes(compileError("return is(1);"),
    "Built-in function 'is' needs a data type, such as is<string> or is_string"));
assert(includes(compileError("return is<nope>(1);"),
    "Unknown built-in function 'is<nope>'"));
assert(includes(compileError("return nope<integer>(1);"),
    "Unknown built-in function 'nope'"));

// A variable named is is still a variable.
const is = function (x) { return x; };
assert(is(1) === 1);
assert(is_in<integer>(1, [1, 2]));
return 0;

//?
//? test: type-check
//? description: under typeCheck, a value whose type is known not to be T is a compile error with the same hint
//? expect: 0
//? source: ...

function compileError(source) {
    try {
        compile<script>(script(source));
    }
    catch (e) {
        assert(e.id === "syntax", e.id);
        return e.message;
    }
    return "compiled";
}

assert(includes(compileError(
    '#compile typeCheck;\nreturn "2026-10-11" as date;'),
    'Type error in type assertion: expected date but got string; write "2026-10-11"date or to_date(v)'),
    "literal");
assert(includes(compileError(
    '#compile typeCheck;\nreturn as_date("2026-10-11");'),
    'Type error in type assertion: expected date but got string; write "2026-10-11"date or to_date(v)'),
    "as_T");
assert(includes(compileError(
    '#compile typeCheck;\nconst s: string = "x";\nreturn s as date;'),
    'Type error in type assertion: expected date but got string; write "..."date or to_date(v)'),
    "typed variable");
assert(includes(compileError(
    "#compile typeCheck;\nreturn 1 + 2 as double;"),
    "Type error in type assertion: expected double but got integer; write to_double(v)"),
    "known result type");
assert(includes(compileError(
    "#compile typeCheck;\nreturn null as string;"),
    "Type error in type assertion: expected string but got null"),
    "null");
assert(includes(compileError(
    "#compile typeCheck;\nreturn undefined as string;"),
    "Type error in type assertion: expected string but got undefined"),
    "undefined");

// Known to be T, or not known: compiles.
assert(compileError('#compile typeCheck;\nreturn "x" as string;') === "compiled");
assert(compileError('#compile typeCheck;\nconst v: any = 1;\nreturn v as string;') === "compiled");
assert(compileError("#compile typeCheck;\nlet v = 1;\nreturn v as string;") === "compiled");

// Without typeCheck, a mismatch is a run time error.
assert(compileError('return "2026-10-11" as date;') === "compiled");

// The result type is T.
assert(includes(compileError(
    "#compile typeCheck;\nconst v: any = 1;\nconst s: string = v as integer;\nreturn s;"),
    "Type error"), "result is T");
assert(compileError(
    "#compile typeCheck;\nconst v: any = 1;\nconst i: integer = v as integer;\nreturn i;") === "compiled");
return 0;

//?
//? test: decompile
//? description: as_T(v) decompiles to (v as T), which compiles again to the same thing, also as a chain base
//? expect: 0
//? source: ...

const text = decompile(compile<script>(script(
    "return function (x) { return [x as integer, as_integer(x), is<integer>(x), as_object({a: x})?.a, (({a: x}) as object).a]; };")));
assert(includes(text, "(x as integer),(x as integer)"), text);
assert(includes(text, "is_integer(x)"), text);
assert(includes(text, "({\"a\":x} as object)?.[\"a\"]"), text);
const g: function = evaluate(compile<script>(script(text)));
assert(stringify(g(1)) === "[1,1,true,1,1]");
assert(decompile(compile<script>(script(text))) === text, "same text again");
return 0;

//?
//? test: is-and-as-agree
//? description: is<T>(v) is true exactly when v as T does not throw, including undefined
//? expect: 0
//? source: ...

const types = ["anyURI", "array", "boolean", "date", "double", "function",
    "integer", "null", "object", "string"];
const values = ["https://example.com/"anyURI, [1], true, "2026-10-11"date,
    1.5, add, 1, null, {a: 1}, "text", undefined];
for (const t of types) {
    const is_t: function = evaluate(compile<script>(script(
        "return function (v) { return is<" + t + ">(v); };")));
    const as_t: function = evaluate(compile<script>(script(
        "return function (v) { return v as " + t + "; };")));
    for (const v of values) {
        let passed = true;
        try {
            as_t(v);
        }
        catch (e) {
            passed = false;
        }
        assert(is_t(v) === passed, t + " " + stringify(v ?? "undefined"));
    }
}
assert(is_string(undefined) === false);
assert(is_null(undefined) === false);
assert(is<integer>(undefined) === false);
return 0;

//?
//? test: built-in-functions-are-functions
//? description: a built-in function value is a function, like a script function
//? expect: 0
//? source: ...

const f = function (a) { return a; };
assert(is<function>(add));
assert(!is<object>(add));
assert((add as function) == add);
assert(is<function>(abs<integer>));
assert(add == add);
assert(add !== subtract);
assert(string(add) === "add");
assert(is<function>(f) && f == f);

// keys and to_string take an object and a convertible value, not a function.
let message = "";
try {
    message = stringify(keys(add));
}
catch (e) {
    message = e.message;
}
assert(includes(message, "must evaluate to data type 'object' but evaluated to be 'function'"), message);
try {
    message = to_string(add);
}
catch (e) {
    message = e.message;
}
assert(includes(message, "'to_string' is not a method of data type 'function'"), message);

// The function's metadata object.
assert(get_object("afw", "_AdaptiveFunction_", "add").functionId === "add");
return 0;

//?
//? test: null-and-undefined-types
//? description: null and undefined are data types in a Type annotation
//? expect: 0
//? source: ...

const x: integer | null = null;
const y: undefined = undefined;
let z: null = null;
assert(x === null);
assert(y === undefined);
assert(z === null);
return 0;
