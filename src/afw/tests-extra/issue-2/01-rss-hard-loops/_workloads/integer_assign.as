/* #2 lab: braced `i = i + 1`. Tracker APR (RSS) plus scalar boxing (in_use).
 * Prefer integer_assign_no_brace for the scalar-only slice.
 */
let i = 0;
while (true) {
    i = i + 1;
}
return 0;
