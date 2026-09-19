const o = evaluate(compile<script>(script(
    "return { n: 1, f: function () { return 4; } };")));
assert(o.n === 1);
assert(o.f() === 4);
o.n = 9;
assert(o.f() === 4);
return true;
