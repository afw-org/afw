/* #2 lab: assign the result of a script function that returns a temp integer.
 * Pin on the caller — no leftover wrapper (PR #326). Still grows ~1.5 MiB/s
 * (2026-09-15); under the fail line. Do not add a function_return_value type
 * back.
 */
let i = 0;
function f() {
    return i + 1;
}
while (true) {
    i = f();
}
return 0;
