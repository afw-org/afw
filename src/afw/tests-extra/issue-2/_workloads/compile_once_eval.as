/* #2 lab: one compiled_value, evaluate over and over in the outer loop.
 * Each inner evaluate should create a heap, clone the result, release the heap.
 */
const cv = compile<script>(script("return 1 + 2;"));
let r = 0;
while (true) {
    r = evaluate(cv);
}
return 0;
