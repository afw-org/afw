const o = evaluate(compile<script>(script(
    "let x = { a: 1 }; x.a = 2; return x;")));
assert(o.a === 2);
o.a = 9;
assert(o.a === 9);
return true;
