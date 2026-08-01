#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: decompile.as
//? customPurpose: Part of compiler category tests
//? description: ...
// Coverage of decompile() for data types and afw_value* compiled kinds.
// Synthetic forms use #implementation_id(...) (pragma_identifier style).
// Value methods exercised (by kind):
//   data-type bindings (write_as_expression), block, assignment_target,
//   call_built_in_function, call, call_script_function, symbol_reference,
//   qualified_variable_reference, reference_by_key, script_function,
//   function_definition, compiled_value, list_expression, object_expression,
//   template_definition, closure_binding.
// Not covered from Adaptive Script (C-only / awkward to construct):
//   function_thunk (model adapter only), call_test_script (test_script root).
//?
//? sourceType: script
//?
//? test: decompile-no-params
//? description: Test decompile without any parameters
//? expect: error
//? source: ...

decompile();

//? test: decompile-data-primitives
//? description: Data-type values decompile to Adaptive expression form
//? skip: false
//? expect: 0
//? source: ...

assert(decompile(true) == "true");
assert(decompile(false) == "false");
assert(decompile(0) == "0");
assert(decompile(-1) == "-1");
assert(decompile(42) == "42");
assert(decompile("abc") == "\"abc\"");
assert(decompile("") == "\"\"");
assert(decompile(null) == "null");

return 0;

//? test: decompile-data-containers
//? description: Object and array decompile like JSON for JSON-shaped data
//? skip: false
//? expect: 0
//? source: ...

assert(decompile([]) == "[]");
assert(decompile(["abc", "def"]) == "[\"abc\",\"def\"]");
assert(decompile({ a: "b", c: 1, d: true }) == "{\"a\":\"b\",\"c\":1,\"d\":true}");

return 0;

//? test: decompile-data-typed
//? description: Non-JSON data types use cast-function form
//? skip: false
//? expect: 0
//? source: ...

const dt = dateTime("2020-01-02T03:04:05Z");
assert(decompile(dt) == "dateTime(\"2020-01-02T03:04:05Z\")");

const b64 = base64Binary("YQ==");
assert(decompile(b64) == "base64Binary(\"YQ==\")");

return 0;

//? test: decompile-array-all-data-types
//? description: ...
// Array with one element per constructible data type (plus function and
// undefined). Skips abstract/internal kinds: any, unevaluated, unknown, void.
//?
//? skip: false
//? expect: 0
//? source: ...

const all = [
    true,
    1,
    1.5,
    "s",
    null,
    undefined,
    [],
    {},
    dateTime("2020-01-02T03:04:05Z"),
    date("2020-01-02"),
    time("03:04:05"),
    dayTimeDuration("PT1H"),
    yearMonthDuration("P1Y"),
    base64Binary("YQ=="),
    hexBinary("61"),
    anyURI("https://example.com/"),
    dnsName("example.com"),
    ipAddress("127.0.0.1"),
    rfc822Name("a@b.com"),
    x500Name("CN=a"),
    ia5String("abc"),
    password("secret"),
    objectId("oid1"),
    objectPath("/a/b/c"),
    regexp("^a$"),
    script("return 1;"),
    template("x${1}"),
    xpathExpression("/a"),
    json("{\"a\":1}"),
    relaxed_json("{a:1}"),
    add
];

const s = decompile(all);
assert(s ==
    "[true,1,1.5E0,\"s\",null,undefined,[],{}," +
    "dateTime(\"2020-01-02T03:04:05Z\")," +
    "date(\"2020-01-02\")," +
    "time(\"03:04:05\")," +
    "dayTimeDuration(\"PT1H\")," +
    "yearMonthDuration(\"P1Y\")," +
    "base64Binary(\"YQ==\")," +
    "hexBinary(\"61\")," +
    "anyURI(\"https://example.com/\")," +
    "dnsName(\"example.com\")," +
    "ipAddress(\"127.0.0.1\")," +
    "rfc822Name(\"a@b.com\")," +
    "x500Name(\"CN=a\")," +
    "ia5String(\"abc\")," +
    "password(\"secret\")," +
    "objectId(\"oid1\")," +
    "objectPath(\"/a/b/c\")," +
    "regexp(\"^a$\")," +
    "script(\"return 1;\")," +
    "template(\"x${1}\")," +
    "xpathExpression(\"/a\")," +
    "json(\"{\\\"a\\\":1}\")," +
    "relaxed_json(\"{a:1}\")," +
    "add]");

