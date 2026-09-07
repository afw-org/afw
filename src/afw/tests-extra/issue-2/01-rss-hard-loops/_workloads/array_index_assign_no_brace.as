/* #2 lab: unbraced array element overwrite, no per-iteration `{ }`.
 * Should stay flat (same as braced array_index_assign).
 */
let a = [0];
let i = 0;
while (true)
    a[0] = i = i + 1;
return 0;
