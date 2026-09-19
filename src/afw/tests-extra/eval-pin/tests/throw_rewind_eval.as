function f() {
    evaluate(compile<script>(script("let a = 1; throw \"boom-nested\";")));
}
try {
    f();
    assert(false, "should have thrown");
} catch (e) {
    assert(e.message === "boom-nested", e.message);
}
const ok = evaluate(compile<script>(script("return 2;")));
assert(ok === 2);
return true;
