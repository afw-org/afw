const o = evaluate(compile<script>(script(
    "let x = { a: 1 }; x.a = 2; return x;")));
assert(o.a === 2);
o.a = 9;
const o2 = evaluate(compile<script>(script("return { a: 1 };")));
assert(o.a === 9);
assert(o2.a === 1);
return true;
