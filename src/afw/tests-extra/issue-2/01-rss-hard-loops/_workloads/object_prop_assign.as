/* #2 lab: overlay set `o.x = i` (issue-2-lifetime S4).
 * Wrapper set should release the previous overlay and reuse.
 */
let o = { x: 0 };
let i = 0;
while (true) {
    o.x = i;
    i = i + 1;
}
return 0;
