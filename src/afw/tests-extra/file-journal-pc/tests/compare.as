/* Bump N here and in produce.as / consume.as together. */
const N = 100;
let i = 0;
for (i = 0; i < N; i = i + 1) {
    const id = string(i);
    const src = get_object("inbox", "Journal", id);
    const dst = get_object("outbox", "Journal", id);
    assert(src.n === dst.n, id);
    assert(src.msg === dst.msg, id);
}
const inbox = retrieve_objects("inbox", "Journal");
const outbox = retrieve_objects("outbox", "Journal");
assert(length(inbox) === N);
assert(length(outbox) === N);
return N;
