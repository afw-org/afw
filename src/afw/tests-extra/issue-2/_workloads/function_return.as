/* #2 lab: assign the result of a script function that returns a temp integer.
 * donate_return + slot_store should not grow the eval heap without bound.
 */
let i = 0;
function f() {
    return i + 1;
}
while (true) {
    i = f();
}
return 0;
