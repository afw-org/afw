/* #2 lab: eval<script> scalar overwrite each iteration.
 * No keep_unit: last-release the compile unit after evaluate.
 */
let i = 0;
let r = 0;
while (true) {
    r = eval<script>(script("return 1 + 2;"));
    i = i + 1;
}
return 0;
