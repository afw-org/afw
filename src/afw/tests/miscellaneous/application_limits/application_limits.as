#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: application_limits.as
//? description: Optional application conf overrides process limit* knobs
//? sourceType: script
//?
//? test: conf-overrides-process-limits
//? description: Present optional limits replace compile-time defaults; absent stay default
//? expect: 0
//? source: ...

assert(process::limitEvaluationStackCount === 40);
assert(process::limitCStackHeadroomBytes === 0);
assert(process::limitRequestPoolBytes === 67108864);
assert(process::chunkMin === 4096);
assert(process::compileChunkMin === 4096);
assert(process::xctxChunkMin === 8192);
assert(process::memoryRegionFreeListMaxBytes === 4096);
return 0;

//?
//? test: conf-eval-stack-limit-trips
//? description: Nested eval xctx uses conf limitEvaluationStackCount
//? expect: 0
//? source: ...

function rec(n) {
    if (n <= 0) {
        return 0;
    }
    return rec(n - 1);
}
assert(rec(2) === 0);
let id = "";
try {
    rec(80);
}
catch (e) {
    id = e.id;
}
assert(id === "payload_too_large", id);
return 0;
