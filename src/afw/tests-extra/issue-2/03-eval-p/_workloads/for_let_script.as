/* Nested for (let) + script() wrap of concat. Does not compile or eval. */
const n = 1024;
let start_in_use = pool_bytes_in_use();
let start_rss = process_rss();
let last = script("1");
for (let i = 0; i < n; i = i + 1) {
    let src = script("let yy = " + i + ";");
    last = src;
}
print("probe=for_let_script N=", n,
    " d_in_use=", pool_bytes_in_use() - start_in_use,
    " d_rss_kb=", process_rss() - start_rss, "\n");
return 0;
