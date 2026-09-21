/* Concurrent-safe file produce/consume: copy one object inbox → outbox. */
const id = generate_uuid();
add_object("inbox", "Journal", { "n": 1, "msg": "pc-" + id }, id);
const a = get_object("inbox", "Journal", id);
add_object("outbox", "Journal", { "n": a.n, "msg": a.msg }, id);
const b = get_object("outbox", "Journal", id);
assert(b.n === 1);
assert(b.msg === a.msg);
delete_object("inbox", "Journal", id);
delete_object("outbox", "Journal", id);
return true;
