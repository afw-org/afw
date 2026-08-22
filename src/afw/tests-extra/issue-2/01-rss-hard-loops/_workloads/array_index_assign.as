/* #2 lab: array element overwrite `a[0] = i`.
 * Same reuse story as overlay set, on the array face.
 */
let a = [0];
let i = 0;
while (true) {
    a[0] = i;
    i = i + 1;
}
return 0;
