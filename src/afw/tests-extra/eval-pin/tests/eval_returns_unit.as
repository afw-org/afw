/* evaluate that returns a compiled_value unit. Pin must not drop the
 * unit before we evaluate it on this dest p. */
const inner = evaluate(compile<script>(script(
    "return compile<script>(script(\"return 4;\"));")));
assert(evaluate(inner) === 4);
return true;
