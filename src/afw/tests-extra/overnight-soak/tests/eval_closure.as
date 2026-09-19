const f = evaluate(compile<script>(script(
    "let n = 3; return function (x) { return n + x; };")));
assert(f(4) === 7);
return f(4);
