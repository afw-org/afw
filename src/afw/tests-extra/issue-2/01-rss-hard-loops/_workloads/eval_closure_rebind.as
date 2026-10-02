/* #2 lab: eval<script> returning a closure, overwrite each iteration.
 * keep_unit transfers the inner pin onto a new binding; slot overwrite
 * last-releases the previous binding then the unit.
 */
let i = 0;
let f = function() { return -1; };
while (true) {
    f = eval<script>(script(
        "let n = 1; return function() { return n; };"));
    i = i + 1;
}
return 0;
