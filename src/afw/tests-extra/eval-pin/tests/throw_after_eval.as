let o;
try {
    o = evaluate(compile<script>(script("return { a: 1, b: \"k\" };")));
    throw "after-eval";
} catch (e) {
    assert(o.a === 1, "pinned result must survive throw after evaluate");
    assert(o.b === "k");
    o.a = 4;
    assert(o.a === 4);
}
return true;
