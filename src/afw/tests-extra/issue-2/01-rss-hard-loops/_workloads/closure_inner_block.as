/* #458 lab: an inner-block closure stored in an outer slot. Flat with
 * cycle collection.
 */
function mk() {
    let g;
    {
        let n = 1;
        g = function() {
            return n;
        };
    }
    return 0;
}
while (true) {
    mk();
}
return 0;
