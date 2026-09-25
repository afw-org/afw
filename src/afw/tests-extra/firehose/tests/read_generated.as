const p = get_object("data", "Person", "gen-12");
assert(p.city === "Austin");
const n = get_object("model", "Note", "note-gen-12");
assert(n.personId === "gen-12");
return true;
