const r = evaluate(compile<script>(script(
    "let o = { a: 1 }; o.a = 2; return o.a;")));
assert(r == 2);
return r;
