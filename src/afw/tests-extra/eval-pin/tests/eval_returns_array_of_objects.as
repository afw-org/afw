const a = evaluate(compile<script>(script(
    "return [{ a: 1 }, { a: 2 }, { a: 3 }];")));
assert(a[0].a === 1);
assert(a[2].a === 3);
a[1].a = 9;
assert(a[1].a === 9);
assert(a[0].a === 1);
return true;
