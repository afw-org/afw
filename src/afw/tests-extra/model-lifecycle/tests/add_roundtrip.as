add_object("model", "Demo", { "msg": "via-model" }, "added");
const f = get_object("backend", "Demo", "added");
assert(f.msg === "via-model", "file did not see model add");
const m = get_object("model", "Demo", "added");
assert(m.msg === "via-model", "model get after add");
return true;
