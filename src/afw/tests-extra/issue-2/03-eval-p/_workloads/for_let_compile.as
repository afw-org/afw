/* Nested for (let) + throwaway compile<script>. Does not evaluate. */
const n = 256;
let start_in_use = pool_bytes_in_use();
let start_rss = process_rss();
let last = compile<script>(script("1+1"));
for (let i = 0; i < n; i = i + 1) {
    last = compile<script>(script("let yy = " + i + ";"));
}
print("probe=for_let_compile N=", n,
    " d_in_use=", pool_bytes_in_use() - start_in_use,
    " d_rss_kb=", process_rss() - start_rss,
    " last_dt=", meta(last).dataType, "\n");
return 0;
