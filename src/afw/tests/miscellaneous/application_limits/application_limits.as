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
assert(process::defaultChunkMin === 4096);
assert(process::smallChunkMin === 4096);
assert(process::xctxChunkMin === 8192);
assert(process::memoryRegionFreeListMaxBytes === 4096);
assert(process::memoryRegionKeepSmallCount === 2);
assert(process::memoryRegionKeepLargeCount === 0);
/* Conf setting only, not the live pthread size. glibc's attr
 * default is RLIMIT_STACK, so this 4MiB value is below a typical
 * 8MiB ulimit -s; afw_os_thread_create still setstacksize. */
assert(process::threadStackBytes === 4194304);
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
