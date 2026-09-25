const id = "churn-" + generate_uuid();
add_object("data", "Note", {
    "title": "temp",
    "body": "gone soon",
    "personId": "ada"
}, id);
const n = get_object("data", "Note", id);
assert(n.title === "temp");
delete_object("data", "Note", id);
return true;
