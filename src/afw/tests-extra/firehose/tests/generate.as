/* Simple stand-in for a type-driven generator.
   Writes a fixed batch through the file adapter and the model.
   A later generator can read the object types and fill properties from them. */
let i = 1;
for (i = 1; i <= 24; i = i + 1) {
    const id = "gen-" + string(i);
    let city = "Dallas";
    if (i > 8) {
        city = "Austin";
    }
    if (i > 16) {
        city = "Lisbon";
    }
    add_object("data", "Person", {
        "name": "Person " + string(i),
        "city": city,
        "active": true
    }, id);
    add_object("model", "Note", {
        "title": "Note " + string(i),
        "body": "About " + id,
        "personId": id
    }, "note-" + id);
}
const p = get_object("model", "Person", "gen-1");
assert(p.name === "Person 1");
const n = get_object("data", "Note", "note-gen-24");
assert(n.personId === "gen-24");
return true;
