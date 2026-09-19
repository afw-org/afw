const r = evaluate(compile<script>(script("return 1 + 2;")));
assert(r === 3);
return r;
