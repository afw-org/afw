#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: comments-bmp-block-0.as
//? customPurpose: Part of test262
//? description: S7.4_A6 BMP \\uXXXX in /* */ comments (part 0)
//? sourceType: script
//?
//? test: S7.4_A6_part0
//? description: Insert \\uXXXX inside /* */ ; comment must stay closed
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
                let y = eval(script("/*let " + xx + "y = 1*/"));
                if (y !== undefined) {
                    throw "Expected y to be undefined";
                }
            }
        }
    }
}
