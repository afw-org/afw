/* Bump N here and in consume.as / compare.as together. */
const N = 100;
let i = 0;
for (i = 0; i < N; i = i + 1) {
    const id = string(i);
    add_object("inbox", "Journal", {
        "n": i,
        "msg": "j-" + id
    }, id);
}
assert(get_object("inbox", "Journal", "0").n === 0);
assert(get_object("inbox", "Journal", string(N - 1)).n === N - 1);
return N;
