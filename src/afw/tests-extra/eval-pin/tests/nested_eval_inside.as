function g() {
    return evaluate(compile<script>(script("return 6 + 1;")));
}
assert(g() === 7);
const r = evaluate(compile<script>(script(
    "return evaluate(compile<script>(script(\"return 7;\")));")));
assert(r === 7);
return r;
