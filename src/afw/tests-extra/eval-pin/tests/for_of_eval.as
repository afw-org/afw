const src = [1, 2, 3, 4];
let out = [];
for (let x of src) {
    const o = evaluate(compile<script>(script("return { n: 0 };")));
    o.n = x;
    out[length(out)] = o;
}
assert(length(out) === 4);
assert(out[0].n === 1);
assert(out[3].n === 4);
return true;
