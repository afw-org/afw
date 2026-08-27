/* #2 lab: assign the result of a script function that returns a temp integer.
 * FRV + slot_store should recycle on the caller's evaluation heap.
 */
let i = 0;
function f() {
    return i + 1;
}
while (true) {
    i = f();
}
return 0;
