const id = generate_uuid();
add_object("backend", "Churn", { "msg": "churn" }, id);
const o = get_object("backend", "Churn", id);
assert(o.msg === "churn");
replace_object("backend", "Churn", id, { "msg": "churn2" });
const o2 = get_object("backend", "Churn", id);
assert(o2.msg === "churn2");
delete_object("backend", "Churn", id);
assert(
    safe_evaluate(get_object("backend", "Churn", id), "error") === "error");
return true;
