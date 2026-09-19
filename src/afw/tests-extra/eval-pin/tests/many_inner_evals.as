let i = 0;
let last = 0;
for (i = 0; i < 40; i = i + 1) {
    const o = evaluate(compile<script>(script(
        "let x = { n: 1 }; x.n = 2; return x;")));
    assert(o.n === 2);
    last = last + 1;
}
return last;
