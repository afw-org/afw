function f() {
    {
        return evaluate(compile<script>(script("return { a: 1, b: \"z\" };")));
    }
}
const o = f();
assert(o.a === 1, "return evaluate() from inner block");
assert(o.b === "z");
o.a = 2;
assert(o.a === 2);
return true;
