const id = "lmdb-" + generate_uuid();
add_object("lmdb", "_AdaptiveObject_", {
    "name": "Ada",
    "city": "Dallas"
}, id);
const o = get_object("lmdb", "_AdaptiveObject_", id);
assert(o.name === "Ada");
delete_object("lmdb", "_AdaptiveObject_", id);
return true;
