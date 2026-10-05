/* #2 lab: object literal with an expression held in the body frame.
 * Script-built container; frame slot isolates. Should stay flat.
 */
let i = 0;
while (true) {
    const o = { n: i };
    i = i + 1;
}
return 0;
