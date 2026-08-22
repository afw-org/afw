/* #2 lab: unbraced scalar overwrite.
 * in_use climbs with RSS until optional free / eval-heap boxing.
 * No per-iteration tracker (that is empty_loop / braced scripts).
 */
let i = 0;
while (true)
    i = i + 1;
return 0;
