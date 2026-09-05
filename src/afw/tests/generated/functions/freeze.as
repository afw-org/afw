#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/freeze.as
//? customPurpose: Part of core function tests
//? description: Only freeze<object> and freeze<array> overloads are registered, so the polymorphic dispatcher rejects any other data type at the call site (statically or dynamically routed) before afw_function_execute_freeze's own "freeze requires an object or array" runtime fallback can ever run. That fallback is currently dead code from script.
//? sourceType: script
//?
//? test: freeze-1
//? description: freeze an array then confirm it rejects mutation
//? expect: error
//? source: ...

let a = freeze([1, 2, 3]);
push(a, 4);
