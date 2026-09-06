/* Compile once, evaluate inside for (let). Compile leak should be one unit. */
const n = 256;
const cv = compile<script>(script("let yy = 0;\n//let x yy = -1;"));
let start_in_use = pool_bytes_in_use();
let start_rss = process_rss();
let last = 0;
for (let i = 0; i < n; i = i + 1) {
    last = evaluate(cv);
}
print("probe=compile_once_for_let N=", n,
    " d_in_use=", pool_bytes_in_use() - start_in_use,
    " d_rss_kb=", process_rss() - start_rss,
    " last=", last, "\n");
return 0;
