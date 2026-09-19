replace_object("model", "Demo", "added", { "msg": "replaced" });
const m = get_object("model", "Demo", "added");
assert(m.msg === "replaced");
const f = get_object("backend", "Demo", "added");
assert(f.msg === "replaced", "file did not see model replace");
return m.msg;
