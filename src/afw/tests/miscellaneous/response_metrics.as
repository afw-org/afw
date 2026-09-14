#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: response_metrics.as
//? description: response:metrics flag stamps this-request numbers on _AdaptiveResponse_
//? sourceType: script
//?
//? test: metrics-absent-when-flag-off
//? description: Default response has no metrics property
//? expect: 0
//? source: ...

const result = perform({
    function: "add",
    values: [1, 1]
});
assert(result.status === "success");
assert(result.metrics === undefined);
return 0;

//?
//? test: metrics-present-when-flag-on
//? description: Flag on adds metrics object with this-request integers
//? expect: 0
//? source: ...

flag_set(["response:metrics"], true);
const result = perform({
    function: "add",
    values: [1, 1]
});
assert(result.status === "success");
assert(meta(result.metrics.poolBytesInUse).dataType === "integer");
assert(result.metrics.poolBytesInUse >= 0);
assert(meta(result.metrics.poolChunkBytes).dataType === "integer");
assert(result.metrics.poolChunkBytes >= 0);
assert(meta(result.metrics.evaluationStackCount).dataType === "integer");
assert(result.metrics.evaluationStackCount >= 0);
return 0;

//?
//? test: metrics-on-error-response
//? description: Metrics are also on error responses
//? expect: 0
//? source: ...

flag_set(["response:metrics"], true);
const result = perform({
    function: "subtract",
    arg1: 3
});
assert(result.status === "error");
assert(meta(result.metrics.poolBytesInUse).dataType === "integer");
assert(result.metrics.poolBytesInUse >= 0);
return 0;
