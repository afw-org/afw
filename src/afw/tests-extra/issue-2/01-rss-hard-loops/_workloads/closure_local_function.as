/* #458 lab: a local function that never escapes. Its frame slot holds a
 * closure that captures that frame. Flat with cycle collection.
 */
function mk() {
    function f() {
        return 1;
    }
    return 0;
}
while (true) {
    mk();
}
return 0;
