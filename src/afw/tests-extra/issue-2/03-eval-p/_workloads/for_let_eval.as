/* Nested for (let) + eval<script> of a tiny source. Closest small BMP core. */
const n = 256;
let start_in_use = pool_bytes_in_use();
let start_rss = process_rss();
let last = 0;
for (let i = 0; i < n; i = i + 1) {
    last = eval<script>(script("let yy = 0;\n//let x yy = -1;"));
}
print("probe=for_let_eval N=", n,
    " d_in_use=", pool_bytes_in_use() - start_in_use,
    " d_rss_kb=", process_rss() - start_rss,
    " last=", last, "\n");
return 0;
