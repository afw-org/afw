let kept = [];
let i = 0;
let throw_this = false;
for (i = 0; i < 12; i = i + 1) {
    try {
        if (throw_this) {
            evaluate(compile<script>(script("throw \"odd\";")));
        }
        const o = evaluate(compile<script>(script("return { n: 1 };")));
        o.n = i;
        kept[length(kept)] = o;
    } catch (e) {
    }
    throw_this = !throw_this;
}
assert(length(kept) === 6);
let j = 0;
for (j = 0; j < length(kept); j = j + 1) {
    assert(kept[j].n === j * 2, "kept pinned objects after mixed throw");
}
return true;
