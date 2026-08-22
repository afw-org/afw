/* #2 lab: throw and catch every iteration (rewind path).
 * Error objects and scope rewind must not accumulate on one evaluate.
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
