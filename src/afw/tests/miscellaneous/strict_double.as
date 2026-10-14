#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: strict_double.as
//? customPurpose: Part of core function tests
//? description: ...
#compile strictDouble (#628): IEEE 754's invalid operation and division
by zero exceptions are errors, and <, <=, >, >= are IEEE 754's signaling
comparisons (an error for a NaN). Without it, doubles follow IEEE 754's
default handling, as ECMAScript does (the test262 tests). Under it,
everything IEEE 754 does not call one of those exceptions is unchanged:
overflow gives an infinity, an infinity over a zero is an infinity, a
NaN operand passes through arithmetic quietly, == is the quiet compare.
The flag compile:strictDouble is the process default; #compile changes
one compile unit only.
//? sourceType: script
//?
//? test: default-is-ieee-default
//? description: without strictDouble, NaN and the infinities come back, as in ECMAScript
//? expect: 0
//? source: ...

flag_set(["compile:strictDouble"], false);
assert(is_NaN(evaluate(compile<script>(script("return 0.0 / 0.0;")))));
assert(evaluate(compile<script>(script("return 1.0 / 0.0;"))) === Infinity);
assert(evaluate(compile<script>(script("return [sqrt(-1.0), NaN < 1.0];")))[1] === false);
assert(evaluate(compile<script>(script("return log(0.0);"))) === -Infinity);
return 0;

//?
//? test: invalid-operation
//? description: a result that would be NaN from operands that are not NaN is an error
//? expect: 0
//? source: ...

#compile strictDouble;
const throws = function(f) {
    try { f(); } catch (e) { return e.id === "argument_error"; }
    return false;
};
assert(throws(function() { return 0.0 / 0.0; }), "0 / 0");
assert(throws(function() { return Infinity - Infinity; }), "inf - inf");
assert(throws(function() { return Infinity + -Infinity; }), "inf + -inf");
assert(throws(function() { return Infinity * 0.0; }), "inf * 0");
assert(throws(function() { return Infinity / Infinity; }), "inf / inf");
assert(throws(function() { return Infinity % 1.0; }), "inf % 1");
assert(throws(function() { return 1.0 % 0.0; }), "1 % 0");
assert(throws(function() { return pow(-1.0, 0.5); }), "pow(-1, 0.5)");
assert(throws(function() { return sqrt(-1.0); }), "sqrt(-1)");
assert(throws(function() { return log(-1.0); }), "log(-1)");
assert(throws(function() { return asin(2.0); }), "asin(2)");
assert(throws(function() { return sin(Infinity); }), "sin(inf)");
return 0;

//?
//? test: invalid-operation-message
//? description: the message names the function and the exception
//? expect: error:Function 'subtract<double>' is an IEEE 754 invalid operation for these arguments: the result would be NaN (#compile strictDouble)
//? source: ...

#compile strictDouble;
return Infinity - Infinity;

//?
//? test: division-by-zero
//? description: an exact infinity from finite operands is an error
//? expect: 0
//? source: ...

#compile strictDouble;
const throws = function(f) {
    try { f(); } catch (e) { return e.id === "argument_error"; }
    return false;
};
assert(throws(function() { return 1.0 / 0.0; }), "1 / 0");
assert(throws(function() { return -1.0 / -0.0; }), "-1 / -0");
assert(throws(function() { return log(0.0); }), "log(0)");
assert(throws(function() { return log10(-0.0); }), "log10(-0)");
assert(throws(function() { return log2(0.0); }), "log2(0)");
assert(throws(function() { return pow(0.0, -1.0); }), "pow(0, -1)");
return 0;

//?
//? test: division-by-zero-message
//? description: the message names the function and the exception
//? expect: error:Function 'divide<double>' is an IEEE 754 division by zero for these arguments (#compile strictDouble)
//? source: ...

#compile strictDouble;
return 1.0 / 0.0;

