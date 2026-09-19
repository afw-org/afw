service_stop("adapter-model");
const s = service_get("adapter-model");
assert(s.status === "stopped", "model should be stopped");
const f = get_object("backend", "Demo", "seed");
assert(f.msg === "seed", "backend should still serve");
return true;
