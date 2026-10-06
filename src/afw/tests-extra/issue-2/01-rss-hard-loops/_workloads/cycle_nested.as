/* #458 lab: a nested object references its root (a.x.y.top = a).
 * Flat with cycle collection.
 */
function mk() {
    let a = { x: { y: {} } };
    a.x.y.top = a;
    return 0;
}
while (true) {
    mk();
}
return 0;
