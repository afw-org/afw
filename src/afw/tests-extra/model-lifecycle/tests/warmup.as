const backend = service_get("adapter-backend");
assert(backend.status === "running", "backend not running");
const model = service_get("adapter-model");
assert(model.status === "running", "model not running");
const a = get_object("afw", "_AdaptiveAdapter_", "model");
assert(a.adapterId === "model");
assert(a.referenceCount >= 1, "model refcount");
return true;
