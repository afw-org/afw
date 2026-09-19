let threw = false;
try {
    evaluate(compile<script>(script("throw \"soak-boom\";")));
} catch (e) {
    threw = true;
}
assert(threw);
const ok = evaluate(compile<script>(script("return 1;")));
assert(ok === 1);
return true;
