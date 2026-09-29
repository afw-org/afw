/* #2 lab: compile listing assigned. Dump is an unmanaged string in
 * dest p; the unit is last-released after the copy. Pair with
 * compile_listing_unassigned. Do not extra-hold compile() of a unit.
 */
let r = "";
while (true) {
    r = compile<script>(script("return 1;"), 1);
}
return 0;
