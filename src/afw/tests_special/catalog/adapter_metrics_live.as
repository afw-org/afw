#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: adapter_metrics_live.as
//? customPurpose: tests_special — adapter metrics live-while-active (#149)
//? description: metrics object present for active adapter afw
//? sourceType: script
//?
//? test: adapter_afw_metrics_present
//? description: live metrics while adapter active
//? expect: 0
//? source: ...

const a = get_object("afw", "_AdaptiveAdapter_", "afw");
assert(a !== null && a !== undefined);
assert(a.metrics !== null && a.metrics !== undefined,
    "metrics should be present while adapter active");
// Counter fields are live integers on the metrics object
assert(a.metrics.getObjectCount !== undefined,
    "metrics.getObjectCount");
return 0;
