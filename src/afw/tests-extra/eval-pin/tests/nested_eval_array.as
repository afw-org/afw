const a = evaluate(compile<script>(script(
    "let x = [1, 2]; x[0] = 3; return x;")));
assert(a[0] === 3);
assert(a[1] === 2);
a[1] = 8;
assert(a[1] === 8);
const a2 = evaluate(compile<script>(script("return [9];")));
assert(a[0] === 3, "later evaluate must not clobber earlier array");
assert(a2[0] === 9);
return true;
