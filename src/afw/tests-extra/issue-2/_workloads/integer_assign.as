/* #2 lab: overwrite a scalar slot in a hard loop.
 * Optional free / same-size reuse should keep RSS flat after warmup.
 */
let i = 0;
while (true) {
    i = i + 1;
}
return 0;
