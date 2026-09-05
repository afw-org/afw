/* Reduced comments-bmp-slash-0: 1*2*8*16 = 256 inner trips, two eval<script> each. */
let hex = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"];
let start_in_use = pool_bytes_in_use();
let start_rss = process_rss();
let n = 0;
for (let i1 = 0; i1 < 1; i1 = i1 + 1) {
    for (let i2 = 0; i2 < 2; i2 = i2 + 1) {
        for (let i3 = 0; i3 < 8; i3 = i3 + 1) {
            for (let i4 = 0; i4 < 16; i4 = i4 + 1) {
                let uu = hex[i1] + hex[i2] + hex[i3] + hex[i4];
                let xx = eval<script>(script("\"\\u" + uu + "\""));
                let LineTerminators = ((uu === "000A") || (uu === "000D") ||
                    (uu === "2028") || (uu === "2029"));
                let strToEval = script("let yy = 0;\n//let " + xx + "yy = -1;");
                let e = eval<script>(strToEval);
                if (LineTerminators) {
                    assert(e === -1, "The value of 'e' is expected to equal -1");
                } else {
                    assert(e === undefined,
                        "The value of 'e' is expected to equal undefined");
                }
                n = n + 1;
            }
        }
    }
}
print("probe=mini_bmp N=", n,
    " d_in_use=", pool_bytes_in_use() - start_in_use,
    " d_rss_kb=", process_rss() - start_rss, "\n");
return 0;
