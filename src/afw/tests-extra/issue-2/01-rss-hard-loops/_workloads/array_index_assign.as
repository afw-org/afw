/* #2 lab: array element overwrite `a[0] = i`.
 * Face SET slot_stores; previous element last-release. Should stay flat.
 */
let a = [0];
let i = 0;
while (true) {
    a[0] = i;
    i = i + 1;
}
return 0;
