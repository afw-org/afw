/* #2 lab: throw and catch every iteration (rewind path).
 * in_use should stay flat; a small RSS-only APR climb is under the fail bar.
 */
let i = 0;
while (true) {
    try {
        throw i;
    } catch {
        i = i + 1;
    }
}
return 0;
