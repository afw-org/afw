/* #2 lab: overlay set `o.x = i`. Face SET slot_stores; previous overlay
 * last-release. Should stay flat.
 */
let o = { x: 0 };
let i = 0;
while (true) {
    o.x = i;
    i = i + 1;
}
return 0;
