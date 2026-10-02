/* #2 lab: eval<script> returning an object of functions, overwrite
 * each iteration. Nested keep_unit get_references the unit onto those
 * bindings; slot overwrite last-releases the previous object then
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
