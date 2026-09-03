#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: comments-bmp-slash-0.as
//? customPurpose: Part of test262
//? description: S7.4_A5 BMP \\uXXXX in // comments (part 0)
//? sourceType: script
//?
//? test: S7.4_A5_part0
//? description: Insert \\uXXXX after // ; line terminators end the comment
//? differences: ...
Adaptive Script stores UTF-8. This is Jeremy's BMP sweep via ES \\uXXXX
(internally converted to UTF-8), split so valgrind can finish. i1 0..6.
//? skip: true
//? skipReason: ...
FIXME: very slow after #277 (~14k nested eval<script> per file; the
four files sequential made `afwdev test -j` ~6 min). Unskipped copy:
src/afw/tests-extra/test262/ (`afwdev test -T src/afw/tests-extra/test262`).
//? expect: success
//? source: ...

let hex = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"];
for (let i1 = 0; i1 < 7; i1 = i1 + 1) {
    for (let i2 = 0; i2 < 8; i2 = i2 + 1) {
        for (let i3 = 0; i3 < 16; i3 = i3 + 1) {
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
            }
        }
    }
}
