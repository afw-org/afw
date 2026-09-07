/* #2 lab: assign the result of a script function that returns a temp integer.
 * Unique FRV consume leaves the wrapper header in caller p — still climbs.
 */
let i = 0;
function f() {
    return i + 1;
}
while (true) {
    i = f();
}
return 0;
