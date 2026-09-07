/* #2 lab: overwrite a string slot with another same-length literal.
 * Last-release of the previous managed string. Should stay flat.
 */
let s = "x";
while (true) {
    s = "y";
    s = "x";
}
return 0;
