/* #458 lab: an object that references itself (o.self = o) each call.
 * Reference counting alone keeps it; cycle collection frees it. Flat.
 */
function mk() {
    let o = { a: 1 };
    o.self = o;
    return 0;
}
while (true) {
    mk();
}
return 0;
