#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: type_check.as
//? customPurpose: Part of compiler category tests
//? description: ...
Type *rules* under #compile typeCheck in the compile unit under test (issue #28).
Covers leaves, unions/intersections, object/interface shapes (required,
optional, extends), array/tuple elements, function types and returns,
Pattern bindings, object-literal excess (compile), call sites for known
named script functions, and Adaptive formal/return checks (length, add,
arity). Flag/pragma plumbing is type_check_flags.as; parse-only is
type_syntax.as.
Nuances: prefer #compile typeCheck / #compile typeCheckCompileOnly inside script(...) so
cases do not fight over process flags. Excess applies to object *literals*
(and call-site literals); variables/spreads/computed keys skip excess;
runtime stays open (check-excess-not-at-runtime uses flag_set for that
split). let reassignment is checked at evaluate. Typed symbol RHS uses
type-to-type (no excess). Adaptive formals only when the call is built
at compile (allow_optimize path); polymorphic hubs need known param1 type.
//? sourceType: script
//?
//? test: check-ok-assign
//? description: matching types succeed with #compile typeCheck
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const x: integer = 1;\n" +
    "const f = function (a: integer): integer { return a + 1; };\n" +
    "return f(x);"
)));
assert(r === 2);
return 0;

//?
//? test: check-leaf-mismatch
//? description: integer vs string literal fails
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const x: integer = \"hello\";\n" +
    "return 0;"
));
return 0;

//?
//? test: check-any-accepts
//? description: any accepts values of other data types
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const a: any = 1;\n" +
    "const b: any = \"x\";\n" +
    "const c: any = true;\n" +
    "return a;"
)));
assert(r === 1);
return 0;

//?
//? test: check-boolean-not-integer
//? description: boolean is not assignable to integer
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const x: integer = true;\n" +
    "return 0;"
));
return 0;

//?
//? test: check-union-ok
//? description: integer|string accepts both arms
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const a: integer|string = 1;\n" +
    "const b: integer|string = \"x\";\n" +
    "return a;"
)));
assert(r === 1);
return 0;

//?
//? test: check-union-rejects-third
//? description: integer|string rejects boolean
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const u: integer|string = true;\n" +
    "return 0;"
));
return 0;

//?
//? test: check-object-shape-ok
//? description: structural object/interface assign succeeds
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "interface Base { id: integer };\n" +
    "interface Person extends Base { name: string, age?: integer };\n" +
    "const p: Person = { id: 1, name: \"a\" };\n" +
    "const o: { host: string, port?: integer } = { host: \"h\" };\n" +
    "return p.id;"
)));
assert(r === 1);
return 0;

//?
//? test: check-object-missing-required
//? description: missing required property fails
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const o: { host: string, port: integer } = { host: \"h\" };\n" +
    "return 0;"
));
return 0;

//?
//? test: check-object-wrong-prop-type
//? description: wrong property type fails
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const o: { host: string } = { host: 1 };\n" +
    "return 0;"
));
return 0;

//?
//? test: check-optional-prop-omitted-ok
//? description: optional property may be omitted
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const o: { host: string, port?: integer } = { host: \"h\" };\n" +
    "return o.host;"
)));
assert(r === "h");
return 0;

//?
//? test: check-optional-prop-wrong-type
//? description: optional property present with wrong type fails
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const o: { host: string, port?: integer } = { host: \"h\", port: \"x\" };\n" +
    "return 0;"
));
return 0;

//?
//? test: check-interface-extends-missing-base
//? description: missing property required by extends base fails
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "interface Base { id: integer };\n" +
    "interface Person extends Base { name: string };\n" +
    "const p: Person = { name: \"a\" };\n" +
    "return 0;"
));
return 0;

//?
//? test: check-array-element-bad
//? description: integer[] rejects string element
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const a: integer[] = [1, \"x\"];\n" +
    "return 0;"
));
return 0;

//?
//? test: check-array-empty-ok
//? description: empty array is fine for integer[]
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const a: integer[] = [];\n" +
    "return length(a);"
)));
assert(r === 0);
return 0;

//?
//? test: check-array-ok
//? description: matching integer[] assigns
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const a: integer[] = [1, 2, 3];\n" +
    "return a[1];"
)));
assert(r === 2);
return 0;

//?
//? test: check-Array-generic-element-bad
//? description: Array<string> rejects integer element
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const a: Array<string> = [\"a\", 1];\n" +
    "return 0;"
));
return 0;

