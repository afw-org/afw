const o = get_object("model", "Demo", "seed");
assert(o !== null && o !== undefined, "missing mapped seed");
assert(o.msg === "seed", "mapped msg");
const f = get_object("backend", "Demo", "seed");
assert(f.msg === "seed", "file msg");
return o.msg;
