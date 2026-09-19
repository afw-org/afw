const cv = compile<script>(script("return { n: 1 };"));
let s = 0;
for (let i = 0; i < 10; i = i + 1) {
    const o = evaluate(cv);
    o.n = i;
    s = s + o.n;
}
assert(s === 45);
return true;
