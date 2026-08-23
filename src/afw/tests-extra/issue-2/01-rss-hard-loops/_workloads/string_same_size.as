/* #2 lab: overwrite a string slot with another same-length literal.
 * Bytes should reuse if optional free of same-size blocks works.
 */
let s = "x";
while (true) {
    s = "y";
    s = "x";
}
return 0;
