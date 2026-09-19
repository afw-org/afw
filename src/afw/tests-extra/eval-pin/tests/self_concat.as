/* s = s + s used to UAF when clone_unmanaged memcpy dest==src (#275). */
const s = evaluate(compile<script>(script(
    "let s = \"ab\"; s = s + s; s = s + s; return s;")));
assert(s === "abababab");
return s;
