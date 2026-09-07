/* #2 lab: `o = { n: i }` each iteration in one long scope.
 * Slot fill isolates; previous occupant last-release. Should stay flat.
 */
let i = 0;
let o = {};
while (true) {
    o = { n: i };
    i = i + 1;
}
return 0;
