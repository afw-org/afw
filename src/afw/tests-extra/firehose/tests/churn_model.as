const id = "mchurn-" + generate_uuid();
add_object("model", "Person", {
    "name": "Temp",
    "city": "Austin",
    "active": false
}, id);
const p = get_object("model", "Person", id);
assert(p.name === "Temp");
delete_object("model", "Person", id);
return true;
