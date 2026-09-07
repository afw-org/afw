/* #2 lab: unbraced scalar overwrite. No per-iteration `{ }`.
 * Should stay flat (same last-release as braced integer_assign).
 */
let i = 0;
while (true)
    i = i + 1;
return 0;
