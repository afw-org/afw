const cv = compile<script>(script(
    "let o = { n: 0 }; o.n = o.n + 1; return o;"));
const a = evaluate(cv);
const b = evaluate(cv);
assert(a.n === 1);
assert(b.n === 1);
a.n = 7;
assert(b.n === 1, "second evaluate must be a distinct managed value");
assert(a.n === 7);
return true;
