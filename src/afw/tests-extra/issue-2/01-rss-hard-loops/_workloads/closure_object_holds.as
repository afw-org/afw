/* #458 lab: an object holds a closure that returns the object
 * (object -> closure -> frame -> object). Flat with cycle collection.
 */
function mk() {
    let o = {};
    o.f = function() {
        return o;
    };
    return 0;
}
while (true) {
    mk();
}
return 0;
