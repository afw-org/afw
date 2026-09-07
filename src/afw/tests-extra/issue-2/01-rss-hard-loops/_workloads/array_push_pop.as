/* #2 lab: push then pop the same slot. Still climbs: managed push calloc's
 * a ring entry in xctx->p; pop transfers and does not free that entry.
 */
let a = [0];
let i = 0;
while (true) {
    push(a, i);
    pop(a);
    i = i + 1;
}
return 0;
