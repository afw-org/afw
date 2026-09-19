const a = evaluate(compile<script>(script(
    "let x = [1, 2]; x[0] = 3; return x;")));
assert(a[0] === 3);
assert(a[1] === 2);
return true;
