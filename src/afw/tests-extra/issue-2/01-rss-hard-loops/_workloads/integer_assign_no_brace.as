/* #2 lab: unbraced scalar overwrite. Compile wraps a `{ }`.
 * Should stay flat (temps die with the trip frame).
 */
let i = 0;
while (true)
    i = i + 1;
return 0;