//?
//? test: check-tuple-length
//? description: tuple length mismatch fails
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const t: [integer, string] = [1];\n" +
    "return 0;"
));
return 0;

//?
//? test: check-tuple-position-type
//? description: tuple position type mismatch fails
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const t: [integer, string] = [1, 2];\n" +
    "return 0;"
));
return 0;

//?
//? test: check-tuple-ok
//? description: matching tuple assigns
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const t: [integer, string] = [1, \"a\"];\n" +
    "return t[0];"
)));
assert(r === 1);
return 0;

//?
//? test: check-intersection-missing
//? description: intersection requires props from both sides
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const o: { a: integer } & { b: string } = { a: 1 };\n" +
    "return 0;"
));
return 0;

//?
//? test: check-intersection-ok
//? description: value matching both sides of object intersection
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const o: { a: integer } & { b: string } = { a: 1, b: \"x\" };\n" +
    "return o.a;"
)));
assert(r === 1);
return 0;

//?
//? test: check-function-type-ok
//? description: matching function type annotation assigns
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const f: (a: integer) => integer = function (a: integer): integer {\n" +
    "    return a + 1;\n" +
    "};\n" +
    "return f(2);"
)));
assert(r === 3);
return 0;

//?
//? test: check-function-type-param-mismatch
//? description: function value param type does not match annotation
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const f: (a: integer) => integer = function (a: string): integer {\n" +
    "    return 1;\n" +
    "};\n" +
    "return 0;"
));
return 0;

//?
//? test: check-function-type-return-mismatch
//? description: function value return type does not match annotation
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const f: (a: integer) => string = function (a: integer): integer {\n" +
    "    return a;\n" +
    "};\n" +
    "return 0;"
));
return 0;

//?
//? test: check-return-type-mismatch
//? description: return value must match function return type
//? expect: error
//? source: ...

evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const f = function (): integer { return \"no\"; };\n" +
    "return f();"
)));
return 0;

//?
//? test: check-return-type-ok
//? description: matching return type
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const f = function (): integer { return 7; };\n" +
    "return f();"
)));
assert(r === 7);
return 0;

//?
//? test: check-expression-body-return
//? description: expression-body function return type mismatch
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const f = function (a: integer): string => a;\n" +
    "return 0;"
));
return 0;

//?
//? test: check-array-pattern-element
//? description: array Pattern element annotation enforced
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const [a: integer, b: string] = [1, 2];\n" +
    "return 0;"
));
return 0;

//?
//? test: check-array-pattern-ok
//? description: matching array Pattern element types
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const [a: integer, b: string] = [1, \"x\"];\n" +
    "return a;"
)));
assert(r === 1);
return 0;

//?
//? test: check-object-pattern-rename-type
//? description: object Pattern rename with typed binding
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const { host: h: string } = { host: 1 };\n" +
    "return 0;"
));
return 0;

//?
//? test: check-object-pattern-ok
//? description: matching object Pattern typed binding
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const { host: h: string } = { host: \"h\" };\n" +
    "return h;"
)));
assert(r === "h");
return 0;

//?
//? test: check-error-mentions-missing-property
//? description: object shape error mentions missing property name
//? expect: 0
//? source: ...

try {
    compile<script>(script(
        "#compile typeCheck;\n" +
        "const o: { host: string, port: integer } = { host: \"h\" };\n" +
        "return 0;"
    ));
    assert(false);
} catch (e) {
    const msg = string(e);
    assert(includes(msg, "port") || includes(msg, "missing"));
}
return 0;

//?
//? test: check-excess-property-object-literal
//? description: compile rejects excess key on object literal
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const o: { host: string } = { host: \"h\", port: 1 };\n" +
    "return 0;"
));
return 0;

//?
//? test: check-excess-property-via-variable-ok
//? description: extras OK when assigned from a variable (not a literal)
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const wider = { host: \"h\", port: 1 };\n" +
    "const o: { host: string } = wider;\n" +
    "return o.host;"
)));
assert(r === "h");
return 0;

//?
//? test: check-excess-nested-literal
//? description: excess property on nested object literal
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const o: { a: { x: integer } } = { a: { x: 1, y: 2 } };\n" +
    "return 0;"
));
return 0;

//?
//? test: check-excess-spread-skips
//? description: object literal with spread skips excess-key check
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const extra = { port: 1 };\n" +
    "const o: { host: string } = { ...extra, host: \"h\" };\n" +
    "return o.host;"
)));
assert(r === "h");
return 0;

