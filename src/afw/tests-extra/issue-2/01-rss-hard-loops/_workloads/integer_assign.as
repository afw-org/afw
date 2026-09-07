/* #2 lab: braced `i = i + 1`. Should stay flat (managed last-release
 * free_memory via xctx->p; temps die with the frame tracker).
 */
let i = 0;
while (true) {
    i = i + 1;
}
return 0;
