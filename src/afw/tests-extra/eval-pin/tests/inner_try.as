const o = evaluate(compile<script>(script(
    "let o = { n: 0 }; try { throw \"x\"; } catch (e) { o.n = 2; } return o;")));
assert(o.n === 2);
const o2 = evaluate(compile<script>(script(
    "let o = { n: 0 }; try { o.n = 1; } finally { o.n = o.n + 1; } return o;")));
assert(o2.n === 2);
return true;