//?
//? test: check-typed-symbol-rhs
//? description: typed variable assignable by type graph to narrower shape
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const wider: { host: string, port: integer } = { host: \"h\", port: 1 };\n" +
    "const o: { host: string } = wider;\n" +
    "return o.host;"
)));
assert(r === "h");
return 0;

//?
//? test: check-typed-symbol-rhs-mismatch
//? description: typed variable not assignable when types conflict
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const s: string = \"x\";\n" +
    "const n: integer = s;\n" +
    "return 0;"
));
return 0;

//?
//? test: check-let-reassign-mismatch
//? description: let reassignment checked against declared type (at evaluate)
//? expect: error
//? source: ...

evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "let n: integer = 1;\n" +
    "n = \"x\";\n" +
    "return 0;"
)));
return 0;

//?
//? test: check-union-excess
//? description: excess key not in either union object member
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const o: { a: integer } | { b: string } = { a: 1, c: true };\n" +
    "return 0;"
));
return 0;

//?
//? test: check-union-object-literal-ok
//? description: object literal matching one union arm
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const o: { a: integer } | { b: string } = { a: 1 };\n" +
    "return o.a;"
)));
assert(r === 1);
return 0;

//?
//? test: check-call-site-excess
//? description: object literal arg excess on known named function call
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "function f(o: { host: string }) { return o.host; };\n" +
    "return f({ host: \"h\", port: 1 });"
));
return 0;

//?
//? test: check-call-site-ok
//? description: matching object literal arg on named function
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "function f(o: { host: string }) { return o.host; };\n" +
    "return f({ host: \"h\" });"
)));
assert(r === "h");
return 0;

//?
//? test: check-call-site-leaf-arg
//? description: named function leaf formal rejects wrong arg
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "function f(a: integer) { return a; };\n" +
    "return f(\"x\");"
));
return 0;

//?
//? test: check-type-alias-used
//? description: type alias is enforced like the underlying type
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "type Id = integer;\n" +
    "const x: Id = \"no\";\n" +
    "return 0;"
));
return 0;

//?
//? test: check-adaptive-length-ok
//? description: built-in length accepts string
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "return length(\"hi\");"
)));
assert(r === 2);
return 0;

//?
//? test: check-adaptive-length-bad
//? description: built-in length rejects integer at compile
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "return length(1);"
));
return 0;

//?
//? test: check-adaptive-add-poly-ok
//? description: polymorphic add with known integer args
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "return add(1, 2);"
)));
assert(r === 3);
return 0;

//?
//? test: check-adaptive-add-poly-bad
//? description: integer add rejects string second arg when specialized
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "return add(1, \"x\");"
));
return 0;

//?
//? test: check-adaptive-return-assign
//? description: length result is integer for typed binding
//? expect: 0
//? source: ...

const r: any = evaluate(compile<script>(script(
    "#compile typeCheck;\n" +
    "const n: integer = length(\"abc\");\n" +
    "return n;"
)));
assert(r === 3);
return 0;

//?
//? test: check-adaptive-return-assign-bad
//? description: length result not assignable to string
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "const s: string = length(\"abc\");\n" +
    "return s;"
));
return 0;

//?
//? test: check-adaptive-arity-too-few
//? description: built-in with too few arguments fails compile check
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheck;\n" +
    "return add(1);"
));
return 0;

//?
//? test: check-compileOnly-pragma-literal
//? description: #compile typeCheckCompileOnly catches bad literal
//? expect: error
//? source: ...

compile<script>(script(
    "#compile typeCheckCompileOnly;\n" +
    "const x: integer = \"hello\";\n" +
    "return 0;"
));
return 0;

//?
//? test: check-excess-not-at-runtime
//? description: runtime typeCheck allows extra props when unit compiled without check
//? expect: 0
//? source: ...

/*
 * Process flags only for this split: compile wide literal with checking
 * off, then evaluate with typeCheck on (runtime excess stays open).
 */
flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
const compiled: any = compile<script>(script(
    "const o: { host: string } = { host: \"h\", port: 99 };\n" +
    "return o.host;"
));
flag_set(["compile:typeCheck"], true);
const r: any = evaluate(compiled);
assert(r === "h");
flag_set([
    "compile:typeCheck",
    "compile:typeCheckCompileOnly",
    "compile:noImplicitAny",
    "compile:strictNullChecks",
    "compile:strict"
], false);
return 0;
