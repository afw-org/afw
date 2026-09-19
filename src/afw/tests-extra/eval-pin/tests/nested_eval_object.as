const o = evaluate(compile<script>(script(
    "let x = { a: 1, b: \"z\" }; x.a = 2; return x;")));
assert(o.a === 2, "pinned object a");
assert(o.b === "z", "pinned object b");
o.a = 9;
assert(o.a === 9, "caller can set after inner evaluate dest p");
const o2 = evaluate(compile<script>(script("return { a: 1 };")));
assert(o.a === 9, "later evaluate must not clobber earlier result");
assert(o2.a === 1);
return true;
