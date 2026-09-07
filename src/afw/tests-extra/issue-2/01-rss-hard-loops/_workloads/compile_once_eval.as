/* #2 lab: one compiled_value, evaluate over and over in the outer loop.
 * Inner heap wrap + clone-out. Should stay flat.
 */
const cv = compile<script>(script("return 1 + 2;"));
let r = 0;
while (true) {
    r = evaluate(cv);
}
return 0;
