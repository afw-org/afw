#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: resource_limits.as
//? description: Issue #329 step 3: payload_too_large on eval-stack cap
//? sourceType: script
//?
//? test: recursion-under-cap
//? description: Shallow recursion stays under limitEvaluationStackCount
//? expect: 0
//? source: ...

function rec(n) {
    if (n <= 0) {
        return 0;
    }
    return rec(n - 1);
}
assert(rec(20) === 0);
return 0;

//?
//? test: recursion-hits-eval-stack-limit
//? description: Deep recursion throws payload_too_large (413)
//? expect: 0
//? source: ...

function rec(n) {
    if (n <= 0) {
        return 0;
    }
    return rec(n - 1);
}
let id = "";
try {
    rec(600);
}
catch (e) {
    id = e.id;
}
assert(id === "payload_too_large", id);
return 0;
