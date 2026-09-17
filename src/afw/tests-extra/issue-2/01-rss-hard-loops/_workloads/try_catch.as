/* #2 lab: throw and catch every iteration (rewind path).
 * in_use is flat: managed hexBinary backtrace is released on caught
 * ENDTRY / overwrite (was ~0.25 MiB/s on xctx->p).
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
