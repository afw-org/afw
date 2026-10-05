/* #2 lab: evaluate(compile()) of a unit with a constant object literal
 * stored in its top frame. The literal is unmanaged in the unit's pool
 * and dies with the unit. (A per-literal pool under the job heap used to
 * survive every unit.)
 */
let r = 0;
while (true) {
    r = evaluate(compile<script>(script(
        "const extra = { port: 1 }; const o = { ...extra, host: \"h\" }; return o.host;")));
}
return 0;
