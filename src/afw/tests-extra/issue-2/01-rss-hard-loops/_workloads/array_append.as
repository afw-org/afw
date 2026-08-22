/* #2 lab: unbounded push. Harness control — RSS must grow. Not a leak of
 * overwritten slots; the array is supposed to get bigger.
 */
let a = [];
let i = 0;
while (true) {
    push(a, i);
    i = i + 1;
}
return 0;
