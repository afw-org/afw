/* eval<script> with in_use samples. Superlinear d_in_use vs k is a hold. */
const n = 512;
let start_in_use = pool_bytes_in_use();
let start_rss = process_rss();
print("probe=for_let_eval_steps start_in_use=", start_in_use, "\n");
for (let i = 0; i < n; i = i + 1) {
    eval<script>(script("1+1"));
    if ((i === 63) || (i === 127) || (i === 255) || (i === 511)) {
        print("k=", i + 1,
            " d_in_use=", pool_bytes_in_use() - start_in_use,
            " d_rss_kb=", process_rss() - start_rss, "\n");
    }
}
print("probe=for_let_eval_steps N=", n,
    " d_in_use=", pool_bytes_in_use() - start_in_use,
    " d_rss_kb=", process_rss() - start_rss, "\n");
return 0;
