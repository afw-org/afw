/* eval<script> in a counted while. No for-let clone. Isolates compile dest. */
const n = 256;
let start_in_use = pool_bytes_in_use();
let start_rss = process_rss();
let i = 0;
let last = 0;
while (i < n) {
    last = eval<script>(script("let yy = 0;\n//let x yy = -1;"));
    i = i + 1;
}
print("probe=while_eval N=", n,
    " d_in_use=", pool_bytes_in_use() - start_in_use,
    " d_rss_kb=", process_rss() - start_rss,
    " last=", last, "\n");
return 0;
