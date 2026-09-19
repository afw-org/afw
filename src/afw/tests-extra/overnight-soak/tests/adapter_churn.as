const id = generate_uuid();
add_object("backend", "Demo", { "msg": "churn" }, id);
const o = get_object("backend", "Demo", id);
assert(o.msg === "churn");
replace_object("backend", "Demo", id, { "msg": "churn2" });
const o2 = get_object("backend", "Demo", id);
assert(o2.msg === "churn2");
delete_object("backend", "Demo", id);
assert(
    safe_evaluate(get_object("backend", "Demo", id), "error") === "error");
return true;
