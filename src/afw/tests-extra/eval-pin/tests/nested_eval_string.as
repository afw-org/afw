const s = evaluate(compile<script>(script(
    "return \"hello\" + \"-\" + \"world\";")));
assert(s === "hello-world");
return s;
