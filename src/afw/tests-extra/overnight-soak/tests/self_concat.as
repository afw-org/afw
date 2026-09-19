const s = evaluate(compile<script>(script(
    "let s = \"ab\"; s = s + s; return s;")));
assert(s === "abab");
return true;
