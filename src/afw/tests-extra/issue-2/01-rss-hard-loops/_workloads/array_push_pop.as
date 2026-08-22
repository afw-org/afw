/* #2 lab: push then pop the same slot. Capacity may grow once, then should
 * stay flat if element overwrite reuses.
 */
let a = [0];
let i = 0;
while (true) {
    push(a, i);
    pop(a);
    i = i + 1;
}
return 0;
