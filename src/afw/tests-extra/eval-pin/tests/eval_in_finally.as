let o;
try {
    throw "x";
} catch (e) {
    o = evaluate(compile<script>(script("return { a: 1 };")));
} finally {
    const p = evaluate(compile<script>(script("return { b: 2 };")));
    assert(p.b === 2);
}
assert(o.a === 1);
return true;
