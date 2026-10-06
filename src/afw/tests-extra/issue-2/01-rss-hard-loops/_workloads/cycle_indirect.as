/* #458 lab: three objects in a loop (a -> b -> c -> a) each call.
 * Flat with cycle collection.
 */
function mk() {
    let a = {};
    let b = {};
    let c = {};
    a.b = b;
    b.c = c;
    c.a = a;
    return 0;
}
while (true) {
    mk();
}
return 0;