return 0;

//? test: decompile-function-definition
//? description: Built-in function value decompiles to functionId
//? skip: false
//? expect: 0
//? source: ...

// function_definition
assert(decompile(add) == "add");
assert(decompile(return) == "return");

return 0;

//? test: decompile-script-block-const-return
//? description: ...
// block, assignment_target (symbol), call_built_in_function, symbol_reference,
// compiled_value (via compile<script> root)
//?
//? skip: false
//? expect: 0
//? source: ...

const s = decompile(compile<script>(script(
    "const x = 1 + 2;\nreturn x;"
)));

assert(s ==
    "#block(const(#assignment_target(\"const\",x),add(1,2),undefined),return(x))");

return 0;

//? test: decompile-script-arithmetic
//? description: Operators lower to built-in calls in decompile
//? skip: false
//? expect: 0
//? source: ...

const s = decompile(compile<script>(script("return 1 + 2 * 3;")));
assert(s == "#block(return(add(1,multiply(2,3))))");

return 0;

//? test: decompile-script-if
//? description: if statement decompiles as if(...) built-in call
//? skip: false
//? expect: 0
//? source: ...

const s = decompile(compile<script>(script(
    "if (true) return 1; else return 2;"
)));
assert(s == "#block(if(true,return(1),return(2)))");

return 0;

//? test: decompile-call-general
//? description: Dynamic call through variable uses call value kind (f(1,2))
//? skip: false
//? expect: 0
//? source: ...

// call (general) — callee is symbol_reference, not a fixed built-in
const s = decompile(compile<script>(script(
    "let f = add;\nreturn f(1, 2);"
)));
assert(s ==
    "#block(let(#assignment_target(\"let\",f),add,undefined),return(f(1,2)))");

return 0;

//? test: decompile-call-script-function
//? description: Calling a script function decompiles as name(args)
//? skip: false
//? expect: 0
//? source: ...

// call_script_function + script_function
const src = script(
    'const f = function (a) { return a + 1; };\nreturn f(10);'
);
const s = decompile(compile<script>(src));
assert(s ==
    "#block(const(#assignment_target(\"const\",f),#script_function(a,#block(return(add(a,1)))),undefined),return(f(10)))");

return 0;

//? test: decompile-script-function-definition
//? description: Script function definition decompiles as #script_function(...)
//? skip: false
//? expect: 0
//? source: ...

const src = script(
    'const f = function (a) { return a; };\nreturn f;'
);
const s = decompile(compile<script>(src));
assert(s ==
    "#block(const(#assignment_target(\"const\",f),#script_function(a,#block(return(a))),undefined),return(f))");

return 0;

//? test: decompile-closure-binding
//? description: Runtime closure decompiles as #closure_binding(#script_function(...))
//? skip: false
//? expect: 0
//? source: ...

// closure_binding — only present after evaluate produces a closed-over function
const make = function (x) {
    return function (y) {
        return x + y;
    };
};
const c = make(1);
const s = decompile(c);
assert(s ==
    "#closure_binding(#script_function(y,#block(return(add(x,y)))))");

return 0;

//? test: decompile-reference-by-key
//? description: Property and index access decompile as agg[key]
//? skip: false
//? expect: 0
//? source: ...

// reference_by_key
let s = decompile(compile<script>(script(
    'const o = {a: 1};\nreturn o["a"];'
)));
assert(s ==
    "#block(const(#assignment_target(\"const\",o),{\"a\":1},undefined),return(o[\"a\"]))");

s = decompile(compile<script>(script(
    "const a = [10, 20];\nreturn a[1];"
)));
assert(s ==
    "#block(const(#assignment_target(\"const\",a),[10,20],undefined),return(a[1]))");

return 0;

//? test: decompile-qualified-variable-reference
//? description: Qualifier::name decompiles as qualified reference
//? skip: false
//? expect: 0
//? source: ...

// qualified_variable_reference
const s = decompile(compile<script>(script(
    "return process::osType;"
)));
assert(s == "#block(return(process::osType))");

return 0;

