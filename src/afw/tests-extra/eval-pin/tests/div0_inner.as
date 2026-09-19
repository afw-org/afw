let threw = false;
try {
    evaluate(compile<script>(script("return 1 / 0;")));
} catch (e) {
    threw = true;
}
assert(threw, "divide-by-zero from inner evaluate");
const ok = evaluate(compile<script>(script("return 3;")));
assert(ok === 3);
return true;
