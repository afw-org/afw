/* Nested for (let) + BMP-style hex concat. No script()/compile/eval. */
const hex = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"];
const n1 = 16;
const n2 = 64;
let start_in_use = pool_bytes_in_use();
let start_rss = process_rss();
let last = "";
for (let i = 0; i < n1; i = i + 1) {
    for (let j = 0; j < n2; j = j + 1) {
        let uu = hex[i] + hex[0] + hex[0] + hex[1];
        last = uu;
    }
}
print("probe=for_let_concat N=", n1 * n2,
    " d_in_use=", pool_bytes_in_use() - start_in_use,
    " d_rss_kb=", process_rss() - start_rss,
    " last=", last, "\n");
return 0;
