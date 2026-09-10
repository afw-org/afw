/* #2 lab: unbraced overlay set. Compile wraps a `{ }`.
 * Should stay flat (same as braced object_prop_assign).
 */
let o = { x: 0 };
let i = 0;
while (true)
    o.x = i = i + 1;
return 0;
