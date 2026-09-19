let a = [];
let i = 0;
for (i = 0; i < 20; i = i + 1) {
    const o = evaluate(compile<script>(script(
        "return { n: 1 };")));
    o.n = i;
    a[i] = o;
}
for (i = 0; i < 20; i = i + 1) {
    assert(a[i].n === i, "earlier pinned objects must still be live");
}
return true;
