const f = evaluate(compile<script>(script(
    "return function (n) { return n + 1; };")));
assert(f(2) === 3);
return f(2);
