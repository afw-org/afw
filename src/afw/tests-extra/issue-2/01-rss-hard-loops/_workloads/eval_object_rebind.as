/* #2 lab: eval<script> returning an object of functions, overwrite
 * each iteration. Nested escaped functions get_reference the unit onto
 * those bindings; slot overwrite last-releases the previous object then
 * the unit.
 */
let i = 0;
let o = {};
while (true) {
    o = eval<script>(script(
        "function f() { return 1; } return { f: f };"));
    i = i + 1;
}
return 0;
