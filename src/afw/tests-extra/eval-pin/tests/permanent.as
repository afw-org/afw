assert(evaluate(compile<script>(script("return true;"))) === true);
assert(evaluate(compile<script>(script("return false;"))) === false);
assert(evaluate(compile<script>(script("return null;"))) === null);
const u = evaluate(compile<script>(script("return undefined;")));
assert(u === undefined);
return true;
