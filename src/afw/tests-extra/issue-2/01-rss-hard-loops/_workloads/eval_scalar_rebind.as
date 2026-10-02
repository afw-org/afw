/* #2 lab: eval<script> scalar overwrite each iteration.
 * No escaped function: dest p of compile last-releases the unit.
 */
let i = 0;
let r = 0;
while (true) {
    r = eval<script>(script("return 1 + 2;"));
    i = i + 1;
}
return 0;
