let o = null;
{
    o = evaluate(compile<script>(script("return { a: 1 };")));
}
assert(o.a === 1, "assign evaluate() in inner block, use outside");
o.a = 5;
assert(o.a === 5);
return true;
