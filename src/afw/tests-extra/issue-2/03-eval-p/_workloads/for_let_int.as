/* Nested for (let) integer only. Names on cloned frames; no concat/compile. */
const n1 = 32;
const n2 = 32;
let start_in_use = pool_bytes_in_use();
let start_rss = process_rss();
let acc = 0;
for (let i = 0; i < n1; i = i + 1) {
    for (let j = 0; j < n2; j = j + 1) {
        let x = i + j;
        acc = acc + x;
    }
}
print("probe=for_let_int N=", n1 * n2,
    " d_in_use=", pool_bytes_in_use() - start_in_use,
    " d_rss_kb=", process_rss() - start_rss,
    " acc=", acc, "\n");
return 0;
