const cv = compile<script>(script("return \"xy\";"));
let s = "";
let i = 0;
for (i = 0; i < 80; i = i + 1) {
    s = s + evaluate(cv);
}
assert(s === evaluate(compile<script>(script(
    "let t = \"\"; let i = 0; for (i = 0; i < 80; i = i + 1) { t = t + \"xy\"; } return t;"))));
return true;
