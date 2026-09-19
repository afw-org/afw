const cv = compile<script>(script("return { n: 1 };"));
const a = evaluate(cv);
const b = evaluate(cv);
assert(a.n === 1);
assert(b.n === 1);
a.n = 7;
assert(b.n === 1);
return true;
