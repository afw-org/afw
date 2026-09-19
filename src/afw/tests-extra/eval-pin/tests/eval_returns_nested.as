const o = evaluate(compile<script>(script(
    "return { a: { b: { c: [1, 2, 3] } } };")));
assert(o.a.b.c[2] === 3);
o.a.b.c[0] = 8;
assert(o.a.b.c[0] === 8);
return true;
