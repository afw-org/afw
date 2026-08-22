/* #2 lab: `o = { n: i }` each iteration in one long scope.
 * Unmanaged faces do not destroy at zero; optional free later if this grows.
 */
let i = 0;
let o = {};
while (true) {
    o = { n: i };
    i = i + 1;
}
return 0;
