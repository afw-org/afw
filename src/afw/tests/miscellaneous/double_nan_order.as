#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: double_nan_order.as
//? customPurpose: Part of core function tests
//? description: ...
Strict NaN (#628). No double operation produces NaN or takes it: once
one appears every operation would pass it on and every comparison would
quietly say false. Dividing by zero throws, as it does for integers.
NaN can still be data (#nan, NaN, to_double("NaN"), stored values), and
is_NaN tests for it. == and === treat NaN as equal to itself, as
includes and index_of do; <, <=, >, >=, max, min and a sort with no
compareFn throw for NaN, which has no order. Overflow still gives
Infinity, which is ordered like any other value.
//? sourceType: script
//?
//? test: operations-do-not-produce-nan
//? description: a result that would be NaN throws
//? expect: 0
//? source: ...

const throws = function(f) {
    try { f(); } catch (e) { return e.id === "argument_error"; }
    return false;
};
assert(throws(function() { return #infinity - #infinity; }), "inf - inf");
assert(throws(function() { return #infinity + #minusInfinity; }), "inf + -inf");
assert(throws(function() { return #infinity * 0.0; }), "inf * 0");
assert(throws(function() { return #infinity / #infinity; }), "inf / inf");
assert(throws(function() { return #infinity % 1.0; }), "inf % 1");
assert(throws(function() { return pow(-1.0, 0.5); }), "pow(-1, 0.5)");
assert(throws(function() { return pow(1.0, #infinity); }), "pow(1, inf)");
assert(throws(function() { return sqrt(-1.0); }), "sqrt(-1)");
assert(throws(function() { return log(-1.0); }), "log(-1)");
assert(throws(function() { return asin(2.0); }), "asin(2)");
assert(throws(function() { return sin(#infinity); }), "sin(inf)");
return 0;

//?
//? test: operations-do-not-take-nan
//? description: a NaN operand throws and names the parameter
//? expect: error:Parameter 2 of function 'add<double>' is NaN; no double operation takes NaN (is_NaN(v) tests for it)
//? source: ...

return 1.0 + #nan;

//?
//? test: divide-by-zero
//? description: a double divided by zero throws, as an integer does, an infinity and a zero included
//? expect: 0
//? source: ...

for (const n of [1.0, -1.0, 0.0, #infinity]) {
    for (const z of [0.0, -0.0]) {
        assert(safe_evaluate(n / z, "error") === "error", string(n) + " / " + string(z));
        assert(safe_evaluate(n % z, "error") === "error", string(n) + " % " + string(z));
    }
}
return 0;

//?
//? test: divide-by-zero-message
//? description: the message is the integer one's
//? expect: error:Double divide by zero error
//? source: ...

return 1.0 / 0.0;

//?
//? test: infinity-is-a-value
//? description: overflow gives Infinity, and Infinity is ordered and usable
//? expect: 0
//? source: ...

assert(#doubleMax * 2.0 === #infinity);
assert(-#doubleMax * 2.0 === #minusInfinity);
assert(1.0 / #infinity === 0.0);
assert(#infinity > #doubleMax && #minusInfinity < -#doubleMax);
assert(abs(#minusInfinity) === #infinity);
assert(log(0.0) === #minusInfinity);
assert(max<double>(1.0, #infinity) === #infinity);
return 0;

//?
//? test: equality
//? description: == and === treat NaN as equal to itself, as includes and index_of do
//? expect: 0
//? source: ...

assert(NaN === NaN && NaN == NaN && #nan === NaN);
assert(!(NaN !== NaN) && !(NaN != NaN));
for (const x of [1.0, 0.0, Infinity, -Infinity]) {
    assert(!(NaN === x) && !(x === NaN) && !(NaN == x), "==");
    assert(NaN !== x && x !== NaN && NaN != x, "!=");
}
assert(includes([1.0, NaN], NaN) && index_of([1.0, NaN], NaN) === 1);
assert(!includes([1.0, 2.0], NaN));
return 0;

//?
//? test: no-order
//? description: <, <=, >, >= with NaN throw
//? expect: 0
//? source: ...

for (const x of [NaN, 1.0, Infinity, -Infinity]) {
    assert(safe_evaluate(NaN < x, "error") === "error" && safe_evaluate(x < NaN, "error") === "error", "<");
    assert(safe_evaluate(NaN <= x, "error") === "error" && safe_evaluate(x <= NaN, "error") === "error", "<=");
    assert(safe_evaluate(NaN > x, "error") === "error" && safe_evaluate(x > NaN, "error") === "error", ">");
    assert(safe_evaluate(NaN >= x, "error") === "error" && safe_evaluate(x >= NaN, "error") === "error", ">=");
}
return 0;

//?
//? test: no-order-message
//? description: the message says NaN has no order
//? expect: error:lt<double> of NaN; NaN has no order (is_NaN(v) tests for it)
//? source: ...

return NaN < 1.0;

//?
//? test: max-min-nan
//? description: max and min throw for a NaN, wherever it is
//? expect: 0
//? source: ...

assert(safe_evaluate(max<double>(NaN, 1.0), "error") === "error");
assert(safe_evaluate(max<double>(1.0, NaN, Infinity), "error") === "error");
assert(safe_evaluate(min<double>(1.0, NaN), "error") === "error");
assert(max<double>(1.0, Infinity, -2.0) === Infinity);
assert(min<double>(1.0, -Infinity, 2.0) === -Infinity);
return 0;

//?
//? test: sort-nan
//? description: a sort with no compareFn throws for NaN; with a compareFn the caller decides
//? expect: 0
//? source: ...

assert(safe_evaluate(sort([NaN, 1.0]), "error") === "error");
assert(safe_evaluate(to_sorted([1.0, NaN]), "error") === "error");
const r = sort([Infinity, 1.0, -Infinity, -0.5]);
assert(r[0] === -Infinity && r[1] === -0.5 && r[2] === 1.0 && r[3] === Infinity);
const nan_last = function(a, b) { return !is_NaN(a) && (is_NaN(b) || a < b); };
const s = to_sorted([NaN, 2.0, 1.0], nan_last);
assert(s[0] === 1.0 && s[1] === 2.0 && is_NaN(s[2]));
return 0;

//?
//? test: nan-is-data
//? description: NaN can be held, passed, converted and tested; only operations reject it
//? expect: 0
//? source: ...

const a = [#nan, to_double("NaN"), NaN];
assert(is_NaN(a[0]) && is_NaN(a[1]) && is_NaN(a[2]));
assert(string(#nan) === "NaN");
assert(a->filter(function(v) { return !is_NaN(v); })->length() === 0);
assert(!is_finite(#nan));
return 0;

//?
//? test: decompile
//? description: decompile writes the # literals for NaN and the infinities
//? expect: "#script_function(#block(return([#nan,#infinity,#minusInfinity,#minusInfinity])))"
//? source: ...

return decompile(function() { return [NaN, Infinity, -Infinity, #minusInfinity]; });
