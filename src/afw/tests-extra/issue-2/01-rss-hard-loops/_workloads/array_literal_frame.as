/* #2 lab: array literal with an expression held in the body frame.
 * Should stay flat.
 */
let i = 0;
while (true) {
    const a = [i, i];
    i = i + 1;
}
return 0;
