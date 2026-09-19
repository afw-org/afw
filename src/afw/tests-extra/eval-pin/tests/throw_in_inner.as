let threw = false;
try {
    evaluate(compile<script>(script(
        "let o = { a: 1 }; throw \"inner-boom\"; return o;")));
} catch (e) {
    threw = true;
}
assert(threw, "inner throw should surface");
const ok = evaluate(compile<script>(script("return { a: 2 };")));
assert(ok.a === 2, "later evaluate after inner throw");
return true;
