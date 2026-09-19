const cv = compile<script>(script("let n = 1; n = n + 1; return n;"));
let i = 0;
let last = 0;
for (i = 0; i < 200; i = i + 1) {
    last = evaluate(cv);
    assert(last === 2);
}
return last;
