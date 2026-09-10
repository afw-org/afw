/* #2 lab: push then pop the same slot. Still climbs: slot_store extra-hold
 * on the managed integer; pop transfers and does not release.
 */
let a = [0];
let i = 0;
while (true) {
    push(a, i);
    pop(a);
    i = i + 1;
}
return 0;
