/* #2 lab: throw and catch every iteration (rewind path).
 * in_use still climbs (~0.25 MiB/s): afw_os_backtrace on xctx->p every
 * throw, previous pointer dropped. Not script RC. Release-on-overwrite
 * is a later sitting.
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
