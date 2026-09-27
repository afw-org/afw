// Hold a snapshot of metrics and properties, then stop the instance.
// The snapshot stays readable. A fresh get after stop finds no
// active adapter.
const a = get_object("afw", "_AdaptiveAdapter_", "file");
assert(a !== null && a !== undefined);
assert(a.adapterId === "file");
const metrics = a.metrics;
const props = a.properties;
assert(metrics !== null && metrics !== undefined,
    "metrics present while adapter active");
assert(props !== null && props !== undefined,
    "properties present while adapter active");
const getCount = metrics.getObjectCount;
assert(getCount !== null && getCount !== undefined);
assert(props.adapterId === "file");

const stopped = service_stop("adapter-file");
assert(stopped !== null && stopped !== undefined);

assert(metrics.getObjectCount === getCount,
    "held metrics stay readable after stop");
assert(props.adapterId === "file",
    "held properties stay readable after stop");

const after = get_object("afw", "_AdaptiveAdapter_", "file");
assert(after.metrics === null || after.metrics === undefined,
    "active metrics gone after stop");
assert(after.properties === null || after.properties === undefined,
    "active properties gone after stop");
return props.adapterId;