//?
//? test: not-exceptions
//? description: what IEEE 754 does not call invalid operation or division by zero is unchanged
//? expect: 0
//? source: ...

#compile strictDouble;
assert(Infinity / 0.0 === Infinity, "an infinity over a zero is exact");
assert(1.0 / Infinity === 0.0);
assert(#doubleMax * 2.0 === Infinity, "overflow");
assert(exp(1000.0) === Infinity, "overflow");
assert(pow(0.0, -Infinity) === Infinity);
assert(abs(-Infinity) === Infinity);
assert(is_NaN(NaN + 1.0) && is_NaN(sqrt(NaN)) && is_NaN(-NaN), "a NaN operand is quiet");
assert(is_NaN(pow(1.0, Infinity)), "ECMAScript's 1 ** Infinity is NaN, not an IEEE 754 exception");
assert(is_NaN(max<double>(1.0, NaN)) && is_NaN(min<double>(NaN, 1.0)), "max and min pass NaN on");
assert(!(NaN == NaN) && NaN != NaN && !(NaN === 1.0), "== is the quiet compare");
assert(includes([NaN], NaN));
const s = sort([NaN, 1.0, -1.0]);
assert(s[0] === -1.0 && s[1] === 1.0 && is_NaN(s[2]), "sort orders as IEEE 754 totalOrder, NaN last");
return 0;

//?
//? test: signaling-comparisons
//? description: <, <=, >, >= with a NaN are an error
//? expect: 0
//? source: ...

#compile strictDouble;
for (const v of [NaN, 1.0, Infinity]) {
    assert(safe_evaluate(NaN < v, "error") === "error" && safe_evaluate(v < NaN, "error") === "error", "<");
    assert(safe_evaluate(NaN <= v, "error") === "error" && safe_evaluate(v <= NaN, "error") === "error", "<=");
    assert(safe_evaluate(NaN > v, "error") === "error" && safe_evaluate(v > NaN, "error") === "error", ">");
    assert(safe_evaluate(NaN >= v, "error") === "error" && safe_evaluate(v >= NaN, "error") === "error", ">=");
}
assert(1.0 < 2.0 && Infinity > 1.0);
return 0;

//?
//? test: signaling-comparison-message
//? description: the message says why
//? expect: error:lt<double> of NaN is an IEEE 754 invalid operation: <, <=, > and >= are signaling comparisons (#compile strictDouble; is_NaN(v) tests for NaN)
//? source: ...

#compile strictDouble;
return NaN < 1.0;

//?
//? test: per-unit
//? description: #compile strictDouble applies to the unit it is in, wherever its code is called from
//? expect: 0
//? source: ...

flag_set(["compile:strictDouble"], false);
const strict_divide = evaluate(compile<script>(script(
    "#compile strictDouble;\nreturn function(a, b) { return a / b; };")));
const plain_divide = evaluate(compile<script>(script(
    "return function(a, b) { return a / b; };")));
assert(safe_evaluate(strict_divide(1.0, 0.0), "error") === "error");
assert(plain_divide(1.0, 0.0) === Infinity);
assert(1.0 / 0.0 === Infinity, "this unit has no pragma");
return 0;

//?
//? test: not-cleared-by-no-type-check
//? description: noTypeCheck turns type checking off, not strictDouble
//? expect: error:Function 'divide<double>' is an IEEE 754 division by zero for these arguments (#compile strictDouble)
//? source: ...

#compile strictDouble noTypeCheck;
return 1.0 / 0.0;

//?
//? test: process-flag
//? description: compile:strictDouble is the default for units compiled after it is set
//? expect: 0
//? source: ...

flag_set(["compile:strictDouble"], true);
const r = safe_evaluate(evaluate(compile<script>(script("return 1.0 / 0.0;"))), "error");
flag_set(["compile:strictDouble"], false);
assert(r === "error", "got: " + string(r));
assert(evaluate(compile<script>(script("return 1.0 / 0.0;"))) === Infinity);
return 0;
