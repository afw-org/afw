/* #2 lab: clone nested object/array never assigned. Member get is
 * not a statement; discard() evaluates it as a parameter. Last
 * statement is add() so deactivate does not slot_store the child
 * into script_result.
 */
function discard(x) {
    return 0;
}
let o = { child: { x: 1, inner: { y: 2 } }, arr: [0, 1, 2, 3, 4] };
while (true) {
    discard(clone(o).child);
    discard(clone(o).arr);
    discard(clone(o.child));
    add(0, 0);
}
return 0;
