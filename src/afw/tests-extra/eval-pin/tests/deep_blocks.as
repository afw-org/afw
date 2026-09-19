const o = evaluate(compile<script>(script(
    "let o = { n: 0 }; { { { { let x = 4; o.n = x; } } } } return o;")));
assert(o.n === 4);
return true;
