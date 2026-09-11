/* #2 lab: push then pop the same slot. Managed pop registers the
 * transferred extra-hold on the current scope (temp). Should stay flat.
 */
let a = [0];
let i = 0;
while (true) {
    push(a, i);
    pop(a);
    i = i + 1;
}
return 0;