//? test: decompile-list-expression-spread
//? description: Array spread decompiles as call-site ... (list_expression marker)
//? skip: false
//? expect: 0
//? source: ...

// list_expression (spread) → surface ... in call args (issue #140)
const s = decompile(compile<script>(script(
    "const a = [1, 2];\nreturn [...a, 3];"
)));
assert(s ==
    "#block(const(#assignment_target(\"const\",a),[1,2],undefined),return(array(...a,3)))");

return 0;

//? test: decompile-object-expression
//? description: Object with expression property values decompiles with nested calls
//? skip: false
//? expect: 0
//? source: ...

// object_expression (object write_as_expression with unevaluated prop values)
const s = decompile(compile<script>(script(
    "return {a: 1 + 2};"
)));
assert(s == "#block(return({\"a\":add(1,2)}))");

return 0;

//? test: decompile-template-definition
//? description: Compiled template decompiles as #template_definition(parts...)
//? skip: false
//? expect: 0
//? source: ...

// template_definition — parts match listing (string segment + expression block)
const s = decompile(compile<template>(template("hello ${1+2}")));
assert(s ==
    "#block(#template_definition(\"hello \",#block(add(1,2))))");

return 0;

//? test: decompile-assignment-target-destructure
//? description: Destructure assignment targets decompile with synthetic tags
//? skip: false
//? expect: 0
//? source: ...

// assignment_target list_destructure / object_destructure as Patterns
let s = decompile(compile<script>(script(
    "const [a, b] = [1, 2];\nreturn a;"
)));
assert(s ==
    "#block(const(#assignment_target(\"const\",[a,b]),[1,2],undefined),return(a))");

s = decompile(compile<script>(script(
    "const {a, b} = {a: 1, b: 2};\nreturn a;"
)));
assert(s ==
    "#block(const(#assignment_target(\"const\",{a,b}),{\"a\":1,\"b\":2},undefined),return(a))");

return 0;

//? test: decompile-whitespace-tab
//? description: Whitespace string "\\t" indents decompile with tabs
//? skip: false
//? expect: 0
//? source: ...

const s = decompile(compile<script>(script("return 1+2;")), "\t");
const expected =
    "#block(\n" +
    "\treturn(\n" +
    "\t\tadd(\n" +
    "\t\t\t1,\n" +
    "\t\t\t2\n" +
    "\t\t)\n" +
    "\t)\n" +
    ")";
assert(s == expected);

return 0;

//? test: decompile-whitespace-list-pattern
//? description: List Pattern pretty-prints elements when tab is set
//? skip: false
//? expect: 0
//? source: ...

const s = decompile(compile<script>(script(
    "const [a, b] = [1, 2];\nreturn a;"
)), 2);
assert(s ==
    "#block(\n" +
    "  const(\n" +
    "    #assignment_target(\n" +
    "      \"const\",\n" +
    "      [\n" +
    "        a,\n" +
    "        b\n" +
    "      ]\n" +
    "    ),\n" +
    "    [\n" +
    "      1,\n" +
    "      2\n" +
    "    ],\n" +
    "    undefined\n" +
    "  ),\n" +
    "  return(\n" +
    "    a\n" +
    "  )\n" +
    ")");
return 0;

//? test: decompile-typed-const
//? description: Typed const decompiles with name:Type on assignment_target
//? skip: false
//? expect: 0
//? source: ...

const s = decompile(compile<script>(script(
    "const x: integer = 1;\nreturn x;"
)));
assert(s ==
    "#block(const(#assignment_target(\"const\",x:integer),1,undefined),return(x))");
return 0;

//? test: decompile-whitespace-spaces
//? description: Integer whitespace still pretty-prints with spaces
//? skip: false
//? expect: 0
//? source: ...

const s = decompile(compile<script>(script(
    "const x = 1 + 2;\nreturn x;"
)), 4);

const expected =
    "#block(\n" +
    "    const(\n" +
    "        #assignment_target(\n" +
    "            \"const\",\n" +
    "            x\n" +
    "        ),\n" +
    "        add(\n" +
    "            1,\n" +
    "            2\n" +
    "        ),\n" +
    "        undefined\n" +
    "    ),\n" +
    "    return(\n" +
    "        x\n" +
    "    )\n" +
    ")";

assert(s == expected);

return 0;
